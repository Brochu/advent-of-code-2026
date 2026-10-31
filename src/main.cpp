#include <cassert>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#include "darray.hpp"
#include "day.hpp"
#include "parse.hpp"
#include "timer.hpp"

void convert_file(const char *from, const char *to);

int main(int argc, char **argv) {
    /*
    // Fully buffer stdout: output reaches the console in one write at exit
    // instead of one slow console write per printf.
    static char out_buf[1 << 20];
    setvbuf(stdout, out_buf, _IOFBF, sizeof(out_buf));
    */

    if (argc >= 4 && strcmp(argv[1], "xvert") == 0) {
        convert_file(argv[2], argv[3]);
        return 0;
    }

    char part1[ANS_SIZE];
    memset(part1, 0, ANS_SIZE);

    char part2[ANS_SIZE];
    memset(part2, 0, ANS_SIZE);

    int res = solve(part1, part2);
    printf("[AoC2026] Day %i solved with code %i\n", DAY, res);
    printf("    - Part1 : '%s'\n", part1);
    printf("    - Part2 : '%s'\n", part2);

    return 0;
}

// !! INPUT CONVERTER !!
void convert_file(const char *from, const char *to) {
    printf("[AoC2026] Convert tool | '%s' -> '%s'\n", from, to);

    FILE *f;
    fopen_s(&f, from, "rb");
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    rewind(f);

    char *content = (char *)malloc(fsize + 1);
    fread_s(content, fsize, 1, fsize, f);
    content[fsize] = '\0';
    printf("[AoC2026] fsize = %ld\n", fsize);
    printf("[AoC2026] Content = \n'%s'\n", content);

    fclose(f);

    fopen_s(&f, to, "wb");
    static int cols = 18;

    for (size_t i = 0; i <= fsize; i++) {
        fprintf_s(f, "0x%02x,", content[i]);

        if (((i+1) % cols) == 0) {
            fprintf_s(f, "\n");
        }
    }

    fclose(f);
    free(content);
}

// -----------------------------------------------------------------------------
// !! STRING MANIPULATION !!
strview sv_find(strview haystack, strview needle) {
    if (needle.len == 0 || needle.len > haystack.len) {
        return { nullptr, 0 };
    }

    for (size_t i = 0; i <= haystack.len - needle.len; i++) {
        if (memcmp(haystack.ptr + i, needle.ptr, needle.len) == 0) {
            return { haystack.ptr + i, needle.len };
        }
    }

    return { nullptr, 0 };
}
strview sv_find(strview haystack, const char *needle) {
    return sv_find(haystack, sv(needle));
}

void sv_split(strview str, strview delim, strview **out) {
    size_t pos = 0;

    while (pos < str.len) {
        strview start(str.ptr + pos, str.len - pos);
        size_t end = (sv_find(start, delim).ptr - str.ptr);
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
void sv_split(strview str, const char *delim, strview **out) {
    sv_split(str, sv(delim), out);
}

bool sv_split_once(strview str, strview delim, strview* first, strview* second) {
    strview found = sv_find(str, delim);
    if (found.ptr == nullptr) {
        return false;
    }

    size_t pos = found.ptr - str.ptr;

    *first = { str.ptr, pos };
    *second = { found.ptr + delim.len, str.len - pos - delim.len };
    return true;
}
bool sv_split_once(strview str, const char *delim, strview* first, strview* second) {
    return sv_split_once(str, sv(delim), first, second);
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
void *_arr_init(size_t it_size, size_t cap) {
    void *ptr = 0;

    size_t size = (it_size * cap) + sizeof(arr_meta);
    arr_meta *h = (arr_meta *)malloc(size);
    if (h) {
        h->size = 0;
        h->cap = cap;
        h->item_size = it_size;
        ptr = h + 1;
    }

    return ptr;
}

void _arr_adapt(void **arr, size_t count) {
    arr_meta *h = arr_header(*arr);
    size_t target_cap = h->size + count;
    //printf("[ARR] Adapting array (count = %lld; item size = %lld)\n", count, h->item_size);

    if (h->cap < target_cap) {
        size_t new_cap = h->cap * 2;
        while (new_cap < target_cap) {
            new_cap *= 2;
        }

        size_t new_size = (new_cap * h->item_size) + sizeof(arr_meta);
        arr_meta *new_h = (arr_meta *)realloc(h, new_size);
        assert(new_h);

        new_h->cap = new_cap;
        *arr = (arr_meta *)new_h + 1;
    }
}

void _arr_erase(void *arr, size_t i) {
    arr_meta *h = arr_header(arr);
    assert(i < h->size);
    char *base = (char *)arr;
    memmove(base + (i * h->item_size), base + ((i + 1) * h->item_size), (h->size - (i + 1)) * h->item_size);
    h->size--;
}
// -----------------------------------------------------------------------------