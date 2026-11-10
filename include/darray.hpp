#pragma once

#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <type_traits>

#include "types.hpp"

typedef struct {
    u64 size;
    u64 cap;
    u64 item_size;
    u64 pad;
} arr_meta;

#define arr_header(a) ((arr_meta *)(a) - 1)
#define arr_size(a) (arr_header(a)->size)
#define arr_cap(a) (arr_header(a)->cap)
#define arr_item_size(a) (arr_header(a)->item_size)

#define arr_clear(a) (arr_header(a)->size = 0)
#define arr_free(a) (free(arr_header(a)))

void *_arr_init(u64 it_size, u64 cap);
void _arr_adapt(void **arr, u64 count);
void _arr_erase(void *arr, u64 i);

#define _ARR_INIT_CAP 256
template <typename T> T *arr_init(u64 capacity = _ARR_INIT_CAP) { return (T *)_arr_init(sizeof(T), capacity); }

template <typename T>
void arr_push(T **a, std::type_identity_t<T> v) {
    _arr_adapt((void **)a, 1);

    T *arr = *a;
    arr[arr_size(arr)] = v;
    arr_header(arr)->size++;
}

template <typename T>
T arr_popback(T *a) {
    assert(arr_size(a) > 0);
    T val = a[arr_size(a) - 1];
    arr_header(a)->size--;
    return val;
}

template <typename T>
T *arr_last(T *a) {
    assert(arr_size(a) > 0);
    return &a[arr_size(a)-1];
}

template <typename T> void arr_erase(T *a, u64 i) { _arr_erase(a, i); }

template <typename T>
void arr_swap_remove(T *a, u64 i) {
    assert(i < arr_size(a));

    a[i] = a[arr_size(a)-1];
    arr_header(a)->size--;
}

template <typename T>
T *arr_copy(T *a) {
    T *out = arr_init<T>(arr_cap(a));
    arr_header(out)->size = arr_size(a);

    u64 bytes = arr_size(a) * arr_item_size(a);
    memcpy_s(out, bytes, a, bytes);
    return out;
}