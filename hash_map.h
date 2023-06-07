#ifndef HASH_MAP_H
#define HASH_MAP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#define CAPACITY 100 // Table Size
#define THRESHOLD 0.7f
#define ENTRIES "entries.txt"

// *FOR PERFORMANCE TESTING

#define max(a,b) \
    ({ __typeof__ (a) _a = (a); \
        __typeof__ (b) _b = (b); \
        _a > _b ? _a : _b; }) \

// *

typedef struct Ht_Item{
    void* key;
    void* val;
    struct Ht_Item* next;
}Ht_Item;

typedef struct HashTable{
    size_t size; // The current size of the Hash Table
    size_t capacity; // Max Size
    int collisions;
    float load_factor;
    Ht_Item** buckets;
    size_t (*hash_func)(const void*, size_t);
    void (*handle_collision)(Ht_Item* item, const void* val, size_t val_size);
}HashTable;

size_t hash_func_str(const void* key, size_t capacity);
HashTable* ht_create(size_t capacity, size_t(*hash_func)(const void*, size_t), void(*handle_collision)(Ht_Item*, const void* val, size_t val_size));
void ht_resize(HashTable* ht, size_t new_capacity, size_t key_size, size_t value_size);
void ht_free(HashTable** ht);
void ht_insert(HashTable* ht, const void* key, size_t key_size, const void* val, size_t val_size);
bool ht_has_key(const HashTable* ht, const void* key);
Ht_Item* ht_remove(HashTable* ht, const void* key);
Ht_Item* ht_get_item(HashTable* ht, const void* key);
Ht_Item* ht_item_create(const void* key, size_t key_size, const void* val, size_t val_size);
void ht_modify_item(HashTable* ht, const void* key, const void* val);
void* ht_search(HashTable* ht, const void* key);
void handle_collision_chaining(Ht_Item* item, const void* val, size_t val_size);
void printHashTableInfo(HashTable* ht);
void free_ht_item(Ht_Item** item);
void print_string_string(size_t hash, const void* key, const void* val);

#endif // HASH_TABLE_H