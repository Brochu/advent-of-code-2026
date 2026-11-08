#pragma once

#include <assert.h>
#include <bit>
#include <stdlib.h>

#include "parse.hpp"
#include "types.hpp"

inline u64 hash(u64 key) {
    return 0;
}
inline u64 hash(strview key) {
    return 0;
}

template <typename T>
inline bool key_eq(T a, T b) { return a == b; }
inline bool key_eq(strview a, strview b) { return a.len == b.len && memcmp(a.ptr, b.ptr, a.len) == 0; }

static constexpr u64 _HASH_TABLE_CAPACITY = 8;

template <typename K, typename V>
struct hash_table {
    K *keys;
    V *values;
    char *used;
    
    u64 cap;
    u64 size;
    
    static hash_table init(u64 capacity = _HASH_TABLE_CAPACITY);
    bool has(K key);
    V *get(K key);

    void put(K key, V val);
    void free_hash();
};

template <typename K, typename V>
hash_table<K, V> hash_table<K, V>::init(u64 capacity) {
    hash_table t = { };
    t.cap = std::bit_ceil(capacity);
    t.size = 0;

    t.keys = (K *)malloc(sizeof(K) * t.cap);
    assert(t.keys);
    t.values = (V *)malloc(sizeof(V) * t.cap);
    assert(t.values);
    t.used = (char *)calloc(t.cap, sizeof(char));
    assert(t.used);
    
    return t;
}

template <typename K, typename V>
bool hash_table<K, V>::has(K key) {
    //TODO: Lookup if key exists
    return false;
}

template <typename K, typename V>
V *hash_table<K, V>::get(K key) {
    //TODO: Hash, probe, return
    return nullptr;
}

template <typename K, typename V>
void hash_table<K, V>::put(K key, V val) {
    //TODO: Handle insertion, maybe grow
}

template <typename K, typename V>
void hash_table<K, V>::free_hash() {
    cap = 0;
    size = 0;
    
    free(used);
    used = nullptr;
    free(values);
    values = nullptr;
    free(keys);
    keys = nullptr;
}