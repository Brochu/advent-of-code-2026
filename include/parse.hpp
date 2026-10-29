#pragma once

#include <string.h>

#include "darray.h"

struct strview {
    const char* ptr;
    size_t len;
};

inline strview sv(const char *s) {
    return { s, strlen(s) };
}

strview sv_find(strview haystack, strview needle);
strview sv_find(strview haystack, const char *needle);

// Appends the parts to *out, a darray owned by the caller.
void sv_split(strview str, strview delim, strview **out);
void sv_split(strview str, const char *delim, strview **out);

bool sv_split_once(strview str, strview delim, strview* first, strview* second);
bool sv_split_once(strview str, const char *delim, strview* first, strview* second);
