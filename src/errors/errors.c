#include "errors.h"

/* -------------------------------------------------------------------------
 * get_line_at
 * ---------------------------------------------------------------------- */

void get_line_at(const char *content, int line_num, char *output,
                 size_t max_len) {
    int         current = 1;
    const char *p       = content;
    output[0] = '\0';

    while (*p && current < line_num) {
        if (*p == '\n') current++;
        p++;
    }

    if (current == line_num) {
        const char *line_start = p;
        while (*p && *p != '\n') p++;
        size_t len = (size_t)(p - line_start);
        if (len < max_len - 1) {
            strncpy(output, line_start, len);
            output[len] = '\0';
        }
    }
}

/* -------------------------------------------------------------------------
 * trim_leading_ws
 * ---------------------------------------------------------------------- */

void trim_leading_ws(char *str) {
    char *start = str;
    while (*start == ' ' || *start == '\t') start++;
    if (start != str) memmove(str, start, strlen(start) + 1);
}

/* -------------------------------------------------------------------------
 * find_line_in_source
 * ---------------------------------------------------------------------- */

int find_line_in_source(SourceFile *file, const char *bundled_content,
                        int error_bundled_line, FileIndex *file_index) {
    int   max_context          = 50;
    int   context_line_count   = 0;
    int  *context_line_offsets = (int *)malloc((size_t)(max_context * 2) * sizeof(int));
    char **context_lines       = (char **)malloc((size_t)(max_context * 2) * sizeof(char *));
    int   bundled_offset       = error_bundled_line - 1;

    for (int radius = 0;
         radius < max_context && context_line_count < max_context * 2;
         radius++) {

        if (radius > 0) {
            int above_line = error_bundled_line - radius;
            if (above_line >= file_index->start_line) {
                char line[MAX_LINE];
                get_line_at(bundled_content, above_line, line, sizeof(line));
                trim_leading_ws(line);
                if (strlen(line) > 0) {
                    context_lines[context_line_count]        = strdup(line);
                    context_line_offsets[context_line_count] =
                        above_line - file_index->start_line + 1;
                    context_line_count++;
                }
            }
        }

        int below_line = error_bundled_line + radius;
        if (below_line < file_index->end_line) {
            char line[MAX_LINE];
            get_line_at(bundled_content, below_line, line, sizeof(line));
            trim_leading_ws(line);
            if (strlen(line) > 0) {
                context_lines[context_line_count]        = strdup(line);
                context_line_offsets[context_line_count] =
                    below_line - file_index->start_line + 1;
                context_line_count++;
            }
        }

        for (int i = 0; i < context_line_count; i++) {
            if (strlen(context_lines[i]) == 0) continue;

            int         occurrences = 0;
            int         found_line  = 0;
            int         current_line = 1;
            const char *p           = file->content;

            while (*p) {
                const char *line_start = p;
                while (*p && *p != '\n') p++;

                char   source_line[MAX_LINE];
                size_t len = (size_t)(p - line_start);
                if (len < sizeof(source_line) - 1) {
                    strncpy(source_line, line_start, len);
                    source_line[len] = '\0';
                    trim_leading_ws(source_line);
                    if (strcmp(source_line, context_lines[i]) == 0) {
                        if (occurrences == 0) found_line = current_line;
                        occurrences++;
                    }
                }
                current_line++;
                if (*p == '\n') p++;
            }

            if (occurrences == 1) {
                int error_offset_in_context = -1;
                for (int j = 0; j < context_line_count; j++) {
                    if (context_line_offsets[j] ==
                        bundled_offset - file_index->start_line + 1) {
                        error_offset_in_context = j;
                        break;
                    }
                }
                if (error_offset_in_context >= 0 && i < context_line_count) {
                    int error_line = found_line + (error_offset_in_context - i);
                    for (int k = 0; k < context_line_count; k++)
                        free(context_lines[k]);
                    free(context_line_offsets);
                    free(context_lines);
                    return error_line;
                }
            }
        }
    }

    for (int k = 0; k < context_line_count; k++) free(context_lines[k]);
    free(context_line_offsets);
    free(context_lines);
    return 0;
}