#pragma once

#include <utility>

#include "darray.hpp"
#include "types.hpp"

template <typename T> void heap_push(T **h, std::type_identity_t<T> v) {
    arr_push(h, v);
    T *arr = *h;
    u64 i = arr_size(arr) - 1;
    
    while (i > 0) {
        u64 p = (i - 1) / 2;
        if (!(arr[i] < arr[p])) {
            break;
        }

        std::swap(arr[i], arr[p]);
        i = p;
    }
}

template <typename T> T heap_pop(T *h) {
    assert(arr_size(h) > 0);
    T out = h[0];
    arr_swap_remove(h, 0);
    
    u64 i = 0;
    
    while (true) {
        u64 l = (2*i) + 1, r = l + 1, m = i;
        
        if (l < arr_size(h) && h[l] < h[m]) m = l;
        if (r < arr_size(h) && h[r] < h[m]) m = r;
        if (m == i) break;
        
        std::swap(h[i], h[m]);
        i = m;
    }
    return out;
}

template <typename T> T *heap_peek(T *h) {
    assert(arr_size(h) > 0);
    return &h[0];
}
