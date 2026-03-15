#ifndef ERRORS_H
#define ERRORS_H

#include "../common/common.h"

/* -------------------------------------------------------------------------
 * File index – maps a range of lines in the bundled output back to the
 * original source file they came from.
 * ---------------------------------------------------------------------- */

typedef struct {
    SourceFile *file;
    char        rel_path[MAX_PATH];
    int         start_line;
    int         end_line;
} FileIndex;

/* -------------------------------------------------------------------------
 * Error-mapping helpers
 * ---------------------------------------------------------------------- */

/* Copy at most max_len-1 characters of the given 1-based line number from
 * content into output (NUL-terminated). */
void get_line_at(const char *content, int line_num, char *output,
                 size_t max_len);

/* Strip leading whitespace from str in-place. */
void trim_leading_ws(char *str);

/* Attempt to map error_bundled_line (a line number inside the combined
 * bundled output) back to the exact line number inside file->content.
 *
 * Uses a sliding context window of surrounding lines to find a unique
 * match inside the original source.
 *
 * Returns the 1-based line number in the original file, or 0 if the
 * mapping could not be determined. */
int find_line_in_source(SourceFile *file, const char *bundled_content,
                        int error_bundled_line, FileIndex *file_index);

#endif /* ERRORS_H */