#include "commands.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <sys/stat.h>
#endif

/* -------------------------------------------------------------------------
 * cmd_update
 *
 * Usage:  pcx update
 *
 * Performs the following steps:
 *   1. Ensures gcc is installed (installs if missing).
 *   2. Ensures the pcx executable directory is in PATH (registers if missing).
 *   3. Downloads the latest binary from the GitHub releases page:
 *        https://github.com/sinistercodes/as_bundler/releases/latest
 *      and replaces the current executable with it.
 *
 * The download is performed using curl or wget (whichever is available).
 * On Windows PowerShell's Invoke-WebRequest is used as a fallback.
 *
 * GitHub release asset names:
 *   Windows : as_bundler-windows-x86_64.exe  (renamed to pcx.exe locally)
 *   Linux   : as_bundler-linux-x86_64        (renamed to pcx locally)
 * ---------------------------------------------------------------------- */

#define GITHUB_API_LATEST \
    "https://api.github.com/repos/sinistercodes/as_bundler/releases/latest"

#define GITHUB_RELEASES_BASE \
    "https://github.com/sinistercodes/as_bundler/releases/latest/download"

#ifdef _WIN32
#  define RELEASE_ASSET  "as_bundler-windows-x86_64.exe"
#else
#  define RELEASE_ASSET  "as_bundler-linux-x86_64"
#endif

/* -------------------------------------------------------------------------
 * has_tool – returns 1 if `tool --version` (or equivalent) exits 0
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
 * download_file
 * Downloads `url` to `dest_path` using curl, wget, or PowerShell.
 * Returns 0 on success, 1 on failure.
 * ---------------------------------------------------------------------- */
static int download_file(const char *url, const char *dest_path) {
    char cmd[2048];

#ifdef _WIN32
    /* Try curl first (ships with Windows 10 1803+) */
    if (has_tool("curl")) {
        snprintf(cmd, sizeof(cmd),
                 "curl -L --fail --progress-bar -o \"%s\" \"%s\"",
                 dest_path, url);
        printf("Downloading via curl...\n");
        return system(cmd) == 0 ? 0 : 1;
    }

    /* Fall back to PowerShell Invoke-WebRequest */
    printf("curl not found – trying PowerShell...\n");
    snprintf(cmd, sizeof(cmd),
             "powershell -NoProfile -Command "
             "\"Invoke-WebRequest -Uri '%s' -OutFile '%s' -UseBasicParsing\"",
             url, dest_path);
    return system(cmd) == 0 ? 0 : 1;

#else
    /* Try curl */
    if (has_tool("curl")) {
        snprintf(cmd, sizeof(cmd),
                 "curl -L --fail --progress-bar -o \"%s\" \"%s\"",
                 dest_path, url);
        printf("Downloading via curl...\n");
        return system(cmd) == 0 ? 0 : 1;
    }

    /* Try wget */
    if (has_tool("wget")) {
        snprintf(cmd, sizeof(cmd),
                 "wget --show-progress -q -O \"%s\" \"%s\"",
                 dest_path, url);
        printf("Downloading via wget...\n");
        return system(cmd) == 0 ? 0 : 1;
    }

    fprintf(stderr,
            "Error: Neither curl nor wget is available.\n"
            "Please install one of them and re-run: pcx update\n"
            "  Debian/Ubuntu : sudo apt-get install -y curl\n"
            "  Fedora/RHEL   : sudo dnf install -y curl\n"
            "  Arch          : sudo pacman -S curl\n");
    return 1;
#endif
}

/* -------------------------------------------------------------------------
 * replace_executable
 * Atomically replaces the running executable with `new_path`.
 *
 * Windows : The running binary cannot be overwritten while in use, so we
 *           rename the current exe to a .old file and move the new binary
 *           into place.  A batch file is written and scheduled to delete
 *           the .old file on next startup.
 * Linux   : We can unlink the running inode and write the new file directly;
 *           the running process keeps its old inode until it exits.
 *
 * Returns 0 on success, non-zero on failure.
 * ---------------------------------------------------------------------- */
static int replace_executable(const char *exe_path, const char *new_path) {
#ifdef _WIN32
    /* Rename old exe */
    char old_path[MAX_PATH];
    snprintf(old_path, sizeof(old_path), "%s.old", exe_path);

    /* Remove any previous .old leftover */
    DeleteFileA(old_path);

    if (!MoveFileExA(exe_path, old_path, MOVEFILE_REPLACE_EXISTING)) {
        fprintf(stderr,
                "Error: Could not rename current executable to .old (code %lu).\n"
                "Try running as administrator.\n",
                (unsigned long)GetLastError());
        return 1;
    }

    /* Move new binary into place */
    if (!MoveFileExA(new_path, exe_path, MOVEFILE_REPLACE_EXISTING)) {
        /* Try to restore the original */
        MoveFileExA(old_path, exe_path, MOVEFILE_REPLACE_EXISTING);
        fprintf(stderr,
                "Error: Could not move new binary to '%s' (code %lu).\n",
                exe_path, (unsigned long)GetLastError());
        return 1;
    }

    /* Schedule deletion of the .old file at next reboot */
    MoveFileExA(old_path, NULL, MOVEFILE_DELAY_UNTIL_REBOOT);

    return 0;

#else
    /* On Linux we can simply rename over the existing file */
    if (rename(new_path, exe_path) != 0) {
        perror("Error: Could not replace executable");
        return 1;
    }

    /* Ensure the new binary is executable */
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
           "  Checks for and installs any missing prerequisites, then\n"
           "  downloads the latest pcx binary from GitHub and replaces\n"
           "  the current executable.\n"
           "\n"
           "  Prerequisites checked:\n"
           "    - gcc         (installed automatically if missing)\n"
           "    - PATH entry  (registered automatically if missing)\n"
           "\n"
           "  The latest release is fetched from:\n"
           "    " GITHUB_RELEASES_BASE "\n"
           "\n"
           "  No arguments are required.\n");
}

/* -------------------------------------------------------------------------
 * cmd_update – entry point
 * ---------------------------------------------------------------------- */
int cmd_update(int argc, char **argv) {
    /* Check for --help */
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
    printf("=== Checking for gcc ===\n");
    if (is_gcc_available()) {
        printf("gcc is already installed.\n");
    } else {
        printf("gcc not found – installing...\n");
        if (install_gcc() != 0) {
            fprintf(stderr,
                    "Warning: gcc installation failed or requires manual steps.\n");
            overall_ok = 0;
        } else {
            if (is_gcc_available())
                printf("gcc is now available.\n");
            else
                printf("Note: gcc may require a terminal restart to appear on PATH.\n");
        }
    }
    printf("\n");

    /* ------------------------------------------------------------------
     * Step 2: Ensure pcx is registered in PATH
     * ------------------------------------------------------------------ */
    printf("=== Checking PATH registration ===\n");
    if (is_registered_in_path()) {
        printf("pcx is already registered in PATH.\n");
    } else {
        printf("pcx not found in PATH – registering...\n");
        if (register_in_path() != 0) {
            fprintf(stderr,
                    "Warning: Could not register pcx in PATH automatically.\n");
            overall_ok = 0;
        }
    }
    printf("\n");

    /* ------------------------------------------------------------------
     * Step 3: Determine current executable path
     * ------------------------------------------------------------------ */
    char exe_path[MAX_PATH];
    if (get_executable_path(exe_path) != 0) {
        fprintf(stderr, "Error: Could not determine the current executable path.\n");
        return 1;
    }

    char exe_dir[MAX_PATH];
    if (get_executable_dir(exe_dir) != 0) {
        fprintf(stderr, "Error: Could not determine the executable directory.\n");
        return 1;
    }

    printf("=== Downloading latest release ===\n");
    printf("Current executable: %s\n", exe_path);

    /* ------------------------------------------------------------------
     * Step 4: Build download URL and temporary destination path
     * ------------------------------------------------------------------ */
    char download_url[1024];
    snprintf(download_url, sizeof(download_url),
             "%s/%s", GITHUB_RELEASES_BASE, RELEASE_ASSET);

    char tmp_path[MAX_PATH];
#ifdef _WIN32
    snprintf(tmp_path, sizeof(tmp_path), "%s\\pcx_update_tmp.exe", exe_dir);
#else
    snprintf(tmp_path, sizeof(tmp_path), "%s/pcx_update_tmp", exe_dir);
#endif

    printf("Download URL : %s\n", download_url);
    printf("Saving to    : %s\n\n", tmp_path);

    /* Remove stale temp file if present */
#ifdef _WIN32
    DeleteFileA(tmp_path);
#else
    unlink(tmp_path);
#endif

    if (download_file(download_url, tmp_path) != 0) {
        fprintf(stderr,
                "\nError: Download failed.\n"
                "Please check your internet connection or download manually:\n"
                "  %s\n", download_url);
        return 1;
    }

    printf("\nDownload complete.\n");

    /* ------------------------------------------------------------------
     * Step 5: Make the downloaded binary executable (Linux only)
     * ------------------------------------------------------------------ */
#ifndef _WIN32
    chmod(tmp_path, 0755);
#endif

    /* ------------------------------------------------------------------
     * Step 6: Replace the current executable
     * ------------------------------------------------------------------ */
    printf("Replacing executable...\n");
    if (replace_executable(exe_path, tmp_path) != 0) {
        /* Clean up temp file on failure */
#ifdef _WIN32
        DeleteFileA(tmp_path);
#else
        unlink(tmp_path);
#endif
        return 1;
    }

    /* ------------------------------------------------------------------
     * Summary
     * ------------------------------------------------------------------ */
    printf("\n=== Update complete ===\n");
    if (!overall_ok) {
        printf("Note: Some prerequisite checks had warnings (see above).\n");
    }
    printf("pcx has been updated to the latest release.\n");
    printf("You may need to restart your terminal for all changes to take effect.\n");

    return 0;
}