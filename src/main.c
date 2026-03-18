#include "as_wrapper.h"
#include "common/common.h"
#include "fs/fs.h"
#include "processing/processing.h"
#include "preprocessor/preprocessor.h"
#include "obfuscator/obfuscator.h"
#include "errors/errors.h"
#include "commands/commands.h"

/* -------------------------------------------------------------------------
 * Help text
 * ---------------------------------------------------------------------- */

#ifndef BUNDLER_VERSION
#define BUNDLER_VERSION "unknown"
#endif

static void print_help(const char *program_name) {
    printf("Usage: %s [OPTIONS] <source_dir|file.as> [file2.as ...]\n",
           program_name);
    printf("       %s <subcommand> [subcommand options]\n", program_name);
    printf("\nSubcommands:\n");
    printf("  restore <bundled.as> [out_dir]  Restore source tree from bundled file\n");
    printf("  install                         Install gcc and register pcx in PATH\n");
    printf("  update                          Update pcx to the latest GitHub release\n");
    printf("  watch [options] <src...>        Re-bundle automatically on file save\n");
    printf("\nOptions:\n");
    printf("  -v, --version          Print the bundler version\n");
    printf("  -o <file>              Output bundled code to specified file\n");
    printf("  --strip, -s            Strip comment-only lines from output\n");
    printf("  --no-preprocess, -n    Skip the C preprocessor step\n");
    printf("  --prepend, -p <file>   Prepend file before preprocessing "
           "(macro definitions)\n");
    printf("  --header, -H <file>    Prepend raw content to final output "
           "(skips preprocessing)\n");
    printf("  -D<NAME>[=VALUE]       Define a preprocessor macro "
           "(repeatable)\n");
    printf("  --obfuscate, -O        Rename user-defined identifiers with\n"
           "                         short names; keep all API / type names\n");
    printf("  --remove-newlines, -R  Collapse whitespace to minimum spaces\n"
           "                         (best combined with --obfuscate)\n");
    printf("  --scramble-strings, -S Replace string literals with runtime\n"
           "                         hash-map lookups; values stored as\n"
           "                         XOR-encrypted byte arrays\n");
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
    printf("  %s -o out.as --obfuscate --remove-newlines src/\n", program_name);
    printf("  %s -o out.as --obfuscate --scramble-strings --remove-newlines src/\n",
           program_name);
    printf("\nWithout -o, only validation errors and warnings are displayed.\n");
}

/* -------------------------------------------------------------------------
 * Entry point
 * ---------------------------------------------------------------------- */

int main(int argc, char **argv) {
    /* ---------------------------------------------------------------
     * Subcommand dispatch
     * ------------------------------------------------------------- */
    if (argc >= 2) {
        const char *sub = argv[1];

        if (strcmp(sub, "restore") == 0) {
            /* Pass: argv[1]="restore", argv[2..] = restore args */
            return cmd_restore(argc - 1, argv + 1);
        } else if (strcmp(sub, "install") == 0) {
            return cmd_install(argc - 1, argv + 1);
        } else if (strcmp(sub, "update") == 0) {
            return cmd_update(argc - 1, argv + 1);
        } else if (strcmp(sub, "watch") == 0) {
            return cmd_watch(argc - 1, argv + 1);
        }
    }

    const char *output_file = NULL;

    /* --- Argument parsing --- */
    int first_source = -1;
    for (int i = 1; i < argc; i++) {
        const char *arg = argv[i];

        if (strcmp(arg, "--help") == 0) {
            print_help(argv[0]);
            return 0;
        } else if (strcmp(arg, "-v") == 0 || strcmp(arg, "--version") == 0) {
            printf("%s\n", BUNDLER_VERSION);
            return 0;
        } else if (strcmp(arg, "--strip") == 0 || strcmp(arg, "-s") == 0) {
            g_strip_comments = 1;
        } else if (strcmp(arg, "--no-preprocess") == 0 || strcmp(arg, "-n") == 0) {
            g_skip_preprocess = 1;
        } else if (strcmp(arg, "--obfuscate") == 0 || strcmp(arg, "-O") == 0) {
            g_obfuscate = 1;
        } else if (strcmp(arg, "--remove-newlines") == 0 || strcmp(arg, "-R") == 0) {
            g_remove_newlines = 1;
        } else if (strcmp(arg, "--scramble-strings") == 0 || strcmp(arg, "-S") == 0) {
            g_scramble_strings = 1;
        } else if (strcmp(arg, "-o") == 0) {
            if (i + 1 < argc) {
                output_file = argv[++i];
                g_verbose   = 1;
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

    /* --- Log active options --- */
    if (g_strip_comments)   printf("Option: Stripping comment-only lines\n");
    if (g_skip_preprocess)  printf("Option: Skipping C preprocessor\n");
    if (g_obfuscate)        printf("Option: Obfuscating identifiers\n");
    if (g_remove_newlines)  printf("Option: Removing newlines\n");
    if (g_scramble_strings) printf("Option: Scrambling string literals\n");
    if (g_prepend_file[0])  printf("Option: Prepend file: %s\n", g_prepend_file);
    if (g_header_file[0])   printf("Option: Header file: %s\n", g_header_file);
    if (g_define_count > 0) {
        printf("Defines:");
        for (int i = 0; i < g_define_count; i++) printf(" %s", g_defines[i]);
        printf("\n");
    }

    /* --- Remember input directory for relative path calculations --- */
    if (is_directory(argv[first_source])) {
        char *real = realpath(argv[first_source], NULL);
        if (real) {
            strncpy(g_input_dir, real, MAX_PATH - 1);
            g_input_dir[MAX_PATH - 1] = '\0';
            free(real);
        }
        printf("Scanning directory: %s\n", argv[first_source]);
    }

    /* --- Collect source files --- */
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

    /* --- Step 1: Resolve dependency order --- */
    const char *empty_chain[1] = {NULL};
    for (int i = 0; i < g_files.count; i++) {
        if (!g_files.files[i].processed)
            process_file(&g_files.files[i], empty_chain, 0);
    }

    /* --- Step 2: Combine files with #pragma BUNDLER_FILE markers --- */
    size_t total_size = 65536;
    for (int i = 0; i < g_order.count; i++) {
        SourceFile *f = find_file(g_order.paths[i]);
        total_size += f->size + 256;
    }

    char *combined = (char *)malloc(total_size);
    char *ptr      = combined;

    for (int i = 0; i < g_order.count; i++) {
        SourceFile *f       = find_file(g_order.paths[i]);
        char       *content = strip_includes(f->content);

        char rel_path[MAX_PATH];
        get_relative_path(f->path, rel_path);

        ptr += sprintf(ptr, "#pragma BUNDLER_FILE %s\n", rel_path);

        size_t content_len = strlen(content);
        if ((size_t)(ptr - combined) + content_len + 4 > total_size) {
            size_t used  = (size_t)(ptr - combined);
            total_size   = used + content_len + 65536;
            combined     = (char *)realloc(combined, total_size);
            ptr          = combined + used;
        }

        strcpy(ptr, content);
        ptr += content_len;
        if (ptr > combined && *(ptr - 1) != '\n') *ptr++ = '\n';
        *ptr++ = '\n';
        free(content);
    }
    *ptr = '\0';

    /* --- Step 3: Prepend macro file (strip STRHASH defs so CPP won't see them) --- */
    if (g_prepend_file[0] != '\0') {
        size_t pre_size;
        char  *pre_content = read_file_content(g_prepend_file, &pre_size);
        if (!pre_content) {
            fprintf(stderr, "Error: Could not read prepend file '%s'\n",
                    g_prepend_file);
            free(combined);
            return 1;
        }
        pre_content = strip_strhash_macros(pre_content);
        size_t pre_len      = strlen(pre_content);
        size_t combined_len = strlen(combined);
        char  *new_combined = (char *)malloc(pre_len + combined_len + 3);
        memcpy(new_combined, pre_content, pre_len);
        new_combined[pre_len]     = '\n';
        new_combined[pre_len + 1] = '\n';
        memcpy(new_combined + pre_len + 2, combined, combined_len + 1);
        free(pre_content);
        free(combined);
        combined = new_combined;
        printf("Prepended: %s\n", g_prepend_file);
    }

    /* --- Step 4: Apply build timestamp macros --- */
    combined = apply_timestamp_macros(combined);

    /* --- Step 5: Evaluate STRHASH() and STRHASH64() calls --- */
    combined = evaluate_string_macros(combined, "STRHASH64", 1);
    combined = evaluate_string_macros(combined, "STRHASH",   0);

    /* --- Step 6: Run C preprocessor --- */
    if (!g_skip_preprocess) {
        printf("Running C preprocessor (single pass)...\n");
        char *preprocessed = run_preprocessor(combined);
        free(combined);
        combined = preprocessed;
        printf("Preprocessor complete\n");
    }

    /* --- Step 8: Convert #pragma BUNDLER_FILE markers to // === ... === --- */
    combined = convert_pragma_markers(combined);

    /* --- Step 9: Prepend raw header file (never preprocessed) --- */
    if (g_header_file[0] != '\0') {
        size_t hdr_size;
        char  *hdr_content = read_file_content(g_header_file, &hdr_size);
        if (!hdr_content) {
            fprintf(stderr, "Error: Could not read header file '%s'\n",
                    g_header_file);
            free(combined);
            return 1;
        }
        size_t hdr_len  = strlen(hdr_content);
        size_t comb_len = strlen(combined);
        char  *new_combined = (char *)malloc(hdr_len + comb_len + 3);
        memcpy(new_combined, hdr_content, hdr_len);
        new_combined[hdr_len]     = '\n';
        new_combined[hdr_len + 1] = '\n';
        memcpy(new_combined + hdr_len + 2, combined, comb_len + 1);
        free(hdr_content);
        free(combined);
        combined = new_combined;
        printf("Header prepended: %s\n", g_header_file);
    }

    /* --- Step 10: Validate bundled script --- */
    void *validator = as_validator_create();
    if (!validator) {
        fprintf(stderr, "Error: Could not create validator\n");
        free(combined);
        return 1;
    }

    /* Grab protected API name list for obfuscator before validation changes state */
    ASNameList *api_names = g_obfuscate ? as_get_registered_names(validator) : NULL;
    if (api_names) printf("Loaded %d protected API name(s)\n", api_names->count);

    ASErrorList *errors = as_error_list_create();

    if (as_add_section(validator, combined, "bundled") < 0) {
        fprintf(stderr, "Error: Could not add bundled script\n");
        as_error_list_destroy(errors);
        as_validator_destroy(validator);
        free(combined);
        return 1;
    }

    /* Build file index from // === ... === markers for error mapping */
    FileIndex *index       = (FileIndex *)malloc((size_t)g_order.count * sizeof(FileIndex));
    int        index_count = 0;
    int        current_bundled_line = 1;

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
                size_t      path_len   = (size_t)(sep_end - path_start);
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

    /* Report validation results */
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
        char        file_path[MAX_PATH] = "bundled";
        int         file_idx = -1;

        for (int j = 0; j < index_count; j++) {
            if (errors->errors[i].line >= index[j].start_line &&
                errors->errors[i].line <  index[j].end_line) {
                found_file = index[j].file;
                strcpy(file_path, index[j].rel_path);
                file_idx   = j;
                break;
            }
        }

        int original_line = 0;
        if (found_file && file_idx >= 0)
            original_line = find_line_in_source(
                found_file, combined, errors->errors[i].line, &index[file_idx]);

        if (original_line > 0) {
            fprintf(stderr, "%s:%d:%d: %s\n",
                    file_path, original_line,
                    errors->errors[i].column, errors->errors[i].message);
        } else if (found_file) {
            fprintf(stderr, "%s:%d:%d: %s (could not map to exact line)\n",
                    file_path, errors->errors[i].line,
                    errors->errors[i].column, errors->errors[i].message);
        } else {
            fprintf(stderr, "bundled:%d:%d: %s\n",
                    errors->errors[i].line,
                    errors->errors[i].column, errors->errors[i].message);
        }
    }
    if (errors->error_count > 0) fprintf(stderr, "\n");

    free(index);

    /* --- Step 10a: Strip comments --- */
    if (g_strip_comments && !validation_failed) {
        char *stripped = strip_comments(combined);
        free(combined);
        combined = stripped;
    }

    /* --- Step 10b: Scramble string literals --- */
    if (g_scramble_strings && !validation_failed) {
        char *scrambled = scramble_strings(combined);
        free(combined);
        combined = scrambled;
    }

    /* --- Step 10c: Obfuscate identifiers --- */
    if (g_obfuscate && !validation_failed) {
        printf("Obfuscating identifiers%s...\n",
               g_remove_newlines ? " and removing newlines" : "");
        char *obfuscated = obfuscate_content(combined, api_names, g_remove_newlines);
        free(combined);
        combined = obfuscated;
    } else if (g_remove_newlines && !validation_failed) {
        /* Newline removal without rename */
        char *obfuscated = obfuscate_content(combined, NULL, 1);
        free(combined);
        combined = obfuscated;
    }

    if (api_names) { as_name_list_destroy(api_names); api_names = NULL; }

    /* --- Step 11: Write output --- */
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

    /* --- Cleanup --- */
    for (int i = 0; i < g_files.count; i++) free(g_files.files[i].content);
    free(g_files.files);
    free(g_order.paths);
    free(g_defines);
    free(combined);
    as_error_list_destroy(errors);
    as_validator_destroy(validator);

    return validation_failed ? 1 : 0;
}
