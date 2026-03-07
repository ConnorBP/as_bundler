#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>

#ifdef _WIN32
#include <direct.h>
#include <io.h>
#include <windows.h>
#include <sys/stat.h>
#define stat _stat
#define S_ISDIR(m) (((m) & _S_IFMT) == _S_IFDIR)
#define PATH_SEPARATOR '\\'
#define PATH_SEPARATOR_STR "\\"
#else
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#define PATH_SEPARATOR '/'
#define PATH_SEPARATOR_STR "/"
#endif

#ifdef _WIN32
static inline char *portable_realpath(const char *path, char *resolved_path) {
    if (resolved_path) {
        return _fullpath(resolved_path, path, 512);
    } else {
        char *buffer = (char *)malloc(512);
        if (!buffer) return NULL;
        char *result = _fullpath(buffer, path, 512);
        if (!result) {
            free(buffer);
            return NULL;
        }
        return buffer;
    }
}
#define realpath portable_realpath
#endif

/* Undefine any system MAX_PATH before defining our own */
#ifdef MAX_PATH
#undef MAX_PATH
#endif

#define MAX_PATH     512
#define MAX_FILES    1024
#define MAX_INCLUDES 128
#define MAX_LINE     2048

/* -------------------------------------------------------------------------
 * Core data types
 * ---------------------------------------------------------------------- */

typedef struct {
    char   path[MAX_PATH];
    char  *content;
    size_t size;
    int    processed;
    int    processing;
} SourceFile;

typedef struct {
    SourceFile *files;
    int         count;
    int         capacity;
} FileList;

typedef struct {
    char **paths;
    int    count;
} ProcessedOrder;

/* -------------------------------------------------------------------------
 * Global configuration – defined in common.c, declared here
 * ---------------------------------------------------------------------- */

extern int   g_verbose;
extern int   g_strip_comments;
extern int   g_skip_preprocess;
extern int   g_obfuscate;
extern int   g_remove_newlines;
extern int   g_scramble_strings;
extern char  g_input_dir[MAX_PATH];
extern char  g_prepend_file[MAX_PATH];
extern char  g_header_file[MAX_PATH];
extern char **g_defines;
extern int   g_define_count;
extern int   g_define_capacity;

/* Global file list and processing order */
extern FileList       g_files;
extern ProcessedOrder g_order;

#endif /* COMMON_H */