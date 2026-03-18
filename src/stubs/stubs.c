#include "stubs.h"
#include "../obfuscator/obfuscator.h"
#include "../fs/fs.h"

/* -------------------------------------------------------------------------
 * Global stub registry
 * ---------------------------------------------------------------------- */

Stub g_stubs[MAX_STUBS];
int  g_stub_count = 0;

/* -------------------------------------------------------------------------
 * Internal helpers
 * ---------------------------------------------------------------------- */

/* Returns 1 if filename ends with ".stub.as" (case-sensitive). */
static int ends_with_stub_as(const char *name) {
    size_t n = strlen(name);
    return n > 8 && strcmp(name + n - 8, ".stub.as") == 0;
}

/* Derive lib_name from a stub file's basename.
 * "uncapi.stub.as" → "uncapi.as"
 * "other.as"        → "other.as"  (no .stub. component) */
static void derive_lib_name(const char *basename, char *lib_name, int lib_name_sz) {
    strncpy(lib_name, basename, (size_t)(lib_name_sz - 1));
    lib_name[lib_name_sz - 1] = '\0';
    size_t n = strlen(lib_name);
    /* Replace ".stub.as" suffix with ".as" */
    if (n > 8 && strcmp(lib_name + n - 8, ".stub.as") == 0) {
        lib_name[n - 8] = '\0';
        strncat(lib_name, ".as", (size_t)(lib_name_sz - (int)strlen(lib_name) - 1));
    }
}

/* -------------------------------------------------------------------------
 * Non-trivial token scanning helpers
 * ---------------------------------------------------------------------- */

static int is_trivial(OBFTokType t) {
    return t == OT_WHITESPACE || t == OT_NEWLINE ||
           t == OT_LINE_CMT   || t == OT_BLOCK_CMT;
}

/* Return index of next non-trivial token after `start`, or -1. */
static int next_sig(const OBFTok *toks, int n, int start) {
    for (int i = start + 1; i < n; i++)
        if (!is_trivial(toks[i].type)) return i;
    return -1;
}

/* Return index of previous non-trivial token before `start`, or -1. */
static int prev_sig(const OBFTok *toks, int start) {
    for (int i = start - 1; i >= 0; i--)
        if (!is_trivial(toks[i].type)) return i;
    return -1;
}

/* -------------------------------------------------------------------------
 * Shared symbol collection
 * ---------------------------------------------------------------------- */

void collect_decl_symbols(const char *content,
                          const char *ns_filter,
                          const char *file_marker,
                          char      **syms,
                          int        *count,
                          int         max_syms) {

    static const char *decl_kws[] = {
        "class", "interface", "enum", "namespace", "mixin", "funcdef", NULL
    };

    TokArr ta = obf_tokenize(content);

    int depth          = 0;       /* brace depth                         */
    int func_depth     = -1;      /* depth we entered at a function body */
    int in_ns          = 0;       /* 1 while inside the target namespace */
    int ns_depth       = 0;       /* depth at which ns_filter was opened */
    int in_file_sec    = 0;       /* 1 while in the target file section  */
    int active         = 1;       /* whether we're collecting right now  */
    int collecting     = 0;       /* expect next ident as a decl name    */

    /* If a namespace filter is requested, start inactive. */
    if (ns_filter)    active = 0;
    /* If a file-section filter is requested, start inactive. */
    if (file_marker)  active = 0;
    /* If both are requested, active when both conditions met. */
    if (ns_filter && file_marker) active = 0;

    char tmp[256];

    for (int i = 0; i < ta.n; i++) {
        OBFTok *tk = &ta.d[i];

        /* --- File-section marker detection (// === name ===) --- */
        if (file_marker && tk->type == OT_LINE_CMT) {
            /* Look for the pattern: // === <name> === */
            const char *s = tk->start + 2; /* skip // */
            while (*s == ' ' || *s == '\t') s++;
            if (strncmp(s, "=== ", 4) == 0) {
                s += 4;
                const char *end = strstr(s, " ===");
                if (end) {
                    size_t nlen = (size_t)(end - s);
                    char   sec[MAX_PATH];
                    if (nlen < MAX_PATH - 1) {
                        memcpy(sec, s, nlen);
                        sec[nlen] = '\0';
                        /* Match basename */
                        const char *bsec = strrchr(sec, '/');
                        if (!bsec) bsec = strrchr(sec, '\\');
                        bsec = bsec ? bsec + 1 : sec;

                        const char *bfm = strrchr(file_marker, '/');
                        if (!bfm) bfm = strrchr(file_marker, '\\');
                        bfm = bfm ? bfm + 1 : file_marker;

                        in_file_sec = (strcmp(bsec, bfm) == 0);
                        if (!ns_filter) active = in_file_sec;
                    }
                }
            }
            continue;
        }

        if (is_trivial(tk->type)) continue;

        /* --- Brace depth + function-body tracking --- */
        if (tk->type == OT_PUNCT && tk->len == 1) {
            char ch = tk->start[0];
            if (ch == '{') {
                /* Check if this opens a function body: prev sig token is ')' */
                int pi = prev_sig(ta.d, i);
                int is_func_body = (pi >= 0 &&
                                    ta.d[pi].type == OT_PUNCT &&
                                    ta.d[pi].start[0] == ')');
                depth++;
                if (is_func_body && func_depth < 0) func_depth = depth;

                /* Namespace-filter: detect opening of target namespace */
                if (ns_filter && !in_ns && depth == 1) {
                    /* Look back for: namespace <ns_filter> { */
                    int ni = prev_sig(ta.d, i);
                    if (ni >= 0 && ta.d[ni].type == OT_IDENT &&
                        (int)strlen(ns_filter) == ta.d[ni].len &&
                        strncmp(ta.d[ni].start, ns_filter, (size_t)ta.d[ni].len) == 0) {
                        int ki = prev_sig(ta.d, ni);
                        if (ki >= 0 && ta.d[ki].type == OT_IDENT &&
                            ta.d[ki].len == 9 &&
                            strncmp(ta.d[ki].start, "namespace", 9) == 0) {
                            in_ns    = 1;
                            ns_depth = depth;
                            if (!file_marker || in_file_sec) active = 1;
                        }
                    }
                }
            } else if (ch == '}') {
                if (func_depth > 0 && depth == func_depth) func_depth = -1;
                if (in_ns && depth == ns_depth) {
                    in_ns  = 0;
                    active = 0;
                }
                depth--;
            }
            collecting = 0;
            continue;
        }

        /* --- Skip inside function bodies --- */
        if (func_depth > 0) { collecting = 0; continue; }
        /* --- Only collect at reasonable depth --- */
        if (depth > 2) { collecting = 0; continue; }
        /* --- Only collect when active --- */
        if (!active)   { collecting = 0; continue; }

        if (tk->type != OT_IDENT) { collecting = 0; continue; }

        /* If we're in "collecting" state, this ident is the decl name */
        if (collecting) {
            int l = tk->len < 255 ? tk->len : 255;
            memcpy(tmp, tk->start, (size_t)l); tmp[l] = '\0';
            int dup = 0;
            for (int d = 0; d < *count; d++)
                if (strcmp(syms[d], tmp) == 0) { dup = 1; break; }
            if (!dup && *count < max_syms - 1) syms[(*count)++] = strdup(tmp);
            collecting = 0;
            continue;
        }

        /* Check if this is a declaration keyword */
        int is_dk = 0;
        for (int k = 0; decl_kws[k]; k++) {
            size_t kwl = strlen(decl_kws[k]);
            if ((size_t)tk->len == kwl &&
                strncmp(tk->start, decl_kws[k], kwl) == 0) {
                is_dk = 1; break;
            }
        }
        if (is_dk) { collecting = 1; continue; }

        /* Check if this ident is immediately followed by '(' → function name */
        int ni = next_sig(ta.d, ta.n, i);
        if (ni >= 0 && ta.d[ni].type == OT_PUNCT && ta.d[ni].start[0] == '(') {
            int l = tk->len < 255 ? tk->len : 255;
            memcpy(tmp, tk->start, (size_t)l); tmp[l] = '\0';
            int dup = 0;
            for (int d = 0; d < *count; d++)
                if (strcmp(syms[d], tmp) == 0) { dup = 1; break; }
            if (!dup && *count < max_syms - 1) syms[(*count)++] = strdup(tmp);
            continue;
        }

        /* Global/class-scope variable: ident at depth 0 or 1 that is
         * followed by another ident (the variable name).  We collect
         * the second ident (the name, not the type). */
        if (depth <= 1 && ni >= 0 && ta.d[ni].type == OT_IDENT) {
            /* Check: the ident after the second ident is = or ; or ( or , */
            int ni2 = next_sig(ta.d, ta.n, ni);
            if (ni2 >= 0 && ta.d[ni2].type == OT_PUNCT) {
                char ch2 = ta.d[ni2].start[0];
                if (ch2 == '=' || ch2 == ';' || ch2 == ',' || ch2 == ')') {
                    int l = ta.d[ni].len < 255 ? ta.d[ni].len : 255;
                    memcpy(tmp, ta.d[ni].start, (size_t)l); tmp[l] = '\0';
                    int dup = 0;
                    for (int d = 0; d < *count; d++)
                        if (strcmp(syms[d], tmp) == 0) { dup = 1; break; }
                    if (!dup && *count < max_syms - 1) syms[(*count)++] = strdup(tmp);
                }
            }
        }
    }

    free(ta.d);
}

/* -------------------------------------------------------------------------
 * Registration
 * ---------------------------------------------------------------------- */

int stub_register(const char *path) {
    if (g_stub_count >= MAX_STUBS) {
        fprintf(stderr, "Warning: maximum stub count (%d) reached, skipping: %s\n",
                MAX_STUBS, path);
        return -1;
    }

    char *real = realpath(path, NULL);
    if (!real) {
        fprintf(stderr, "Warning: stub file not found: %s\n", path);
        return -1;
    }

    /* Derive lib_name from basename */
    char base[256] = {0};
    const char *sep = strrchr(real, PATH_SEPARATOR);
    strncpy(base, sep ? sep + 1 : real, 255);

    char lib_name[256];
    derive_lib_name(base, lib_name, (int)sizeof(lib_name));

    /* Reject duplicates */
    for (int i = 0; i < g_stub_count; i++) {
        if (strcmp(g_stubs[i].lib_name, lib_name) == 0) {
            free(real);
            return 0; /* already registered */
        }
    }

    Stub *s = &g_stubs[g_stub_count++];
    strncpy(s->path,     real,     MAX_PATH - 1); s->path[MAX_PATH - 1]   = '\0';
    strncpy(s->lib_name, lib_name, 255);          s->lib_name[255]        = '\0';
    s->raw_content = NULL;
    s->val_content = NULL;
    free(real);

    printf("Registered stub: %s  →  #include_lib \"%s\"\n", base, lib_name);
    return 0;
}

int stub_register_dir(const char *dir) {
    int added = 0;

#ifdef _WIN32
    char pattern[MAX_PATH];
    snprintf(pattern, sizeof(pattern), "%s\\*.stub.as", dir);

    WIN32_FIND_DATAA fd;
    HANDLE h = FindFirstFileA(pattern, &fd);
    if (h == INVALID_HANDLE_VALUE) return 0;

    do {
        if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
            char full[MAX_PATH];
            snprintf(full, sizeof(full), "%s\\%s", dir, fd.cFileName);
            if (stub_register(full) == 0) added++;
        }
    } while (FindNextFileA(h, &fd));
    FindClose(h);
#else
    DIR *d = opendir(dir);
    if (!d) return 0;

    struct dirent *ent;
    while ((ent = readdir(d)) != NULL) {
        if (ends_with_stub_as(ent->d_name)) {
            char full[MAX_PATH];
            snprintf(full, sizeof(full), "%s/%s", dir, ent->d_name);
            if (stub_register(full) == 0) added++;
        }
    }
    closedir(d);
#endif

    return added;
}

/* -------------------------------------------------------------------------
 * Validation content generation
 *
 * Replaces every function/method body { ... } with {} (empty body).
 * AngelScript issues warnings (not errors) for missing return values,
 * so empty bodies are safe for validation purposes.
 * ---------------------------------------------------------------------- */
static char *make_val_content(const char *raw) {
    TokArr ta  = obf_tokenize(raw);
    DStr   out;
    dstr_init(&out, strlen(raw) + 256);

    int depth      = 0;
    int skip_depth = -1; /* brace depth at which we started skipping */

    for (int i = 0; i < ta.n; i++) {
        OBFTok *tk = &ta.d[i];

        if (tk->type == OT_PUNCT && tk->len == 1) {
            char ch = tk->start[0];
            if (ch == '{') {
                depth++;
                if (skip_depth < 0) {
                    /* Is this opening a function body?
                     * Previous significant token must be ')'. */
                    int pi = prev_sig(ta.d, i);
                    if (pi >= 0 &&
                        ta.d[pi].type == OT_PUNCT &&
                        ta.d[pi].start[0] == ')') {
                        /* Enter skip mode: emit '{}' and skip until match */
                        dstr_push_str(&out, "{}");
                        skip_depth = depth;
                        continue;
                    }
                }
                if (skip_depth < 0) dstr_push_c(&out, ch);
            } else if (ch == '}') {
                if (skip_depth > 0 && depth == skip_depth) {
                    skip_depth = -1;
                    depth--;
                    continue; /* already emitted } as part of {} */
                }
                depth--;
                if (skip_depth < 0) dstr_push_c(&out, ch);
            } else {
                if (skip_depth < 0) dstr_push_n(&out, tk->start, tk->len);
            }
        } else {
            if (skip_depth < 0) dstr_push_n(&out, tk->start, tk->len);
        }
    }

    free(ta.d);
    return out.d;
}

/* -------------------------------------------------------------------------
 * Loading
 * ---------------------------------------------------------------------- */

void stub_load_all(void) {
    for (int i = 0; i < g_stub_count; i++) {
        Stub *s = &g_stubs[i];
        size_t sz;
        s->raw_content = read_file_content(s->path, &sz);
        if (!s->raw_content) {
            fprintf(stderr, "Warning: could not read stub: %s\n", s->path);
            continue;
        }
        s->val_content = make_val_content(s->raw_content);
        printf("Loaded stub: %s (%d bytes)\n", s->lib_name, (int)sz);
    }
}

/* -------------------------------------------------------------------------
 * Lookup
 * ---------------------------------------------------------------------- */

Stub *stub_find(const char *lib_name) {
    for (int i = 0; i < g_stub_count; i++) {
        /* Exact match: "uncapi.as" == "uncapi.as" */
        if (strcmp(g_stubs[i].lib_name, lib_name) == 0) return &g_stubs[i];
        /* Strip .as extension and compare: "uncapi" matches "uncapi.as" */
        size_t qlen = strlen(lib_name);
        size_t slen = strlen(g_stubs[i].lib_name);
        if (slen > 3 && strcmp(g_stubs[i].lib_name + slen - 3, ".as") == 0 &&
            slen - 3 == qlen &&
            strncmp(g_stubs[i].lib_name, lib_name, qlen) == 0)
            return &g_stubs[i];
    }
    return NULL;
}

/* -------------------------------------------------------------------------
 * Symbol extraction
 * ---------------------------------------------------------------------- */

char **stub_collect_symbols(void) {
    char **syms = (char **)calloc(MAX_STUB_SYMBOLS, sizeof(char *));
    if (!syms) return NULL;
    int count = 0;

    for (int i = 0; i < g_stub_count; i++) {
        if (!g_stubs[i].raw_content) continue;
        collect_decl_symbols(g_stubs[i].raw_content,
                             NULL, NULL,
                             syms, &count, MAX_STUB_SYMBOLS);
    }

    printf("Extracted %d symbol(s) from stubs (protected from obfuscation)\n", count);
    return syms; /* NULL-terminated: calloc zeroed the array */
}

void stub_symbols_free(char **syms) {
    if (!syms) return;
    for (int i = 0; syms[i]; i++) free(syms[i]);
    free(syms);
}

/* -------------------------------------------------------------------------
 * Stub emission (generate .stub.as from bundled content)
 * ---------------------------------------------------------------------- */

char *emit_stub(const char *content) {
    TokArr ta  = obf_tokenize(content);
    DStr   out;
    dstr_init(&out, strlen(content) / 2 + 1024);

    int depth      = 0;
    int skip_depth = -1;

    /* Emit a header comment */
    dstr_push_str(&out,
        "// Auto-generated stub — declarations only, no implementation.\n"
        "// Distribute this file so consumers can validate against your\n"
        "// library without seeing the source.\n\n");

    for (int i = 0; i < ta.n; i++) {
        OBFTok *tk = &ta.d[i];

        /* Drop bundler section markers */
        if (tk->type == OT_LINE_CMT) {
            const char *s = tk->start + 2;
            while (*s == ' ' || *s == '\t') s++;
            if (strncmp(s, "===", 3) == 0) continue;
        }

        if (tk->type == OT_PUNCT && tk->len == 1) {
            char ch = tk->start[0];
            if (ch == '{') {
                depth++;
                if (skip_depth < 0) {
                    int pi = prev_sig(ta.d, i);
                    if (pi >= 0 &&
                        ta.d[pi].type == OT_PUNCT &&
                        ta.d[pi].start[0] == ')') {
                        dstr_push_str(&out, "{}\n");
                        skip_depth = depth;
                        continue;
                    }
                }
                if (skip_depth < 0) dstr_push_c(&out, ch);
            } else if (ch == '}') {
                if (skip_depth > 0 && depth == skip_depth) {
                    skip_depth = -1;
                    depth--;
                    continue;
                }
                depth--;
                if (skip_depth < 0) dstr_push_c(&out, ch);
            } else {
                if (skip_depth < 0) dstr_push_n(&out, tk->start, tk->len);
            }
        } else {
            if (skip_depth < 0) dstr_push_n(&out, tk->start, tk->len);
        }
    }

    free(ta.d);
    return out.d;
}

/* -------------------------------------------------------------------------
 * Write stub to disk
 * ---------------------------------------------------------------------- */

void write_stub_file(const char *content, const char *path) {
    FILE *f = fopen(path, "wb");
    if (!f) {
        fprintf(stderr, "Error: could not write stub to '%s'\n", path);
        return;
    }
    fwrite(content, 1, strlen(content), f);
    fclose(f);
    printf("Stub written: %s\n", path);
}
