#ifndef OBFUSCATOR_H
#define OBFUSCATOR_H

#include "../common/common.h"
#include "../as_wrapper.h"

/* -------------------------------------------------------------------------
 * Token types for the obfuscator mini-tokenizer
 * ---------------------------------------------------------------------- */

typedef enum {
    OT_IDENT      = 0,  /* [a-zA-Z_][a-zA-Z0-9_]*                  */
    OT_NUMBER,          /* numeric literal (decimal, hex, float)     */
    OT_STRING,          /* "..." (with \-escapes)                    */
    OT_CHAR_LIT,        /* '.' (with \-escape)                       */
    OT_HEREDOC,         /* """..."""                                  */
    OT_LINE_CMT,        /* //...  (dropped in obfuscated output)     */
    OT_BLOCK_CMT,       /* / *...* / (dropped)                         */
    OT_NEWLINE,         /* \n or \r\n                                */
    OT_WHITESPACE,      /* spaces / tabs                             */
    OT_PUNCT,           /* operators, brackets, punctuation          */
} OBFTokType;

typedef struct {
    OBFTokType  type;
    const char *start;  /* points into original buffer (not NUL-terminated) */
    int         len;
} OBFTok;

/* -------------------------------------------------------------------------
 * Dynamic token array
 * ---------------------------------------------------------------------- */

typedef struct {
    OBFTok *d;
    int     n;
    int     cap;
} TokArr;

void tokarr_push(TokArr *a, OBFTok t);

/* -------------------------------------------------------------------------
 * Dynamic output string
 * ---------------------------------------------------------------------- */

typedef struct {
    char  *d;
    size_t len;
    size_t cap;
} DStr;

void dstr_init(DStr *s, size_t cap);
void dstr_grow(DStr *s, size_t need);
void dstr_push_c(DStr *s, char c);
void dstr_push_n(DStr *s, const char *t, int n);
void dstr_push_str(DStr *s, const char *t);

/* -------------------------------------------------------------------------
 * Tokenizer
 * ---------------------------------------------------------------------- */

/* Tokenize AngelScript source into a TokArr.
 * Caller must free the returned TokArr's .d member. */
TokArr obf_tokenize(const char *src);

/* Is c an identifier character (alpha, digit, or underscore)? */
#define IS_IC(c) (isalnum((unsigned char)(c)) || (c) == '_')

/* -------------------------------------------------------------------------
 * Symbol map
 *
 * Records every identifier rename, every protected identifier that
 * appeared in the source, and every string scramble entry (key → plaintext).
 * Written to disk after obfuscation/scrambling via symmap_write().
 * ---------------------------------------------------------------------- */

typedef struct {
    /* Renamed identifiers */
    char **idents_orig;
    char **idents_obf;
    int    idents_n, idents_cap;
    /* Protected identifiers encountered in source (not renamed) */
    char **prot_names;
    int    prot_n, prot_cap;
    /* Scrambled strings: hex_key -> plaintext */
    char **str_keys;
    char **str_plains;
    int    strs_n, strs_cap;
} SymbolMap;

void symmap_init(SymbolMap *m);
void symmap_free(SymbolMap *m);
void symmap_add_rename(SymbolMap *m, const char *orig, const char *obf);
void symmap_add_protected(SymbolMap *m, const char *name);
void symmap_add_string(SymbolMap *m, const char *hex_key, const char *plaintext);
/* Write symbol map to path in the given format ("text", "csv", "json") */
void symmap_write(const SymbolMap *m, const char *path, const char *format);

/* -------------------------------------------------------------------------
 * Identifier obfuscation
 *
 * Renames every user-defined identifier in content to a short generated
 * name, while preserving all AngelScript keywords, engine-registered API
 * names, and any user-specified protected names (g_protect_names).
 *
 * api_names       – engine-registered names to protect (may be NULL)
 * remove_newlines – collapse all whitespace/newlines; inject spaces only
 *                   where required to keep tokens distinct
 * map_out         – if non-NULL, populated with all renames and protected
 *                   names that appeared in the source
 *
 * Returns a new malloc'd string; caller must free it.
 * ---------------------------------------------------------------------- */

char *obfuscate_content(const char *content, ASNameList *api_names,
                        int remove_newlines, SymbolMap *map_out);

/* -------------------------------------------------------------------------
 * String scrambler
 *
 * Replaces every non-empty string literal with a runtime lookup:
 *   __gs(0xFNV64_HEX_KEY)
 * and prepends helper functions + an __init_str_table() function that
 * initialises a global hash_map@ from XOR-encrypted byte arrays.
 *
 * protect_literals – if non-zero, string literals are left untouched
 *                    (honours --protect-string-literals)
 * map_out          – if non-NULL, populated with hex_key → plaintext pairs
 *
 * Does NOT free `content`; caller frees old pointer and keeps the return
 * value.
 * ---------------------------------------------------------------------- */

char *scramble_strings(char *content, int protect_literals, SymbolMap *map_out);

#endif /* OBFUSCATOR_H */
