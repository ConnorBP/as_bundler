#include "commands.h"
#include "../fs/fs.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#endif

/* -------------------------------------------------------------------------
 * cmd_watch
 *
 * Usage:  pcx watch [bundler options] <source_dir|file.as> [file2.as ...]
 *
 * Monitors the given source files / directories for modifications and
 * re-invokes the bundler (this same executable, without "watch") every
 * time a change is detected – similar to how `npm run dev` triggers hot
 * reloads on save.
 *
 * All arguments after "watch" are passed verbatim to the bundle command,
 * so any combination of bundler flags (-o, --obfuscate, --strip, etc.)
 * continues to work exactly as normal.
 *
 * Implementation:
 *   - We collect every .as file that is covered by the source arguments
 *     (same logic the bundler itself uses: directories are scanned
 *     recursively, individual files are taken as-is).
 *   - Every POLL_INTERVAL_MS milliseconds we check the last-write
 *     timestamp of each watched file.  If any timestamp has changed since
 *     the last check we trigger a rebuild.
 *   - We also detect new / deleted files inside watched directories so
 *     that adding a new .as file to the project triggers a rebuild too.
 *   - The rebuild is performed by re-executing the bundler binary with
 *     the original arguments (minus "watch").  The child process inherits
 *     stdout / stderr so the user sees the full build output.
 * ---------------------------------------------------------------------- */

#define POLL_INTERVAL_MS  500   /* check for changes every 500 ms */
#define MAX_WATCH_FILES   2048

/* -------------------------------------------------------------------------
 * File-stamp record
 * ---------------------------------------------------------------------- */
typedef struct {
    char path[MAX_PATH];
    long long mtime;   /* seconds since epoch, or Windows FILETIME ticks */
} FileStamp;

static FileStamp  g_watched[MAX_WATCH_FILES];
static int        g_watch_count = 0;

/* -------------------------------------------------------------------------
 * Portable sleep
 * ---------------------------------------------------------------------- */
static void sleep_ms(int ms) {
#ifdef _WIN32
    Sleep((DWORD)ms);
#else
    struct timespec ts;
    ts.tv_sec  = ms / 1000;
    ts.tv_nsec = (long)(ms % 1000) * 1000000L;
    nanosleep(&ts, NULL);
#endif
}

/* -------------------------------------------------------------------------
 * get_mtime – return last-write time as a 64-bit integer.
 * Returns -1 on error.
 * ---------------------------------------------------------------------- */
static long long get_mtime(const char *path) {
#ifdef _WIN32
    WIN32_FILE_ATTRIBUTE_DATA data;
    if (!GetFileAttributesExA(path, GetFileExInfoStandard, &data))
        return -1;
    /* Combine low and high parts of FILETIME into a single 64-bit value */
    ULARGE_INTEGER ui;
    ui.LowPart  = data.ftLastWriteTime.dwLowDateTime;
    ui.HighPart = data.ftLastWriteTime.dwHighDateTime;
    return (long long)ui.QuadPart;
#else
    struct stat st;
    if (stat(path, &st) != 0)
        return -1;
    return (long long)st.st_mtime;
#endif
}

/* -------------------------------------------------------------------------
 * add_watch_file – add a path to the watched list if not already present
 * ---------------------------------------------------------------------- */
static void add_watch_file(const char *path) {
    if (g_watch_count >= MAX_WATCH_FILES) {
        fprintf(stderr, "Warning: Watch limit reached (%d files). "
                        "Some files will not be monitored.\n", MAX_WATCH_FILES);
        return;
    }
    /* Deduplicate */
    for (int i = 0; i < g_watch_count; i++)
        if (strcmp(g_watched[i].path, path) == 0)
            return;

    strncpy(g_watched[g_watch_count].path, path, MAX_PATH - 1);
    g_watched[g_watch_count].path[MAX_PATH - 1] = '\0';
    g_watched[g_watch_count].mtime = get_mtime(path);
    g_watch_count++;
}

/* -------------------------------------------------------------------------
 * scan_watch_dir – recursively collect .as files from a directory
 * ---------------------------------------------------------------------- */
static void scan_watch_dir(const char *dir_path) {
#ifdef _WIN32
    WIN32_FIND_DATAA find_data;
    char search_path[MAX_PATH];
    snprintf(search_path, MAX_PATH, "%s\\*", dir_path);
    HANDLE hFind = FindFirstFileA(search_path, &find_data);
    if (hFind == INVALID_HANDLE_VALUE) return;
    do {
        if (strcmp(find_data.cFileName, ".") == 0 ||
            strcmp(find_data.cFileName, "..") == 0)
            continue;
        char full_path[MAX_PATH];
        snprintf(full_path, MAX_PATH, "%s\\%s", dir_path, find_data.cFileName);
        if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            scan_watch_dir(full_path);
        } else if (ends_with_as(find_data.cFileName)) {
            add_watch_file(full_path);
        }
    } while (FindNextFileA(hFind, &find_data) != 0);
    FindClose(hFind);
#else
    DIR *dir = opendir(dir_path);
    if (!dir) return;
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (strcmp(entry->d_name, ".") == 0 ||
            strcmp(entry->d_name, "..") == 0)
            continue;
        char full_path[MAX_PATH];
        snprintf(full_path, MAX_PATH, "%s/%s", dir_path, entry->d_name);
        if (is_directory(full_path)) {
            scan_watch_dir(full_path);
        } else if (ends_with_as(entry->d_name)) {
            add_watch_file(full_path);
        }
    }
    closedir(dir);
#endif
}

/* -------------------------------------------------------------------------
 * collect_watched_dirs – paths in the bundle args that are directories.
 * We store these so we can re-scan for new/deleted files each poll.
 * ---------------------------------------------------------------------- */
#define MAX_WATCH_DIRS 64
static char g_watch_dirs[MAX_WATCH_DIRS][MAX_PATH];
static int  g_watch_dir_count = 0;

static void add_watch_dir(const char *path) {
    if (g_watch_dir_count >= MAX_WATCH_DIRS) return;
    for (int i = 0; i < g_watch_dir_count; i++)
        if (strcmp(g_watch_dirs[i], path) == 0) return;
    strncpy(g_watch_dirs[g_watch_dir_count], path, MAX_PATH - 1);
    g_watch_dirs[g_watch_dir_count][MAX_PATH - 1] = '\0';
    g_watch_dir_count++;
}

/* -------------------------------------------------------------------------
 * refresh_watch_list – rebuild the watch list from scratch by re-scanning
 * all watched directories, then reconcile against the previous list:
 *   - entries for files that no longer exist are removed
 *   - entries for newly appeared files are added
 *   - timestamps for surviving files are preserved
 *
 * Returns 1 if the set of watched files changed (added or removed),
 * 0 otherwise.
 * ---------------------------------------------------------------------- */
static int refresh_watch_list(void) {
    /* ------------------------------------------------------------------
     * Build a fresh list of every .as file currently on disk.
     * We reuse a temporary FileStamp array on the stack (same max size).
     * ------------------------------------------------------------------ */
    static FileStamp fresh[MAX_WATCH_FILES];
    int fresh_count = 0;

    /* Collect all .as files from every watched directory */
    for (int d = 0; d < g_watch_dir_count; d++) {
        /* We temporarily redirect add_watch_file writes into `fresh` by
         * doing the scan ourselves inline so we don't touch g_watched. */
#ifdef _WIN32
        /* Recursive helper via a small local stack */
        char dir_stack[64][MAX_PATH];
        int  dir_top = 0;
        strncpy(dir_stack[dir_top++], g_watch_dirs[d], MAX_PATH - 1);

        while (dir_top > 0) {
            char cur_dir[MAX_PATH];
            strncpy(cur_dir, dir_stack[--dir_top], MAX_PATH - 1);

            WIN32_FIND_DATAA fd;
            char pattern[MAX_PATH];
            snprintf(pattern, MAX_PATH, "%s\\*", cur_dir);
            HANDLE h = FindFirstFileA(pattern, &fd);
            if (h == INVALID_HANDLE_VALUE) continue;
            do {
                if (strcmp(fd.cFileName, ".") == 0 ||
                    strcmp(fd.cFileName, "..") == 0) continue;
                char full[MAX_PATH];
                snprintf(full, MAX_PATH, "%s\\%s", cur_dir, fd.cFileName);
                if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                    if (dir_top < 64)
                        strncpy(dir_stack[dir_top++], full, MAX_PATH - 1);
                } else if (ends_with_as(fd.cFileName)) {
                    /* Deduplicate */
                    int dup = 0;
                    for (int k = 0; k < fresh_count; k++)
                        if (strcmp(fresh[k].path, full) == 0) { dup = 1; break; }
                    if (!dup && fresh_count < MAX_WATCH_FILES) {
                        strncpy(fresh[fresh_count].path, full, MAX_PATH - 1);
                        fresh[fresh_count].path[MAX_PATH - 1] = '\0';
                        fresh[fresh_count].mtime = 0; /* filled below */
                        fresh_count++;
                    }
                }
            } while (FindNextFileA(h, &fd));
            FindClose(h);
        }
#else
        /* Recursive helper via a small local stack */
        char dir_stack[64][MAX_PATH];
        int  dir_top = 0;
        strncpy(dir_stack[dir_top++], g_watch_dirs[d], MAX_PATH - 1);

        while (dir_top > 0) {
            char cur_dir[MAX_PATH];
            strncpy(cur_dir, dir_stack[--dir_top], MAX_PATH - 1);

            DIR *dp = opendir(cur_dir);
            if (!dp) continue;
            struct dirent *ent;
            while ((ent = readdir(dp)) != NULL) {
                if (strcmp(ent->d_name, ".") == 0 ||
                    strcmp(ent->d_name, "..") == 0) continue;
                char full[MAX_PATH];
                snprintf(full, MAX_PATH, "%s/%s", cur_dir, ent->d_name);
                if (is_directory(full)) {
                    if (dir_top < 64)
                        strncpy(dir_stack[dir_top++], full, MAX_PATH - 1);
                } else if (ends_with_as(ent->d_name)) {
                    int dup = 0;
                    for (int k = 0; k < fresh_count; k++)
                        if (strcmp(fresh[k].path, full) == 0) { dup = 1; break; }
                    if (!dup && fresh_count < MAX_WATCH_FILES) {
                        strncpy(fresh[fresh_count].path, full, MAX_PATH - 1);
                        fresh[fresh_count].path[MAX_PATH - 1] = '\0';
                        fresh[fresh_count].mtime = 0;
                        fresh_count++;
                    }
                }
            }
            closedir(dp);
        }
#endif
    }

    /* Also include individually watched files (non-directory sources) */
    for (int i = 0; i < g_watch_count; i++) {
        /* If this entry didn't come from a scanned directory (i.e. it was
         * added as an explicit file argument), carry it forward as long as
         * it still exists on disk. */
        int already = 0;
        for (int k = 0; k < fresh_count; k++)
            if (strcmp(fresh[k].path, g_watched[i].path) == 0)
                { already = 1; break; }
        if (!already) {
            /* Only keep it if it still exists */
            if (get_mtime(g_watched[i].path) >= 0 &&
                fresh_count < MAX_WATCH_FILES) {
                strncpy(fresh[fresh_count].path, g_watched[i].path,
                        MAX_PATH - 1);
                fresh[fresh_count].path[MAX_PATH - 1] = '\0';
                fresh[fresh_count].mtime = 0;
                fresh_count++;
            }
        }
    }

    /* ------------------------------------------------------------------
     * Reconcile: carry over stored mtimes for files that were already
     * being watched, so check_for_changes only fires on real changes.
     * ------------------------------------------------------------------ */
    for (int f = 0; f < fresh_count; f++) {
        long long old_mtime = 0;  /* 0 = unknown / newly appeared */
        for (int i = 0; i < g_watch_count; i++) {
            if (strcmp(g_watched[i].path, fresh[f].path) == 0) {
                old_mtime = g_watched[i].mtime;
                break;
            }
        }
        fresh[f].mtime = old_mtime;
    }

    /* ------------------------------------------------------------------
     * Detect structural change: any addition or removal.
     * ------------------------------------------------------------------ */
    int changed = (fresh_count != g_watch_count);
    if (!changed) {
        /* Same count – check whether it's the exact same set of paths */
        for (int f = 0; f < fresh_count && !changed; f++) {
            int found = 0;
            for (int i = 0; i < g_watch_count; i++)
                if (strcmp(fresh[f].path, g_watched[i].path) == 0)
                    { found = 1; break; }
            if (!found) changed = 1;
        }
    }

    /* ------------------------------------------------------------------
     * Commit the fresh list as the new watch list.
     * ------------------------------------------------------------------ */
    g_watch_count = fresh_count;
    for (int i = 0; i < fresh_count; i++)
        g_watched[i] = fresh[i];

    return changed;
}

/* -------------------------------------------------------------------------
 * resync_mtimes – re-snapshot every watched file's mtime.
 * Called after a rebuild so that files touched during the build (e.g. the
 * output file being written) don't immediately trigger the next cycle.
 * ---------------------------------------------------------------------- */
static void resync_mtimes(void) {
    for (int i = 0; i < g_watch_count; i++) {
        long long cur = get_mtime(g_watched[i].path);
        if (cur >= 0)
            g_watched[i].mtime = cur;
    }
}

/* -------------------------------------------------------------------------
 * check_for_changes – compare current mtimes against recorded ones.
 * Returns 1 if any file changed, 0 otherwise.
 * Also updates the stored timestamps when a change is detected.
 * ---------------------------------------------------------------------- */
static int check_for_changes(void) {
    int changed = 0;
    for (int i = 0; i < g_watch_count; i++) {
        long long cur = get_mtime(g_watched[i].path);

        /* mtime == 0 means the file was just added this poll cycle –
         * seed it with the real timestamp and don't treat it as a change.
         * Structural additions are already reported by refresh_watch_list. */
        if (g_watched[i].mtime == 0) {
            g_watched[i].mtime = cur;
            continue;
        }

        /* cur == -1 means the file was deleted – refresh_watch_list will
         * remove it from g_watched and report the structural change.
         * Don't fire a content-change event for it here. */
        if (cur < 0)
            continue;

        if (cur != g_watched[i].mtime) {
            if (!changed)
                printf("\n[watch] Change detected: %s\n", g_watched[i].path);
            g_watched[i].mtime = cur;
            changed = 1;
        }
    }
    return changed;
}

/* -------------------------------------------------------------------------
 * run_bundle_command
 * Re-executes this same binary with the bundle arguments (without "watch").
 * Returns the exit code of the child process.
 * ---------------------------------------------------------------------- */
static int run_bundle_command(const char *exe_path,
                               char **bundle_argv, int bundle_argc) {
#ifdef _WIN32
    /* On Windows, build a command string and run it with CreateProcess */
    char cmd[8192];
    int  pos = 0;

    /* Quote the executable */
    pos += snprintf(cmd + pos, sizeof(cmd) - (size_t)pos, "\"%s\"", exe_path);

    for (int i = 0; i < bundle_argc && pos < (int)sizeof(cmd) - 2; i++) {
        /* Simple quoting: wrap each argument in double quotes, escaping
         * any existing double-quote characters with a backslash */
        pos += snprintf(cmd + pos, sizeof(cmd) - (size_t)pos, " \"");
        const char *arg = bundle_argv[i];
        while (*arg && pos < (int)sizeof(cmd) - 4) {
            if (*arg == '"')
                cmd[pos++] = '\\';
            cmd[pos++] = *arg++;
        }
        if (pos < (int)sizeof(cmd) - 2)
            cmd[pos++] = '"';
    }
    cmd[pos] = '\0';

    STARTUPINFOA        si;
    PROCESS_INFORMATION pi;
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    if (!CreateProcessA(NULL, cmd, NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi)) {
        fprintf(stderr, "[watch] Error: Could not start bundler process (code %lu)\n",
                (unsigned long)GetLastError());
        return 1;
    }

    WaitForSingleObject(pi.hProcess, INFINITE);

    DWORD exit_code = 1;
    GetExitCodeProcess(pi.hProcess, &exit_code);
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    return (int)exit_code;

#else
    /* On POSIX, build argv array and use execv in a fork */
    pid_t pid = fork();
    if (pid < 0) {
        perror("[watch] fork");
        return 1;
    }

    if (pid == 0) {
        /* Child: build execv argument array */
        char **exec_argv = (char **)malloc(
            (size_t)(bundle_argc + 2) * sizeof(char *));
        exec_argv[0] = (char *)exe_path;
        for (int i = 0; i < bundle_argc; i++)
            exec_argv[i + 1] = bundle_argv[i];
        exec_argv[bundle_argc + 1] = NULL;

        execv(exe_path, exec_argv);
        /* If we get here execv failed */
        perror("[watch] execv");
        free(exec_argv);
        _exit(1);
    }

    /* Parent: wait for child */
    int status = 0;
    waitpid(pid, &status, 0);
    return WIFEXITED(status) ? WEXITSTATUS(status) : 1;
#endif
}

/* -------------------------------------------------------------------------
 * print_watch_help
 * ---------------------------------------------------------------------- */
static void print_watch_help(const char *program_name) {
    printf("Usage: %s watch [bundler options] <source_dir|file.as> [file2.as ...]\n",
           program_name);
    printf("\n");
    printf("  Watches the given source files or directories for changes and\n");
    printf("  automatically re-runs the bundler whenever a .as file is saved.\n");
    printf("\n");
    printf("  All options that the bundler normally accepts can be used here:\n");
    printf("    -o <file>              Output file\n");
    printf("    --strip, -s            Strip comment-only lines\n");
    printf("    --obfuscate, -O        Obfuscate identifiers\n");
    printf("    --remove-newlines, -R  Collapse whitespace\n");
    printf("    --scramble-strings, -S Scramble string literals\n");
    printf("    --no-preprocess, -n    Skip C preprocessor\n");
    printf("    --prepend, -p <file>   Prepend macro definitions file\n");
    printf("    --header, -H <file>    Prepend raw header to output\n");
    printf("    -D<NAME>[=VALUE]       Define a preprocessor macro\n");
    printf("\n");
    printf("  Press Ctrl+C to stop watching.\n");
    printf("\n");
    printf("Examples:\n");
    printf("  %s watch src/\n", program_name);
    printf("  %s watch -o bundle.as src/\n", program_name);
    printf("  %s watch -o out.as --obfuscate --strip src/\n", program_name);
}

/* -------------------------------------------------------------------------
 * cmd_watch – entry point
 * ---------------------------------------------------------------------- */
int cmd_watch(int argc, char **argv) {
    /* argv[0]    == "watch"
     * argv[1..n] == bundler options and source paths */

    if (argc < 2) {
        print_watch_help("pcx");
        return 1;
    }

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
            print_watch_help("pcx");
            return 0;
        }
    }

    /* ------------------------------------------------------------------
     * Determine the path to the running executable so we can re-invoke it
     * ------------------------------------------------------------------ */
    char exe_path[MAX_PATH];
    if (get_executable_path(exe_path) != 0) {
        fprintf(stderr, "[watch] Error: Could not determine executable path.\n");
        return 1;
    }

    /* ------------------------------------------------------------------
     * Build the bundle argument list (everything after "watch")
     * ------------------------------------------------------------------ */
    /* argv here is offset: argv[0]="watch", argv[1..] = real args */
    char **bundle_argv = argv + 1;
    int    bundle_argc = argc - 1;

    /* ------------------------------------------------------------------
     * Identify which arguments are source paths (dirs or .as files) so we
     * know what to watch.  We replicate the same heuristic that main.c
     * uses: the first non-option argument and everything after it.
     * ------------------------------------------------------------------ */
    g_watch_count     = 0;
    g_watch_dir_count = 0;

    /* Find first source argument (mirrors main.c parsing) */
    int first_source = -1;
    for (int i = 0; i < bundle_argc; i++) {
        const char *arg = bundle_argv[i];
        if (strcmp(arg, "-o") == 0 ||
            strcmp(arg, "--prepend") == 0 || strcmp(arg, "-p") == 0 ||
            strcmp(arg, "--header") == 0  || strcmp(arg, "-H") == 0) {
            i++;  /* skip the value */
            continue;
        }
        if (arg[0] != '-') {
            first_source = i;
            break;
        }
    }

    if (first_source < 0) {
        fprintf(stderr, "[watch] Error: No source files or directories specified.\n");
        print_watch_help("pcx");
        return 1;
    }

    /* Collect files / directories to watch */
    for (int i = first_source; i < bundle_argc; i++) {
        const char *arg = bundle_argv[i];
        if (is_directory(arg)) {
            add_watch_dir(arg);
            scan_watch_dir(arg);
        } else if (ends_with_as(arg)) {
            add_watch_file(arg);
        }
    }

    if (g_watch_count == 0 && g_watch_dir_count == 0) {
        fprintf(stderr, "[watch] Error: No .as files found to watch.\n");
        return 1;
    }

    /* ------------------------------------------------------------------
     * Print what we are watching
     * ------------------------------------------------------------------ */
    printf("[watch] Watching %d file(s)", g_watch_count);
    if (g_watch_dir_count > 0) {
        printf(" across %d director%s",
               g_watch_dir_count,
               g_watch_dir_count == 1 ? "y" : "ies");
    }
    printf(".\n");
    printf("[watch] Press Ctrl+C to stop.\n\n");

    /* ------------------------------------------------------------------
     * Initial build
     * ------------------------------------------------------------------ */
    printf("[watch] Running initial build...\n");
    int last_exit = run_bundle_command(exe_path, bundle_argv, bundle_argc);

    /* Seed all mtimes after the initial build for the same reason */
    refresh_watch_list();
    resync_mtimes();

    if (last_exit != 0)
        printf("[watch] Build finished with errors.\n");
    else
        printf("[watch] Build succeeded.\n");
    printf("[watch] Waiting for changes...\n");

    /* ------------------------------------------------------------------
     * Watch loop
     * ------------------------------------------------------------------ */
    for (;;) {
        sleep_ms(POLL_INTERVAL_MS);

        /* Re-scan directories for new / deleted files */
        int structure_changed = refresh_watch_list();

        /* Check mtimes */
        int content_changed = check_for_changes();

        if (structure_changed || content_changed) {
            if (structure_changed && !content_changed)
                printf("[watch] File structure changed (new or deleted files).\n");

            printf("[watch] Rebuilding...\n");
            last_exit = run_bundle_command(exe_path, bundle_argv, bundle_argc);

            /* Re-snapshot all mtimes after the build so that any file the
             * bundler touched during the build (output file, temp files, etc.)
             * doesn't immediately fire another change event. */
            refresh_watch_list();
            resync_mtimes();

            if (last_exit != 0)
                printf("[watch] Build finished with errors. Watching for changes...\n");
            else
                printf("[watch] Build succeeded. Watching for changes...\n");
        }
    }

    /* Unreachable – the loop runs until Ctrl+C */
    return 0;
}