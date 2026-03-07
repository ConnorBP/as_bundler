#ifndef PROCESSING_H
#define PROCESSING_H

#include "../common/common.h"

/* -------------------------------------------------------------------------
 * Comment and include stripping
 * ---------------------------------------------------------------------- */

/* Remove comment-only lines from content. Returns new malloc'd string. */
char *strip_comments(const char *content);

/* Remove all #include lines from content. Returns new malloc'd string. */
char *strip_includes(const char *content);

/* -------------------------------------------------------------------------
 * String utility
 * ---------------------------------------------------------------------- */

/* Replace all occurrences of old_val with new_val inside str.
 * Takes ownership of str (frees it). Returns new malloc'd string. */
char *str_replace_all(char *str, const char *old_val, const char *new_val);

/*
 *
 * Build timestamp macros
 *
 * Replaced as raw tokens before any other processing step.
 *
 * String variants (include surrounding quotes):
 *   __BUILD_TIMESTAMP_STR__   "YYYY-MM-DD HH:MM:SS"
 *   __BUILD_DATE_STR__        "YYYY-MM-DD"
 *   __BUILD_TIME_STR__        "HH:MM:SS"
 *
 * Integer variants (no leading zeros):
 *   __BUILD_YEAR__            e.g. 2026
 *   __BUILD_MONTH__           e.g. 3
 *   __BUILD_DAY__             e.g. 4
 *   __BUILD_HOUR__            e.g. 9
 *   __BUILD_MINUTE__          e.g. 5
 *   __BUILD_SECOND__          e.g. 7
 *   __BUILD_UNIX_TS__         Unix timestamp (seconds since epoch)
 *
 * Takes ownership of content. Returns new malloc'd string.
 */

char *apply_timestamp_macros(char *content);

/* -------------------------------------------------------------------------
 * FNV-1a hash functions
 * ---------------------------------------------------------------------- */

/* 32 bit FNV-1a string hashing */
uint32_t fnv1a_32(const char *str, size_t len);
/* 64 bit FNV-1a string hashing */
uint64_t fnv1a_64(const char *str, size_t len);

/*
 * STRHASH / STRHASH64 macro evaluation
 *
 * Evaluates STRHASH("...") and STRHASH64("...") calls in source text,
 * replacing each with the computed decimal hash value.
 *
 *   STRHASH("name")    -> FNV-1a 32-bit decimal (e.g. 2949673445)
 *   STRHASH64("name")  -> FNV-1a 64-bit decimal
 *
 * Takes ownership of content. Returns new malloc'd string.
 */

char *evaluate_string_macros(char *content, const char *macro_name,
                              int is_64bit);

/*
 * STRHASH macro definition stripping
 *
 * Strips STRHASH / FNV helper #define blocks from prepend file content
 * so the C preprocessor does not try to expand already-evaluated calls.
 *
 * Stripped patterns:
 *   #define STRHASH(...)
 *   #define STRHASH64(...)
 *   #define HF<digit>(...)   (helper macros)
 *   #define FNV_OFFSET[_64]
 *   #define FNV_PRIME[_64]
 *
 * Takes ownership of content. Returns new malloc'd string.
 */

char *strip_strhash_macros(char *content);

#endif /* PROCESSING_H */
