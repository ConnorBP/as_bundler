#include "commands.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <sys/stat.h>
#endif

#ifndef BUNDLER_VERSION
#define BUNDLER_VERSION "unknown"
#endif

/* -------------------------------------------------------------------------
 * cmd_update
 *
 * Usage:  pcx update
 *
 * Steps:
 *   1. Ensures gcc is installed (installs if missing).
 *   2. Ensures the pcx binary is installed (copies if missing).
 *   3. Queries the GitHub API for BOTH the latest full release and the
 *      latest pre-release, picks whichever was published more recently,
 *      then downloads the correct named asset for the current platform:
 *        Windows : <tag>/as_bundler.exe
 *        Linux   : <tag>/as_bundler
 *
 * The download is performed using curl, wget, or PowerShell (Windows).
 *
 * API endpoints used:
 *   Latest full release  : GET /repos/sinistercodes/as_bundler/releases/latest
 *   All releases (page1) : GET /repos/sinistercodes/as_bundler/releases
 *                          → first entry whose "prerelease":true is taken
 *                            as the latest pre-release.
 * ---------------------------------------------------------------------- */

#define GITHUB_API_BASE   "https://api.github.com/repos/sinistercodes/as_bundler"
#define GITHUB_REPO_BASE  "https://github.com/sinistercodes/as_bundler"

/* Platform asset name – no suffix on Linux, .exe on Windows */
#ifdef _WIN32
#  define ASSET_NAME  "as_bundler.exe"
#else
#  define ASSET_NAME  "as_bundler"
#endif

/* Maximum size we are willing to read from the GitHub API response */
#define API_BUF_SIZE  (512 * 1024)

/* -------------------------------------------------------------------------
 * has_tool – returns 1 if running `tool --version` exits 0
 * ---------------------------------------------------------------------- */
static int has_tool(const char *tool) {
#ifdef _WIN32
    char cmd[256];
    snprintf(cmd, sizeof(cmd), "%s --version >nul 2>&1", tool);
#else
    char cmd[256];
    snprintf(cmd, sizeof(cmd), "%s --version >/dev/null 2>&1", tool);
#endif
    return system(cmd) == 0 ? 1 : 0;
}

/* -------------------------------------------------------------------------
 * fetch_url_to_file
 * Downloads `url` into the file at `dest_path`.
 * Returns 0 on success, 1 on failure.
 * ---------------------------------------------------------------------- */
static int fetch_url_to_file(const char *url, const char *dest_path,
                              int show_progress) {
    char cmd[2048];

#ifdef _WIN32
    if (has_tool("curl")) {
        if (show_progress)
            snprintf(cmd, sizeof(cmd),
                     "curl -L --fail --progress-bar -o \"%s\" \"%s\"",
                     dest_path, url);
        else
            snprintf(cmd, sizeof(cmd),
                     "curl -L --fail -s -o \"%s\" \"%s\"",
                     dest_path, url);
        return system(cmd) == 0 ? 0 : 1;
    }
    /* PowerShell fallback */
    snprintf(cmd, sizeof(cmd),
             "powershell -NoProfile -Command "
             "\"Invoke-WebRequest -Uri '%s' -OutFile '%s' -UseBasicParsing\"",
             url, dest_path);
    return system(cmd) == 0 ? 0 : 1;
#else
    if (has_tool("curl")) {
        if (show_progress)
            snprintf(cmd, sizeof(cmd),
                     "curl -L --fail --progress-bar -o \"%s\" \"%s\"",
                     dest_path, url);
        else
            snprintf(cmd, sizeof(cmd),
                     "curl -L --fail -s -o \"%s\" \"%s\"",
                     dest_path, url);
        return system(cmd) == 0 ? 0 : 1;
    }
    if (has_tool("wget")) {
        if (show_progress)
            snprintf(cmd, sizeof(cmd),
                     "wget --show-progress -q -O \"%s\" \"%s\"",
                     dest_path, url);
        else
            snprintf(cmd, sizeof(cmd),
                     "wget -q -O \"%s\" \"%s\"",
                     dest_path, url);
        return system(cmd) == 0 ? 0 : 1;
    }
    fprintf(stderr,
            "Error: Neither curl nor wget is available.\n"
            "Install one and re-run: pcx update\n"
            "  Debian/Ubuntu : sudo apt-get install -y curl\n"
            "  Fedora/RHEL   : sudo dnf install -y curl\n"
            "  Arch          : sudo pacman -S curl\n");
    return 1;
#endif
}

/* -------------------------------------------------------------------------
 * fetch_url_to_buffer
 * Downloads `url` into a heap-allocated NUL-terminated buffer.
 * Caller must free() the returned pointer.
 * Returns NULL on failure.
 * ---------------------------------------------------------------------- */
static char *fetch_url_to_buffer(const char *url) {
    /* Write to a temporary file then read it back */
#ifdef _WIN32
    char tmp_path[MAX_PATH];
    DWORD ret = GetTempPathA(MAX_PATH, tmp_path);
    if (ret == 0 || ret >= MAX_PATH) return NULL;
    strncat(tmp_path, "pcx_api_tmp.json", MAX_PATH - strlen(tmp_path) - 1);
    DeleteFileA(tmp_path);
#else
    const char *tmp_path = "/tmp/pcx_api_tmp.json";
    unlink(tmp_path);
#endif

    if (fetch_url_to_file(url, tmp_path, 0) != 0)
        return NULL;

    FILE *f = fopen(tmp_path, "rb");
    if (!f) return NULL;

    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);

    if (fsize <= 0 || fsize > API_BUF_SIZE) {
        fclose(f);
#ifdef _WIN32
        DeleteFileA(tmp_path);
#else
        unlink(tmp_path);
#endif
        return NULL;
    }

    char *buf = (char *)malloc((size_t)fsize + 1);
    if (!buf) { fclose(f); return NULL; }

    size_t n = fread(buf, 1, (size_t)fsize, f);
    buf[n] = '\0';
    fclose(f);

#ifdef _WIN32
    DeleteFileA(tmp_path);
#else
    unlink(tmp_path);
#endif
    return buf;
}

/* -------------------------------------------------------------------------
 * json_str_field
 * Minimal JSON string extractor.
 * Finds the first occurrence of  "key":"value"  or  "key": "value"  in
 * `json` and copies the value into `out` (up to out_size-1 chars).
 * Returns 1 on success, 0 if the key was not found.
 *
 * Only handles simple unescaped string values – sufficient for tag_name,
 * published_at, and browser_download_url.
 * ---------------------------------------------------------------------- */
static int json_str_field(const char *json, const char *key,
                           char *out, size_t out_size) {
    /* Build search pattern: "key" */
    char pattern[256];
    snprintf(pattern, sizeof(pattern), "\"%s\"", key);
    size_t pat_len = strlen(pattern);

    const char *p = json;
    while ((p = strstr(p, pattern)) != NULL) {
        p += pat_len;
        /* Skip whitespace and the colon */
        while (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r') p++;
        if (*p != ':') continue;
        p++;
        while (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r') p++;
        if (*p != '"') continue;
        p++; /* skip opening quote */

        /* Copy until closing quote (ignoring escaped quotes for simplicity) */
        size_t i = 0;
        while (*p && *p != '"' && i < out_size - 1) {
            if (*p == '\\' && *(p + 1)) {
                p++; /* skip backslash, copy next char raw */
            }
            out[i++] = *p++;
        }
        out[i] = '\0';
        return 1;
    }
    return 0;
}

/* -------------------------------------------------------------------------
 * json_bool_field
 * Finds  "key": true  or  "key": false  in `json`.
 * Returns 1 for true, 0 for false, -1 if not found.
 * ---------------------------------------------------------------------- */
static int json_bool_field(const char *json, const char *key) {
    char pattern[256];
    snprintf(pattern, sizeof(pattern), "\"%s\"", key);
    size_t pat_len = strlen(pattern);

    const char *p = json;
    while ((p = strstr(p, pattern)) != NULL) {
        p += pat_len;
        while (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r') p++;
        if (*p != ':') continue;
        p++;
        while (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r') p++;
        if (strncmp(p, "true", 4) == 0)  return 1;
        if (strncmp(p, "false", 5) == 0) return 0;
    }
    return -1;
}

/* -------------------------------------------------------------------------
 * find_asset_url
 * Scans the "assets" array in a release JSON object and returns (in `out`)
 * the browser_download_url of the asset whose "name" equals `asset_name`.
 * Returns 1 on success, 0 if not found.
 * ---------------------------------------------------------------------- */
static int find_asset_url(const char *json, const char *asset_name,
                           char *out, size_t out_size) {
    /* Find the assets array */
    const char *assets = strstr(json, "\"assets\"");
    if (!assets) return 0;
    assets = strchr(assets, '[');
    if (!assets) return 0;

    const char *p = assets;
    while ((p = strstr(p, "\"name\"")) != NULL) {
        /* Extract name value */
        char name_val[256] = {0};
        const char *name_start = p;
        if (!json_str_field(p, "name", name_val, sizeof(name_val))) {
            p++;
            continue;
        }

        if (strcmp(name_val, asset_name) == 0) {
            /* Find browser_download_url in the same asset object.
             * Search forward from the name field – it's always in the
             * same object (within ~1 KB). */
            char url_val[MAX_PATH * 2];
            if (json_str_field(name_start, "browser_download_url",
                               url_val, sizeof(url_val))) {
                strncpy(out, url_val, out_size - 1);
                out[out_size - 1] = '\0';
                return 1;
            }
        }
        p++;
    }
    return 0;
}

/* -------------------------------------------------------------------------
 * ReleaseInfo – holds the parsed data for one GitHub release
 * ---------------------------------------------------------------------- */
typedef struct {
    char tag_name[64];
    char published_at[32];   /* ISO 8601 – sorts lexicographically */
    char asset_url[1024];
    int  prerelease;
    int  valid;              /* 1 if successfully parsed */
} ReleaseInfo;

/* -------------------------------------------------------------------------
 * parse_release
 * Parses the minimal fields we need from a single release JSON object.
 * `json` should point at the start of the object (or the full response
 * for a /releases/latest reply).
 * ---------------------------------------------------------------------- */
static ReleaseInfo parse_release(const char *json) {
    ReleaseInfo r;
    memset(&r, 0, sizeof(r));

    if (!json_str_field(json, "tag_name",     r.tag_name,     sizeof(r.tag_name))  ||
        !json_str_field(json, "published_at", r.published_at, sizeof(r.published_at))) {
        return r; /* valid stays 0 */
    }

    r.prerelease = json_bool_field(json, "prerelease");
    if (r.prerelease < 0) r.prerelease = 0;

    if (!find_asset_url(json, ASSET_NAME, r.asset_url, sizeof(r.asset_url))) {
        fprintf(stderr,
                "Warning: Release %s has no '%s' asset.\n",
                r.tag_name, ASSET_NAME);
        return r; /* valid stays 0 */
    }

    r.valid = 1;
    return r;
}

/* -------------------------------------------------------------------------
 * fetch_latest_release
 * Queries /releases/latest (newest non-pre-release).
 * Returns a ReleaseInfo; check .valid.
 * ---------------------------------------------------------------------- */
static ReleaseInfo fetch_latest_release(void) {
    ReleaseInfo r;
    memset(&r, 0, sizeof(r));

    char url[512];
    snprintf(url, sizeof(url), "%s/releases/latest", GITHUB_API_BASE);

    // printf("  Querying latest release...\n");
    char *json = fetch_url_to_buffer(url);
    if (!json) {
        fprintf(stderr, "Error: Failed to fetch %s\n", url);
        return r;
    }

    r = parse_release(json);
    free(json);
    return r;
}

/* -------------------------------------------------------------------------
 * fetch_latest_prerelease
 * Queries /releases (list, newest first) and returns the first entry
 * whose "prerelease" field is true.
 * Returns a ReleaseInfo; check .valid.
 * ---------------------------------------------------------------------- */
static ReleaseInfo fetch_latest_prerelease(void) {
    ReleaseInfo r;
    memset(&r, 0, sizeof(r));

    /* Fetch the first page of releases (up to 30, newest first by default) */
    char url[512];
    snprintf(url, sizeof(url),
             "%s/releases?per_page=10", GITHUB_API_BASE);

    // printf("  Querying latest pre-release...\n");
    char *json = fetch_url_to_buffer(url);
    if (!json) {
        fprintf(stderr, "Error: Failed to fetch %s\n", url);
        return r;
    }

    /* Walk through the array of release objects, find first prerelease */
    const char *p = json;
    /* Each release object starts with {  – find them by looking for
     * "tag_name" occurrences and walking back to the enclosing { */
    while ((p = strstr(p, "\"tag_name\"")) != NULL) {
        /* Walk backwards to find the opening { of this release object */
        const char *obj_start = p;
        while (obj_start > json && *obj_start != '{') obj_start--;

        ReleaseInfo candidate = parse_release(obj_start);
        if (candidate.valid && candidate.prerelease) {
            free(json);
            return candidate;
        }
        p++;
    }

    free(json);
    return r; /* none found */
}

/* -------------------------------------------------------------------------
 * parse_version
 * Basic semantic version parser. Expects "v1.2.3" or "1.2.3".
 * ---------------------------------------------------------------------- */
static int parse_version(const char *v, int *maj, int *min, int *patch, int *commits) {
    *maj = 0; *min = 0; *patch = 0; *commits = 0;
    while (*v && (*v < '0' || *v > '9')) v++;
    if (!*v) return 0;
    *maj = atoi(v);
    while (*v >= '0' && *v <= '9') v++;
    if (*v == '.') v++;
    *min = atoi(v);
    while (*v >= '0' && *v <= '9') v++;
    if (*v == '.') v++;
    *patch = atoi(v);
    while (*v >= '0' && *v <= '9') v++;
    if (*v == '-') {
        v++;
        *commits = atoi(v);
    }
    return 1;
}

/* -------------------------------------------------------------------------
 * is_newer_version
 * Returns 1 if 'remote' is newer than 'local', 0 otherwise.
 * ---------------------------------------------------------------------- */
static int is_newer_version(const char *remote, const char *local) {
    int rm=0, rmi=0, rp=0, rc=0;
    int lm=0, lmi=0, lp=0, lc=0;

    if (!parse_version(remote, &rm, &rmi, &rp, &rc)) return 1;
    if (!parse_version(local, &lm, &lmi, &lp, &lc)) return 1;

    if (rm > lm) return 1;
    if (rm < lm) return 0;

    if (rmi > lmi) return 1;
    if (rmi < lmi) return 0;

    if (rp > lp) return 1;
    if (rp < lp) return 0;

    if (rc > lc) return 1;
    if (rc < lc) return 0;

    return 0;
}

/* -------------------------------------------------------------------------
 * pick_newer
 * Compares two ReleaseInfo structs by published_at (ISO 8601 string –
 * lexicographic comparison is correct for this format).
 * Returns a pointer to whichever is newer, or the one that is valid if
 * only one is.
 * ---------------------------------------------------------------------- */
static const ReleaseInfo *pick_newer(const ReleaseInfo *a, const ReleaseInfo *b) {
    if (!a->valid && !b->valid) return NULL;
    if (!a->valid) return b;
    if (!b->valid) return a;
    /* ISO 8601 "2025-01-02T15:04:05Z" sorts correctly as a plain string */
    return strcmp(a->published_at, b->published_at) >= 0 ? a : b;
}

/* -------------------------------------------------------------------------
 * replace_executable
 * Atomically replaces `exe_path` with `new_path`.
 *
 * Windows : running exe cannot be overwritten; rename it to .old, move
 *           the new binary in, schedule .old for deletion on reboot.
 * Linux   : rename() over the running inode works fine; the process keeps
 *           its old inode until it exits.
 * ---------------------------------------------------------------------- */
static int replace_executable(const char *exe_path, const char *new_path) {
#ifdef _WIN32
    char old_path[MAX_PATH];
    snprintf(old_path, sizeof(old_path), "%s.old", exe_path);

    DeleteFileA(old_path);

    if (!MoveFileExA(exe_path, old_path, MOVEFILE_REPLACE_EXISTING)) {
        fprintf(stderr,
                "Error: Could not rename current executable (code %lu).\n"
                "Try running as administrator.\n",
                (unsigned long)GetLastError());
        return 1;
    }

    if (!MoveFileExA(new_path, exe_path, MOVEFILE_REPLACE_EXISTING)) {
        MoveFileExA(old_path, exe_path, MOVEFILE_REPLACE_EXISTING);
        fprintf(stderr,
                "Error: Could not move new binary to '%s' (code %lu).\n",
                exe_path, (unsigned long)GetLastError());
        return 1;
    }

    MoveFileExA(old_path, NULL, MOVEFILE_DELAY_UNTIL_REBOOT);
    return 0;

#else
    if (rename(new_path, exe_path) != 0) {
        perror("Error: Could not replace executable");
        return 1;
    }
    chmod(exe_path, 0755);
    return 0;
#endif
}

/* -------------------------------------------------------------------------
 * print_update_help
 * ---------------------------------------------------------------------- */
static void print_update_help(void) {
    printf("Usage: pcx update\n"
           "\n"
           "  Checks prerequisites and downloads the newest pcx binary from\n"
           "  GitHub, considering both full releases and pre-releases.\n"
           "  Whichever was published most recently is installed.\n"
           "\n"
           "  Prerequisites checked:\n"
           "    - gcc           (installed automatically if missing)\n"
           "    - pcx installed (copied to system bin dir if missing)\n"
           "\n"
           "  GitHub repository:\n"
           "    " GITHUB_REPO_BASE "\n"
           "\n"
           "  Asset downloaded:\n"
           "    Windows : " ASSET_NAME "\n"
           "    Linux   : " ASSET_NAME "\n"
           "\n"
           "  No arguments are required.\n");
}

/* -------------------------------------------------------------------------
 * cmd_update – entry point
 * ---------------------------------------------------------------------- */
int cmd_update(int argc, char **argv) {
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
            print_update_help();
            return 0;
        }
    }

    int overall_ok = 1;

    /* ------------------------------------------------------------------
     * Step 1: Ensure gcc is installed
     * ------------------------------------------------------------------ */
    if (!is_gcc_available()) {
        printf("gcc not found - installing...\n");
        if (install_gcc() != 0) {
            fprintf(stderr, "Warning: gcc installation failed or requires manual steps.\n");
            overall_ok = 0;
        } else {
            if (is_gcc_available())
                printf("gcc is now available.\n");
            else
                printf("Note: gcc may require a terminal restart to appear on PATH.\n");
        }
        printf("\n");
    }

    /* ------------------------------------------------------------------
     * Step 2: Ensure pcx binary is installed
     * ------------------------------------------------------------------ */
    if (!is_registered_in_path()) {
        printf("pcx not installed - installing now...\n");
        if (register_in_path() != 0) {
            fprintf(stderr, "Warning: Could not install pcx automatically.\n");
            overall_ok = 0;
        }
        printf("\n");
    }

    /* ------------------------------------------------------------------
     * Step 3: Determine the install path – always the fixed system bin
     * location (C:\bin\<name> on Windows, /usr/local/bin/<name> on Linux)
     * so that running `pcx update` from any directory always replaces the
     * installed copy rather than the binary that happened to be invoked.
     * ------------------------------------------------------------------ */
    char install_path[MAX_PATH];
    get_install_path(install_path);

    /* ------------------------------------------------------------------
     * Step 4: Query GitHub API for newest release (full or pre-release)
     * ------------------------------------------------------------------ */
    printf("Getting latest release\n");

    ReleaseInfo full_release = fetch_latest_release();
    ReleaseInfo pre_release  = fetch_latest_prerelease();

    const ReleaseInfo *chosen = pick_newer(&full_release, &pre_release);

    if (!chosen) {
        fprintf(stderr,
                "\nError: Could not retrieve any release information from GitHub.\n"
                "Please check your internet connection.\n"
                "You can also download manually from:\n"
                "  %s/releases\n", GITHUB_REPO_BASE);
        return 1;
    }

    // printf("\n  Latest full release  : %s",
    //        full_release.valid ? full_release.tag_name : "(none found)");
    // if (full_release.valid)
    //     printf("  (%s)", full_release.published_at);
    // printf("\n");

    // printf("  Latest pre-release   : %s",
    //        pre_release.valid ? pre_release.tag_name : "(none found)");
    // if (pre_release.valid)
    //     printf("  (%s)", pre_release.published_at);
    // printf("\n");

    // printf("Selected: %s%s\n\n",
    //        chosen->tag_name,
    //        chosen->prerelease ? " (pre-release)" : "");
    printf("\n");

    if (!is_newer_version(chosen->tag_name, BUNDLER_VERSION)) {
        printf("You already have the latest version (%s). No update needed.\n", BUNDLER_VERSION);
        return 0;
    }

    /* ------------------------------------------------------------------
     * Step 5: Download the asset to a temp file
     * ------------------------------------------------------------------ */
    // printf("=== Downloading %s ===\n", ASSET_NAME);
    // printf("  URL  : %s\n", chosen->asset_url);

    /* Write the temp file next to the install destination so that the
     * final rename/move stays on the same filesystem (avoids cross-device
     * link errors on Linux and simplifies the Windows MoveFileEx call). */
    char tmp_path[MAX_PATH];
#ifdef _WIN32
    snprintf(tmp_path, sizeof(tmp_path), "C:\\bin\\pcx_update_tmp.exe");
    DeleteFileA(tmp_path);
#else
    snprintf(tmp_path, sizeof(tmp_path), "/usr/local/bin/pcx_update_tmp");
    unlink(tmp_path);
#endif

    // printf("  Dest : %s\n\n", tmp_path);

    if (fetch_url_to_file(chosen->asset_url, tmp_path, 1) != 0) {
        fprintf(stderr,
                "\nError: Download failed.\n"
                "You can download manually from:\n"
                "  %s/releases/tag/%s\n",
                GITHUB_REPO_BASE, chosen->tag_name);
        return 1;
    }
    // printf("\nDownload complete.\n");

    /* ------------------------------------------------------------------
     * Step 6: Make executable (Linux) then replace
     * ------------------------------------------------------------------ */
#ifndef _WIN32
    chmod(tmp_path, 0755);
#endif

    printf("\nInstalling to: %s\n", install_path);
    if (replace_executable(install_path, tmp_path) != 0) {
#ifdef _WIN32
        DeleteFileA(tmp_path);
#else
        unlink(tmp_path);
#endif
        return 1;
    }

    /* ------------------------------------------------------------------
     * Summary
     * ------------------------------------------------------------------  */
    // printf("\n=== Update complete ===\n");
    printf("Installed: %s%s\n\n",
           chosen->tag_name,
           chosen->prerelease ? " (pre-release)" : "");
    if (!overall_ok)
        printf("Note: Some prerequisite checks had warnings (see above).\n");
    printf("You may need to restart your terminal for all changes to take effect.\n");

    return 0;
}
