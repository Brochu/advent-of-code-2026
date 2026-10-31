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

#define arr_header(a) ((arr_meta *)(a) - 1)
#define arr_size(a) (arr_header(a)->size)
#define arr_cap(a) (arr_header(a)->cap)
#define arr_item_size(a) (arr_header(a)->item_size)

#define arr_clear(a) (arr_header(a)->size = 0)
#define arr_free(a) (free(arr_header(a)))

void *_arr_init(size_t it_size, size_t cap);
void _arr_adapt(void **arr, size_t count);
void _arr_erase(void *arr, size_t i);

#define _ARR_INIT_CAP 256
template <typename T> T *arr_init(size_t capacity = _ARR_INIT_CAP) { return (T *)_arr_init(sizeof(T), capacity); }

template <typename T>
void arr_push(T **a, T v) {
    _arr_adapt((void **)a, 1);

    T *arr = *a;
    arr[arr_size(arr)] = v;
    arr_header(arr)->size++;
}

template <typename T>
void arr_popback(T *a) {
    assert(arr_size(a) > 0);
    arr_header(a)->size--;
}

template <typename T> void arr_erase(T *a, size_t i) { _arr_erase(a, i); }
