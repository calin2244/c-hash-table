#include "hash_map.h"


/*
    * Hash Functions!
    * I've implemented below a normal hash function
    * and a double hash one too
    ! You can create your own hash function too!!
    ? FNV stands for Fowler-Noll-Vo
*/ 

size_t hash_func(const char* key, size_t capacity){
    size_t hash = 0xcbf29ce484222325; // FNV_offset_basis
    while(*key){
        hash ^= (uint8_t)*key++;
        hash *= 0x100000001b3; // FNV_prime
    }

    return hash % capacity;
}

size_t double_hash_func(const char* key, size_t capacity){
    size_t hash1 = hash_func(key, capacity);
    size_t hash2 = 0xf3d7b4d88c356f19; 
    
    while(*key){
        hash2 ^= (uint8_t)*key++;
        hash2 *= 0x811C9DC5; // Another FNV_prime
    }

    return (hash1 + (hash2 % (capacity - 1))) % capacity;
}

// END OF HASHING



HashTable* ht_create(size_t capacity, size_t(*hash_func)(const char*, size_t), CollisionResolution coll_res){
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

void ht_resize(HashTable* ht, size_t new_capacity, size_t value_size){
    // Create a new hash table with the new capacity
    HashTable* new_ht = ht_create(new_capacity, ht->hash_func, ht->coll_resolution);
    if(!new_ht){
        return;
    }

    // Iterate through the old buckets and rehash the items into the new hash table
    for(size_t i = 0; i < ht->capacity; ++i){
        Ht_Item* item = ht->buckets[i];
        while(item){
            ht_insert(new_ht, item->key, item->val, value_size);
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

Ht_Item* ht_item_create(const void* key, const void* val, size_t val_size){
    Ht_Item* new_item = (Ht_Item*)malloc(sizeof(Ht_Item));
    if(!new_item)
        return NULL;

    new_item->key = malloc(strlen((char*)key) + 1);
    if(!new_item->key){
        free(new_item);
        return NULL;
    }

    (void)memcpy(new_item->key, key, strlen((char*)key) + 1);

    // Or just do strdup for strings
    new_item->val = strdup(val);
    if(!new_item->val){
        free(new_item->key);
        free(new_item);
        return NULL;
    }

    // TODO: Look more into this issue(invalid read size)
    // (void)memcpy(new_item->val, val, val_size);

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
void ht_insert(HashTable* ht, const char* key, const void* val, size_t val_size){
    size_t idx = ht->hash_func(key, ht->capacity);
    
    // Resizing, the threshold being 0.7 and 0.45
    // When working with linear probing, there will be a lot more clustering
    // so the THRESHOLD for linear-probing HashTable will be a little lower 
    if(ht->coll_resolution >= CHAINING && ht->load_factor > CHAINING_THRESHOLD){
        ht_resize(ht, ht->capacity * 2, val_size);
        idx = ht->hash_func(key, ht->capacity);
    }
    else if(ht->coll_resolution >= LINEAR_PROBING && ht->load_factor >= LP_THRESHOLD){
        ht_resize(ht, ht->capacity * 2, val_size);
        idx = ht->hash_func(key, ht->capacity);
    }
    ht->load_factor = (float)ht->size / (float)ht->capacity;

    Ht_Item* curr_item = ht->buckets[idx];
    
    //TODO: Update correctly the number of collisions
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
        Ht_Item* new_item = ht_item_create(key, val, val_size);
        new_item->next = ht->buckets[idx];
        ht->buckets[idx] = new_item;
        ht->size++;
    }else if(ht->coll_resolution == LINEAR_PROBING){
        size_t initial_idx = idx;
        while(ht->buckets[idx]){
            if(strcmp((char*)(ht->buckets[idx])->key, (char*)key) == 0){
                // Update the VALUE of the KEY
                Ht_Item* item = ht->buckets[idx];
                if(item->val)
                    free(item->val);

                item->val = strdup((char*)val);

                return;
            }

            idx = (idx + 1) % ht->capacity;
            
            if(idx == initial_idx){
                ht->collisions++;
                return;
            }
        }
                    
        ht->buckets[idx] = ht_item_create(key, val, val_size);
        ht->size++;
    }
}

bool ht_has_key(const HashTable* ht, const void* key){
    size_t idx = ht->hash_func(key, ht->capacity);
    
    if(ht->coll_resolution == CHAINING){
        Ht_Item* item = ht->buckets[idx];
        while(item){
            if(strcmp((const char*)item->key, (const char*)key) == 0){
                return true; // Key found
            }
            item = item->next;
        }

        return false; // Key not found
    }else if(ht->coll_resolution == LINEAR_PROBING){
        while(idx < ht->capacity){
            Ht_Item* item = ht->buckets[idx];
            if(item){
                if(strcmp((const char*)item->key, (const char*)key) == 0)
                    return true;
            }
            else return false;
            
            idx = (idx + 1) % ht->capacity;
        }

        // No key has been found
        return false;
    }

    return false;
}

// TODO: Implement the other collision methods
/*  
    *UPDATE: The function will now free the memory, it's safer
    * It returns 1 if it was able to find the item and remove it
    * else it returns 0 (no item with that key was found)
*/

bool ht_remove(HashTable* ht, const void* key) {
    size_t idx = ht->hash_func(key, ht->capacity);
    Ht_Item** curr_item = &ht->buckets[idx];

    if(ht->coll_resolution == CHAINING){
        
        if(*curr_item == NULL)
            return false;
        
        while(*curr_item){
            // This will act as a PREV pointer
            Ht_Item* curr = *curr_item;
            if(strcmp((char*)curr->key, (char*)key) == 0){
                *curr_item = (*curr_item)->next;
                ht->size--;
                
                free_ht_item(curr);
                curr = NULL;
                // Return, we have deleted the item
                return true;
            }

            curr_item = &(*curr_item)->next;
        }
    }
    else if(ht->coll_resolution == LINEAR_PROBING){
        while(*curr_item){
            if(strcmp((char*)(*curr_item)->key, (char*)key) == 0){
                free_ht_item(*curr_item);
                *curr_item = NULL;

                return true;
            }

            idx = (idx + 1) % ht->capacity;

            curr_item = &ht->buckets[idx];
        }

        return false;
    }

    return false;
}

/*
    * Not safe, it borrows the item // pointer to item
    * Assumes user won't free the memory
*/
Ht_Item* ht_get_item(HashTable* ht, const void* key){
    size_t idx = ht->hash_func(key, ht->capacity);
    Ht_Item* curr_item = ht->buckets[idx];

    if(ht->coll_resolution == CHAINING){
        while (curr_item) {
            if (strcmp((char*)curr_item->key, (char*)key) == 0) {
                return curr_item;
            }
            curr_item = curr_item->next;
        }
    }else if(ht->coll_resolution == LINEAR_PROBING){
        size_t start_idx = idx;
        while (curr_item) {
            if (strcmp((char*)curr_item->key, (char*)key) == 0) {
                return curr_item;
            }

            idx = (idx + 1) % ht->capacity;

            if (idx == start_idx) {
                break; // Reached the starting index, no match found
            }

            curr_item = ht->buckets[idx];
        }
    }

    return NULL; // Key not found
}


// !TODO: ADD SUPPORT FOR MULTIPLE COLLISION RESOLUTIONS 
void ht_modify_item(HashTable* ht, const void* key, const void* val){
    if(ht_has_key(ht, key)){
        Ht_Item* to_modify = ht_get_item(ht, key);
        size_t value_bytes = sizeof((char*)to_modify->val);
        (void)strncpy((char*)to_modify->val, (char*)val, value_bytes);
    }
}

// FREEING!
void free_ht_item(Ht_Item* item){
    if(item){
        if(item->key)
            free(item->key);
        
        if(item->val)
            free(item->val);
        
        free(item);
    }
}

void free_ht(HashTable** ht){
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