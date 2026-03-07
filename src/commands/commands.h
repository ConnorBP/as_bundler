#ifndef COMMANDS_H
#define COMMANDS_H

#include "../common/common.h"

/* -------------------------------------------------------------------------
 * Subcommand entry points
 * ---------------------------------------------------------------------- */

/* restore <bundled.as>
 * Splits a bundled .as file back into individual source files using the
 * // === <path> === section markers written by the bundler. */
int cmd_restore(int argc, char **argv);

/* install
 * Installs gcc (via official methods) and registers the bundler executable
 * in the system PATH so it can be called from anywhere. */
int cmd_install(int argc, char **argv);

/* update
 * Ensures gcc is installed and the executable is registered in PATH, then
 * fetches the latest binary from the GitHub releases page and replaces the
 * current executable. */
int cmd_update(int argc, char **argv);

/* watch <bundler options> <source_dir|file.as ...>
 * Monitors the given source files / directories for changes and re-runs the
 * bundle command automatically whenever a save is detected, similar to how
 * `npm run dev` works with hot reloading. */
int cmd_watch(int argc, char **argv);

/* -------------------------------------------------------------------------
 * Shared helpers
 * ---------------------------------------------------------------------- */

/* Returns 1 if the bundler executable directory is already on PATH,
 * 0 otherwise. */
int is_registered_in_path(void);

/* Adds the directory that contains the running executable to the system PATH
 * permanently (registry on Windows, /etc/profile.d on Linux).
 * Returns 0 on success, non-zero on failure. */
int register_in_path(void);

/* Returns 1 if gcc can be found / executed, 0 otherwise. */
int is_gcc_available(void);

/* Installs gcc using the platform's official package manager or installer.
 * Returns 0 on success, non-zero on failure. */
int install_gcc(void);

/* Writes the full path of the running executable into `out` (size MAX_PATH).
 * Returns 0 on success, non-zero on failure. */
int get_executable_path(char *out);

/* Writes the directory that contains the running executable into `out`.
 * Returns 0 on success, non-zero on failure. */
int get_executable_dir(char *out);

#endif /* COMMANDS_H */