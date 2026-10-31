#pragma once

#include <assert.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    size_t size;
    size_t cap;
    size_t item_size;
    size_t pad;
} arr_meta;

#define _ARR_INIT_CAP 256
#define arr_init(T) (_arr_init(sizeof(T), _ARR_INIT_CAP))
#define arr_header(a) ((arr_meta *)(a) - 1)
#define arr_size(a) (arr_header(a)->size)
#define arr_cap(a) (arr_header(a)->cap)
#define arr_itemsize(a) (arr_header(a)->item_size)
#define arr_push(a, v)                  \
    do {                                \
        _arr_adapt((void**)&(a), 1);    \
        (a)[arr_size(a)] = (v);         \
        arr_header(a)->size++;          \
    } while(0)
#define arr_popback(a)                   \
    do {                                 \
        assert(arr_header(a)->size > 0); \
        arr_header(a)->size--;           \
    } while(0)
#define arr_erase(a, i) (_arr_erase((a), (i)))
#define arr_clear(a) (arr_header(a)->size = 0)
#define arr_free(a) (free(arr_header(a)))

void *_arr_init(size_t it_size, size_t cap);

void _arr_adapt(void **arr, size_t count);

void _arr_erase(void *arr, size_t i);
