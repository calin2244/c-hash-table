#ifndef HASH_MAP_H
#define HASH_MAP_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#define CAPACITY 100 // Table Size
#define CHAINING_THRESHOLD 0.7f
#define OA_THRESHOLD 0.45f
#define INCREMENTAL_RESIZING 0.5f
// OA = Open-Addressing, so for both LP and QP


// *FOR PERFORMANCE TESTING
#define max(a,b) \
    ({ __typeof__ (a) _a = (a); \
        __typeof__ (b) _b = (b); \
        _a > _b ? _a : _b; }) \

// *

typedef enum{
    CHAINING = 0,
    LINEAR_PROBING,
    QUADRATIC_PROBING, // TODO: Add tombstone Logic
    ROBIN_HOOD // TODO: Implemnt this
}CollisionResolution;

/* 
    *NOTES!!
    * KEYs currently work only as char*,
    * because template metaprogramming isn't a thing in C
    * it's hard to know the type of the key beforehand
    * and it would require a more messy implementation to do that.
    * If you need numbers as keys, just convert your integers
    * into a string using sprintf/itoa
*/

typedef struct Ht_Item{
    char* key;
    void* val;
    size_t val_size;
    bool is_tombstone; // *Used for Open Addresing Coll Resolutions
    struct Ht_Item* next;
}Ht_Item;

typedef struct HashTable{
    size_t size; // *The current size of the Hash Table
    size_t capacity; // *Max Size
    int collisions; // *Total number of collisions
    float load_factor; // *Resizing Mechanism
    Ht_Item** buckets;
    size_t (*hash_func)(const char*, size_t);
    CollisionResolution coll_resolution;
}HashTable;

// Hash Functions
size_t fnv_hash_func(const char* key, size_t capacity);
size_t fnv_double_hash_func(const char* key, size_t capacity);
size_t jenkins_hash_func(const char* key, size_t capacity);

// Collision Handling
void handle_collision(Ht_Item* item, const void* val, size_t val_size);    
void handle_collision_chaining(HashTable* ht, size_t idx, const char* key, const void* val, size_t val_size);
void handle_collision_lp(HashTable* ht, size_t idx, const char* key, const void* val, size_t val_size);
void handle_collision_qp(HashTable* ht, size_t idx, const char* key, const void* val, size_t val_size);
void handle_tombstones(Ht_Item* itm, const char* key, const char* val, size_t val_size);

// Hash Table Functions
HashTable* ht_create(size_t capacity, size_t (*hash_func)(const char*, size_t), CollisionResolution coll_res);
void ht_resize(HashTable* ht, size_t new_capacity);
void free_ht(HashTable** ht);
void ht_insert(HashTable* ht, const char* key, const void* val, size_t val_size);
bool ht_has_key(const HashTable* ht, const char* key);
bool ht_remove(HashTable* ht, const char* key);
void* ht_get_item(HashTable* ht, const char* key);
Ht_Item* ht_item_create(const char* key, const void* val, size_t val_size);
void ht_modify_item(HashTable* ht, const char* key, const void* val, size_t val_size);
void free_ht_item(Ht_Item* item);
void clear_ht(HashTable* ht);
void ht_purge_slot(HashTable* ht);

#endif // HASH_TABLE_H