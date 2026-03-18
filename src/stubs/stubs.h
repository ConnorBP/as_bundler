#ifndef STUBS_H
#define STUBS_H

#include "../common/common.h"

/* Maximum number of stub files that can be registered in one build. */
#define MAX_STUBS       64
/* Maximum number of symbols that can be extracted across all stubs. */
#define MAX_STUB_SYMBOLS 8192

/* -------------------------------------------------------------------------
 * A registered stub file.
 *
 * Stub files let the bundler validate scripts that reference external
 * libraries (included at runtime via #include_lib "x.as") without
 * bundling the library source into the output.
 *
 * Naming convention: a stub is stored as  "libname.stub.as".
 * The bundler strips ".stub" so the lib_name matches the directive:
 *   uncapi.stub.as  →  lib_name = "uncapi.as"
 * ---------------------------------------------------------------------- */
typedef struct {
    char  path[MAX_PATH]; /* absolute path to the .stub.as file on disk    */
    char  lib_name[256];  /* bare name used in #include_lib, e.g "unc.as"  */
    char *raw_content;    /* raw file content (owned, NULL if load failed)  */
    char *val_content;    /* validation-ready version with empty bodies     */
} Stub;

extern Stub g_stubs[MAX_STUBS];
extern int  g_stub_count;

/* -------------------------------------------------------------------------
 * Registration
 * ---------------------------------------------------------------------- */

/* Register a single .stub.as file.
 * lib_name is derived from the basename: "uncapi.stub.as" → "uncapi.as".
 * Returns 0 on success, -1 on error (file not found, table full, etc.). */
int stub_register(const char *path);

/* Register all .stub.as files found directly inside a directory.
 * Returns the number of stubs added. */
int stub_register_dir(const char *dir);

/* -------------------------------------------------------------------------
 * Loading
 * ---------------------------------------------------------------------- */

/* Read each registered stub from disk and generate val_content.
 * Must be called after all stub_register*() calls and before anything
 * that queries stub content. */
void stub_load_all(void);

/* -------------------------------------------------------------------------
 * Lookup
 * ---------------------------------------------------------------------- */

/* Find a registered stub by the name used in #include_lib.
 * Accepts both "uncapi.as" and "uncapi" as keys.
 * Returns NULL if not found. */
Stub *stub_find(const char *lib_name);

/* -------------------------------------------------------------------------
 * Symbol extraction
 * ---------------------------------------------------------------------- */

/* Collect every exported symbol name from all loaded stubs (function
 * names, class/namespace/enum/interface names, global variable names,
 * enum values).  Used to build the obfuscator's protected-name set so
 * the bundler never renames symbols that come from external libraries.
 *
 * Returns a NULL-terminated, malloc'd array of malloc'd strings.
 * Call stub_symbols_free() when done. */
char **stub_collect_symbols(void);
void   stub_symbols_free(char **syms);

/* -------------------------------------------------------------------------
 * Stub emission
 * ---------------------------------------------------------------------- */

/* Generate a .stub.as file (declarations + empty function bodies) from
 * fully-processed bundled content.  The result is suitable for
 * distribution to consumers who want to validate against your library
 * without seeing the implementation.
 *
 * Returns a new malloc'd string; caller frees. */
char *emit_stub(const char *content);

/* Write stub content to path (creates or overwrites the file). */
void write_stub_file(const char *content, const char *path);

/* -------------------------------------------------------------------------
 * Shared helper: collect declaration-level symbol names from source
 *
 * Walks `content` collecting identifiers that appear in declaration
 * positions (after class/namespace/enum/interface keywords, or immediately
 * before a `(` at brace depth <= max_depth).  When ns_filter is non-NULL,
 * only collect symbols declared inside that namespace.  When
 * file_marker is non-NULL, only collect from the // === file_marker ===
 * section.
 *
 * Appends up to max_syms entries to syms[] (NULL-terminated on exit).
 * *count is updated with the new total. ---------------------------------------------------------------------- */
void collect_decl_symbols(const char *content,
                          const char *ns_filter,    /* NULL = all */
                          const char *file_marker,  /* NULL = all */
                          char      **syms,
                          int        *count,
                          int         max_syms);

#endif /* STUBS_H */
