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
#define SMALL_CAPACITY 1000
#define MEDIUM_CAPACITY 10000
#define SENTINEL {NULL, NULL, 0}
#define IS_SENTINEL(kv) (!(kv).key && !(kv).value && !(kv).val_size)
#define IS_CHAINING(ht) (ht->coll_resolution == CHAINING)
#define IS_OA(ht) (ht->coll_resolution >= LINEAR_PROBING)
// OA = Open-Addressing, so for both LP and QP

typedef enum{
    HT_SUCCESS = 0,
    HT_NO_ACTION
}Ht_ExitCode;

typedef enum{
    CHAINING = 0,
    LINEAR_PROBING,
    QUADRATIC_PROBING, 
    DOUBLE_HASHING,
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

/***********************
 *       STRUCTS       *
 ***********************/
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

typedef struct ResizeStrategy{
    size_t capacity_threshold;
    float resize_factor;
}ResizeStrategy;

typedef struct HashTable {
    // Size and Capacity
    size_t size;                // Current number of elements
    size_t capacity;            // Maximum number of elements before resizing

    // Load and Resizing
    float load_factor;          // Current load of the table (size/capacity)
    float resize_threshold;     // Threshold for when to resize
    float resize_factor;        // Current factor by which to resize
    ResizeStrategy* resize_strategies; // User-provided resize strategies
    size_t num_resize_strategies;

    // Collision info
    int collisions;             // Total collisions encountered
    CollisionResolution coll_resolution; // Strategy for handling collisions

    // Hashing
    size_t (*hash_func)(const char*, size_t);       // Primary hash function
    size_t (*hash_func2)(const char*, size_t);      // Secondary hash function (for double hashing)

    // Data
    Ht_Item** buckets;          // Array of pointers to items

} HashTable;

/**************************
 *      HASH FUNCTIONS    *
 **************************/
size_t fnv_hash_func(const char* key, size_t capacity);
size_t fnv_double_hash_func(const char* key, size_t capacity);
size_t jenkins_hash_func(const char* key, size_t capacity);

/*******************************
 *    COLLISION HANDLING       *
 *******************************/
void handle_collision(Ht_Item* item, const void* val, size_t val_size);    
void handle_insertion_chaining(HashTable* ht, size_t idx, const char* key, const void* val, size_t val_size);
void handle_insertion_lp(HashTable* ht, size_t idx, const char* key, const void* val, size_t val_size);
void handle_insertion_qp(HashTable* ht, size_t idx, const char* key, const void* val, size_t val_size);
void handle_insertion_double_hashing(HashTable* ht, size_t idx, const char* key, const void* val, size_t val_size);
// void handle_insertion_robin_hood(HashTable* ht, size_t idx, const char* key, const void* val, size_t val_size);
void handle_tombstones(Ht_Item* itm, const char* key, const void* val, size_t val_size);
Ht_ExitCode handle_tombstones_removal(HashTable* ht, size_t idx, const char* key);

/*****************************
 *   CORE FUNCTIONALITIES    *
 *****************************/
HashTable* ht_create(size_t capacity, size_t (*hash_func)(const char*, size_t), CollisionResolution coll_res);
void ht_set_resize_threshold(HashTable* ht, float resize_threshold);
void ht_resize(HashTable* ht, size_t new_capacity);
void ht_set_custom_resize_strategy(HashTable* ht, size_t capacity_threshold, float associated_resize_factor);
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
void update_resize_factor(HashTable* ht);

/****************************
 *   HELPER FUNCTIONS       *
 ****************************/
bool isPrime(size_t num);
size_t generateNextGreaterPrimeNumber(size_t num);
void ht_print(HashTable* ht, PrintHelper printHasht);
void print_str_str(size_t hash, const char* key, const void* val);
void ht_set_second_hash_func(HashTable* ht, size_t (*hash_func2)(const char*, size_t));
void calculate_next_probe_index(const HashTable* ht, size_t* idx, size_t* curr_power, size_t* probes, size_t hash1, size_t hash2);

#endif // HASH_TABLE_H
