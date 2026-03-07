#include "commands.h"

/* -------------------------------------------------------------------------
 * cmd_install
 *
 * Usage:  pcx install
 *
 * Performs two tasks:
 *   1. Installs gcc if it is not already available.
 *   2. Copies the pcx executable to the platform install directory so it
 *      can be called from anywhere without specifying the full path.
 *
 * Platform behaviour:
 *   Windows – Copies the binary to C:\bin\<name> (creates C:\bin if needed)
 *             and adds C:\bin to the system PATH via the registry.
 *             gcc is installed via winget (MSYS2 toolchain).
 *   Linux   – Copies the binary to /usr/local/bin/<name> (already on PATH
 *             on all mainstream distributions; requires sudo if needed).
 *             gcc is installed via the distro's package manager.
 * ---------------------------------------------------------------------- */

static void print_install_help(void) {
    printf("Usage: pcx install\n"
           "\n"
           "  Installs gcc (if not already present) and copies the pcx binary\n"
           "  to a system-wide location so it can be invoked from any directory.\n"
           "\n"
           "  No arguments are required.\n"
           "\n"
           "  On Windows the binary is copied to C:\\bin\\<name> and C:\\bin is\n"
           "  added to the system PATH via the registry. Run as administrator\n"
           "  for a system-wide PATH change, otherwise HKCU is used.\n"
           "\n"
           "  On Linux the binary is copied to /usr/local/bin/<name> which is\n"
           "  already on PATH on all mainstream distributions. Run with sudo\n"
           "  if the copy fails due to insufficient permissions.\n");
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
     * Step 2: Install pcx to system bin directory
     * ------------------------------------------------------------------ */
    printf("=== Installing pcx ===\n");

    if (is_registered_in_path()) {
        printf("pcx is already installed.\n");
    } else {
        if (register_in_path() != 0) {
            fprintf(stderr, "Warning: Could not install pcx "
                            "(see above for details).\n");
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