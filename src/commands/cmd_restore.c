#include "commands.h"
#include "../fs/fs.h"

/* -------------------------------------------------------------------------
 * cmd_restore
 *
 * Usage:  pcx restore <bundled.as> [output_dir]
 *
 * Reads a bundled .as file that was produced by the bundler and splits it
 * back into individual source files.  The section boundaries are the
 * comment markers written by convert_pragma_markers():
 *
 *   // === relative/path/to/file.as ===
 *
 * Everything between two such markers (or between the last marker and EOF)
 * is written to  <output_dir>/<relative_path>.
 *
 * If no output directory is given the files are restored into the current
 * working directory.
 *
 * NOTE: Obfuscation is NOT reversed.  When the input was produced with
 * --obfuscate the output files will still contain the short obfuscated
 * identifiers – they are just split back into the original file structure
 * so that each section lives in its own .as file again.
 * ---------------------------------------------------------------------- */

/* Forward declaration for the recursive mkdir helper below */
static int make_dirs(const char *path);

/* -------------------------------------------------------------------------
 * make_dirs – portable recursive directory creation
 * ---------------------------------------------------------------------- */
static int make_dirs(const char *path) {
    char tmp[MAX_PATH];
    strncpy(tmp, path, MAX_PATH - 1);
    tmp[MAX_PATH - 1] = '\0';

    /* Normalise separators to the platform default */
#ifdef _WIN32
    for (char *p = tmp; *p; p++)
        if (*p == '/') *p = '\\';
#else
    for (char *p = tmp; *p; p++)
        if (*p == '\\') *p = '/';
#endif

    size_t len = strlen(tmp);
    /* Strip trailing separator */
    if (len > 0 && (tmp[len - 1] == '/' || tmp[len - 1] == '\\'))
        tmp[--len] = '\0';

    for (char *p = tmp + 1; *p; p++) {
        if (*p == '/' || *p == '\\') {
            char saved = *p;
            *p = '\0';
#ifdef _WIN32
            _mkdir(tmp);
#else
            mkdir(tmp, 0755);
#endif
            *p = saved;
        }
    }

#ifdef _WIN32
    return _mkdir(tmp);
#else
    return mkdir(tmp, 0755);
#endif
}

/* -------------------------------------------------------------------------
 * write_section – write collected lines to disk, creating dirs as needed
 * ---------------------------------------------------------------------- */
static int write_section(const char *out_dir,
                          const char *rel_path,
                          const char *content,
                          size_t      content_len) {
    /* Build full destination path */
    char full_path[MAX_PATH];
    snprintf(full_path, MAX_PATH, "%s%c%s",
             out_dir, PATH_SEPARATOR, rel_path);

    /* Normalise any cross-platform separators in rel_path */
#ifdef _WIN32
    for (char *p = full_path; *p; p++)
        if (*p == '/') *p = '\\';
#else
    for (char *p = full_path; *p; p++)
        if (*p == '\\') *p = '/';
#endif

    /* Create parent directories */
    char dir_path[MAX_PATH];
    strncpy(dir_path, full_path, MAX_PATH - 1);
    dir_path[MAX_PATH - 1] = '\0';

    char *last_sep = NULL;
    for (char *p = dir_path; *p; p++)
        if (*p == PATH_SEPARATOR) last_sep = p;

    if (last_sep) {
        *last_sep = '\0';
        make_dirs(dir_path);
    }

    /* Write the file */
    FILE *f = fopen(full_path, "wb");
    if (!f) {
        fprintf(stderr, "Error: Could not write '%s': ", full_path);
        perror(NULL);
        return 1;
    }

    fwrite(content, 1, content_len, f);
    fclose(f);
    printf("  Restored: %s\n", rel_path);
    return 0;
}

/* -------------------------------------------------------------------------
 * cmd_restore – entry point
 * ---------------------------------------------------------------------- */
static void print_restore_help(void) {
    printf("Usage: pcx restore <bundled.as> [output_dir]\n"
           "\n"
           "  bundled.as   A .as file previously produced by the bundler.\n"
           "  output_dir   Directory to restore files into (default: current dir).\n"
           "\n"
           "  Each '// === relative/path/to/file.as ===' section marker in the\n"
           "  bundled file becomes a separate .as file under output_dir, with\n"
           "  the original subdirectory structure recreated automatically.\n"
           "\n"
           "Note: Obfuscated identifiers are NOT decoded – the restored source\n"
           "files will still contain the same obfuscated names.  Only the file\n"
           "structure (one section per file) is reconstructed.\n"
           "\n"
           "Examples:\n"
           "  pcx restore bundle.as\n"
           "  pcx restore bundle.as restored_src/\n");
}

int cmd_restore(int argc, char **argv) {
    /* argv[0] == "restore"
     * argv[1] == bundled file path  (or --help)
     * argv[2] == (optional) output directory */

    /* Handle --help / -h anywhere in the argument list */
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
            print_restore_help();
            return 0;
        }
    }

    if (argc < 2) {
        print_restore_help();
        return 1;
    }

    const char *bundle_path = argv[1];
    const char *out_dir     = argc >= 3 ? argv[2] : ".";

    /* ------------------------------------------------------------------
     * Read the bundled file
     * ------------------------------------------------------------------ */
    size_t bundle_size;
    char  *bundle = read_file_content(bundle_path, &bundle_size);
    if (!bundle) {
        fprintf(stderr, "Error: Could not read bundled file '%s'\n", bundle_path);
        return 1;
    }

    printf("Restoring from: %s\n", bundle_path);
    printf("Output directory: %s\n", out_dir);

    /* Ensure the output directory exists */
    make_dirs(out_dir);

    /* ------------------------------------------------------------------
     * Parse section markers:  // === <rel_path> ===
     *
     * Strategy:
     *   - Walk line by line.
     *   - When we see a marker line, flush the accumulated content for the
     *     previous section (if any), then start accumulating for the new one.
     *   - At EOF flush the last section.
     * ------------------------------------------------------------------ */
    const char *MARKER_PREFIX = "// === ";
    const char *MARKER_SUFFIX = " ===";
    size_t      PREFIX_LEN    = 7;  /* strlen("// === ") */
    size_t      SUFFIX_LEN    = 4;  /* strlen(" ===")    */

    char   current_rel_path[MAX_PATH] = {0};
    int    in_section                 = 0;
    size_t section_start              = 0;   /* byte offset into bundle */
    int    files_written              = 0;
    int    errors                     = 0;

    /* Walk the bundle byte-by-byte, line by line.  We use a simple index
     * into the buffer so there is no pointer arithmetic confusion and the
     * termination condition is unambiguous: we stop once we have consumed
     * every byte (pos == bundle_size after advancing past the last char). */
    size_t pos = 0;

    while (pos < bundle_size) {
        /* Record where this line starts */
        size_t line_start = pos;

        /* Advance to the end of the line (stop before '\n' or at EOF) */
        while (pos < bundle_size && bundle[pos] != '\n') pos++;

        /* line spans [line_start, pos), not including the '\n' */
        size_t line_len = pos - line_start;

        /* Advance past the '\n' so next iteration starts on the next line */
        if (pos < bundle_size) pos++;

        /* Trim a trailing '\r' for Windows CRLF bundles */
        size_t trimmed_len = line_len;
        if (trimmed_len > 0 && bundle[line_start + trimmed_len - 1] == '\r')
            trimmed_len--;

        /* ---- Check whether this line is a section marker ---- */
        int  is_marker = 0;
        char new_rel_path[MAX_PATH] = {0};

        if (trimmed_len > PREFIX_LEN + SUFFIX_LEN) {
            const char *lp = bundle + line_start;
            if (strncmp(lp, MARKER_PREFIX, PREFIX_LEN) == 0) {
                const char *path_start = lp + PREFIX_LEN;
                size_t      path_max   = trimmed_len - PREFIX_LEN;
                if (path_max > SUFFIX_LEN) {
                    const char *path_end = lp + trimmed_len - SUFFIX_LEN;
                    if (strncmp(path_end, MARKER_SUFFIX, SUFFIX_LEN) == 0) {
                        size_t path_len = (size_t)(path_end - path_start);
                        if (path_len > 0 && path_len < MAX_PATH - 1) {
                            strncpy(new_rel_path, path_start, path_len);
                            new_rel_path[path_len] = '\0';
                            is_marker = 1;
                        }
                    }
                }
            }
        }

        if (is_marker) {
            /* Flush the previous section before starting the new one.
             * The previous section's content runs from section_start up to
             * (but not including) the start of this marker line. */
            if (in_section && current_rel_path[0] != '\0') {
                const char *sec_ptr = bundle + section_start;
                size_t      sec_len = line_start - section_start;

                /* Strip the single leading newline that follows the marker */
                if (sec_len > 0 && *sec_ptr == '\n') { sec_ptr++; sec_len--; }
                /* Strip any trailing blank lines */
                while (sec_len > 0 &&
                       (sec_ptr[sec_len - 1] == '\n' ||
                        sec_ptr[sec_len - 1] == '\r'))
                    sec_len--;

                if (write_section(out_dir, current_rel_path,
                                  sec_ptr, sec_len) != 0)
                    errors++;
                else
                    files_written++;
            }

            /* Begin accumulating the new section.  Content starts at `pos`
             * which is already past the '\n' that terminated the marker line. */
            strncpy(current_rel_path, new_rel_path, MAX_PATH - 1);
            current_rel_path[MAX_PATH - 1] = '\0';
            section_start = pos;
            in_section    = 1;
        }
    }

    /* Flush the final section (no following marker to trigger the flush) */
    if (in_section && current_rel_path[0] != '\0') {
        const char *sec_ptr = bundle + section_start;
        size_t      sec_len = bundle_size - section_start;

        if (sec_len > 0 && *sec_ptr == '\n') { sec_ptr++; sec_len--; }
        while (sec_len > 0 &&
               (sec_ptr[sec_len - 1] == '\n' ||
                sec_ptr[sec_len - 1] == '\r'))
            sec_len--;

        if (write_section(out_dir, current_rel_path, sec_ptr, sec_len) != 0)
            errors++;
        else
            files_written++;
    }

    free(bundle);

    if (files_written == 0 && errors == 0) {
        fprintf(stderr,
                "Warning: No section markers (// === ... ===) found in '%s'.\n"
                "         Is this a valid bundled file?\n",
                bundle_path);
        return 1;
    }

    printf("\nRestore complete: %d file(s) written", files_written);
    if (errors > 0)
        printf(", %d error(s)", errors);
    printf(".\n");

    return errors > 0 ? 1 : 0;
}