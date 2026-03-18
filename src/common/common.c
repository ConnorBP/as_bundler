#include "common.h"

/* Global configuration flags */
int  g_verbose          = 0;
int  g_strip_comments   = 0;
int  g_skip_preprocess  = 0;
int  g_obfuscate        = 0;
int  g_remove_newlines  = 0;
int  g_scramble_strings = 0;

/* Global path configuration */
char g_input_dir[MAX_PATH]    = {0};
char g_prepend_file[MAX_PATH] = {0};
char g_header_file[MAX_PATH]  = {0};

/* Preprocessor defines (-D flags) */
char **g_defines        = NULL;
int    g_define_count   = 0;
int    g_define_capacity = 0;

/* Global file list and processing order */
FileList       g_files = {0};
ProcessedOrder g_order = {0};

/* Stub system configuration */
char g_stub_paths[MAX_STUB_DIRS][MAX_PATH] = {{0}};
int  g_stub_path_count  = 0;
char g_stub_dirs[MAX_STUB_DIRS][MAX_PATH]  = {{0}};
int  g_stub_dir_count   = 0;
int  g_emit_stub        = 0;

/* Obfuscation protection configuration */
char *g_protect_names[MAX_PROTECT_NAMES] = {NULL};
int   g_protect_name_count       = 0;
int   g_protect_string_literals  = 0;

/* Symbol map configuration */
char g_symbol_map_path[MAX_PATH]  = {0};
char g_symbol_map_format[16]      = "text";
int  g_write_symbol_map           = 0;