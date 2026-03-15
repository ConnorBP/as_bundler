#ifndef FS_H
#define FS_H

#include "../common/common.h"

/* -------------------------------------------------------------------------
 * Path helpers
 * ---------------------------------------------------------------------- */

int  is_directory(const char *path);
int  ends_with_as(const char *filename);

void get_dirname(const char *path, char *out);
void get_basename(const char *path, char *out);
void get_relative_path(const char *file_path, char *out);
void path_join(const char *base, const char *rel, char *out);
void normalize_path(const char *input, char *out);
void resolve_include_path(const char *base_dir, const char *rel_path, char *out);

/* -------------------------------------------------------------------------
 * File list management
 * ---------------------------------------------------------------------- */

SourceFile *find_file(const char *path);
SourceFile *find_file_by_section(const char *section);

char *read_file_content(const char *path, size_t *size);

void add_file(const char *path);
void scan_directory(const char *dir_path);

/* -------------------------------------------------------------------------
 * Dependency resolution
 * ---------------------------------------------------------------------- */

void extract_includes(SourceFile *file, char includes[][MAX_PATH],
                      int *include_count);
void process_file(SourceFile *file, const char *chain[], int chain_len);

#endif /* FS_H */