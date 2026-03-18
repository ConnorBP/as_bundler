#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H

#include "../common/common.h"

/* -------------------------------------------------------------------------
 * Directory helpers
 * ---------------------------------------------------------------------- */

/* Create directory (and any missing parents) if it does not already exist. */
void ensure_dir(const char *dir);

/* -------------------------------------------------------------------------
 * C preprocessor execution
 *
 * Writes combined content to a temp file under .build_cache/, runs
 * gcc -E -P -x c with any -D flags and -I source dir, then reads and
 * returns the preprocessed result as a new malloc'd string.
 * Exits the process on failure.
 * ---------------------------------------------------------------------- */

char *run_preprocessor(const char *content);

/* -------------------------------------------------------------------------
 * Pragma-marker conversion
 *
 * Converts every:
 *   #pragma BUNDLER_FILE <path>
 * line into:
 *   // === <path> ===
 *
 * Called AFTER preprocessing (and after strip_comments) so the pragma
 * markers survive both steps.
 *
 * Takes ownership of content (frees it). Returns new malloc'd string.
 * ---------------------------------------------------------------------- */

char *convert_pragma_markers(char *content);

/* -------------------------------------------------------------------------
 * #include_lib protection
 *
 * protect_lib_includes  – called BEFORE run_preprocessor.
 *   Converts  #include_lib "x"  →  // @@BUNDLER_LIB@@ "x"
 *   so the C preprocessor ignores the unknown directive.
 *
 * restore_lib_includes  – called AFTER run_preprocessor (and after
 *   strip_comments, so the marker comment survives that pass too).
 *   Converts  // @@BUNDLER_LIB@@ "x"  →  #include_lib "x"
 *
 * Both take ownership of content (free it) and return a new malloc'd string.
 * ---------------------------------------------------------------------- */

char *protect_lib_includes(char *content);
char *restore_lib_includes(char *content);

#endif /* PREPROCESSOR_H */