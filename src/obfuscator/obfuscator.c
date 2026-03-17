#include "obfuscator.h"
#include "../processing/processing.h"

/* -------------------------------------------------------------------------
 * AngelScript keywords and built-in add-on type names that must
 * never be renamed by the obfuscator.
 * ---------------------------------------------------------------------- */

static const char *OBF_KEYWORDS[] = {
    "abstract", "and", "any", "array", "auto", "bool", "break", "by",
    "case", "cast", "catch", "class", "const", "continue", "default",
    "dictionary", "do", "double", "else", "enum", "explicit", "external",
    "false", "final", "float", "for", "from", "funcdef", "grid", "if",
    "import", "in", "inout", "int", "int8", "int16", "int32", "int64",
    "interface", "is", "mixin", "namespace", "not", "null", "or", "out",
    "override", "private", "protected", "public", "ref", "return",
    "shared", "string", "super", "switch", "this", "true", "try",
    "typedef", "uint", "uint8", "uint16", "uint32", "uint64", "void",
    "while", "xor",
    /* Entry/exit-point name that must never be renamed */
    "main", "on_unload",
    /* Generated string-scrambler symbols that must never be renamed */
    "__dec", "__gs", "__gs_init", "__init_str_table", "__str_table",
    NULL
};

/* -------------------------------------------------------------------------
 * Dynamic token array
 * ---------------------------------------------------------------------- */

void tokarr_push(TokArr *a, OBFTok t) {
    if (a->n >= a->cap) {
        a->cap = a->cap ? a->cap * 2 : 512;
        a->d   = (OBFTok *)realloc(a->d, (size_t)a->cap * sizeof(OBFTok));
    }
    a->d[a->n++] = t;
}

/* -------------------------------------------------------------------------
 * Dynamic output string
 * ---------------------------------------------------------------------- */

void dstr_init(DStr *s, size_t cap) {
    s->d    = (char *)malloc(cap);
    s->len  = 0;
    s->cap  = cap;
    s->d[0] = '\0';
}

void dstr_grow(DStr *s, size_t need) {
    if (s->len + need + 1 > s->cap) {
        s->cap = (s->len + need + 1) * 2;
        s->d   = (char *)realloc(s->d, s->cap);
    }
}

void dstr_push_c(DStr *s, char c) {
    dstr_grow(s, 1);
    s->d[s->len++] = c;
    s->d[s->len]   = '\0';
}

void dstr_push_n(DStr *s, const char *t, int n) {
    dstr_grow(s, (size_t)n);
    memcpy(s->d + s->len, t, (size_t)n);
    s->len       += (size_t)n;
    s->d[s->len]  = '\0';
}

void dstr_push_str(DStr *s, const char *t) {
    dstr_push_n(s, t, (int)strlen(t));
}

/* -------------------------------------------------------------------------
 * Simple string set (unique, unsorted; linear search fine for < ~5000 idents)
 * ---------------------------------------------------------------------- */

typedef struct { char **d; int n, cap; } StrSet;

static void strset_add_unique(StrSet *s, const char *str) {
    for (int i = 0; i < s->n; i++)
        if (strcmp(s->d[i], str) == 0) return;
    if (s->n >= s->cap) {
        s->cap = s->cap ? s->cap * 2 : 64;
        s->d   = (char **)realloc(s->d, (size_t)s->cap * sizeof(char *));
    }
    s->d[s->n++] = strdup(str);
}

static void strset_free(StrSet *s) {
    for (int i = 0; i < s->n; i++) free(s->d[i]);
    free(s->d);
}

/* -------------------------------------------------------------------------
 * Rename map: original identifier -> short obfuscated name
 * ---------------------------------------------------------------------- */

typedef struct { char *orig; char renamed[12]; } RenEnt;
typedef struct { RenEnt *d; int n, cap; }         RenMap;

static void renmap_put(RenMap *m, const char *orig, const char *repl) {
    if (m->n >= m->cap) {
        m->cap = m->cap ? m->cap * 2 : 64;
        m->d   = (RenEnt *)realloc(m->d, (size_t)m->cap * sizeof(RenEnt));
    }
    m->d[m->n].orig = strdup(orig);
    strncpy(m->d[m->n].renamed, repl, 11);
    m->d[m->n].renamed[11] = '\0';
    m->n++;
}

static const char *renmap_get(const RenMap *m, const char *s, int len) {
    for (int i = 0; i < m->n; i++)
        if (strncmp(m->d[i].orig, s, (size_t)len) == 0 &&
            m->d[i].orig[len] == '\0')
            return m->d[i].renamed;
    return NULL;
}

static void renmap_free(RenMap *m) {
    for (int i = 0; i < m->n; i++) free(m->d[i].orig);
    free(m->d);
}

/* -------------------------------------------------------------------------
 * Sorted-set helpers (qsort / bsearch on const char * arrays)
 * ---------------------------------------------------------------------- */

static int cmp_pstr(const void *a, const void *b) {
    return strcmp(*(const char *const *)a, *(const char *const *)b);
}

static int pset_contains(const char *const *set, int n, const char *tmp) {
    const char **r = (const char **)bsearch(
        &tmp, set, (size_t)n, sizeof(char *), cmp_pstr);
    return r != NULL;
}

static int pset_has(const char *const *set, int n, const char *s, int len) {
    char tmp[256];
    if (len >= 255) return 0;
    memcpy(tmp, s, (size_t)len);
    tmp[len] = '\0';
    return pset_contains(set, n, tmp);
}

/* -------------------------------------------------------------------------
 * Short obfuscated name generator
 *
 * Produces the Nth name in the sequence:
 *   _0.._9, _a.._z, _A.._Z          (62 one-char-suffix names)
 *   _00.._ZZ                         (3844 two-char-suffix names)
 *   _000...                          (238328 three-char-suffix names)
 * ---------------------------------------------------------------------- */

static void gen_obf_name(int idx, char *out) {
    static const char ch[] =
        "0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    int b  = 62;
    out[0] = '_';
    if (idx < b) {
        out[1] = ch[idx];
        out[2] = '\0';
    } else if (idx < b + b * b) {
        idx   -= b;
        out[1] = ch[idx / b];
        out[2] = ch[idx % b];
        out[3] = '\0';
    } else {
        idx   -= b + b * b;
        out[1] = ch[idx / (b * b)];
        out[2] = ch[(idx / b) % b];
        out[3] = ch[idx % b];
        out[4] = '\0';
    }
}

/* -------------------------------------------------------------------------
 * Tokenizer
 * ---------------------------------------------------------------------- */

TokArr obf_tokenize(const char *src) {
    TokArr     ta = {NULL, 0, 0};
    const char *p = src;

    while (*p) {
        const char *s = p;
        OBFTok      t;

        /* Line comment  //... */
        if (p[0] == '/' && p[1] == '/') {
            p += 2;
            while (*p && *p != '\n') p++;
            t.type = OT_LINE_CMT; t.start = s; t.len = (int)(p - s);
            tokarr_push(&ta, t);
            continue;
        }
        /* Block comment  / *...* / */
        if (p[0] == '/' && p[1] == '*') {
            p += 2;
            while (*p && !(p[0] == '*' && p[1] == '/')) p++;
            if (*p) p += 2;
            t.type = OT_BLOCK_CMT; t.start = s; t.len = (int)(p - s);
            tokarr_push(&ta, t);
            continue;
        }
        /* Heredoc  """...""" */
        if (p[0] == '"' && p[1] == '"' && p[2] == '"') {
            p += 3;
            while (*p && !(p[0] == '"' && p[1] == '"' && p[2] == '"')) p++;
            if (*p) p += 3;
            t.type = OT_HEREDOC; t.start = s; t.len = (int)(p - s);
            tokarr_push(&ta, t);
            continue;
        }
        /* String  "..." */
        if (*p == '"') {
            p++;
            while (*p && *p != '"') {
                if (*p == '\\' && p[1]) p++;
                p++;
            }
            if (*p == '"') p++;
            t.type = OT_STRING; t.start = s; t.len = (int)(p - s);
            tokarr_push(&ta, t);
            continue;
        }
        /* Char literal  '.' */
        if (*p == '\'') {
            p++;
            while (*p && *p != '\'') {
                if (*p == '\\' && p[1]) p++;
                p++;
            }
            if (*p == '\'') p++;
            t.type = OT_CHAR_LIT; t.start = s; t.len = (int)(p - s);
            tokarr_push(&ta, t);
            continue;
        }
        /* Newline */
        if (*p == '\r' || *p == '\n') {
            if (*p == '\r') p++;
            if (*p == '\n') p++;
            t.type = OT_NEWLINE; t.start = s; t.len = (int)(p - s);
            tokarr_push(&ta, t);
            continue;
        }
        /* Whitespace */
        if (*p == ' ' || *p == '\t') {
            while (*p == ' ' || *p == '\t') p++;
            t.type = OT_WHITESPACE; t.start = s; t.len = (int)(p - s);
            tokarr_push(&ta, t);
            continue;
        }
        /* Identifier  [a-zA-Z_][a-zA-Z0-9_]* */
        if (isalpha((unsigned char)*p) || *p == '_') {
            while (IS_IC(*p)) p++;
            t.type = OT_IDENT; t.start = s; t.len = (int)(p - s);
            tokarr_push(&ta, t);
            continue;
        }
        /* Number  (decimal, 0x hex, float, suffixes) */
        if (isdigit((unsigned char)*p)) {
            while (isdigit((unsigned char)*p)) p++;
            if ((*p == 'x' || *p == 'X') && (p - s) == 1 && s[0] == '0') {
                p++;
                while (isxdigit((unsigned char)*p)) p++;
            } else {
                if (*p == '.') {
                    p++;
                    while (isdigit((unsigned char)*p)) p++;
                }
                if (*p == 'e' || *p == 'E') {
                    p++;
                    if (*p == '+' || *p == '-') p++;
                    while (isdigit((unsigned char)*p)) p++;
                }
            }
            while (*p == 'f' || *p == 'F' || *p == 'u' || *p == 'U' ||
                   *p == 'l' || *p == 'L')
                p++;
            t.type = OT_NUMBER; t.start = s; t.len = (int)(p - s);
            tokarr_push(&ta, t);
            continue;
        }
        /* Single-character punctuation / operator */
        p++;
        t.type = OT_PUNCT; t.start = s; t.len = 1;
        tokarr_push(&ta, t);
    }
    return ta;
}

/* -------------------------------------------------------------------------
 * Identifier obfuscation
 * ---------------------------------------------------------------------- */

char *obfuscate_content(const char *content, ASNameList *api_names,
                        int remove_newlines) {
    /* ------------------------------------------------------------------
     * 1. Build a sorted protected-name set: keywords + API names
     * ------------------------------------------------------------------ */
    int kw_n    = 0;
    while (OBF_KEYWORDS[kw_n]) kw_n++;
    int api_n   = api_names ? api_names->count : 0;
    int total_p = kw_n + api_n;

    const char **prot = (const char **)malloc((size_t)total_p * sizeof(char *));
    int pi = 0;
    for (int i = 0; i < kw_n;  i++) prot[pi++] = OBF_KEYWORDS[i];
    for (int i = 0; i < api_n; i++) prot[pi++] = api_names->names[i];
    qsort(prot, (size_t)pi, sizeof(char *), cmp_pstr);

    /* Deduplicate in-place */
    int up = 0;
    for (int i = 0; i < pi; i++)
        if (up == 0 || strcmp(prot[i], prot[up - 1]) != 0)
            prot[up++] = prot[i];
    int prot_n = up;

    /* ------------------------------------------------------------------
     * 2. Tokenize
     * ------------------------------------------------------------------ */
    TokArr ta = obf_tokenize(content);

    /* ------------------------------------------------------------------
     * 3. Collect unique user-defined identifiers (unprotected)
     * ------------------------------------------------------------------ */
    StrSet idents = {NULL, 0, 0};
    char   tmp[256];

    for (int i = 0; i < ta.n; i++) {
        OBFTok *tk = &ta.d[i];
        if (tk->type != OT_IDENT) continue;
        if (pset_has(prot, prot_n, tk->start, tk->len)) continue;
        int l = tk->len < 255 ? tk->len : 255;
        memcpy(tmp, tk->start, (size_t)l);
        tmp[l] = '\0';
        strset_add_unique(&idents, tmp);
    }
    /* Sort for stable / reproducible output */
    qsort(idents.d, (size_t)idents.n, sizeof(char *), cmp_pstr);

    /* ------------------------------------------------------------------
     * 4. Assign short obfuscated names (skip any that are protected)
     * ------------------------------------------------------------------ */
    RenMap rmap    = {NULL, 0, 0};
    int    gen_idx = 0;

    for (int i = 0; i < idents.n; i++) {
        char new_name[12];
        do { gen_obf_name(gen_idx++, new_name); }
        while (pset_contains(prot, prot_n, new_name));
        renmap_put(&rmap, idents.d[i], new_name);
    }
    printf("Obfuscated %d user identifier(s)\n", rmap.n);

    /* ------------------------------------------------------------------
     * 5. Reconstruct output with renamed identifiers
     * ------------------------------------------------------------------ */
    DStr out;
    dstr_init(&out, strlen(content) + 1024);
    char prev = 0;

    for (int i = 0; i < ta.n; i++) {
        OBFTok *tk = &ta.d[i];
        switch (tk->type) {

            case OT_LINE_CMT:
            case OT_BLOCK_CMT:
                /* Drop all comments in obfuscated output */
                break;

            case OT_NEWLINE:
                if (!remove_newlines) { dstr_push_c(&out, '\n'); prev = '\n'; }
                break;

            case OT_WHITESPACE:
                if (!remove_newlines) {
                    dstr_push_n(&out, tk->start, tk->len);
                    prev = ' ';
                }
                break;

            case OT_IDENT: {
                const char *emit = tk->start;
                int         elen = tk->len;
                const char *repl = renmap_get(&rmap, tk->start, tk->len);
                if (repl) { emit = repl; elen = (int)strlen(repl); }
                if (elen > 0 && IS_IC(prev) && IS_IC(emit[0]))
                    dstr_push_c(&out, ' ');
                dstr_push_n(&out, emit, elen);
                if (elen > 0) prev = emit[elen - 1];
                break;
            }

            case OT_NUMBER:
                if (tk->len > 0 && IS_IC(prev) && IS_IC(tk->start[0]))
                    dstr_push_c(&out, ' ');
                dstr_push_n(&out, tk->start, tk->len);
                if (tk->len > 0) prev = tk->start[tk->len - 1];
                break;

            default:
                /* OT_STRING, OT_CHAR_LIT, OT_HEREDOC, OT_PUNCT: emit verbatim */
                if (tk->len > 0 && IS_IC(prev) && IS_IC(tk->start[0]))
                    dstr_push_c(&out, ' ');
                dstr_push_n(&out, tk->start, tk->len);
                if (tk->len > 0) prev = tk->start[tk->len - 1];
                break;
        }
    }

    /* Trim trailing whitespace */
    while (out.len > 0 &&
           (out.d[out.len - 1] == ' '  ||
            out.d[out.len - 1] == '\n' ||
            out.d[out.len - 1] == '\r'))
        out.d[--out.len] = '\0';

    /* ------------------------------------------------------------------
     * 6. Cleanup
     * ------------------------------------------------------------------ */
    free(ta.d);
    strset_free(&idents);
    renmap_free(&rmap);
    free(prot);

    return out.d;
}

/* -------------------------------------------------------------------------
 * String scrambler – internal helpers
 * ---------------------------------------------------------------------- */

/* Decode C/AngelScript escape sequences inside a string literal body
 * (content between the outer quotes, quotes not included).
 * Writes decoded bytes to `out` (caller must allocate body_len+1 bytes).
 * Returns number of bytes written. */
static int decode_string_body(const char *body, int body_len,
                               unsigned char *out) {
    int n = 0;
    for (int i = 0; i < body_len; ) {
        if (body[i] == '\\' && i + 1 < body_len) {
            i++;
            switch (body[i]) {
                case 'n':  out[n++] = '\n'; i++; break;
                case 't':  out[n++] = '\t'; i++; break;
                case 'r':  out[n++] = '\r'; i++; break;
                case '0':  out[n++] = '\0'; i++; break;
                case '\\': out[n++] = '\\'; i++; break;
                case '"':  out[n++] = '"';  i++; break;
                case '\'': out[n++] = '\''; i++; break;
                case 'x':
                    if (i + 2 < body_len &&
                        isxdigit((unsigned char)body[i + 1]) &&
                        isxdigit((unsigned char)body[i + 2])) {
                        char h[3] = { body[i + 1], body[i + 2], '\0' };
                        out[n++] = (unsigned char)strtol(h, NULL, 16);
                        i += 3;
                    } else {
                        out[n++] = (unsigned char)body[i++];
                    }
                    break;
                default:
                    out[n++] = (unsigned char)body[i++];
                    break;
            }
        } else {
            out[n++] = (unsigned char)body[i++];
        }
    }
    return n;
}

/* One entry in the scrambled-string table */
typedef struct {
    uint64_t       hash;
    unsigned char *raw;       /* decoded original bytes (owned) */
    int            raw_len;
    uint8_t        xor_key;
    char           key_hex[20]; /* "0x" + 16 hex digits + NUL */
} ScrStr;

typedef struct { ScrStr *d; int n, cap; } ScrArr;

static void scrarr_push(ScrArr *a, ScrStr s) {
    if (a->n >= a->cap) {
        a->cap = a->cap ? a->cap * 2 : 16;
        a->d   = (ScrStr *)realloc(a->d, (size_t)a->cap * sizeof(ScrStr));
    }
    a->d[a->n++] = s;
}

static int scrarr_find(const ScrArr *a, uint64_t hash) {
    for (int i = 0; i < a->n; i++)
        if (a->d[i].hash == hash) return i;
    return -1;
}

static void scrarr_free(ScrArr *a) {
    for (int i = 0; i < a->n; i++) free(a->d[i].raw);
    free(a->d);
}

/* -------------------------------------------------------------------------
 * String scrambler – main transform
 * ---------------------------------------------------------------------- */

char *scramble_strings(char *content) {
    TokArr ta = obf_tokenize(content);

    /* ------------------------------------------------------------------
     * 1. Collect unique string literals
     * ------------------------------------------------------------------ */
    ScrArr strs = {0};

    for (int i = 0; i < ta.n; i++) {
        OBFTok *tk = &ta.d[i];
        if (tk->type != OT_STRING) continue;
        if (tk->len < 2) continue;

        const char    *body = tk->start + 1;
        int            blen = tk->len   - 2;
        if (blen == 0) continue; /* skip empty strings */

        unsigned char *raw  = (unsigned char *)malloc((size_t)(blen + 1));
        if (!raw) continue;

        int      rlen = decode_string_body(body, blen, raw);
        uint64_t hash = fnv1a_64((const char *)raw, (size_t)rlen);

        if (scrarr_find(&strs, hash) >= 0) { free(raw); continue; }

        /* XOR key: derive from FNV-32, never zero */
        uint8_t xk = (uint8_t)(fnv1a_32((const char *)raw, (size_t)rlen) & 0xFF);
        if (xk == 0) xk = 0xA5;

        ScrStr s;
        s.hash    = hash;
        s.raw     = raw;
        s.raw_len = rlen;
        s.xor_key = xk;
        snprintf(s.key_hex, sizeof(s.key_hex), "0x%016llx",
                 (unsigned long long)hash);
        scrarr_push(&strs, s);
    }

    if (strs.n == 0) { free(ta.d); return content; }
    printf("Scrambling %d unique string literal(s)\n", strs.n);

    /* ------------------------------------------------------------------
     * 2. Build prologue: hash_map handle + helper functions + init stub
     * ------------------------------------------------------------------ */
    DStr pro;
    dstr_init(&pro, 8192);

    static const char hdr[] =
        "// === __str_scramble_helpers ===\n"
        /* Global init trigger – evaluated before any function runs */
        "int __gs_init=__init_str_table();\n"
        /* Global hash_map handle: uint64 key -> string value */
        "hash_map@ __str_table;\n"
        /* __dec(bytes, key): XOR-decrypt byte array back to string */
        "string __dec(const array<uint8>&in b,uint8 k){"
            "string r;r.resize(b.length());"
            "for(uint i=0;i<b.length();i++){r[i]=b[i]^k;}"
            "return r;}\n"
        /* __gs(key): look up a scrambled string by its FNV-64 uint64 key */
        "string __gs(uint64 k){"
            "string v;if(__str_table.get(k,v))return v;return \"\";}\n"
        "int __init_str_table(){\n"
        "\t@__str_table=hash_map();\n";

    dstr_push_n(&pro, hdr, (int)(sizeof(hdr) - 1));

    char buf[32];
    for (int i = 0; i < strs.n; i++) {
        ScrStr *s = &strs.d[i];
        dstr_push_str(&pro, "\t__str_table.set(");
        dstr_push_str(&pro, s->key_hex);
        dstr_push_str(&pro, ",__dec({");
        for (int j = 0; j < s->raw_len; j++) {
            if (j > 0) dstr_push_c(&pro, ',');
            snprintf(buf, sizeof(buf), "0x%02x",
                     (unsigned)(s->raw[j] ^ s->xor_key));
            dstr_push_str(&pro, buf);
        }
        snprintf(buf, sizeof(buf), "},0x%02x", (unsigned)s->xor_key);
        dstr_push_str(&pro, buf);
        dstr_push_str(&pro, "));\n");
    }
    dstr_push_str(&pro, "\treturn 1;\n}\n\n");

    /* ------------------------------------------------------------------
     * 3. Rebuild source: replace string literals with __gs() calls
     * ------------------------------------------------------------------ */
    DStr out;
    dstr_init(&out, strlen(content) + 512);
    char prev_ch = 0;

    for (int i = 0; i < ta.n; i++) {
        OBFTok *tk = &ta.d[i];

        if (tk->type == OT_STRING && tk->len >= 2) {
            const char    *body = tk->start + 1;
            int            blen = tk->len   - 2;
            if (blen > 0) {
                unsigned char *raw = (unsigned char *)malloc((size_t)(blen + 1));
                if (raw) {
                    int      rlen = decode_string_body(body, blen, raw);
                    uint64_t hash = fnv1a_64((const char *)raw, (size_t)rlen);
                    free(raw);
                    int idx = scrarr_find(&strs, hash);
                    if (idx >= 0) {
                        if (IS_IC(prev_ch)) dstr_push_c(&out, ' ');
                        dstr_push_str(&out, "__gs(");
                        dstr_push_str(&out, strs.d[idx].key_hex);
                        dstr_push_c(&out, ')');
                        prev_ch = ')';
                        continue;
                    }
                }
            }
        }

        /* Normal emit */
        dstr_push_n(&out, tk->start, tk->len);
        if (tk->len > 0) prev_ch = tk->start[tk->len - 1];
    }

    /* ------------------------------------------------------------------
     * 4. Final result: prologue prepended to rewritten source
     * ------------------------------------------------------------------ */
    char *final = (char *)malloc(pro.len + out.len + 2);
    if (!final) {
        free(pro.d);
        free(out.d);
        free(ta.d);
        scrarr_free(&strs);
        return content;
    }
    memcpy(final,           pro.d, pro.len);
    memcpy(final + pro.len, out.d, out.len + 1);

    free(ta.d);
    free(pro.d);
    free(out.d);
    scrarr_free(&strs);
    return final;
}
