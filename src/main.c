#include "as_wrapper.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
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
// Windows replacement for realpath
static char *portable_realpath(const char *path, char *resolved_path) {
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

// Undefine any system MAX_PATH before defining our own
#ifdef MAX_PATH
#undef MAX_PATH
#endif

#define MAX_PATH 512
#define MAX_FILES 1024
#define MAX_INCLUDES 128
#define MAX_LINE 2048

typedef struct {
  char path[MAX_PATH];
  char *content;
  size_t size;
  int processed;
  int processing;
} SourceFile;

typedef struct {
  SourceFile *files;
  int count;
  int capacity;
} FileList;

typedef struct {
  char **paths;
  int count;
} ProcessedOrder;

// Global configuration
static int g_verbose = 0;          // -o flag set
static int g_strip_comments = 0;   // --strip flag
static int g_skip_preprocess = 0;  // --no-preprocess / -n flag
static char g_input_dir[MAX_PATH] = {0};
static char g_prepend_file[MAX_PATH] = {0};  // --prepend / -p
static char g_header_file[MAX_PATH] = {0};   // --header / -H
static char **g_defines = NULL;
static int g_define_count = 0;
static int g_define_capacity = 0;

// Global file list
static FileList g_files = {0};
static ProcessedOrder g_order = {0};

// ---------------------------------------------------------------------------
// Path helpers
// ---------------------------------------------------------------------------

int is_directory(const char *path) {
  struct stat st;
  if (stat(path, &st) != 0)
    return 0;
  return S_ISDIR(st.st_mode);
}

int ends_with_as(const char *filename) {
  size_t len = strlen(filename);
  return len > 3 && strcmp(filename + len - 3, ".as") == 0;
}

void get_dirname(const char *path, char *out) {
  strcpy(out, path);
  char *last_slash = strrchr(out, '/');
  char *last_backslash = strrchr(out, '\\');
  char *separator = last_slash > last_backslash ? last_slash : last_backslash;
  if (separator) {
    *separator = '\0';
  } else {
    strcpy(out, ".");
  }
}

void get_basename(const char *path, char *out) {
  const char *forward_slash = strrchr(path, '/');
  const char *backslash = strrchr(path, '\\');
  const char *filename = forward_slash > backslash ? forward_slash : backslash;
  if (filename) {
    filename++;
  } else {
    filename = path;
  }
  strcpy(out, filename);
  size_t len = strlen(out);
  if (len > 3 && strcmp(out + len - 3, ".as") == 0) {
    out[len - 3] = '\0';
  }
}

void get_relative_path(const char *file_path, char *out) {
  if (g_input_dir[0] == '\0') {
    get_basename(file_path, out);
    return;
  }
  size_t input_len = strlen(g_input_dir);
  if (strncmp(file_path, g_input_dir, input_len) == 0) {
    const char *rel_start = file_path + input_len;
    if (*rel_start == '/' || *rel_start == '\\')
      rel_start++;
    strcpy(out, rel_start);
  } else {
    get_basename(file_path, out);
  }
}

void path_join(const char *base, const char *rel, char *out) {
  while (rel[0] == '/' && base[strlen(base) - 1] == '/') {
    rel++;
  }
  snprintf(out, MAX_PATH, "%s/%s", base, rel);
}

void normalize_path(const char *input, char *out) {
  char temp[MAX_PATH];
  strcpy(temp, input);
  for (char *p = temp; *p; p++) {
    if (*p == '\\') *p = '/';
  }
  char *parts[256];
  int part_count = 0;
  char *token = strtok(temp, "/");
  while (token != NULL && part_count < 256) {
    if (strcmp(token, ".") == 0) {
    } else if (strcmp(token, "..") == 0) {
      if (part_count > 0) part_count--;
    } else {
      parts[part_count++] = strdup(token);
    }
    token = strtok(NULL, "/");
  }
  out[0] = '\0';
  if (input[0] == '/' || input[0] == '\\') {
    strcat(out, "/");
  }
  for (int i = 0; i < part_count; i++) {
    if (i > 0) strcat(out, "/");
    strcat(out, parts[i]);
    free(parts[i]);
  }
}

void resolve_include_path(const char *base_dir, const char *rel_path, char *out) {
  char combined[MAX_PATH];
  char normalized[MAX_PATH];
  if (rel_path[0] == '/') {
    strcpy(out, rel_path);
    return;
  }
  path_join(base_dir, rel_path, combined);
  normalize_path(combined, normalized);
  char *resolved = realpath(normalized, NULL);
  if (resolved) {
    strncpy(out, resolved, MAX_PATH - 1);
    out[MAX_PATH - 1] = '\0';
    free(resolved);
  } else {
    strcpy(out, normalized);
  }
}

// ---------------------------------------------------------------------------
// File list management
// ---------------------------------------------------------------------------

SourceFile *find_file(const char *path) {
  for (int i = 0; i < g_files.count; i++) {
    if (strcmp(g_files.files[i].path, path) == 0)
      return &g_files.files[i];
  }
  return NULL;
}

SourceFile *find_file_by_section(const char *section) {
  for (int i = 0; i < g_files.count; i++) {
    char rel_path[MAX_PATH];
    get_relative_path(g_files.files[i].path, rel_path);
    size_t len = strlen(rel_path);
    if (len > 3 && strcmp(rel_path + len - 3, ".as") == 0)
      rel_path[len - 3] = '\0';
    if (strcmp(rel_path, section) == 0)
      return &g_files.files[i];
  }
  return NULL;
}

char *read_file_content(const char *path, size_t *size) {
  FILE *f = fopen(path, "rb");
  if (!f) return NULL;
  fseek(f, 0, SEEK_END);
  *size = (size_t)ftell(f);
  fseek(f, 0, SEEK_SET);
  char *content = (char *)malloc(*size + 1);
  fread(content, 1, *size, f);
  content[*size] = '\0';
  fclose(f);
  return content;
}

void add_file(const char *path) {
  char resolved[MAX_PATH];
  char *real = realpath(path, NULL);
  if (real) {
    strncpy(resolved, real, MAX_PATH - 1);
    resolved[MAX_PATH - 1] = '\0';
    free(real);
  } else {
    strncpy(resolved, path, MAX_PATH - 1);
    resolved[MAX_PATH - 1] = '\0';
  }
  if (find_file(resolved)) return;
  if (g_files.count >= g_files.capacity) {
    g_files.capacity = g_files.capacity == 0 ? 64 : g_files.capacity * 2;
    g_files.files = (SourceFile *)realloc(
        g_files.files, g_files.capacity * sizeof(SourceFile));
  }
  SourceFile *sf = &g_files.files[g_files.count++];
  strncpy(sf->path, resolved, MAX_PATH - 1);
  sf->content = read_file_content(resolved, &sf->size);
  sf->processed = 0;
  sf->processing = 0;
  if (!sf->content) {
    fprintf(stderr, "Error: Could not read file '%s'\n", resolved);
    exit(1);
  }
}

void scan_directory(const char *dir_path) {
#ifdef _WIN32
  WIN32_FIND_DATAA find_data;
  char search_path[MAX_PATH];
  snprintf(search_path, MAX_PATH, "%s\\*", dir_path);
  HANDLE hFind = FindFirstFileA(search_path, &find_data);
  if (hFind == INVALID_HANDLE_VALUE) {
    fprintf(stderr, "Error: Could not open directory '%s'\n", dir_path);
    exit(1);
  }
  do {
    if (strcmp(find_data.cFileName, ".") == 0 ||
        strcmp(find_data.cFileName, "..") == 0)
      continue;
    char full_path[MAX_PATH];
    snprintf(full_path, MAX_PATH, "%s\\%s", dir_path, find_data.cFileName);
    if (find_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
      scan_directory(full_path);
    } else if (ends_with_as(find_data.cFileName)) {
      add_file(full_path);
    }
  } while (FindNextFileA(hFind, &find_data) != 0);
  FindClose(hFind);
#else
  DIR *dir = opendir(dir_path);
  if (!dir) {
    fprintf(stderr, "Error: Could not open directory '%s'\n", dir_path);
    exit(1);
  }
  struct dirent *entry;
  while ((entry = readdir(dir)) != NULL) {
    if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
      continue;
    char full_path[MAX_PATH];
    snprintf(full_path, MAX_PATH, "%s/%s", dir_path, entry->d_name);
    if (is_directory(full_path)) {
      scan_directory(full_path);
    } else if (ends_with_as(entry->d_name)) {
      add_file(full_path);
    }
  }
  closedir(dir);
#endif
}

void extract_includes(SourceFile *file, char includes[][MAX_PATH],
                      int *include_count) {
  *include_count = 0;
  int in_block_comment = 0;
  int in_line_comment = 0;
  int in_string = 0;
  int in_char = 0;
  char string_char = 0;
  const char *p = file->content;
  while (*p) {
    if (in_block_comment) {
      if (p[0] == '*' && p[1] == '/') { in_block_comment = 0; p += 2; }
      else p++;
      continue;
    }
    if (in_line_comment) {
      if (*p == '\n') in_line_comment = 0;
      p++;
      continue;
    }
    if (in_string || in_char) {
      if (*p == '\\' && p[1]) p += 2;
      else if (*p == string_char) { in_string = 0; in_char = 0; p++; }
      else p++;
      continue;
    }
    if (p[0] == '/' && p[1] == '*') { in_block_comment = 1; p += 2; continue; }
    if (p[0] == '/' && p[1] == '/') { in_line_comment = 1; p += 2; continue; }
    if (*p == '"') { in_string = 1; string_char = '"'; p++; continue; }
    if (*p == '\'') { in_char = 1; string_char = '\''; p++; continue; }
    if (p[0] == '#' && strncmp(p, "#include", 8) == 0) {
      const char *after_include = p + 8;
      while (*after_include == ' ' || *after_include == '\t') after_include++;
      if (*after_include == '"') {
        after_include++;
        const char *quote_end = strchr(after_include, '"');
        if (quote_end) {
          size_t path_len = quote_end - after_include;
          if (path_len < MAX_PATH && *include_count < MAX_INCLUDES) {
            strncpy(includes[*include_count], after_include, path_len);
            includes[*include_count][path_len] = '\0';
            (*include_count)++;
          }
          p = quote_end + 1;
          continue;
        }
      }
    }
    p++;
  }
}

void process_file(SourceFile *file, const char *chain[], int chain_len) {
  if (file->processed) return;
  if (file->processing) {
    fprintf(stderr, "\nError: Circular dependency detected:\n");
    for (int i = 0; i < chain_len; i++) {
      char rel_path[MAX_PATH];
      get_relative_path(chain[i], rel_path);
      fprintf(stderr, "  -> %s\n", rel_path);
    }
    char rel_path[MAX_PATH];
    get_relative_path(file->path, rel_path);
    fprintf(stderr, "  -> %s\n", rel_path);
    exit(1);
  }
  file->processing = 1;
  char file_dir[MAX_PATH];
  get_dirname(file->path, file_dir);
  char includes[MAX_INCLUDES][MAX_PATH];
  int include_count;
  extract_includes(file, includes, &include_count);
  const char *new_chain[128];
  for (int i = 0; i < chain_len; i++) new_chain[i] = chain[i];
  new_chain[chain_len] = file->path;
  for (int i = 0; i < include_count; i++) {
    char inc_path[MAX_PATH];
    resolve_include_path(file_dir, includes[i], inc_path);
    SourceFile *inc_file = find_file(inc_path);
    if (!inc_file) {
      char rel_path[MAX_PATH];
      get_relative_path(file->path, rel_path);
      fprintf(stderr, "Error: Include file not found: '%s'\n", includes[i]);
      fprintf(stderr, "Referenced from: %s\n", rel_path);
      exit(1);
    }
    process_file(inc_file, new_chain, chain_len + 1);
  }
  file->processing = 0;
  file->processed = 1;
  if (g_order.count >= MAX_FILES) {
    fprintf(stderr, "Error: Too many files\n");
    exit(1);
  }
  g_order.paths =
      (char **)realloc(g_order.paths, (g_order.count + 1) * sizeof(char *));
  g_order.paths[g_order.count++] = file->path;
}

// ---------------------------------------------------------------------------
// Comment / include stripping
// ---------------------------------------------------------------------------

char *strip_comments(const char *content) {
  size_t size = strlen(content);
  char *result = (char *)malloc(size + 1);
  char *dst = result;
  const char *src = content;

  while (*src) {
    const char *check = src;
    int in_string = 0;
    char string_char = '\0';

    while (*check == ' ' || *check == '\t') check++;

    if (*check == '/' && *(check + 1) == '/') {
      while (*src && *src != '\n') src++;
      if (*src == '\n') src++;
      continue;
    } else if (*check == '/' && *(check + 1) == '*') {
      const char *comment_end = check + 2;
      while (*comment_end && !(*comment_end == '*' && *(comment_end + 1) == '/'))
        comment_end++;
      if (*comment_end == '*' && *(comment_end + 1) == '/')
        comment_end += 2;
      const char *after = comment_end;
      while (*after == ' ' || *after == '\t') after++;
      if (*after == '\n' || *after == '\r' || *after == '\0') {
        src = comment_end;
        while (*src == ' ' || *src == '\t') src++;
        if (*src == '\r') src++;
        if (*src == '\n') src++;
        continue;
      }
    }

    while (*src && *src != '\n') {
      if (!in_string) {
        if (*src == '"' || *src == '\'') {
          in_string = 1;
          string_char = *src;
          *dst++ = *src++;
        } else {
          *dst++ = *src++;
        }
      } else {
        if (*src == string_char) {
          const char *ck = src - 1;
          int escape_count = 0;
          while (ck >= content && *ck == '\\') { escape_count++; ck--; }
          if (escape_count % 2 == 0) in_string = 0;
        }
        *dst++ = *src++;
      }
    }
    if (*src == '\r') *dst++ = *src++;
    if (*src == '\n') *dst++ = *src++;
  }
  *dst = '\0';
  return result;
}

char *strip_includes(const char *content) {
  size_t size = strlen(content);
  char *result = (char *)malloc(size + 1);
  char *dst = result;
  const char *src = content;
  const char *line_start = src;

  while (*src) {
    if (*src == '\n') {
      size_t line_len = src - line_start + 1;
      char line[MAX_LINE];
      size_t copy_len = line_len < MAX_LINE - 1 ? line_len : MAX_LINE - 1;
      strncpy(line, line_start, copy_len);
      line[copy_len] = '\0';
      if (!strstr(line, "#include")) {
        memcpy(dst, line_start, line_len);
        dst += line_len;
      }
      line_start = src + 1;
    }
    src++;
  }
  if (line_start < src) {
    size_t line_len = src - line_start;
    char line[MAX_LINE];
    size_t copy_len = line_len < MAX_LINE - 1 ? line_len : MAX_LINE - 1;
    strncpy(line, line_start, copy_len);
    line[copy_len] = '\0';
    if (!strstr(line, "#include")) {
      memcpy(dst, line_start, line_len);
      dst += line_len;
    }
  }
  *dst = '\0';
  return result;
}

// ---------------------------------------------------------------------------
// String utility: replace all occurrences of 'old' with 'new_val'.
// Takes ownership of str (frees it), returns new malloc'd string.
// ---------------------------------------------------------------------------

static char *str_replace_all(char *str, const char *old_val,
                              const char *new_val) {
  size_t old_len = strlen(old_val);
  size_t new_len = strlen(new_val);
  if (old_len == 0) return str;

  int count = 0;
  const char *p = str;
  while ((p = strstr(p, old_val)) != NULL) {
    count++;
    p += old_len;
  }
  if (count == 0) return str;

  size_t old_total = strlen(str);
  size_t new_total = old_total - (size_t)count * old_len + (size_t)count * new_len;
  char *result = (char *)malloc(new_total + 1);
  char *dst = result;
  const char *src = str;

  while ((p = strstr(src, old_val)) != NULL) {
    size_t before = p - src;
    memcpy(dst, src, before);
    dst += before;
    memcpy(dst, new_val, new_len);
    dst += new_len;
    src = p + old_len;
  }
  strcpy(dst, src);
  free(str);
  return result;
}

// ---------------------------------------------------------------------------
// Build timestamp macros
// Replaced as raw tokens before any other processing step.
//
// String variants (include surrounding quotes, usable as string literals):
//   __BUILD_TIMESTAMP_STR__   "YYYY-MM-DD HH:MM:SS"
//   __BUILD_DATE_STR__        "YYYY-MM-DD"
//   __BUILD_TIME_STR__        "HH:MM:SS"
//
// Integer variants (no leading zeros, usable as numeric literals):
//   __BUILD_YEAR__            e.g. 2026
//   __BUILD_MONTH__           e.g. 3
//   __BUILD_DAY__             e.g. 4
//   __BUILD_HOUR__            e.g. 9
//   __BUILD_MINUTE__          e.g. 5
//   __BUILD_SECOND__          e.g. 7
//   __BUILD_UNIX_TS__         Unix timestamp (seconds since epoch)
// ---------------------------------------------------------------------------

static char *apply_timestamp_macros(char *content) {
  time_t now = time(NULL);
  struct tm *t = localtime(&now);

  char ts_str[32], date_str[16], time_str[12];
  strftime(ts_str, sizeof(ts_str), "\"%Y-%m-%d %H:%M:%S\"", t);
  strftime(date_str, sizeof(date_str), "\"%Y-%m-%d\"", t);
  strftime(time_str, sizeof(time_str), "\"%H:%M:%S\"", t);

  // Integer variants without leading zeros
  char year_str[8], month_str[4], day_str[4];
  char hour_str[4], min_str[4], sec_str[4], unix_str[24];
  snprintf(year_str, sizeof(year_str), "%d", t->tm_year + 1900);
  snprintf(month_str, sizeof(month_str), "%d", t->tm_mon + 1);
  snprintf(day_str, sizeof(day_str), "%d", t->tm_mday);
  snprintf(hour_str, sizeof(hour_str), "%d", t->tm_hour);
  snprintf(min_str, sizeof(min_str), "%d", t->tm_min);
  snprintf(sec_str, sizeof(sec_str), "%d", t->tm_sec);
  snprintf(unix_str, sizeof(unix_str), "%lld", (long long)now);

  content = str_replace_all(content, "__BUILD_TIMESTAMP_STR__", ts_str);
  content = str_replace_all(content, "__BUILD_DATE_STR__", date_str);
  content = str_replace_all(content, "__BUILD_TIME_STR__", time_str);
  content = str_replace_all(content, "__BUILD_YEAR__", year_str);
  content = str_replace_all(content, "__BUILD_MONTH__", month_str);
  content = str_replace_all(content, "__BUILD_DAY__", day_str);
  content = str_replace_all(content, "__BUILD_HOUR__", hour_str);
  content = str_replace_all(content, "__BUILD_MINUTE__", min_str);
  content = str_replace_all(content, "__BUILD_SECOND__", sec_str);
  content = str_replace_all(content, "__BUILD_UNIX_TS__", unix_str);

  return content;
}

// ---------------------------------------------------------------------------
// FNV-1a hash functions
// ---------------------------------------------------------------------------

static uint32_t fnv1a_32(const char *str, size_t len) {
  uint32_t hash = 2166136261u;  // FNV offset basis
  for (size_t i = 0; i < len; i++) {
    hash ^= (uint8_t)str[i];
    hash *= 16777619u;  // FNV prime
  }
  return hash;
}

static uint64_t fnv1a_64(const char *str, size_t len) {
  uint64_t hash = 14695981039346656037ull;  // FNV offset basis 64-bit
  for (size_t i = 0; i < len; i++) {
    hash ^= (uint8_t)str[i];
    hash *= 1099511628211ull;  // FNV prime 64-bit
  }
  return hash;
}

// ---------------------------------------------------------------------------
// Evaluate STRHASH("...") or STRHASH64("...") calls in source text.
// Each call is replaced with the computed decimal hash value.
// Operates on the literal byte values between the quotes (no escape processing).
// Takes ownership of content, returns new malloc'd string.
//
//   STRHASH("name")    -> FNV-1a 32-bit decimal (e.g. 2949673445)
//   STRHASH64("name")  -> FNV-1a 64-bit decimal (e.g. 14631297582076101717)
// ---------------------------------------------------------------------------

static char *evaluate_string_macros(char *content, const char *macro_name,
                                    int is_64bit) {
  char prefix[64];
  snprintf(prefix, sizeof(prefix), "%s(\"", macro_name);
  size_t prefix_len = strlen(prefix);
  int count = 0;
  char *pos = content;

  while (1) {
    char *match = strstr(pos, prefix);
    if (!match) break;

    // Locate the closing quote (no escape handling, matches JS behaviour)
    char *str_start = match + prefix_len;
    char *str_end = str_start;
    while (*str_end && *str_end != '"') str_end++;

    // Must be followed by closing paren
    if (*str_end != '"' || str_end[1] != ')') {
      pos = match + prefix_len;
      continue;
    }

    size_t str_len = (size_t)(str_end - str_start);
    char hash_str[32];
    if (is_64bit) {
      uint64_t hash = fnv1a_64(str_start, str_len);
      snprintf(hash_str, sizeof(hash_str), "%llu", (unsigned long long)hash);
    } else {
      uint32_t hash = fnv1a_32(str_start, str_len);
      snprintf(hash_str, sizeof(hash_str), "%u", (unsigned int)hash);
    }

    size_t match_full_len = (size_t)(str_end + 2 - match);
    size_t hash_len = strlen(hash_str);
    size_t old_total = strlen(content);
    size_t new_total = old_total - match_full_len + hash_len;

    char *new_content = (char *)malloc(new_total + 1);
    size_t before_len = (size_t)(match - content);
    memcpy(new_content, content, before_len);
    memcpy(new_content + before_len, hash_str, hash_len);
    strcpy(new_content + before_len + hash_len, str_end + 2);

    free(content);
    content = new_content;
    pos = content + before_len + hash_len;
    count++;
  }

  if (count > 0)
    printf("Evaluated %d %s() call(s)\n", count, macro_name);

  return content;
}

// ---------------------------------------------------------------------------
// Strip STRHASH/FNV macro *definitions* from prepend file content so the
// C preprocessor does not try to expand already-evaluated calls.
// Handles multi-line continuations (lines ending with \).
//
// Stripped patterns:
//   #define STRHASH(...)
//   #define STRHASH64(...)
//   #define HF<digit>(...)   (helper macros)
//   #define FNV_OFFSET[_64]
//   #define FNV_PRIME[_64]
// ---------------------------------------------------------------------------

static char *strip_strhash_macros(char *content) {
  size_t content_len = strlen(content);
  char *result = (char *)malloc(content_len + 1);
  char *dst = result;
  const char *p = content;
  int skip_continuation = 0;

  while (*p) {
    const char *line_start = p;
    const char *line_end = p;
    while (*line_end && *line_end != '\n') line_end++;
    size_t line_len = (size_t)(line_end - line_start);

    if (skip_continuation) {
      // Check if this continuation line itself continues
      const char *check = line_end - 1;
      while (check > line_start &&
             (*check == ' ' || *check == '\t' || *check == '\r'))
        check--;
      skip_continuation = (check >= line_start && *check == '\\');
      p = line_end;
      if (*p == '\n') p++;
      continue;
    }

    // Trim leading whitespace for comparison
    const char *trimmed = line_start;
    while (trimmed < line_end && (*trimmed == ' ' || *trimmed == '\t'))
      trimmed++;

    int should_strip = 0;
    if ((size_t)(line_end - trimmed) > 7 &&
        strncmp(trimmed, "#define", 7) == 0) {
      const char *after = trimmed + 7;
      while (after < line_end && (*after == ' ' || *after == '\t')) after++;

      // Helper: check if identifier at 'after' matches 'name' (length 'nlen')
      // followed by a non-identifier character
#define MATCH_DEFINE(name, nlen)                                          \
      (strncmp(after, (name), (nlen)) == 0 &&                            \
       (after + (nlen) >= line_end ||                                    \
        (!isalnum((unsigned char)after[(nlen)]) && after[(nlen)] != '_')))

      if (MATCH_DEFINE("STRHASH64", 9))   should_strip = 1;
      else if (MATCH_DEFINE("STRHASH", 7))    should_strip = 1;
      else if (MATCH_DEFINE("FNV_OFFSET_64", 13)) should_strip = 1;
      else if (MATCH_DEFINE("FNV_PRIME_64", 12))  should_strip = 1;
      else if (MATCH_DEFINE("FNV_OFFSET", 10))    should_strip = 1;
      else if (MATCH_DEFINE("FNV_PRIME", 9))      should_strip = 1;
      else if ((size_t)(line_end - after) >= 3 &&
               after[0] == 'H' && after[1] == 'F' &&
               isdigit((unsigned char)after[2]))   should_strip = 1;

#undef MATCH_DEFINE
    }

    if (should_strip) {
      // Check if line has a continuation backslash
      const char *check = line_end - 1;
      while (check > line_start &&
             (*check == ' ' || *check == '\t' || *check == '\r'))
        check--;
      skip_continuation = (check >= line_start && *check == '\\');
      p = line_end;
      if (*p == '\n') p++;
    } else {
      // Copy line verbatim
      memcpy(dst, line_start, line_len);
      dst += line_len;
      if (*line_end == '\n') *dst++ = '\n';
      p = line_end;
      if (*p == '\n') p++;
    }
  }

  *dst = '\0';
  free(content);
  return result;
}

// ---------------------------------------------------------------------------
// C preprocessor execution
// Writes combined content to a temp file, runs gcc -E -P -x c, reads result.
// ---------------------------------------------------------------------------

static void ensure_dir(const char *dir) {
#ifdef _WIN32
  CreateDirectoryA(dir, NULL);
#else
  mkdir(dir, 0755);
#endif
}

static char *run_preprocessor(const char *content) {
  // Place temp files in .build_cache next to the source directory (or cwd)
  char cache_dir[MAX_PATH];
  if (g_input_dir[0] != '\0') {
    char parent[MAX_PATH];
    get_dirname(g_input_dir, parent);
    snprintf(cache_dir, sizeof(cache_dir), "%s/.build_cache", parent);
  } else {
    snprintf(cache_dir, sizeof(cache_dir), ".build_cache");
  }

  // Clean and recreate cache dir
  ensure_dir(cache_dir);

  char input_path[MAX_PATH], output_path[MAX_PATH], err_path[MAX_PATH];
  snprintf(input_path, sizeof(input_path), "%s/_combined_input.c", cache_dir);
  snprintf(output_path, sizeof(output_path), "%s/_combined_output.c", cache_dir);
  snprintf(err_path, sizeof(err_path), "%s/_preproc_error.txt", cache_dir);

  // Write input
  FILE *f = fopen(input_path, "wb");
  if (!f) {
    fprintf(stderr, "Error: Could not write preprocessor input to '%s'\n",
            input_path);
    exit(1);
  }
  fwrite(content, 1, strlen(content), f);
  fclose(f);

  // Build command: gcc -E -P -x c [-D...] [-I src_dir] input -o output
  size_t cmd_size = 1024 + (size_t)g_define_count * 256 + MAX_PATH * 3;
  char *cmd = (char *)malloc(cmd_size);
  int pos = 0;
  pos += snprintf(cmd + pos, cmd_size - (size_t)pos, "gcc -E -P -x c");

  for (int i = 0; i < g_define_count; i++) {
    pos += snprintf(cmd + pos, cmd_size - (size_t)pos, " %s", g_defines[i]);
  }

  if (g_input_dir[0] != '\0') {
    pos += snprintf(cmd + pos, cmd_size - (size_t)pos,
                    " -I \"%s\"", g_input_dir);
  }

  pos += snprintf(cmd + pos, cmd_size - (size_t)pos,
                  " \"%s\" -o \"%s\" 2>\"%s\"",
                  input_path, output_path, err_path);

  int ret = system(cmd);
  free(cmd);

  if (ret != 0) {
    fprintf(stderr, "\nError: Preprocessor failed.\n");
    // Print captured stderr
    size_t err_size;
    char *err_content = read_file_content(err_path, &err_size);
    if (err_content && err_size > 0) {
      fprintf(stderr, "%s\n", err_content);
      free(err_content);
    }
    fprintf(stderr, "Hint: Ensure gcc is in your PATH, or use --no-preprocess "
                    "to skip this step.\n");
    remove(input_path);
    remove(err_path);
    exit(1);
  }

  size_t out_size;
  char *output = read_file_content(output_path, &out_size);
  if (!output) {
    fprintf(stderr, "Error: Could not read preprocessor output '%s'\n",
            output_path);
    remove(input_path);
    remove(err_path);
    exit(1);
  }

  remove(input_path);
  remove(output_path);
  remove(err_path);

  return output;
}

// ---------------------------------------------------------------------------
// Convert #pragma BUNDLER_FILE <path> markers to // === <path> === comments.
// Used AFTER preprocessing (and after strip_comments) so markers survive both.
// Takes ownership of content, returns new malloc'd string.
// ---------------------------------------------------------------------------

static char *convert_pragma_markers(char *content) {
  static const char prefix[] = "#pragma BUNDLER_FILE ";
  static const size_t prefix_len = sizeof(prefix) - 1;

  size_t old_len = strlen(content);
  // Replacement is always shorter than or equal to the pragma line
  char *result = (char *)malloc(old_len + 1);
  char *dst = result;
  const char *p = content;

  while (*p) {
    const char *line_start = p;
    const char *line_end = p;
    while (*line_end && *line_end != '\n') line_end++;

    // Skip leading whitespace when checking the prefix
    const char *check = line_start;
    while (check < line_end && (*check == ' ' || *check == '\t')) check++;

    if ((size_t)(line_end - check) > prefix_len &&
        strncmp(check, prefix, prefix_len) == 0) {
      const char *path_start = check + prefix_len;
      size_t path_len = (size_t)(line_end - path_start);
      // Trim trailing whitespace / CR from path
      while (path_len > 0 &&
             (path_start[path_len - 1] == ' ' ||
              path_start[path_len - 1] == '\t' ||
              path_start[path_len - 1] == '\r'))
        path_len--;

      dst += sprintf(dst, "// === ");
      memcpy(dst, path_start, path_len);
      dst += path_len;
      dst += sprintf(dst, " ===");
    } else {
      size_t line_len = (size_t)(line_end - line_start);
      memcpy(dst, line_start, line_len);
      dst += line_len;
    }

    if (*line_end == '\n') {
      *dst++ = '\n';
      p = line_end + 1;
    } else {
      p = line_end;
    }
  }

  *dst = '\0';
  free(content);
  return result;
}

// ---------------------------------------------------------------------------
// Help text
// ---------------------------------------------------------------------------

void print_help(const char *program_name) {
  printf("Usage: %s [OPTIONS] <source_dir|file.as> [file2.as ...]\n",
         program_name);
  printf("\nOptions:\n");
  printf("  -o <file>              Output bundled code to specified file\n");
  printf("  --strip, -s            Strip comment-only lines from output\n");
  printf("  --no-preprocess, -n    Skip the C preprocessor step\n");
  printf("  --prepend, -p <file>   Prepend file before preprocessing "
         "(macro definitions)\n");
  printf("  --header, -H <file>    Prepend raw content to final output "
         "(skips preprocessing)\n");
  printf("  -D<NAME>[=VALUE]       Define a preprocessor macro "
         "(repeatable)\n");
  printf("  --help                 Show this help message\n");
  printf("\nBuild timestamp macros (replaced before all other steps):\n");
  printf("  __BUILD_TIMESTAMP_STR__   String: \"YYYY-MM-DD HH:MM:SS\"\n");
  printf("  __BUILD_DATE_STR__        String: \"YYYY-MM-DD\"\n");
  printf("  __BUILD_TIME_STR__        String: \"HH:MM:SS\"\n");
  printf("  __BUILD_YEAR__            Integer: e.g. 2026\n");
  printf("  __BUILD_MONTH__           Integer: e.g. 3\n");
  printf("  __BUILD_DAY__             Integer: e.g. 4\n");
  printf("  __BUILD_HOUR__            Integer: e.g. 9\n");
  printf("  __BUILD_MINUTE__          Integer: e.g. 5\n");
  printf("  __BUILD_SECOND__          Integer: e.g. 7\n");
  printf("  __BUILD_UNIX_TS__         Integer: Unix timestamp\n");
  printf("\nFNV hash macros (evaluated before C preprocessor):\n");
  printf("  STRHASH(\"text\")           FNV-1a 32-bit decimal hash\n");
  printf("  STRHASH64(\"text\")         FNV-1a 64-bit decimal hash\n");
  printf("\nExamples:\n");
  printf("  %s src/\n", program_name);
  printf("  %s -o bundle.as src/\n", program_name);
  printf("  %s -o out.as --strip -DDEBUG src/\n", program_name);
  printf("  %s -o out.as -p macros.h -H license.txt src/\n", program_name);
  printf("  %s -o out.as --no-preprocess src/\n", program_name);
  printf("\nWithout -o, only validation errors and warnings are displayed.\n");
}

// ---------------------------------------------------------------------------
// Error-mapping helpers (unchanged from original)
// ---------------------------------------------------------------------------

typedef struct {
  SourceFile *file;
  char rel_path[MAX_PATH];
  int start_line;
  int end_line;
} FileIndex;

void get_line_at(const char *content, int line_num, char *output,
                 size_t max_len) {
  int current = 1;
  const char *p = content;
  output[0] = '\0';
  while (*p && current < line_num) {
    if (*p == '\n') current++;
    p++;
  }
  if (current == line_num) {
    const char *line_start = p;
    while (*p && *p != '\n') p++;
    size_t len = p - line_start;
    if (len < max_len - 1) {
      strncpy(output, line_start, len);
      output[len] = '\0';
    }
  }
}

void trim_leading_ws(char *str) {
  char *start = str;
  while (*start == ' ' || *start == '\t') start++;
  if (start != str) memmove(str, start, strlen(start) + 1);
}

int find_line_in_source(SourceFile *file, const char *bundled_content,
                        int error_bundled_line, FileIndex *file_index) {
  int max_context = 50;
  int context_line_count = 0;
  int *context_line_offsets = (int *)malloc(max_context * 2 * sizeof(int));
  char **context_lines = (char **)malloc(max_context * 2 * sizeof(char *));
  int bundled_offset = error_bundled_line - 1;

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
          context_lines[context_line_count] = strdup(line);
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
        context_lines[context_line_count] = strdup(line);
        context_line_offsets[context_line_count] =
            below_line - file_index->start_line + 1;
        context_line_count++;
      }
    }

    for (int i = 0; i < context_line_count; i++) {
      if (strlen(context_lines[i]) == 0) continue;
      int occurrences = 0;
      int found_line = 0;
      int current_line = 1;
      const char *p = file->content;
      while (*p) {
        const char *line_start = p;
        while (*p && *p != '\n') p++;
        char source_line[MAX_LINE];
        size_t len = p - line_start;
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
          for (int k = 0; k < context_line_count; k++) free(context_lines[k]);
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

// ---------------------------------------------------------------------------
// main
// ---------------------------------------------------------------------------

int main(int argc, char **argv) {
  const char *output_file = NULL;

  // --- Argument parsing ---
  int first_source = -1;
  for (int i = 1; i < argc; i++) {
    const char *arg = argv[i];
    if (strcmp(arg, "--help") == 0) {
      print_help(argv[0]);
      return 0;
    } else if (strcmp(arg, "--strip") == 0 || strcmp(arg, "-s") == 0) {
      g_strip_comments = 1;
    } else if (strcmp(arg, "--no-preprocess") == 0 || strcmp(arg, "-n") == 0) {
      g_skip_preprocess = 1;
    } else if (strcmp(arg, "-o") == 0) {
      if (i + 1 < argc) {
        output_file = argv[++i];
        g_verbose = 1;
      } else {
        fprintf(stderr, "Error: -o requires a filename argument\n");
        return 1;
      }
    } else if (strcmp(arg, "--prepend") == 0 || strcmp(arg, "-p") == 0) {
      if (i + 1 < argc) {
        char *real = realpath(argv[++i], NULL);
        if (real) {
          strncpy(g_prepend_file, real, MAX_PATH - 1);
          free(real);
        } else {
          strncpy(g_prepend_file, argv[i], MAX_PATH - 1);
        }
        g_prepend_file[MAX_PATH - 1] = '\0';
      } else {
        fprintf(stderr, "Error: --prepend requires a file path argument\n");
        return 1;
      }
    } else if (strcmp(arg, "--header") == 0 || strcmp(arg, "-H") == 0) {
      if (i + 1 < argc) {
        char *real = realpath(argv[++i], NULL);
        if (real) {
          strncpy(g_header_file, real, MAX_PATH - 1);
          free(real);
        } else {
          strncpy(g_header_file, argv[i], MAX_PATH - 1);
        }
        g_header_file[MAX_PATH - 1] = '\0';
      } else {
        fprintf(stderr, "Error: --header requires a file path argument\n");
        return 1;
      }
    } else if (strncmp(arg, "-D", 2) == 0) {
      if (g_define_count >= g_define_capacity) {
        g_define_capacity = g_define_capacity == 0 ? 16 : g_define_capacity * 2;
        g_defines = (char **)realloc(
            g_defines, (size_t)g_define_capacity * sizeof(char *));
      }
      g_defines[g_define_count++] = (char *)arg;
    } else if (arg[0] != '-') {
      first_source = i;
      break;
    } else {
      fprintf(stderr, "Unknown option: %s\n", arg);
      return 1;
    }
  }

  if (first_source < 0) {
    print_help(argv[0]);
    return 1;
  }

  // --- Log active options ---
  if (g_strip_comments)        printf("Option: Stripping comment-only lines\n");
  if (g_skip_preprocess)       printf("Option: Skipping C preprocessor\n");
  if (g_prepend_file[0])       printf("Option: Prepend file: %s\n", g_prepend_file);
  if (g_header_file[0])        printf("Option: Header file: %s\n", g_header_file);
  if (g_define_count > 0) {
    printf("Defines:");
    for (int i = 0; i < g_define_count; i++) printf(" %s", g_defines[i]);
    printf("\n");
  }

  // --- Remember input directory for relative path calculations ---
  if (is_directory(argv[first_source])) {
    char *real = realpath(argv[first_source], NULL);
    if (real) {
      strncpy(g_input_dir, real, MAX_PATH - 1);
      g_input_dir[MAX_PATH - 1] = '\0';
      free(real);
    }
    printf("Scanning directory: %s\n", argv[first_source]);
  }

  // --- Collect source files ---
  for (int i = first_source; i < argc; i++) {
    if (is_directory(argv[i])) {
      scan_directory(argv[i]);
    } else if (ends_with_as(argv[i])) {
      add_file(argv[i]);
    } else {
      fprintf(stderr, "Warning: Skipping non-.as file: %s\n", argv[i]);
    }
  }

  if (g_files.count == 0) {
    fprintf(stderr, "Error: No .as files found\n");
    return 1;
  }
  printf("Found %d .as file(s)\n", g_files.count);

  // --- Step 1: Resolve dependency order ---
  const char *empty_chain[1] = {NULL};
  for (int i = 0; i < g_files.count; i++) {
    if (!g_files.files[i].processed)
      process_file(&g_files.files[i], empty_chain, 0);
  }

  // --- Step 2: Combine files into one string with #pragma BUNDLER_FILE markers ---
  size_t total_size = 0;
  for (int i = 0; i < g_order.count; i++) {
    SourceFile *f = find_file(g_order.paths[i]);
    total_size += f->size + 256;
  }
  // Add room for prepend file and timestamps
  total_size += 65536;

  char *combined = (char *)malloc(total_size);
  char *ptr = combined;

  for (int i = 0; i < g_order.count; i++) {
    SourceFile *f = find_file(g_order.paths[i]);
    char *content = strip_includes(f->content);

    char rel_path[MAX_PATH];
    get_relative_path(f->path, rel_path);

    // Use pragma marker (survives C preprocessor; converted to comment later)
    ptr += sprintf(ptr, "#pragma BUNDLER_FILE %s\n", rel_path);

    size_t content_len = strlen(content);
    // Grow buffer if needed
    if ((size_t)(ptr - combined) + content_len + 4 > total_size) {
      size_t used = (size_t)(ptr - combined);
      total_size = used + content_len + 65536;
      combined = (char *)realloc(combined, total_size);
      ptr = combined + used;
    }

    strcpy(ptr, content);
    ptr += content_len;
    if (ptr > combined && *(ptr - 1) != '\n') *ptr++ = '\n';
    *ptr++ = '\n';
    free(content);
  }
  *ptr = '\0';

  // --- Step 3: Prepend macro file (strip STRHASH defs so CPP won't see them) ---
  if (g_prepend_file[0] != '\0') {
    size_t pre_size;
    char *pre_content = read_file_content(g_prepend_file, &pre_size);
    if (!pre_content) {
      fprintf(stderr, "Error: Could not read prepend file '%s'\n",
              g_prepend_file);
      free(combined);
      return 1;
    }
    pre_content = strip_strhash_macros(pre_content);
    size_t pre_len = strlen(pre_content);
    size_t combined_len = strlen(combined);
    char *new_combined = (char *)malloc(pre_len + combined_len + 3);
    memcpy(new_combined, pre_content, pre_len);
    new_combined[pre_len] = '\n';
    new_combined[pre_len + 1] = '\n';
    memcpy(new_combined + pre_len + 2, combined, combined_len + 1);
    free(pre_content);
    free(combined);
    combined = new_combined;
    printf("Prepended: %s\n", g_prepend_file);
  }

  // --- Step 4: Apply build timestamp macros (first, before everything else) ---
  combined = apply_timestamp_macros(combined);

  // --- Step 5: Evaluate STRHASH() and STRHASH64() calls ---
  combined = evaluate_string_macros(combined, "STRHASH64", 1);
  combined = evaluate_string_macros(combined, "STRHASH", 0);

  // --- Step 6: Run C preprocessor (single pass over entire combined file) ---
  if (!g_skip_preprocess) {
    printf("Running C preprocessor (single pass)...\n");
    char *preprocessed = run_preprocessor(combined);
    free(combined);
    combined = preprocessed;
    printf("Preprocessor complete\n");
  }

  // --- Step 7: Strip comments (before converting pragma markers) ---
  // #pragma BUNDLER_FILE is not a comment so it survives this step.
  if (g_strip_comments) {
    char *stripped = strip_comments(combined);
    free(combined);
    combined = stripped;
  }

  // --- Step 8: Convert #pragma BUNDLER_FILE markers to // === ... === ---
  combined = convert_pragma_markers(combined);

  // --- Step 9: Prepend raw header file (never preprocessed) ---
  if (g_header_file[0] != '\0') {
    size_t hdr_size;
    char *hdr_content = read_file_content(g_header_file, &hdr_size);
    if (!hdr_content) {
      fprintf(stderr, "Error: Could not read header file '%s'\n",
              g_header_file);
      free(combined);
      return 1;
    }
    size_t hdr_len = strlen(hdr_content);
    size_t comb_len = strlen(combined);
    char *new_combined = (char *)malloc(hdr_len + comb_len + 3);
    memcpy(new_combined, hdr_content, hdr_len);
    new_combined[hdr_len] = '\n';
    new_combined[hdr_len + 1] = '\n';
    memcpy(new_combined + hdr_len + 2, combined, comb_len + 1);
    free(hdr_content);
    free(combined);
    combined = new_combined;
    printf("Header prepended: %s\n", g_header_file);
  }

  // --- Step 10: Validate bundled script ---
  void *validator = as_validator_create();
  if (!validator) {
    fprintf(stderr, "Error: Could not create validator\n");
    free(combined);
    return 1;
  }

  ASErrorList *errors = as_error_list_create();

  if (as_add_section(validator, combined, "bundled") < 0) {
    fprintf(stderr, "Error: Could not add bundled script\n");
    as_error_list_destroy(errors);
    as_validator_destroy(validator);
    free(combined);
    return 1;
  }

  // Build file index from // === ... === markers for error mapping
  FileIndex *index = (FileIndex *)malloc(g_order.count * sizeof(FileIndex));
  int index_count = 0;
  int current_bundled_line = 1;

  const char *bp = combined;
  while (*bp) {
    const char *p = bp;
    while (*p == ' ' || *p == '\t') p++;

    if (strncmp(p, "// === ", 7) == 0 && index_count < g_order.count) {
      const char *sep_end = strstr(p + 7, " ===");
      if (sep_end) {
        if (index_count > 0)
          index[index_count - 1].end_line = current_bundled_line;

        const char *path_start = p + 7;
        size_t path_len = sep_end - path_start;
        if (path_len < MAX_PATH - 1) {
          strncpy(index[index_count].rel_path, path_start, path_len);
          index[index_count].rel_path[path_len] = '\0';
        }

        index[index_count].file = NULL;
        for (int j = 0; j < g_files.count; j++) {
          char rel_path[MAX_PATH];
          get_relative_path(g_files.files[j].path, rel_path);
          if (strcmp(rel_path, index[index_count].rel_path) == 0) {
            index[index_count].file = &g_files.files[j];
            break;
          }
        }
        index[index_count].start_line = current_bundled_line + 1;
        index_count++;
      }
    }

    while (*bp && *bp != '\n') bp++;
    if (*bp == '\n') { current_bundled_line++; bp++; }
  }
  if (index_count > 0)
    index[index_count - 1].end_line = current_bundled_line;

  // Report validation results
  int validation_failed = 0;
  if (as_build(validator, errors) < 0) {
    validation_failed = 1;
    fprintf(stderr, "\nValidation failed with %d error(s):\n\n",
            errors->error_count);
  } else if (errors->error_count > 0) {
    fprintf(stderr, "\nValidation succeeded with %d warning(s):\n\n",
            errors->error_count);
  }

  for (int i = 0; i < errors->error_count; i++) {
    SourceFile *found_file = NULL;
    char file_path[MAX_PATH] = "bundled";
    int file_idx = -1;

    for (int j = 0; j < index_count; j++) {
      if (errors->errors[i].line >= index[j].start_line &&
          errors->errors[i].line < index[j].end_line) {
        found_file = index[j].file;
        strcpy(file_path, index[j].rel_path);
        file_idx = j;
        break;
      }
    }

    int original_line = 0;
    if (found_file && file_idx >= 0)
      original_line = find_line_in_source(
          found_file, combined, errors->errors[i].line, &index[file_idx]);

    if (original_line > 0) {
      fprintf(stderr, "%s:%d:%d: %s\n", file_path, original_line,
              errors->errors[i].column, errors->errors[i].message);
    } else if (found_file) {
      fprintf(stderr, "%s:%d:%d: %s (could not map to exact line)\n",
              file_path, errors->errors[i].line, errors->errors[i].column,
              errors->errors[i].message);
    } else {
      fprintf(stderr, "bundled:%d:%d: %s\n", errors->errors[i].line,
              errors->errors[i].column, errors->errors[i].message);
    }
  }
  if (errors->error_count > 0) fprintf(stderr, "\n");

  free(index);

  // --- Step 11: Write output ---
  if (g_verbose && output_file) {
    FILE *out = fopen(output_file, "wb");
    if (!out) {
      fprintf(stderr, "Error: Could not write to '%s'\n", output_file);
      as_error_list_destroy(errors);
      as_validator_destroy(validator);
      free(combined);
      return 1;
    }
    fwrite(combined, 1, strlen(combined), out);
    fclose(out);
    printf("Successfully bundled %d file(s) in dependency order\n",
           g_order.count);
    printf("Output: %s\n", output_file);
  }

  // --- Cleanup ---
  for (int i = 0; i < g_files.count; i++) free(g_files.files[i].content);
  free(g_files.files);
  free(g_order.paths);
  free(g_defines);
  free(combined);
  as_error_list_destroy(errors);
  as_validator_destroy(validator);

  return validation_failed ? 1 : 0;
}
