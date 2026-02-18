#include "as_wrapper.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    return _fullpath(resolved_path, path, MAX_PATH);
  } else {
    // Allocate memory like Linux realpath does when passed NULL
    char *buffer = (char *)malloc(MAX_PATH);
    if (!buffer) return NULL;
    char *result = _fullpath(buffer, path, MAX_PATH);
    if (!result) {
      free(buffer);
      return NULL;
    }
    return buffer;
  }
}
#define realpath portable_realpath
#endif

#define MAX_PATH 512
#define MAX_FILES 1024
#define MAX_INCLUDES 128
#define MAX_LINE 2048


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
static int g_verbose = 0; // -o flag: output bundled code
static int g_strip_comments = 0; // --strip flag: strip comments from output
static char g_input_dir[MAX_PATH] = {
    0}; // Remember the input directory for relative paths

// Global file list
static FileList g_files = {0};
static ProcessedOrder g_order = {0};

// Helper to check if path is a directory
int is_directory(const char *path) {
  struct stat st;
  if (stat(path, &st) != 0)
    return 0;
  return S_ISDIR(st.st_mode);
}

// Helper to check if file ends with .as
int ends_with_as(const char *filename) {
  size_t len = strlen(filename);
  return len > 3 && strcmp(filename + len - 3, ".as") == 0;
}

// Get directory part of a path (dirname)
void get_dirname(const char *path, char *out) {
  strcpy(out, path);
  char *last_slash = strrchr(out, '/');
  char *last_backslash = strrchr(out, '\\');

  // Use whichever is later in the string
  char *separator = last_slash > last_backslash ? last_slash : last_backslash;

  if (separator) {
    *separator = '\0';
  } else {
    strcpy(out, ".");
  }
}

// Get just the filename without path or extension
void get_basename(const char *path, char *out) {
  const char *forward_slash = strrchr(path, '/');
  const char *backslash = strrchr(path, '\\');
  const char *filename = forward_slash > backslash ? forward_slash : backslash;

  if (filename) {
    filename++; // Skip the slash
  } else {
    filename = path;
  }
  strcpy(out, filename);

  // Remove .as extension
  size_t len = strlen(out);
  if (len > 3 && strcmp(out + len - 3, ".as") == 0) {
    out[len - 3] = '\0';
  }
}

// Get relative path from input directory to file
void get_relative_path(const char *file_path, char *out) {
  if (g_input_dir[0] == '\0') {
    get_basename(file_path, out);
    return;
  }

  // Check if file_path starts with input_dir
  size_t input_len = strlen(g_input_dir);
  if (strncmp(file_path, g_input_dir, input_len) == 0) {
    // Skip past input_dir and any following slash
    const char *rel_start = file_path + input_len;
    if (*rel_start == '/')
      rel_start++;

    strcpy(out, rel_start);
  } else {
    get_basename(file_path, out);
  }
}

// Join two path components, avoiding duplicate slashes
void path_join(const char *base, const char *rel, char *out) {
  // Skip leading slashes from relative path to avoid duplicates
  while (rel[0] == '/' && base[strlen(base) - 1] == '/') {
    rel++;
  }

  snprintf(out, MAX_PATH, "%s/%s", base, rel);
}

// Normalize path by resolving . and .. components
// Normalize path by resolving . and .. components
void normalize_path(const char* input, char* out) {
	char temp[MAX_PATH];
	strcpy(temp, input);

	// Normalize separators to forward slashes for consistency
	for (char* p = temp; *p; p++) {
		if (*p == '\\') *p = '/';
	}

	// Split into parts
	char* parts[256];
	int part_count = 0;

	char* token = strtok(temp, "/");
	while (token != NULL && part_count < 256) {
		if (strcmp(token, ".") == 0) {
			// Skip current directory reference
		} else if (strcmp(token, "..") == 0) {
			// Go up one directory
			if (part_count > 0) {
				part_count--;
			}
		} else {
			parts[part_count++] = strdup(token);
		}
		token = strtok(NULL, "/");
	}

	// Rebuild path
	out[0] = '\0';

	// If original was absolute, start with /
	if (input[0] == '/' || input[0] == '\\') {
		strcat(out, "/");
	}

	for (int i = 0; i < part_count; i++) {
		if (i > 0) strcat(out, "/");
		strcat(out, parts[i]);
		free(parts[i]);  // Free the strdup'd strings
	}
}


// Resolve a relative include path against a base directory
void resolve_include_path(const char *base_dir, const char *rel_path,
                          char *out) {
  char combined[MAX_PATH];
  char normalized[MAX_PATH];

  // Check if rel_path is already absolute
  if (rel_path[0] == '/') {
    strcpy(out, rel_path);
    return;
  }

  // Join base_dir with rel_path
  path_join(base_dir, rel_path, combined);

  // Normalize to resolve any .. components
  normalize_path(combined, normalized);

  // Resolve to real path if possible
  char *resolved = realpath(normalized, NULL);
  if (resolved) {
    strncpy(out, resolved, MAX_PATH - 1);
    out[MAX_PATH - 1] = '\0';
    free(resolved);
  } else {
    strcpy(out, normalized);
  }
}

// Find file in global list by path
SourceFile *find_file(const char *path) {
  for (int i = 0; i < g_files.count; i++) {
    if (strcmp(g_files.files[i].path, path) == 0) {
      return &g_files.files[i];
    }
  }
  return NULL;
}

// Find file by section name (relative path without .as)
SourceFile *find_file_by_section(const char *section) {
  for (int i = 0; i < g_files.count; i++) {
    char rel_path[MAX_PATH];
    get_relative_path(g_files.files[i].path, rel_path);

    // Remove .as extension for comparison
    size_t len = strlen(rel_path);
    if (len > 3 && strcmp(rel_path + len - 3, ".as") == 0) {
      rel_path[len - 3] = '\0';
    }

    if (strcmp(rel_path, section) == 0) {
      return &g_files.files[i];
    }
  }
  return NULL;
}

// Read file content
char *read_file_content(const char *path, size_t *size) {
  FILE *f = fopen(path, "rb");
  if (!f)
    return NULL;

  fseek(f, 0, SEEK_END);
  *size = ftell(f);
  fseek(f, 0, SEEK_SET);

  char *content = (char *)malloc(*size + 1);
  fread(content, 1, *size, f);
  content[*size] = '\0';
  fclose(f);

  return content;
}

// Add file to global list
void add_file(const char *path) {
  // Resolve to canonical path first
  char resolved[MAX_PATH];
  char *real = realpath(path, NULL);
  if (real) {
    strncpy(resolved, real, MAX_PATH - 1);
    resolved[MAX_PATH - 1] = '\0';
    free(real);
  } else {
    // If realpath fails, use original path
    strncpy(resolved, path, MAX_PATH - 1);
    resolved[MAX_PATH - 1] = '\0';
  }

  // Check if already exists with canonical path
  if (find_file(resolved))
    return;

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

// Recursively scan directory for .as files
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

// Extract includes from file content, ignoring those in comments or strings
void extract_includes(SourceFile *file, char includes[][MAX_PATH],
                      int *include_count) {
  *include_count = 0;

  // Track parsing state
  int in_block_comment = 0;
  int in_line_comment = 0;
  int in_string = 0;
  int in_char = 0;
  char string_char = 0; // '"' or '\''

  const char *p = file->content;

  while (*p) {
    // Handle block comments first
    if (in_block_comment) {
      if (p[0] == '*' && p[1] == '/') {
        in_block_comment = 0;
        p += 2;
      } else {
        p++;
      }
      continue;
    }

    // Handle line comments
    if (in_line_comment) {
      if (*p == '\n') {
        in_line_comment = 0;
      }
      p++;
      continue;
    }

    // Handle strings and character literals
    if (in_string || in_char) {
      if (*p == '\\' && p[1]) {
        // Escape sequence - skip next character
        p += 2;
      } else if (*p == string_char) {
        in_string = 0;
        in_char = 0;
        p++;
      } else {
        p++;
      }
      continue;
    }

    // Check for comment/string starters
    if (p[0] == '/' && p[1] == '*') {
      in_block_comment = 1;
      p += 2;
      continue;
    }

    if (p[0] == '/' && p[1] == '/') {
      in_line_comment = 1;
      p += 2;
      continue;
    }

    if (*p == '"') {
      in_string = 1;
      string_char = '"';
      p++;
      continue;
    }

    if (*p == '\'') {
      in_char = 1;
      string_char = '\'';
      p++;
      continue;
    }

    // Look for #include "..." (only when not in comment/string)
    if (p[0] == '#' && strncmp(p, "#include", 8) == 0) {
      // Skip whitespace after #include
      const char *after_include = p + 8;
      while (*after_include == ' ' || *after_include == '\t') {
        after_include++;
      }

      // Check for opening quote
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

// Process file and its dependencies recursively
void process_file(SourceFile *file, const char *chain[], int chain_len) {
  if (file->processed)
    return;

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

  // Get directory of current file
  char file_dir[MAX_PATH];
  get_dirname(file->path, file_dir);

  // Extract and process includes
  char includes[MAX_INCLUDES][MAX_PATH];
  int include_count;
  extract_includes(file, includes, &include_count);

  const char *new_chain[128];
  for (int i = 0; i < chain_len; i++)
    new_chain[i] = chain[i];
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

  // Add to processing order
  if (g_order.count >= MAX_FILES) {
    fprintf(stderr, "Error: Too many files\n");
    exit(1);
  }
  g_order.paths =
      (char **)realloc(g_order.paths, (g_order.count + 1) * sizeof(char *));
  g_order.paths[g_order.count++] = file->path;
}

// Remove comment-only lines from content (keeps inline comments and code)
char *strip_comments(const char *content) {
  size_t size = strlen(content);
  char *result = (char *)malloc(size + 1);
  char *dst = result;
  const char *src = content;
  
  while (*src) {
    const char *line_start = src;
    const char *check = src;
    int in_string = 0;
    char string_char = '\0';
    
    // Skip leading whitespace
    while (*check == ' ' || *check == '\t') {
      check++;
    }
    
    // Check if line starts with a comment
    if (*check == '/' && *(check + 1) == '/') {
      // Single-line comment - skip entire line
      while (*src && *src != '\n') {
        src++;
      }
      if (*src == '\n') {
        src++;
      }
      continue;
    } else if (*check == '/' && *(check + 1) == '*') {
      // Multi-line comment at start of line
      const char *comment_end = check + 2;
      while (*comment_end && !(*comment_end == '*' && *(comment_end + 1) == '/')) {
        comment_end++;
      }
      if (*comment_end == '*' && *(comment_end + 1) == '/') {
        comment_end += 2;
      }
      
      // Check if there's any code after the comment on the last line of the comment
      const char *after = comment_end;
      while (*after == ' ' || *after == '\t') {
        after++;
      }
      
      if (*after == '\n' || *after == '\r' || *after == '\0') {
        // Comment-only - skip to after the comment and skip the newline(s)
        src = comment_end;
        while (*src == ' ' || *src == '\t') {
          src++;
        }
        if (*src == '\r') src++;
        if (*src == '\n') {
          src++;
        }
        continue;
      }
    }
    
    // This line has code - copy it but check for inline comments
    while (*src && *src != '\n') {
      if (!in_string) {
        if (*src == '"' || *src == '\'') {
          in_string = 1;
          string_char = *src;
          *dst++ = *src++;
        } else if (*src == '/' && *(src + 1) == '/') {
          // Inline single-line comment - keep it
          *dst++ = *src++;
        } else if (*src == '/' && *(src + 1) == '*') {
          // Inline multi-line comment - keep it
          *dst++ = *src++;
        } else {
          *dst++ = *src++;
        }
      } else {
        // Inside string
        if (*src == string_char) {
          const char *ck = src - 1;
          int escape_count = 0;
          while (ck >= content && *ck == '\\') {
            escape_count++;
            ck--;
          }
          if (escape_count % 2 == 0) {
            in_string = 0;
          }
        }
        *dst++ = *src++;
      }
    }
    
    if (*src == '\r') {
      *dst++ = *src++;
    }
    if (*src == '\n') {
      *dst++ = *src++;
    }
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

      // Check if line contains #include
      if (!strstr(line, "#include")) {
        memcpy(dst, line_start, line_len);
        dst += line_len;
      }

      line_start = src + 1;
    }
    src++;
  }

  // Handle last line (without newline)
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

void print_help(const char *program_name) {
  printf("Usage: %s [OPTIONS] <source_dir|file.as> [file2.as ...]\n",
         program_name);
  printf("\nOptions:\n");
  printf("  -o <file>    Output bundled code to specified file\n");
  printf("  --strip      Strip comments from bundled output\n");
  printf("  --help       Show this help message\n");
  printf("\nExamples:\n");
  printf("  %s src/\n", program_name);
  printf("  %s -o bundle.as src/\n", program_name);
  printf("  %s -o output.as --strip src/\n", program_name);
  printf("  %s -o output.as main.as utils.as\n", program_name);
  printf("\nWithout -o, only errors and warnings are displayed.\n");
}

// FileIndex structure used by helper functions
typedef struct {
  SourceFile *file;
  char rel_path[MAX_PATH];
  int start_line;
  int end_line;
} FileIndex;

// Helper to extract a single line from content
void get_line_at(const char *content, int line_num, char *output,
                 size_t max_len) {
  int current = 1;
  const char *p = content;
  output[0] = '\0';

  while (*p && current < line_num) {
    if (*p == '\n')
      current++;
    p++;
  }

  if (current == line_num) {
    const char *line_start = p;
    while (*p && *p != '\n')
      p++;
    size_t len = p - line_start;
    if (len < max_len - 1) {
      strncpy(output, line_start, len);
      output[len] = '\0';
    }
  }
}

// Trim leading whitespace from a line
void trim_leading_ws(char *str) {
  char *start = str;
  while (*start == ' ' || *start == '\t')
    start++;
  if (start != str) {
    memmove(str, start, strlen(start) + 1);
  }
}

// Find the line in source file by searching for matching lines with expanding
// context Uses alternating up/down search: try error line, then error-1,
// error+1, error-2, error+2, etc.
int find_line_in_source(SourceFile *file, const char *bundled_content,
                        int error_bundled_line, FileIndex *file_index) {
  // First, extract context lines from bundled around the error
  // We'll build up context until we find a unique match

  int max_context = 50;       // Maximum lines of context to try
  char bundled_context[5120]; // 50 lines * ~100 chars each
  int context_line_count = 0;
  int *context_line_offsets = (int *)malloc(max_context * 2 * sizeof(int));
  char **context_lines = (char **)malloc(max_context * 2 * sizeof(char *));

  // Get the starting line in bundled for this file
  int file_start_offset = file_index->start_line -
                          1; // Convert to 0-based offset from bundled start

  // Extract context lines by alternating up and down from error
  int bundled_offset = error_bundled_line - 1; // 0-based

  for (int radius = 0;
       radius < max_context && context_line_count < max_context * 2;
       radius++) {
    // Add line from above (error - radius)
    if (radius > 0) {
      int above_line = error_bundled_line - radius;
      if (above_line >= file_index->start_line) {
        char line[MAX_LINE];
        get_line_at(bundled_content, above_line, line, sizeof(line));
        trim_leading_ws(line);
        if (strlen(line) > 0) {
          context_lines[context_line_count] = strdup(line);
          context_line_offsets[context_line_count] =
              above_line - file_index->start_line +
              1; // Relative to file start
          context_line_count++;
        }
      }
    }

    // Add line from below (error + radius)
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

    // Try to find this sequence in source
    // Search for the first unique line in our context
    for (int i = 0; i < context_line_count; i++) {
      if (strlen(context_lines[i]) == 0)
        continue;

      // Count occurrences of this line in source
      int occurrences = 0;
      int found_line = 0;
      int current_line = 1;
      const char *p = file->content;

      while (*p) {
        const char *line_start = p;
        while (*p && *p != '\n')
          p++;

        char source_line[MAX_LINE];
        size_t len = p - line_start;
        if (len < sizeof(source_line) - 1) {
          strncpy(source_line, line_start, len);
          source_line[len] = '\0';
          trim_leading_ws(source_line);

          if (strcmp(source_line, context_lines[i]) == 0) {
            if (occurrences == 0)
              found_line = current_line;
            occurrences++;
          }
        }

        current_line++;
        if (*p == '\n')
          p++;
      }

      // If unique match found, calculate the error line
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

          // Cleanup
          for (int k = 0; k < context_line_count; k++) {
            free(context_lines[k]);
          }
          free(context_line_offsets);
          free(context_lines);

          return error_line;
        }
      }
    }
  }

  // Cleanup
  for (int k = 0; k < context_line_count; k++) {
    free(context_lines[k]);
  }
  free(context_line_offsets);
  free(context_lines);

  return 0; // Could not find unique match
}


int main(int argc, char **argv) {
  const char *output_file = NULL;

  // Parse arguments
  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "--help") == 0) {
      print_help(argv[0]);
      return 0;
    } else if (strcmp(argv[i], "--strip") == 0) {
      g_strip_comments = 1;
    } else if (strcmp(argv[i], "-o") == 0) {
      if (i + 1 < argc) {
        output_file = argv[++i];
        g_verbose = 1;
      } else {
        fprintf(stderr, "Error: -o requires a filename argument\n");
        return 1;
      }
    }
  }

  // Find first non-option argument (source file/directory)
  int first_source = 1;
  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-o") == 0) {
      i++; // Skip the next argument too
    } else if (strcmp(argv[i], "--strip") == 0) {
      // Skip this flag
    } else if (argv[i][0] != '-') {
      first_source = i;
      break;
    }
  }

  if (first_source >= argc) {
    print_help(argv[0]);
    return 1;
  }

  // Remember the input directory for relative path calculations
  if (is_directory(argv[first_source])) {
    char *real = realpath(argv[first_source], NULL);
    if (real) {
      strncpy(g_input_dir, real, MAX_PATH - 1);
      g_input_dir[MAX_PATH - 1] = '\0';
      free(real);
    }
  }

  // Scan directories and collect files
  for (int i = first_source; i < argc; i++) {
    if (strcmp(argv[i], "-o") == 0) {
      i++; // Skip output file
      continue;
    }

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

  // Process all files to resolve dependencies
  const char *empty_chain[1] = {NULL};
  for (int i = 0; i < g_files.count; i++) {
    if (!g_files.files[i].processed) {
      process_file(&g_files.files[i], empty_chain, 0);
    }
  }

  // Create a preprocessed bundle where includes are inlined
  size_t total_size = 0;
  for (int i = 0; i < g_order.count; i++) {
    SourceFile *f = find_file(g_order.paths[i]);
    total_size += f->size + 256; // Extra space for comments
  }

  char *bundled = (char *)malloc(total_size);
  char *ptr = bundled;

  for (int i = 0; i < g_order.count; i++) {
    SourceFile *f = find_file(g_order.paths[i]);
    char *content = strip_includes(f->content);
    
    // Strip comments if requested
    char *processed_content = content;
    if (g_strip_comments) {
      processed_content = strip_comments(content);
      free(content);
      content = processed_content;
    }

    // Add file marker comment with relative path from input directory
    char rel_path[MAX_PATH];
    get_relative_path(f->path, rel_path);
    ptr += sprintf(ptr, "// === %s ===\n", rel_path);

    // Add content
    strcpy(ptr, content);
    ptr += strlen(content);

    // Ensure ends with newline
    if (ptr > bundled && *(ptr - 1) != '\n') {
      *ptr++ = '\n';
    }
    *ptr++ = '\n';

    free(content);
  }
  *ptr = '\0';

  // Validate bundled script
  void *validator = as_validator_create();
  if (!validator) {
    fprintf(stderr, "Error: Could not create validator\n");
    free(bundled);
    return 1;
  }

  ASErrorList *errors = as_error_list_create();

  // Add the bundled content as a single section
  if (as_add_section(validator, bundled, "bundled") < 0) {
    fprintf(stderr, "Error: Could not add bundled script\n");
    as_error_list_destroy(errors);
    as_validator_destroy(validator);
    free(bundled);
    return 1;
  }

  FileIndex *index = (FileIndex *)malloc(g_order.count * sizeof(FileIndex));
  int index_count = 0;
  int current_bundled_line = 1;

  const char *bp = bundled;
  while (*bp) {
    // Check if this line is a separator comment
    // Save current position to move past this line later
    const char *line_start = bp;
    const char *p = bp;

    // Skip leading whitespace
    while (*p == ' ' || *p == '\t')
      p++;

    if (strncmp(p, "// === ", 7) == 0 && index_count < g_order.count) {
      const char *sep_end = strstr(p + 7, " ===");
      if (sep_end) {
        // End the previous file's range
        if (index_count > 0) {
          index[index_count - 1].end_line = current_bundled_line;
        }

        // Extract the path (between "// === " and " ===")
        const char *path_start = p + 7;
        size_t path_len = sep_end - path_start;
        if (path_len < MAX_PATH - 1) {
          strncpy(index[index_count].rel_path, path_start, path_len);
          index[index_count].rel_path[path_len] = '\0';
        }

        // Find the corresponding source file
        index[index_count].file = NULL;
        for (int j = 0; j < g_files.count; j++) {
          char rel_path[MAX_PATH];
          get_relative_path(g_files.files[j].path, rel_path);
          if (strcmp(rel_path, index[index_count].rel_path) == 0) {
            index[index_count].file = &g_files.files[j];
            break;
          }
        }

        // Start the new file's range after the separator line
        index[index_count].start_line = current_bundled_line + 1;
        index_count++; // Important: increment the index count!
      }
    }

    // Move to next line
    while (*bp && *bp != '\n')
      bp++;
    if (*bp == '\n') {
      current_bundled_line++;
      bp++;
    }
  }

  // Set end line for the last file
  if (index_count > 0) {
    index[index_count - 1].end_line = current_bundled_line;
  }

  int validation_failed = 0;
  if (as_build(validator, errors) < 0) {
    validation_failed = 1;
    fprintf(stderr, "\nValidation failed with %d error(s):\n\n",
            errors->error_count);

    for (int i = 0; i < errors->error_count; i++) {
      // Find which file contains this error line
      SourceFile *found_file = NULL;
      char file_path[MAX_PATH] = "bundled";
      int file_idx = -1;

      for (int j = 0; j < g_order.count; j++) {
        if (errors->errors[i].line >= index[j].start_line &&
            errors->errors[i].line < index[j].end_line) {
          found_file = index[j].file;
          strcpy(file_path, index[j].rel_path);
          file_idx = j;
          break;
        }
      }

      int original_line = 0;
      if (found_file && file_idx >= 0) {
        // Try to find this line in the source file using expanding context
        original_line = find_line_in_source(
            found_file, bundled, errors->errors[i].line, &index[file_idx]);
      }

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
    fprintf(stderr, "\n");
  } else if (errors->error_count > 0) {
    // Warnings only
    fprintf(stderr, "\nValidation succeeded with %d warning(s):\n\n",
            errors->error_count);

    for (int i = 0; i < errors->error_count; i++) {
      // Find which file contains this error line
      SourceFile *found_file = NULL;
      char file_path[MAX_PATH] = "bundled";
      int file_idx = -1;

      for (int j = 0; j < g_order.count; j++) {
        if (errors->errors[i].line >= index[j].start_line &&
            errors->errors[i].line < index[j].end_line) {
          found_file = index[j].file;
          strcpy(file_path, index[j].rel_path);
          file_idx = j;
          break;
        }
      }

      int original_line = 0;
      if (found_file && file_idx >= 0) {
        original_line = find_line_in_source(
            found_file, bundled, errors->errors[i].line, &index[file_idx]);
      }

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
    fprintf(stderr, "\n");
  }

  free(index);

  // Write output only if -o flag was specified
  if (g_verbose && output_file) {
    FILE *out = fopen(output_file, "wb");
    if (!out) {
      fprintf(stderr, "Error: Could not write to '%s'\n", output_file);
      as_error_list_destroy(errors);
      as_validator_destroy(validator);
      free(bundled);
      return 1;
    }

    fwrite(bundled, 1, strlen(bundled), out);
    fclose(out);
  }

  // Cleanup
  for (int i = 0; i < g_files.count; i++) {
    free(g_files.files[i].content);
  }
  free(g_files.files);
  free(g_order.paths);
  free(bundled);
  as_error_list_destroy(errors);
  as_validator_destroy(validator);

  return validation_failed ? 1 : 0;
}
