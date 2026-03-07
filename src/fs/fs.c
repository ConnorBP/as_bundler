#include "fs.h"
#include <time.h>

/* -------------------------------------------------------------------------
 * Path helpers
 * ---------------------------------------------------------------------- */

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
    char *last_slash     = strrchr(out, '/');
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
    const char *backslash     = strrchr(path, '\\');
    const char *filename = forward_slash > backslash ? forward_slash : backslash;
    if (filename) {
        filename++;
    } else {
        filename = path;
    }
    strcpy(out, filename);
    size_t len = strlen(out);
    if (len > 3 && strcmp(out + len - 3, ".as") == 0)
        out[len - 3] = '\0';
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
    while (rel[0] == '/' && base[strlen(base) - 1] == '/')
        rel++;
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
            /* skip */
        } else if (strcmp(token, "..") == 0) {
            if (part_count > 0) part_count--;
        } else {
            parts[part_count++] = strdup(token);
        }
        token = strtok(NULL, "/");
    }
    out[0] = '\0';
    if (input[0] == '/' || input[0] == '\\')
        strcat(out, "/");
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

/* -------------------------------------------------------------------------
 * File list management
 * ---------------------------------------------------------------------- */

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
        g_files.files    = (SourceFile *)realloc(
            g_files.files, g_files.capacity * sizeof(SourceFile));
    }
    SourceFile *sf = &g_files.files[g_files.count++];
    strncpy(sf->path, resolved, MAX_PATH - 1);
    sf->content    = read_file_content(resolved, &sf->size);
    sf->processed  = 0;
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
        if (strcmp(entry->d_name, ".") == 0 ||
            strcmp(entry->d_name, "..") == 0)
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

/* -------------------------------------------------------------------------
 * Include extraction and dependency resolution
 * ---------------------------------------------------------------------- */

void extract_includes(SourceFile *file, char includes[][MAX_PATH],
                      int *include_count) {
    *include_count = 0;
    int  in_block_comment = 0;
    int  in_line_comment  = 0;
    int  in_string = 0;
    int  in_char   = 0;
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
        if (p[0] == '/' && p[1] == '/') { in_line_comment  = 1; p += 2; continue; }
        if (*p == '"')  { in_string = 1; string_char = '"';  p++; continue; }
        if (*p == '\'') { in_char   = 1; string_char = '\''; p++; continue; }
        if (p[0] == '#' && strncmp(p, "#include", 8) == 0) {
            const char *after_include = p + 8;
            while (*after_include == ' ' || *after_include == '\t') after_include++;
            if (*after_include == '"') {
                after_include++;
                const char *quote_end = strchr(after_include, '"');
                if (quote_end) {
                    size_t path_len = (size_t)(quote_end - after_include);
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
    int  include_count;
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
    file->processed  = 1;

    if (g_order.count >= MAX_FILES) {
        fprintf(stderr, "Error: Too many files\n");
        exit(1);
    }
    g_order.paths = (char **)realloc(g_order.paths,
                                     (size_t)(g_order.count + 1) * sizeof(char *));
    g_order.paths[g_order.count++] = file->path;
}
