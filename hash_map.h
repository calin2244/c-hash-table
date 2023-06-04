#ifndef HASH_MAP_H
#define HASH_MAP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#define CAPACITY 100 // Table Size
#define ENTRIES "entries.txt"

// *FOR PERFORMANCE TESTING

#define max(a,b) \
    ({ __typeof__ (a) _a = (a); \
        __typeof__ (b) _b = (b); \
        _a > _b ? _a : _b; }) \

// *

typedef struct Ht_Item{
    void* key;
    size_t key_size; // in bytes
    void* val;
    struct Ht_Item* next;
}Ht_Item;

typedef struct HashTable{
    Ht_Item** buckets;
    size_t size; // The current size of the Hash Table
    size_t capacity; // Max Size
    int collisions;
    float load_factor;
    size_t (*hash_func)(const void*, size_t);
}HashTable;

typedef struct pair{
    size_t maxChainLen;
    size_t hashOfMaxChain;
}pair;

typedef void (*PrintHelper)(size_t, const void*, const void*);

size_t hash_func_str(const void* key, size_t capacity);
size_t get_num_of_buckets(HashTable* ht);
HashTable* ht_create(size_t capacity, size_t (*hash_func)(const void*, size_t));
void ht_resize(HashTable* ht, size_t new_capacity);
void ht_free(HashTable** ht);
void ht_insert(HashTable* ht, const void* key, const void* val);
bool ht_has_key(const HashTable* ht, const void* key);
Ht_Item* ht_remove(HashTable* ht, const void* key);
Ht_Item* ht_get(HashTable* ht, const void* key);
Ht_Item* ht_item_create(const void* key, const void* val);
void ht_modify_item(HashTable* ht, const void* key, const void* val);
void print_ht(HashTable* ht, PrintHelper);
void handle_collision_chaining(HashTable* ht, Ht_Item* item, const void* val);
void printHashTableInfo(HashTable* ht);
void parseFileAndPopulateHashTable(HashTable* ht, const char* file_name);
void parseFileAndRemoveEntries(HashTable* ht, const char* file_name);
void free_ht_item(Ht_Item** item);
void print_string_string(size_t hash, const void* key, const void* val);
pair maximumChainLength(HashTable* ht);

#endif // HASH_TABLE_H