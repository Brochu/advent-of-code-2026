#pragma once

#include <assert.h>
#include <bit>
#include <stdlib.h>

static constexpr size_t _HASH_TABLE_CAPACITY = 8;

template <typename K, typename V>
struct hash_table {
    K *keys;
    V *values;
    char *used;
    
    size_t cap;
    size_t size;
    
    static hash_table init(size_t capacity = _HASH_TABLE_CAPACITY);
    V *get(K key);
    void put(K key, V val);
    void free_hash();
};

template <typename K, typename V>
hash_table<K, V> hash_table<K, V>::init(size_t capacity) {
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