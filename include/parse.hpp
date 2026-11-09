#pragma once

#include <string.h>

#include "darray.hpp"
#include "types.hpp"

struct strview {
    const char* ptr;
    u64 len;
};

inline strview sv(const char *s) {
    return { s, strlen(s) };
}

inline bool is_digit(char c) {
    return c >= '0' && c <= '9';
}

bool sv_eq(strview a, strview b);
strview sv_find(strview haystack, strview needle);

void sv_split(strview str, strview delim, strview **out);
bool sv_split_once(strview str, strview delim, strview* first, strview* second);

bool sv_next_int(strview *str, i64 *out);

strview sv_substr(strview str, u64 start);
strview sv_substr(strview str, u64 start, u64 len);
