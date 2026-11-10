#include <cassert>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#include "darray.hpp"
#include "day.hpp"
#include "grid.hpp"
#include "parse.hpp"
#include "timer.hpp"
#include "types.hpp"

i32 main(i32 argc, char **argv) {
    /*
    // Fully buffer stdout: output reaches the console in one write at exit
    // instead of one slow console write per printf.
    static char out_buf[1 << 20];
    setvbuf(stdout, out_buf, _IOFBF, sizeof(out_buf));
    */
    char part1[ANS_SIZE];
    char part2[ANS_SIZE];

    memset(part1, 0, ANS_SIZE);
    memset(part2, 0, ANS_SIZE);

    i32 res = solve(part1, part2);
    printf("[AoC2026] Day %i solved with code %i\n", DAY, res);
    printf("    - Part1 : '%s'\n", part1);
    printf("    - Part2 : '%s'\n", part2);

    return 0;
}

// -----------------------------------------------------------------------------
// !! STRING MANIPULATION !!
bool sv_eq(strview a, strview b) {
    return a.len == b.len && memcmp(a.ptr, b.ptr, a.len) == 0;
}

strview sv_find(strview haystack, strview needle) {
    if (needle.len == 0 || needle.len > haystack.len) {
        return { nullptr, 0 };
    }

    for (u64 i = 0; i <= haystack.len - needle.len; i++) {
        if (memcmp(haystack.ptr + i, needle.ptr, needle.len) == 0) {
            return { haystack.ptr + i, needle.len };
        }
    }

    return { nullptr, 0 };
}

void sv_split(strview str, strview delim, strview **out) {
    u64 pos = 0;

    while (pos < str.len) {
        strview start(str.ptr + pos, str.len - pos);
        u64 end = str.len;

        strview found = sv_find(start, delim);
        if (found.ptr != nullptr) {
            end = found.ptr - str.ptr;
        }

        if (end >= str.len) {
            strview last = { str.ptr + pos, str.len - pos };
            arr_push(out, last);
            break;
        }
        strview part = { str.ptr + pos, end - pos };
        arr_push(out, part);
        pos = end + delim.len; // skip over the whole delimiter
    }
}

bool sv_split_once(strview str, strview delim, strview* first, strview* second) {
    strview found = sv_find(str, delim);
    if (found.ptr == nullptr) {
        return false;
    }

    u64 pos = found.ptr - str.ptr;

    *first = { str.ptr, pos };
    *second = { found.ptr + delim.len, str.len - pos - delim.len };
    return true;
}

bool sv_next_int(strview *str, i64 *out) {
    u64 i = 0;
    for (; i < str->len; i++) {
        if (str->ptr[i] == '-' && (i + 1) < str->len && is_digit(str->ptr[i+1])) {
            break;
        }
        if (is_digit(str->ptr[i])) {
            break;
        }
    }
    if (i >= str->len) {
        // Cannot find the start of a number in the string
        return false;
    }
    *out = 0;
    
    i8 sign = 1;
    if (str->ptr[i] == '-') {
        sign = -1;
        i++;
    }
    
    while (i < str->len && is_digit(str->ptr[i])) {
        *out *= 10;
        *out += (str->ptr[i++] - '0');
    }
    *str = sv_substr(*str, i);
    *out *= sign;
    return true;
}

strview sv_substr(strview str, u64 start) {
    assert(start <= str.len);
    return { str.ptr + start, str.len - start };
}
strview sv_substr(strview str, u64 start, u64 len) {
    assert(start <= str.len);
    assert(len <= str.len - start);
    return { str.ptr + start, len };
}
// -----------------------------------------------------------------------------
// !! TIMER !!
LARGE_INTEGER freq = { 0 };

u64 time_get_counter() {
    LARGE_INTEGER counter;
    if (!QueryPerformanceCounter(&counter)) {
        return 0; // Could not query perf counter
    }

    return counter.QuadPart;
}

f64 time_elapsed_seconds(u64 a, u64 b) {
    if (freq.QuadPart == 0) {
        if (!QueryPerformanceFrequency(&freq)) {
            return -1.0; // Could not query perf frequency
        }
    }

    uint64_t diff = b - a;
    double time = (double)diff / (double)freq.QuadPart;
    return time;
}
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// !! DARRAY !!
void *_arr_init(u64 it_size, u64 cap) {
    assert(cap != 0);
    void *ptr = 0;

    u64 size = (it_size * cap) + sizeof(arr_meta);
    arr_meta *h = (arr_meta *)malloc(size);
    if (h) {
        h->size = 0;
        h->cap = cap;
        h->item_size = it_size;
        ptr = h + 1;
    }

    return ptr;
}

void _arr_adapt(void **arr, u64 count) {
    arr_meta *h = arr_header(*arr);
    u64 target_cap = h->size + count;
    //printf("[ARR] Adapting array (count = %lld; item size = %lld)\n", count, h->item_size);

    if (h->cap < target_cap) {
        u64 new_cap = h->cap * 2;
        while (new_cap < target_cap) {
            new_cap *= 2;
        }

        u64 new_size = (new_cap * h->item_size) + sizeof(arr_meta);
        arr_meta *new_h = (arr_meta *)realloc(h, new_size);
        assert(new_h);

        new_h->cap = new_cap;
        *arr = (arr_meta *)new_h + 1;
    }
}

void _arr_erase(void *arr, u64 i) {
    arr_meta *h = arr_header(arr);
    assert(i < h->size);
    char *base = (char *)arr;
    memmove(base + (i * h->item_size), base + ((i + 1) * h->item_size), (h->size - (i + 1)) * h->item_size);
    h->size--;
}
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// !! GRID !!
extern const vec2 DIRS4[4] = {
    {  0,  1 },
    {  1,  0 },
    {  0, -1 },
    { -1,  0 },
};
extern const vec2 DIRS8[8] = {
    {  0,  1 },
    {  1,  1 },
    {  1,  0 },
    {  1, -1 },
    {  0, -1 },
    { -1, -1 },
    { -1,  0 },
    { -1,  1 },
};