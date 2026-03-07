#include "commands.h"

/* -------------------------------------------------------------------------
 * cmd_install
 *
 * Usage:  pcx install
 *
 * Performs two tasks:
 *   1. Installs gcc if it is not already available.
 *   2. Registers the pcx executable directory in the system PATH so it can
 *      be called from anywhere without specifying the full path.
 *
 * Platform behaviour:
 *   Windows – PATH is written to the registry (HKLM or HKCU fallback).
 *             gcc is installed via winget (MSYS2 toolchain).
 *   Linux   – PATH is written to /etc/profile.d/pcx.sh (falls back to
 *             ~/.bashrc / ~/.zshrc).  gcc is installed via the distro's
 *             package manager (apt-get, dnf, pacman, zypper, apk).
 * ---------------------------------------------------------------------- */

static void print_install_help(void) {
    printf("Usage: pcx install\n"
           "\n"
           "  Installs gcc (if not already present) and registers the pcx\n"
           "  executable in your system PATH so it can be invoked from any\n"
           "  directory.\n"
           "\n"
           "  No arguments are required.\n"
           "\n"
           "  On Windows this writes to the registry and may require\n"
           "  administrator privileges for a system-wide PATH change.\n"
           "\n"
           "  On Linux this writes to /etc/profile.d/pcx.sh (requires root)\n"
           "  or falls back to the current user's shell profile.\n");
}

int cmd_install(int argc, char **argv) {
    /* Check for --help */
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
            print_install_help();
            return 0;
        }
    }

    int overall_ok = 1;

    /* ------------------------------------------------------------------
     * Step 1: gcc
     * ------------------------------------------------------------------ */
    printf("=== Checking for gcc ===\n");
    if (is_gcc_available()) {
        printf("gcc is already installed.\n");
    } else {
        printf("gcc not found – installing...\n");
        if (install_gcc() != 0) {
            fprintf(stderr, "Warning: gcc installation failed or requires "
                            "manual steps (see above).\n");
            overall_ok = 0;
        } else {
            /* Verify the install actually worked */
            if (is_gcc_available()) {
                printf("gcc is now available.\n");
            } else {
                printf("Note: gcc may require a terminal restart to become "
                       "available on PATH.\n");
            }
        }
    }

    printf("\n");

    /* ------------------------------------------------------------------
     * Step 2: Register pcx in PATH
     * ------------------------------------------------------------------ */
    printf("=== Registering pcx in PATH ===\n");

    char exe_dir[MAX_PATH];
    if (get_executable_dir(exe_dir) != 0) {
        fprintf(stderr, "Error: Could not determine the executable directory.\n");
        return 1;
    }

    printf("Executable directory: %s\n", exe_dir);

    if (is_registered_in_path()) {
        printf("pcx is already registered in PATH.\n");
    } else {
        if (register_in_path() != 0) {
            fprintf(stderr, "Warning: Could not register pcx in PATH "
                            "(see above for manual steps).\n");
            overall_ok = 0;
        }
    }

    printf("\n");

    /* ------------------------------------------------------------------
     * Summary
     * ------------------------------------------------------------------ */
    if (overall_ok) {
        printf("=== Installation complete ===\n");
        printf("You may need to restart your terminal for all changes to "
               "take effect.\n");
    } else {
        printf("=== Installation finished with warnings ===\n");
        printf("Some steps could not be completed automatically.\n"
               "Please review the messages above and follow any manual "
               "instructions provided.\n");
    }

    return overall_ok ? 0 : 1;
}