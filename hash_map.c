#include "hash_map.h"


// Hash Functions
size_t hash_func_str(const void* key, size_t capacity){
    size_t hash = 0xcbf29ce484222325; // FNV_offset_basis
    const char* str = (const char*)key;
    while(*str){
        hash ^= (uint8_t)*str++;
        hash *= 0x100000001b3; // FNV_prime
    }

    return hash % capacity;
}

HashTable* ht_create(size_t capacity, size_t(*hash_func)(const void*, size_t), CollisionResolution coll_res){
    HashTable* hash_t = (HashTable*)malloc(sizeof(HashTable));

    if(!hash_t){ // malloc failed
        return NULL;
    } 

    hash_t->size = 0;
    hash_t->capacity = capacity;
    hash_t->collisions = 0;
    hash_t->load_factor = 0;
    hash_t->buckets = (Ht_Item**)calloc(hash_t->capacity, sizeof(Ht_Item*)); 

    if(!hash_t->buckets){
        free(hash_t);
        return NULL;
    }

    hash_t->hash_func = hash_func;
    hash_t->coll_resolution = coll_res;
    return hash_t;
}

void ht_resize(HashTable* ht, size_t new_capacity, size_t key_size, size_t value_size){
    // Create a new hash table with the new capacity
    HashTable* new_ht = ht_create(new_capacity, ht->hash_func, ht->coll_resolution);
    if(!new_ht){
        return;
    }

    // Iterate through the old buckets and rehash the items into the new hash table
    for(size_t i = 0; i < ht->capacity; ++i){
        Ht_Item* item = ht->buckets[i];
        while(item){
            ht_insert(new_ht, item->key, key_size, item->val, value_size);
            Ht_Item* next = item->next;
            free(item->key);
            free(item->val);
            free(item);
            item = next;
        }
    }

    // Free the old buckets
    free(ht->buckets);

    // Update the hash table properties
    ht->size = new_ht->size;
    ht->capacity = new_ht->capacity;
    ht->collisions = new_ht->collisions;
    ht->load_factor = 0;

    // Update the buckets pointer to point to the new buckets
    ht->buckets = new_ht->buckets;

    // Free the new hash table structure
    free(new_ht);
}

Ht_Item* ht_item_create(const void* key, size_t key_size, const void* val, size_t val_size){
    Ht_Item* new_item = (Ht_Item*)malloc(sizeof(Ht_Item));
    if(!new_item)
        return NULL;

    new_item->key = malloc(key_size);
    if(!new_item->key){
        free(new_item);
        return NULL;
    }

    (void)memcpy(new_item->key, key, key_size);

    // Or just do strdup for strings
    new_item->val = malloc(val_size);
    if(!new_item->val){
        free(new_item->key);
        free(new_item);
        return NULL;
    }

    (void)memcpy(new_item->val, val, val_size);

    new_item->next = NULL;
    return new_item;
}

void handle_collision_chaining(Ht_Item* item, const void* val, size_t val_size){
    if(item->val){
        if(strcmp((char*)item->val, (char*)val) == 0)
            return;

        free(item->val);
        item->val = malloc(val_size);
        (void)memcpy(item->val, val, val_size);
    }
}

// Key as a string, value as a string
void ht_insert(HashTable* ht, const void* key, size_t key_size, const void* val, size_t val_size){
    size_t idx = ht->hash_func(key, ht->capacity);
    
    // Resizing, the threshold being 0.7
    if(ht->load_factor > THRESHOLD){
        ht_resize(ht, ht->capacity * 2, key_size, val_size);
        idx = ht->hash_func(key, ht->capacity);
    }
    ht->load_factor = (float)ht->size / (float)ht->capacity;

    Ht_Item* curr_item = ht->buckets[idx];
    
    if(ht->coll_resolution == CHAINING){
        while(curr_item){
            if(strcmp((char*)curr_item->key, (char*)key) == 0){
                handle_collision_chaining(curr_item, val, val_size);
                ht->collisions++;
                return;
            }

            // Pointing to the next item in the list, not destroying the linkage
            curr_item = curr_item->next;
        }

        // Key doesn't exist, just create a new one
        Ht_Item* new_item = ht_item_create(key, key_size, val, val_size);
        new_item->next = ht->buckets[idx];
        ht->buckets[idx] = new_item;
        ht->size++;
    }else if(ht->coll_resolution == LINEAR_PROBING){
        size_t initial_idx = idx;
        while(ht->buckets[idx])
            idx = (idx + 1) % ht->capacity;

        ht->collisions = initial_idx != idx ? ht->collisions += 1 : ht->collisions;

        ht->buckets[idx] = ht_item_create(key, key_size, val, val_size);
        ht->size++;
    }
}

bool ht_has_key(const HashTable* ht, const void* key){
    size_t idx = ht->hash_func(key, ht->capacity);
    Ht_Item* item = ht->buckets[idx];

    while(item){
        if (strcmp((const char*)item->key, (const char*)key) == 0){
            return true; // Key found
        }
        item = item->next;
    }

    return false; // Key not found
}

// Will let the user to free the memory
Ht_Item* ht_remove(HashTable* ht, const void* key) {
    size_t idx = ht->hash_func(key, ht->capacity);
    Ht_Item** curr_item = &ht->buckets[idx];

    if(*curr_item == NULL){
        return NULL;
    }else{
        while(*curr_item){
            Ht_Item* curr = *curr_item;
            if(strcmp((char*)curr->key, (char*)key) == 0){
                *curr_item = (*curr_item)->next;
                ht->size--;
                return curr;
            }

            curr_item = &(*curr_item)->next;
        }
    }

    return NULL;
}

Ht_Item* ht_get_item(HashTable* ht, const void* key){
    size_t idx = ht->hash_func(key, ht->capacity);
    Ht_Item* curr_item = ht->buckets[idx];

    if(!curr_item){
        return NULL;
    }else{
        while(curr_item){
            if(strcmp((char*)curr_item->key, (char*)key) == 0){
                return curr_item;
            }

            curr_item = curr_item->next;
        }
    }

    return NULL;
}

void ht_modify_item(HashTable* ht, const void* key, const void* val){
    if(ht_has_key(ht, key)){
        Ht_Item* to_modify = ht_get_item(ht, key);
        size_t value_bytes = sizeof((char*)to_modify->val);
        (void)strncpy((char*)to_modify->val, (char*)val, value_bytes);
    }
}

// Not safe, it borrows the item // pointer to item
// Assumes user won't free the memory
void* ht_search(HashTable* ht, const void* key){
    size_t idx = ht->hash_func(key, ht->capacity);
    Ht_Item* curr_item = ht->buckets[idx];

    // In case there is chaining
    while(curr_item){
        if(strcmp((char*)curr_item->key, (char*)key) == 0)
            return curr_item->val;
        
        curr_item = curr_item->next;
    }

    // if curr_item is NULL
    return NULL;

}

// FREEING!

void free_ht_item(Ht_Item** item){
    if(*item){
        free((*item)->key);
        free((*item)->val);
        free(*item);
        *item = NULL;
    }
}

void ht_free(HashTable** ht){
    for (size_t i = 0; i < (*ht)->capacity; ++i){
        Ht_Item* item = (*ht)->buckets[i];
        while(item){
            Ht_Item* next = item->next;
            free(item->key);
            free(item->val);
            free(item);
            item = next;
        }
    }

    free((*ht)->buckets);
    (*ht)->buckets = NULL;
    free(*ht);
    *ht = NULL;
}