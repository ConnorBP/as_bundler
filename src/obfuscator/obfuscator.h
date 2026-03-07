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
 * Identifier obfuscation
 *
 * Renames every user-defined identifier in content to a short generated
 * name, while preserving all AngelScript keywords and engine-registered
 * API names.
 *
 * api_names       – engine-registered names to protect (may be NULL)
 * remove_newlines – collapse all whitespace/newlines; inject spaces only
 *                   where required to keep tokens distinct
 *
 * Returns a new malloc'd string; caller must free it.
 * ---------------------------------------------------------------------- */

char *obfuscate_content(const char *content, ASNameList *api_names,
                        int remove_newlines);

/* -------------------------------------------------------------------------
 * String scrambler
 *
 * Replaces every non-empty string literal with a runtime lookup:
 *   __gs(0xFNV64_HEX_KEY)
 * and prepends helper functions + an __init_str_table() function that
 * initialises a global hash_map@ from XOR-encrypted byte arrays.
 *
 * Does NOT free `content`; caller frees old pointer and keeps the return
 * value.
 * ---------------------------------------------------------------------- */

char *scramble_strings(char *content);

#endif /* OBFUSCATOR_H */
