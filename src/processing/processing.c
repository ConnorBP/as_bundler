#include "processing.h"
#include <time.h>

/* -------------------------------------------------------------------------
 * Comment stripping
 * ---------------------------------------------------------------------- */

char *strip_comments(const char *content) {
    size_t size   = strlen(content);
    char  *result = (char *)malloc(size + 1);
    char  *dst    = result;
    const char *src = content;

    while (*src) {
        const char *check = src;
        int in_string   = 0;
        char string_char = '\0';

        while (*check == ' ' || *check == '\t') check++;

        if (*check == '/' && *(check + 1) == '/') {
            while (*src && *src != '\n') src++;
            if (*src == '\n') src++;
            continue;
        } else if (*check == '/' && *(check + 1) == '*') {
            const char *comment_end = check + 2;
            while (*comment_end && !(*comment_end == '*' && *(comment_end + 1) == '/'))
                comment_end++;
            if (*comment_end == '*' && *(comment_end + 1) == '/')
                comment_end += 2;
            const char *after = comment_end;
            while (*after == ' ' || *after == '\t') after++;
            if (*after == '\n' || *after == '\r' || *after == '\0') {
                src = comment_end;
                while (*src == ' ' || *src == '\t') src++;
                if (*src == '\r') src++;
                if (*src == '\n') src++;
                continue;
            }
        }

        while (*src && *src != '\n') {
            if (!in_string) {
                if (*src == '"' || *src == '\'') {
                    in_string   = 1;
                    string_char = *src;
                    *dst++ = *src++;
                } else {
                    *dst++ = *src++;
                }
            } else {
                if (*src == string_char) {
                    const char *ck = src - 1;
                    int escape_count = 0;
                    while (ck >= content && *ck == '\\') { escape_count++; ck--; }
                    if (escape_count % 2 == 0) in_string = 0;
                }
                *dst++ = *src++;
            }
        }
        if (*src == '\r') *dst++ = *src++;
        if (*src == '\n') *dst++ = *src++;
    }
    *dst = '\0';
    return result;
}

/* -------------------------------------------------------------------------
 * Include stripping
 * ---------------------------------------------------------------------- */

char *strip_includes(const char *content) {
    size_t size   = strlen(content);
    char  *result = (char *)malloc(size + 1);
    char  *dst    = result;
    const char *src        = content;
    const char *line_start = src;

    while (*src) {
        if (*src == '\n') {
            size_t line_len  = (size_t)(src - line_start) + 1;
            char   line[MAX_LINE];
            size_t copy_len  = line_len < MAX_LINE - 1 ? line_len : MAX_LINE - 1;
            strncpy(line, line_start, copy_len);
            line[copy_len] = '\0';
            if (!strstr(line, "#include")) {
                memcpy(dst, line_start, line_len);
                dst += line_len;
            }
            line_start = src + 1;
        }
        src++;
    }
    /* Handle last line (no trailing newline) */
    if (line_start < src) {
        size_t line_len = (size_t)(src - line_start);
        char   line[MAX_LINE];
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

/* -------------------------------------------------------------------------
 * String utility
 * ---------------------------------------------------------------------- */

char *str_replace_all(char *str, const char *old_val, const char *new_val) {
    size_t old_len = strlen(old_val);
    size_t new_len = strlen(new_val);
    if (old_len == 0) return str;

    int count = 0;
    const char *p = str;
    while ((p = strstr(p, old_val)) != NULL) {
        count++;
        p += old_len;
    }
    if (count == 0) return str;

    size_t old_total = strlen(str);
    size_t new_total = old_total - (size_t)count * old_len + (size_t)count * new_len;
    char  *result    = (char *)malloc(new_total + 1);
    char  *dst       = result;
    const char *src  = str;

    while ((p = strstr(src, old_val)) != NULL) {
        size_t before = (size_t)(p - src);
        memcpy(dst, src, before);
        dst += before;
        memcpy(dst, new_val, new_len);
        dst += new_len;
        src  = p + old_len;
    }
    strcpy(dst, src);
    free(str);
    return result;
}

/* -------------------------------------------------------------------------
 * Build timestamp macros
 * ---------------------------------------------------------------------- */

char *apply_timestamp_macros(char *content) {
    time_t     now = time(NULL);
    struct tm *t   = localtime(&now);

    char ts_str[32], date_str[16], time_str[12];
    strftime(ts_str,   sizeof(ts_str),   "\"%Y-%m-%d %H:%M:%S\"", t);
    strftime(date_str, sizeof(date_str), "\"%Y-%m-%d\"",           t);
    strftime(time_str, sizeof(time_str), "\"%H:%M:%S\"",           t);

    char year_str[8], month_str[4], day_str[4];
    char hour_str[4], min_str[4],   sec_str[4], unix_str[24];
    snprintf(year_str,  sizeof(year_str),  "%d",   t->tm_year + 1900);
    snprintf(month_str, sizeof(month_str), "%d",   t->tm_mon  + 1);
    snprintf(day_str,   sizeof(day_str),   "%d",   t->tm_mday);
    snprintf(hour_str,  sizeof(hour_str),  "%d",   t->tm_hour);
    snprintf(min_str,   sizeof(min_str),   "%d",   t->tm_min);
    snprintf(sec_str,   sizeof(sec_str),   "%d",   t->tm_sec);
    snprintf(unix_str,  sizeof(unix_str),  "%lld", (long long)now);

    content = str_replace_all(content, "__BUILD_TIMESTAMP_STR__", ts_str);
    content = str_replace_all(content, "__BUILD_DATE_STR__",      date_str);
    content = str_replace_all(content, "__BUILD_TIME_STR__",      time_str);
    content = str_replace_all(content, "__BUILD_YEAR__",          year_str);
    content = str_replace_all(content, "__BUILD_MONTH__",         month_str);
    content = str_replace_all(content, "__BUILD_DAY__",           day_str);
    content = str_replace_all(content, "__BUILD_HOUR__",          hour_str);
    content = str_replace_all(content, "__BUILD_MINUTE__",        min_str);
    content = str_replace_all(content, "__BUILD_SECOND__",        sec_str);
    content = str_replace_all(content, "__BUILD_UNIX_TS__",       unix_str);

    return content;
}

/* -------------------------------------------------------------------------
 * FNV-1a hash functions
 * ---------------------------------------------------------------------- */

uint32_t fnv1a_32(const char *str, size_t len) {
    uint32_t hash = 2166136261u;
    for (size_t i = 0; i < len; i++) {
        hash ^= (uint8_t)str[i];
        hash *= 16777619u;
    }
    return hash;
}

uint64_t fnv1a_64(const char *str, size_t len) {
    uint64_t hash = 14695981039346656037ull;
    for (size_t i = 0; i < len; i++) {
        hash ^= (uint8_t)str[i];
        hash *= 1099511628211ull;
    }
    return hash;
}

/* -------------------------------------------------------------------------
 * STRHASH / STRHASH64 macro evaluation
 * ---------------------------------------------------------------------- */

char *evaluate_string_macros(char *content, const char *macro_name,
                              int is_64bit) {
    char   prefix[64];
    snprintf(prefix, sizeof(prefix), "%s(\"", macro_name);
    size_t prefix_len = strlen(prefix);
    int    count      = 0;
    char  *pos        = content;

    while (1) {
        char *match = strstr(pos, prefix);
        if (!match) break;

        char *str_start = match + prefix_len;
        char *str_end   = str_start;
        while (*str_end && *str_end != '"') str_end++;

        if (*str_end != '"' || str_end[1] != ')') {
            pos = match + prefix_len;
            continue;
        }

        size_t str_len = (size_t)(str_end - str_start);
        char   hash_str[32];
        if (is_64bit) {
            uint64_t hash = fnv1a_64(str_start, str_len);
            snprintf(hash_str, sizeof(hash_str), "%llu",
                     (unsigned long long)hash);
        } else {
            uint32_t hash = fnv1a_32(str_start, str_len);
            snprintf(hash_str, sizeof(hash_str), "%u", (unsigned int)hash);
        }

        size_t match_full_len = (size_t)(str_end + 2 - match);
        size_t hash_len       = strlen(hash_str);
        size_t old_total      = strlen(content);
        size_t new_total      = old_total - match_full_len + hash_len;

        char  *new_content  = (char *)malloc(new_total + 1);
        size_t before_len   = (size_t)(match - content);
        memcpy(new_content, content, before_len);
        memcpy(new_content + before_len, hash_str, hash_len);
        strcpy(new_content + before_len + hash_len, str_end + 2);

        free(content);
        content = new_content;
        pos     = content + before_len + hash_len;
        count++;
    }

    if (count > 0)
        printf("Evaluated %d %s() call(s)\n", count, macro_name);

    return content;
}

/* -------------------------------------------------------------------------
 * STRHASH macro definition stripping
 * ---------------------------------------------------------------------- */

char *strip_strhash_macros(char *content) {
    size_t content_len = strlen(content);
    char  *result      = (char *)malloc(content_len + 1);
    char  *dst         = result;
    const char *p      = content;
    int skip_continuation = 0;

    while (*p) {
        const char *line_start = p;
        const char *line_end   = p;
        while (*line_end && *line_end != '\n') line_end++;
        size_t line_len = (size_t)(line_end - line_start);

        if (skip_continuation) {
            const char *check = line_end - 1;
            while (check > line_start &&
                   (*check == ' ' || *check == '\t' || *check == '\r'))
                check--;
            skip_continuation = (check >= line_start && *check == '\\');
            p = line_end;
            if (*p == '\n') p++;
            continue;
        }

        const char *trimmed = line_start;
        while (trimmed < line_end && (*trimmed == ' ' || *trimmed == '\t'))
            trimmed++;

        int should_strip = 0;

        if ((size_t)(line_end - trimmed) > 7 &&
            strncmp(trimmed, "#define", 7) == 0) {
            const char *after = trimmed + 7;
            while (after < line_end && (*after == ' ' || *after == '\t'))
                after++;

#define MATCH_DEFINE(name, nlen) \
            (strncmp(after, (name), (nlen)) == 0 && \
             (after + (nlen) >= line_end || \
              (!isalnum((unsigned char)after[(nlen)]) && after[(nlen)] != '_')))

            if      (MATCH_DEFINE("STRHASH64",     9))  should_strip = 1;
            else if (MATCH_DEFINE("STRHASH",        7))  should_strip = 1;
            else if (MATCH_DEFINE("FNV_OFFSET_64", 13))  should_strip = 1;
            else if (MATCH_DEFINE("FNV_PRIME_64",  12))  should_strip = 1;
            else if (MATCH_DEFINE("FNV_OFFSET",    10))  should_strip = 1;
            else if (MATCH_DEFINE("FNV_PRIME",      9))  should_strip = 1;
            else if ((size_t)(line_end - after) >= 3 &&
                     after[0] == 'H' && after[1] == 'F' &&
                     isdigit((unsigned char)after[2]))    should_strip = 1;

#undef MATCH_DEFINE
        }

        if (should_strip) {
            const char *check = line_end - 1;
            while (check > line_start &&
                   (*check == ' ' || *check == '\t' || *check == '\r'))
                check--;
            skip_continuation = (check >= line_start && *check == '\\');
            p = line_end;
            if (*p == '\n') p++;
        } else {
            memcpy(dst, line_start, line_len);
            dst += line_len;
            if (*line_end == '\n') *dst++ = '\n';
            p = line_end;
            if (*p == '\n') p++;
        }
    }

    *dst = '\0';
    free(content);
    return result;
}