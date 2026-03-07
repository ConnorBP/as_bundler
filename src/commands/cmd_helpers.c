#include "commands.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <limits.h>
#include <sys/stat.h>
#endif

/* -------------------------------------------------------------------------
 * Install directories
 *
 * Windows : C:\bin\<exe_name>   (C:\bin is a conventional user-managed bin
 *           directory that is easy to add to PATH once and forget)
 * Linux   : /usr/local/bin/<exe_name>  (standard system-wide location,
 *           already on PATH on every mainstream distribution)
 * ---------------------------------------------------------------------- */
#ifdef _WIN32
#  define INSTALL_DIR   "C:\\bin"
#  define INSTALL_DIR_DISPLAY "C:\\bin"
#else
#  define INSTALL_DIR   "/usr/local/bin"
#  define INSTALL_DIR_DISPLAY "/usr/local/bin"
#endif

/* -------------------------------------------------------------------------
 * get_executable_path
 * Writes the full path of the running executable into `out` (MAX_PATH bytes).
 * Returns 0 on success, 1 on failure.
 * ---------------------------------------------------------------------- */
int get_executable_path(char *out) {
#ifdef _WIN32
    DWORD len = GetModuleFileNameA(NULL, out, MAX_PATH);
    if (len == 0 || len >= MAX_PATH)
        return 1;
    return 0;
#else
    ssize_t len = readlink("/proc/self/exe", out, MAX_PATH - 1);
    if (len < 0) {
        /* Fallback: FreeBSD */
        len = readlink("/proc/curproc/file", out, MAX_PATH - 1);
    }
    if (len < 0)
        return 1;
    out[len] = '\0';
    return 0;
#endif
}

/* -------------------------------------------------------------------------
 * get_executable_dir
 * Writes the directory that contains the running executable into `out`.
 * Returns 0 on success, 1 on failure.
 * ---------------------------------------------------------------------- */
int get_executable_dir(char *out) {
    char exe_path[MAX_PATH];
    if (get_executable_path(exe_path) != 0)
        return 1;

    strncpy(out, exe_path, MAX_PATH - 1);
    out[MAX_PATH - 1] = '\0';

    char *last = NULL;
    for (char *p = out; *p; p++)
        if (*p == '/' || *p == '\\')
            last = p;

    if (last)
        *last = '\0';
    else
        strcpy(out, ".");

    return 0;
}

/* -------------------------------------------------------------------------
 * get_exe_basename
 * Extracts just the filename (with extension) from a full path into `out`.
 * ---------------------------------------------------------------------- */
static void get_exe_basename(const char *exe_path, char *out) {
    const char *last = exe_path;
    for (const char *p = exe_path; *p; p++)
        if (*p == '/' || *p == '\\')
            last = p + 1;
    strncpy(out, last, MAX_PATH - 1);
    out[MAX_PATH - 1] = '\0';
}

/* -------------------------------------------------------------------------
 * copy_file
 * Copies the file at `src` to `dst`, overwriting if it already exists.
 * Returns 0 on success, 1 on failure.
 * ---------------------------------------------------------------------- */
static int copy_file(const char *src, const char *dst) {
#ifdef _WIN32
    if (!CopyFileA(src, dst, FALSE)) {
        fprintf(stderr, "Error: Could not copy '%s' to '%s' (code %lu)\n",
                src, dst, (unsigned long)GetLastError());
        return 1;
    }
    return 0;
#else
    FILE *in = fopen(src, "rb");
    if (!in) {
        fprintf(stderr, "Error: Could not open source '%s': ", src);
        perror(NULL);
        return 1;
    }

    FILE *out = fopen(dst, "wb");
    if (!out) {
        fprintf(stderr, "Error: Could not open destination '%s': ", dst);
        perror(NULL);
        fclose(in);
        return 1;
    }

    char buf[65536];
    size_t n;
    int ok = 1;
    while ((n = fread(buf, 1, sizeof(buf), in)) > 0) {
        if (fwrite(buf, 1, n, out) != n) {
            fprintf(stderr, "Error: Write failed to '%s'\n", dst);
            ok = 0;
            break;
        }
    }

    fclose(in);
    fclose(out);

    if (!ok) return 1;

    /* Make the installed binary executable */
    if (chmod(dst, 0755) != 0) {
        fprintf(stderr, "Warning: Could not set executable bit on '%s'\n", dst);
        /* Non-fatal */
    }

    return 0;
#endif
}

/* -------------------------------------------------------------------------
 * get_install_path
 * Builds the full destination path: <INSTALL_DIR>/<exe_basename>
 * ---------------------------------------------------------------------- */
void get_install_path(char *out) {
    char exe_path[MAX_PATH];
    char basename[MAX_PATH];

    if (get_executable_path(exe_path) != 0) {
        /* Fallback name */
#ifdef _WIN32
        snprintf(out, MAX_PATH, "%s\\pcx.exe", INSTALL_DIR);
#else
        snprintf(out, MAX_PATH, "%s/pcx", INSTALL_DIR);
#endif
        return;
    }

    get_exe_basename(exe_path, basename);

#ifdef _WIN32
    snprintf(out, MAX_PATH, "%s\\%s", INSTALL_DIR, basename);
#else
    snprintf(out, MAX_PATH, "%s/%s", INSTALL_DIR, basename);
#endif
}

/* -------------------------------------------------------------------------
 * is_registered_in_path
 * Returns 1 if the binary has already been copied to the install directory.
 * ---------------------------------------------------------------------- */
int is_registered_in_path(void) {
    char install_path[MAX_PATH];
    get_install_path(install_path);

#ifdef _WIN32
    DWORD attrs = GetFileAttributesA(install_path);
    return (attrs != INVALID_FILE_ATTRIBUTES &&
            !(attrs & FILE_ATTRIBUTE_DIRECTORY)) ? 1 : 0;
#else
    return (access(install_path, F_OK) == 0) ? 1 : 0;
#endif
}

/* -------------------------------------------------------------------------
 * register_in_path
 *
 * Windows : Creates C:\bin\ if needed, then copies the running executable
 *           there.  Also adds C:\bin to the user PATH via the registry if
 *           it isn't already present, since C:\bin is not a default PATH
 *           entry on Windows.
 * Linux   : Copies the running executable to /usr/local/bin/<name> (which
 *           is already on PATH on all mainstream distributions).  Falls back
 *           with an error message if the copy fails due to permissions.
 *
 * Returns 0 on success, non-zero on failure.
 * ---------------------------------------------------------------------- */
int register_in_path(void) {
    char exe_path[MAX_PATH];
    if (get_executable_path(exe_path) != 0) {
        fprintf(stderr, "Error: Could not determine the current executable path.\n");
        return 1;
    }

    char install_path[MAX_PATH];
    get_install_path(install_path);

#ifdef _WIN32
    /* ---------------------------------------------------------------
     * Create C:\bin if it doesn't exist yet
     * ------------------------------------------------------------- */
    DWORD dir_attrs = GetFileAttributesA(INSTALL_DIR);
    if (dir_attrs == INVALID_FILE_ATTRIBUTES) {
        if (!CreateDirectoryA(INSTALL_DIR, NULL)) {
            fprintf(stderr,
                    "Error: Could not create install directory '%s' (code %lu).\n"
                    "Try running as administrator.\n",
                    INSTALL_DIR, (unsigned long)GetLastError());
            return 1;
        }
        printf("Created directory: %s\n", INSTALL_DIR);
    }

    /* ---------------------------------------------------------------
     * Copy the binary
     * ------------------------------------------------------------- */
    printf("Installing to: %s\n", install_path);
    if (copy_file(exe_path, install_path) != 0)
        return 1;
    printf("Installed successfully.\n");

    /* ---------------------------------------------------------------
     * Ensure C:\bin is on the user PATH in the registry.
     * C:\bin is not a default Windows PATH entry so we add it once.
     * ------------------------------------------------------------- */
    const char *reg_key =
        "SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment";
    HKEY  hkey = NULL;
    LONG  res;
    DWORD dtype = REG_EXPAND_SZ;

    /* Try HKLM (system-wide, requires admin) first */
    res = RegOpenKeyExA(HKEY_LOCAL_MACHINE, reg_key, 0,
                        KEY_READ | KEY_WRITE, &hkey);
    if (res != ERROR_SUCCESS) {
        /* Fall back to HKCU (current user only) */
        res = RegOpenKeyExA(HKEY_CURRENT_USER, "Environment", 0,
                            KEY_READ | KEY_WRITE, &hkey);
        if (res != ERROR_SUCCESS) {
            fprintf(stderr,
                    "Warning: Could not open registry PATH key (code %ld).\n"
                    "Please add '%s' to your PATH manually.\n",
                    res, INSTALL_DIR);
            /* Binary is still copied – non-fatal */
            return 0;
        }
    }

    /* Read existing PATH */
    char  old_path[8192] = {0};
    DWORD old_size = sizeof(old_path);
    DWORD old_type = REG_EXPAND_SZ;
    RegQueryValueExA(hkey, "Path", NULL, &old_type,
                     (LPBYTE)old_path, &old_size);

    /* Check whether C:\bin is already present (case-insensitive) */
    char check[8192];
    strncpy(check, old_path, sizeof(check) - 1);
    check[sizeof(check) - 1] = '\0';
    for (char *p = check; *p; p++) *p = (char)tolower((unsigned char)*p);

    char lower_dir[] = "c:\\bin";  /* INSTALL_DIR lowercased */

    if (!strstr(check, lower_dir)) {
        char new_path[8192];
        size_t old_len = strlen(old_path);
        if (old_len > 0 && old_path[old_len - 1] != ';')
            snprintf(new_path, sizeof(new_path), "%s;%s", old_path, INSTALL_DIR);
        else
            snprintf(new_path, sizeof(new_path), "%s%s", old_path, INSTALL_DIR);

        res = RegSetValueExA(hkey, "Path", 0, dtype,
                             (const BYTE *)new_path,
                             (DWORD)(strlen(new_path) + 1));
        if (res == ERROR_SUCCESS) {
            /* Notify open shells */
            DWORD_PTR notify_result = 0;
            SendMessageTimeoutA(HWND_BROADCAST, WM_SETTINGCHANGE, 0,
                                (LPARAM)"Environment", SMTO_ABORTIFHUNG,
                                5000, &notify_result);
            printf("Added '%s' to system PATH.\n", INSTALL_DIR);
        } else {
            fprintf(stderr,
                    "Warning: Could not write PATH to registry (code %ld).\n"
                    "Please add '%s' to your PATH manually.\n",
                    res, INSTALL_DIR);
        }
    } else {
        printf("'%s' is already in PATH.\n", INSTALL_DIR);
    }

    RegCloseKey(hkey);
    printf("Note: You may need to restart your terminal for PATH changes to take effect.\n");
    return 0;

#else
    /* ---------------------------------------------------------------
     * Linux: copy to /usr/local/bin (already on PATH everywhere)
     * ------------------------------------------------------------- */
    printf("Installing to: %s\n", install_path);

    if (copy_file(exe_path, install_path) != 0) {
        fprintf(stderr,
                "Note: If the copy failed due to permissions, try running with sudo:\n"
                "  sudo pcx install\n");
        return 1;
    }

    printf("Installed successfully.\n");
    printf("The binary is now available as a system command.\n");
    return 0;
#endif
}

/* -------------------------------------------------------------------------
 * is_gcc_available
 * Returns 1 if `gcc --version` exits successfully, 0 otherwise.
 * ---------------------------------------------------------------------- */
int is_gcc_available(void) {
#ifdef _WIN32
    int ret = system("gcc --version >nul 2>&1");
#else
    int ret = system("gcc --version >/dev/null 2>&1");
#endif
    return ret == 0 ? 1 : 0;
}

/* -------------------------------------------------------------------------
 * install_gcc
 * Installs gcc using the platform's official package manager / installer.
 *
 * Windows : Uses winget to install the MSYS2 toolchain (official Microsoft
 *           package manager, ships with Windows 10 1709+ / Windows 11).
 * Linux   : Uses the distro's package manager (apt-get, dnf, yum, pacman,
 *           zypper, or apk), whichever is found first.
 *
 * Returns 0 on success, non-zero on failure.
 * ---------------------------------------------------------------------- */
int install_gcc(void) {
    printf("Installing gcc...\n");

#ifdef _WIN32
    printf("Attempting to install GCC via winget (MSYS2 toolchain)...\n");

    int ret = system("winget install --id MSYS2.MSYS2 --silent "
                     "--accept-package-agreements --accept-source-agreements");
    if (ret == 0) {
        printf("\nMSYS2 installed successfully.\n");
        printf("To complete GCC setup, open the MSYS2 terminal and run:\n");
        printf("  pacman -S --noconfirm mingw-w64-x86_64-gcc\n");
        printf("Then add C:\\msys64\\mingw64\\bin to your PATH.\n");
        return 0;
    }

    fprintf(stderr,
            "\nwinget is not available on this system.\n"
            "Please install GCC manually:\n"
            "\n"
            "  Option 1 - MSYS2 (recommended):\n"
            "    1. Download from https://www.msys2.org/\n"
            "    2. Install MSYS2, open the MSYS2 MinGW 64-bit terminal\n"
            "    3. Run: pacman -S --noconfirm mingw-w64-x86_64-gcc\n"
            "    4. Add C:\\msys64\\mingw64\\bin to your PATH\n"
            "\n"
            "  Option 2 - winlibs standalone:\n"
            "    Download from https://winlibs.com/ and extract to C:\\mingw64\n"
            "    then add C:\\mingw64\\bin to your PATH\n");
    return 1;

#else
    struct {
        const char *test;
        const char *cmd;
        const char *name;
    } managers[] = {
        { "apt-get --version >/dev/null 2>&1",
          "apt-get update && apt-get install -y gcc",
          "apt-get" },
        { "dnf --version >/dev/null 2>&1",
          "dnf install -y gcc",
          "dnf" },
        { "yum --version >/dev/null 2>&1",
          "yum install -y gcc",
          "yum" },
        { "pacman --version >/dev/null 2>&1",
          "pacman -S --noconfirm gcc",
          "pacman" },
        { "zypper --version >/dev/null 2>&1",
          "zypper install -y gcc",
          "zypper" },
        { "apk --version >/dev/null 2>&1",
          "apk add --no-cache gcc",
          "apk" },
        { NULL, NULL, NULL }
    };

    for (int i = 0; managers[i].test; i++) {
        if (system(managers[i].test) == 0) {
            printf("Using %s to install gcc...\n", managers[i].name);
            int ret = system(managers[i].cmd);
            if (ret != 0) {
                fprintf(stderr,
                        "Error: %s failed (exit code %d).\n"
                        "Try running manually with sudo:\n"
                        "  sudo %s\n",
                        managers[i].name, ret, managers[i].cmd);
                return 1;
            }
            printf("gcc installed successfully.\n");
            return 0;
        }
    }

    fprintf(stderr,
            "Error: No supported package manager found.\n"
            "Please install gcc manually using your distribution's package manager.\n");
    return 1;
#endif
}