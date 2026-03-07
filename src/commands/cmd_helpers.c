#include "commands.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <limits.h>
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
        /* Fallback: try /proc/curproc/file (FreeBSD) or argv[0] via realpath */
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

    /* Strip filename – find last separator */
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
 * is_registered_in_path
 * Returns 1 if the directory containing the executable is already in PATH.
 * ---------------------------------------------------------------------- */
int is_registered_in_path(void) {
    char exe_dir[MAX_PATH];
    if (get_executable_dir(exe_dir) != 0)
        return 0;

    const char *path_env = getenv("PATH");
    if (!path_env)
        return 0;

    /* Walk the PATH entries */
    char path_copy[4096];
    strncpy(path_copy, path_env, sizeof(path_copy) - 1);
    path_copy[sizeof(path_copy) - 1] = '\0';

#ifdef _WIN32
    const char *delim = ";";
#else
    const char *delim = ":";
#endif

    char *token = strtok(path_copy, delim);
    while (token) {
        /* Normalise trailing separator */
        char entry[MAX_PATH];
        strncpy(entry, token, MAX_PATH - 1);
        entry[MAX_PATH - 1] = '\0';
        size_t elen = strlen(entry);
        if (elen > 0 && (entry[elen - 1] == '/' || entry[elen - 1] == '\\'))
            entry[--elen] = '\0';

#ifdef _WIN32
        if (_stricmp(entry, exe_dir) == 0)
            return 1;
#else
        if (strcmp(entry, exe_dir) == 0)
            return 1;
#endif
        token = strtok(NULL, delim);
    }
    return 0;
}

/* -------------------------------------------------------------------------
 * register_in_path  (Windows)
 * Adds the executable directory to the SYSTEM PATH via the registry key
 * HKLM\SYSTEM\CurrentControlSet\Control\Session Manager\Environment.
 * Falls back to the USER PATH (HKCU) if the process lacks admin rights.
 * Broadcasts WM_SETTINGCHANGE so open shells pick up the change.
 * ---------------------------------------------------------------------- */
#ifdef _WIN32
static int register_in_path_windows(const char *exe_dir) {
    const char *reg_key =
        "SYSTEM\\CurrentControlSet\\Control\\Session Manager\\Environment";

    HKEY  hkey  = NULL;
    LONG  res;
    DWORD dtype = REG_EXPAND_SZ;

    /* Try HKLM first (requires admin) */
    res = RegOpenKeyExA(HKEY_LOCAL_MACHINE, reg_key, 0,
                        KEY_READ | KEY_WRITE, &hkey);
    if (res != ERROR_SUCCESS) {
        /* Fall back to HKCU */
        res = RegOpenKeyExA(HKEY_CURRENT_USER, "Environment", 0,
                            KEY_READ | KEY_WRITE, &hkey);
        if (res != ERROR_SUCCESS) {
            fprintf(stderr, "Error: Could not open registry PATH key (code %ld)\n",
                    res);
            return 1;
        }
        dtype = REG_EXPAND_SZ;
    }

    /* Read existing PATH value */
    char   old_path[8192] = {0};
    DWORD  old_size       = sizeof(old_path);
    DWORD  old_type       = REG_EXPAND_SZ;
    RegQueryValueExA(hkey, "Path", NULL, &old_type,
                     (LPBYTE)old_path, &old_size);

    /* Check if already present (case-insensitive on Windows) */
    char check[MAX_PATH];
    strncpy(check, old_path, MAX_PATH - 1);
    check[MAX_PATH - 1] = '\0';

    /* Simple substring search */
    char *p = check;
    while (*p) { *p = (char)tolower((unsigned char)*p); p++; }
    char lower_dir[MAX_PATH];
    strncpy(lower_dir, exe_dir, MAX_PATH - 1);
    lower_dir[MAX_PATH - 1] = '\0';
    for (char *q = lower_dir; *q; q++) *q = (char)tolower((unsigned char)*q);

    if (strstr(check, lower_dir)) {
        printf("Executable directory already in PATH.\n");
        RegCloseKey(hkey);
        return 0;
    }

    /* Append the new directory */
    char new_path[8192];
    size_t old_len = strlen(old_path);
    if (old_len > 0 && old_path[old_len - 1] != ';')
        snprintf(new_path, sizeof(new_path), "%s;%s", old_path, exe_dir);
    else
        snprintf(new_path, sizeof(new_path), "%s%s", old_path, exe_dir);

    res = RegSetValueExA(hkey, "Path", 0, dtype,
                         (const BYTE *)new_path,
                         (DWORD)(strlen(new_path) + 1));
    RegCloseKey(hkey);

    if (res != ERROR_SUCCESS) {
        fprintf(stderr,
                "Error: Could not write PATH to registry (code %ld)\n", res);
        return 1;
    }

    /* Notify the system */
    DWORD_PTR result = 0;
    SendMessageTimeoutA(HWND_BROADCAST, WM_SETTINGCHANGE, 0,
                        (LPARAM)"Environment", SMTO_ABORTIFHUNG, 5000,
                        &result);

    printf("Added to system PATH: %s\n", exe_dir);
    printf("Note: You may need to restart your terminal for PATH changes to take effect.\n");
    return 0;
}
#endif /* _WIN32 */

/* -------------------------------------------------------------------------
 * register_in_path  (Linux / macOS)
 * Writes an export line to /etc/profile.d/pcx.sh (system-wide, requires
 * root).  Falls back to ~/.bashrc and ~/.zshrc for the current user.
 * ---------------------------------------------------------------------- */
#ifndef _WIN32
static int register_in_path_linux(const char *exe_dir) {
    /* Try system-wide first */
    const char *system_profile = "/etc/profile.d/pcx.sh";
    FILE *f = fopen(system_profile, "a");
    if (f) {
        fprintf(f, "\nexport PATH=\"%s:$PATH\"\n", exe_dir);
        fclose(f);
        /* Make it executable */
        chmod(system_profile, 0644);
        printf("Added to system PATH via %s\n", system_profile);
        printf("Note: Restart your terminal or run: source %s\n",
               system_profile);
        return 0;
    }

    /* Fall back to user profiles */
    int written = 0;
    const char *home = getenv("HOME");
    if (!home) {
        fprintf(stderr, "Error: $HOME not set; cannot write shell profile\n");
        return 1;
    }

    const char *profiles[] = { ".bashrc", ".zshrc", ".profile", NULL };
    for (int i = 0; profiles[i]; i++) {
        char profile_path[MAX_PATH];
        snprintf(profile_path, MAX_PATH, "%s/%s", home, profiles[i]);

        /* Only append to files that already exist */
        FILE *pf = fopen(profile_path, "r");
        if (!pf) continue;
        fclose(pf);

        pf = fopen(profile_path, "a");
        if (!pf) continue;
        fprintf(pf, "\n# Added by pcx installer\nexport PATH=\"%s:$PATH\"\n",
                exe_dir);
        fclose(pf);
        printf("Added to PATH via ~/%s\n", profiles[i]);
        written = 1;
    }

    if (!written) {
        fprintf(stderr,
                "Error: Could not write to any shell profile.\n"
                "Please add the following line to your shell profile manually:\n"
                "  export PATH=\"%s:$PATH\"\n", exe_dir);
        return 1;
    }

    printf("Note: Restart your terminal or source your shell profile for the change to take effect.\n");
    return 0;
}
#endif /* !_WIN32 */

int register_in_path(void) {
    char exe_dir[MAX_PATH];
    if (get_executable_dir(exe_dir) != 0) {
        fprintf(stderr, "Error: Could not determine executable directory\n");
        return 1;
    }

    if (is_registered_in_path()) {
        printf("Executable directory is already in PATH.\n");
        return 0;
    }

#ifdef _WIN32
    return register_in_path_windows(exe_dir);
#else
    return register_in_path_linux(exe_dir);
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
 * Windows : Downloads and installs the latest winlibs GCC standalone build
 *           (MinGW-w64 / MSVCRT) via PowerShell + the official winlibs
 *           archive – no external package manager required.
 * Linux   : Uses the distribution's package manager (apt, dnf, pacman, zypper
 *           or apk), whichever is found first.
 *
 * Returns 0 on success, non-zero on failure.
 * ---------------------------------------------------------------------- */
int install_gcc(void) {
    printf("Installing gcc...\n");

#ifdef _WIN32
    /*
     * Use winget (available on Windows 10 1709+ / Windows 11) to install
     * the official MSYS2 toolchain, which provides up-to-date GCC.
     * winget is the official Microsoft package manager shipped with Windows.
     */
    printf("Attempting to install GCC via winget (MSYS2 toolchain)...\n");

    int ret = system("winget install --id MSYS2.MSYS2 --silent --accept-package-agreements --accept-source-agreements");
    if (ret == 0) {
        printf("\nMSYS2 installed successfully.\n");
        printf("To complete GCC setup, open the MSYS2 terminal and run:\n");
        printf("  pacman -S --noconfirm mingw-w64-x86_64-gcc\n");
        printf("Then add C:\\msys64\\mingw64\\bin to your PATH.\n");
        return 0;
    }

    /* winget not available – print manual instructions */
    fprintf(stderr,
            "\nwinget is not available on this system.\n"
            "Please install GCC manually:\n"
            "\n"
            "  Option 1 – MSYS2 (recommended):\n"
            "    1. Download from https://www.msys2.org/\n"
            "    2. Install MSYS2, open the MSYS2 MinGW 64-bit terminal\n"
            "    3. Run: pacman -S --noconfirm mingw-w64-x86_64-gcc\n"
            "    4. Add C:\\msys64\\mingw64\\bin to your PATH\n"
            "\n"
            "  Option 2 – winlibs standalone:\n"
            "    Download from https://winlibs.com/ and extract to C:\\mingw64\n"
            "    then add C:\\mingw64\\bin to your PATH\n");
    return 1;

#else
    /* Try common Linux package managers in order of popularity */
    struct { const char *test; const char *cmd; const char *name; } managers[] = {
        /* Debian / Ubuntu */
        { "apt-get --version >/dev/null 2>&1",
          "apt-get update && apt-get install -y gcc",
          "apt-get" },
        /* Fedora / RHEL / CentOS */
        { "dnf --version >/dev/null 2>&1",
          "dnf install -y gcc",
          "dnf" },
        /* CentOS 7 / older RHEL */
        { "yum --version >/dev/null 2>&1",
          "yum install -y gcc",
          "yum" },
        /* Arch Linux */
        { "pacman --version >/dev/null 2>&1",
          "pacman -S --noconfirm gcc",
          "pacman" },
        /* openSUSE */
        { "zypper --version >/dev/null 2>&1",
          "zypper install -y gcc",
          "zypper" },
        /* Alpine */
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