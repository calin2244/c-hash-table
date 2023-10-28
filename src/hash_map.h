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
#define OA_THRESHOLD 0.65f
#define INCREMENTAL_RESIZING 0.5f
#define SENTINEL {NULL, NULL, 0}
#define IS_SENTINEL(kv) (!(kv).key && !(kv).value && !(kv).val_size)
// OA = Open-Addressing, so for both LP and QP

typedef enum{
    CHAINING = 0,
    LINEAR_PROBING,
    QUADRATIC_PROBING, 
    ROBIN_HOOD // TODO: Implemnt this
}CollisionResolution;

typedef void (*PrintHelper)(size_t, const char*, const void*);

/* 
    !NOTES!!
    * KEYs currently work only as char*,
    * because template metaprogramming isn't a thing in C
    * it's hard to know the type of the key beforehand
    * and it would require a more messy implementation to do that.
    * If you need numbers as keys, just convert your integers
    * into a string using sprintf/itoa
   
    !SIDE NOTE
    * Talking about Open-Addressing Resolutions
    * For example the hash of they key return by the hashing function is 7, 
    * the capacity is 10 and slots 7 through 9 are all full. The algorithm
    * will go back to the start of the hash table and find the first empty
    * slot, where he will insert the new key into.
*/ 

typedef struct Ht_Item{
    char* key;
    void* val;
    size_t val_size;
    bool is_tombstone; // *Used for Open Addresing Coll Resolutions
    int64_t distance; // *Probing distance(Used for Robin Hood CollRes)
    struct Ht_Item* next;
}Ht_Item;

// Used for Bulk-Inserting
typedef struct KeyValuePair{
    const char* key;
    const void* value;
    size_t val_size;
}KeyValuePair;

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
void handle_insertion_chaining(HashTable* ht, size_t idx, const char* key, const void* val, size_t val_size);
void handle_insertion_lp(HashTable* ht, size_t idx, const char* key, const void* val, size_t val_size);
void handle_insertion_qp(HashTable* ht, size_t idx, const char* key, const void* val, size_t val_size);
// void handle_insertion_robin_hood(HashTable* ht, size_t idx, const char* key, const void* val, size_t val_size);
void handle_tombstones(Ht_Item* itm, const char* key, const void* val, size_t val_size);

// Hash Table Functions
HashTable* ht_create(size_t capacity, size_t (*hash_func)(const char*, size_t), CollisionResolution coll_res);
void ht_resize(HashTable* ht, size_t new_capacity);
void free_ht(HashTable** ht);
void ht_insert(HashTable* ht, const char* key, const void* val, size_t val_size);
void ht_bulk_insert(HashTable* ht, const KeyValuePair* kv, size_t length);
bool ht_has_key(const HashTable* ht, const char* key);
size_t ht_remove(HashTable* ht, const char* key);
void ht_bulk_remove(HashTable* ht, const char** keys, size_t rows);
void* ht_get_item(HashTable* ht, const char* key);
Ht_Item* ht_item_create(const char* key, const void* val, size_t val_size);
void ht_modify_item(HashTable* ht, const char* key, const void* val, size_t val_size);
void free_ht_item(Ht_Item* item);
void ht_clear(HashTable* ht);
bool ht_purge_slot(HashTable* ht, size_t idx);
void ht_print(HashTable* ht, PrintHelper printHasht);

// Helper Functions
bool isPrime(size_t num);
size_t generateNextGreaterPrimeNumber(size_t num);
void print_str_str(size_t hash, const char* key, const void* val);

#endif // HASH_TABLE_H