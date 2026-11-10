#pragma once

#include <assert.h>
#include <bit>
#include <stdlib.h>

#include "parse.hpp"
#include "types.hpp"

inline u64 hash(u64 key) {
    key ^= key >> 30;
    key *= 0xbf58476d1ce4e5b9ull;
    key ^= key >> 27;
    key *= 0x94d049bb133111ebull;
    key ^= key >> 31;
    return key;
}
inline u64 hash(strview key) {
    u64 h = 0xcbf29ce484222325ull;
    for (u64 i = 0; i < key.len; i++) {
        h ^= (u8)key.ptr[i];
        h *= 0x100000001b3ull;
    }
    return hash(h);
}

template <typename T>
inline bool key_eq(T a, T b) { return a == b; }
inline bool key_eq(strview a, strview b) { return sv_eq(a, b); }

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
    void clear();
    void free_hash();
    
private:
    u64 find_slot(K key);
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
    u64 index = find_slot(key);
    return used[index] == 1;
}

template <typename K, typename V>
V *hash_table<K, V>::get(K key) {
    u64 index = find_slot(key);
    return (used[index] == 1) ? &values[index] : nullptr;
}

template <typename K, typename V>
void hash_table<K, V>::put(K key, V val) {
    if ((size + 1) * 10 > cap * 7) {
        // We will be using more than 70% of allocated space for the table
        K *old_keys = keys;
        V *old_values = values;
        char *old_used = used;
        u64 old_cap = cap;
        
        size = 0;
        cap *= 2;

        keys = (K *)malloc(sizeof(K) * cap);
        assert(keys);
        values = (V *)malloc(sizeof(V) * cap);
        assert(values);
        used = (char *)calloc(cap, sizeof(char));
        assert(used);
        
        for (u64 i = 0; i < old_cap; i++) {
            if (old_used[i] == 0) {
                continue;
            }
            
            u64 index = find_slot(old_keys[i]);
            used[index] = 1;
            keys[index] = old_keys[i];
            values[index] = old_values[i];
            size++;
        }
        
        free(old_keys);
        free(old_values);
        free(old_used);
    }

    u64 index = find_slot(key);
    if (used[index] == 1) {
        values[index] = val;
    } else {
        used[index] = 1;
        keys[index] = key;
        values[index] = val;
        size++;
    }
}

template <typename K, typename V>
void hash_table<K, V>::clear() {
    size = 0;
    memset(used, 0, cap * sizeof(char));
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

template <typename K, typename V>
u64 hash_table<K, V>::find_slot(K key) {
    u64 pos = hash(key) & (cap - 1);

    for (pos; used[pos] == 1; pos = (pos + 1) & (cap - 1)) {
        if (key_eq(keys[pos], key)) {
            return pos;
        }
    }
    
    return pos;
}