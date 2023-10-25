#include "hash_map.h"

/*
    * Hash Functions!
    * I've implemented below a normal hash function
    * and a double hash one too
    ! You can create your own hash function too!!
    ? FNV stands for Fowler-Noll-Vo
*/ 

size_t fnv_hash_func(const char* key, size_t capacity){
    size_t hash = 0xcbf29ce484222325; // FNV_offset_basis
    while(*key){
        hash ^= (uint8_t)*key++;
        hash *= 0x100000001b3; // FNV_prime
    }

    return hash % capacity;
}

size_t fnv_double_hash_func(const char* key, size_t capacity){
    size_t hash1 = fnv_hash_func(key, capacity);
    size_t hash2 = 0xf3d7b4d88c356f19; 
    
    while(*key){
        hash2 ^= (uint8_t)*key++;
        hash2 *= 0x811C9DC5; // Another FNV_prime
    }

    return (hash1 + (hash2 % (capacity - 1))) % capacity;
}

/*
    ! WARNING
    * This hash function usually returns big hash values
    * so resizings are going to happen a lot more often than usual
    ? (Implemented for testing purposes)
*/
size_t jenkins_hash_func(const char* key, size_t capacity){
    size_t hash = 0;
    while(*key){
        hash += (uint8_t)*key;
        hash += ((uint16_t)*key << 10);
        hash ^= ((uint8_t)*key >> 6);
        key++;
    }

    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);

    return hash % capacity;
}

// END OF HASHING

// HELPER FUNCTIONS
bool isPrime(size_t num){
    if(num < 2)
        return false;
    else if(num == 2)
        return true;
    else if(num % 2 == 0)
        return false;

    for(size_t d = 3; d * d <= num; d += 2){
        if(num % d == 0)
            return false;
    }

    return true;
}

size_t generateNextGreaterPrimeNumber(size_t num){
    while(!isPrime(num))
        num++;

    return num;
}

// END OF HELPER FUNCTIONS

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

void ht_resize(HashTable* ht, size_t new_capacity){
    // Create a new hash table with the new capacity
    HashTable* new_ht = ht_create(new_capacity, ht->hash_func, ht->coll_resolution);
    if(!new_ht){
        return;
    }

    // Iterate through the old buckets and rehash the items into the new hash table
    for(size_t i = 0; i < ht->capacity; ++i){
        Ht_Item* item = ht->buckets[i];
        while(item){
            ht_insert(new_ht, item->key, item->val, item->val_size);
            Ht_Item* next = item->next;
            free_ht_item(item);
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

Ht_Item* ht_item_create(const char* key, const void* val, size_t val_size){ 
    Ht_Item* new_item = (Ht_Item*)malloc(sizeof(Ht_Item));
    if(!new_item)
        return NULL;

    new_item->key = (char*)malloc(strlen(key) + 1);
    if(!new_item->key){
        free(new_item);
        return NULL;
    }

    (void)strncpy(new_item->key, key, strlen(key) + 1);
    new_item->key[strlen(key)] = '\0';

    // Or just do strdup for strings
    new_item->val = malloc(val_size + 1);
    if(!new_item->val){
        free(new_item->key);
        free(new_item);
        return NULL;
    }

    (void)memcpy(new_item->val, val, val_size);

    new_item->val_size = val_size;
    new_item->is_tombstone = false;
    new_item->next = NULL;
    return new_item;
}

/*
    * Collosion Handling
*/
void handle_tombstones(Ht_Item* itm, const char* key, const void* val, size_t val_size){
    itm->is_tombstone = false;
            
    itm->key = (char*)malloc(strlen(key) + 1);
    (void)strncpy(itm->key, key, strlen(key) + 1);

    itm->val = malloc(val_size + 1);
    (void)memcpy(itm->val, val, val_size);
}

// Normal collision, keys coincide
void handle_collision(Ht_Item* item, const void* val, size_t val_size){    
    if(item->val){
        free(item->val);
    }

    item->val = malloc(val_size + 1);
    (void)memcpy(item->val, val, val_size);
}

void handle_collision_chaining(HashTable* ht, size_t idx, const char* key, const void* val, size_t val_size){
    Ht_Item* curr_item = ht->buckets[idx];
    
    while(curr_item){
        ht->collisions++;
        
        if(strcmp((char*)curr_item->key, (char*)key) == 0){
            handle_collision(curr_item, val, val_size);
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
}

void handle_collision_lp(HashTable* ht, size_t idx, const char* key, const void* val, size_t val_size){
    Ht_Item* itm = ht->buckets[idx];

    while(ht->buckets[idx]){
        if(itm->is_tombstone){
            handle_tombstones(itm, key, val, val_size);
            ht->size++;

            return;
        }

        ht->collisions++;
        
        if(strcmp((char*)(ht->buckets[idx])->key, (char*)key) == 0){
            // Handle the collision
            handle_collision(ht->buckets[idx], val, val_size);
            return;
        }
        
        idx = (idx + 1) % ht->capacity;
        itm = ht->buckets[idx];
    }

    ht->buckets[idx] = ht_item_create(key, val, val_size);
    ht->size++;
}

void handle_collision_qp(HashTable* ht, size_t idx, const char* key, const void* val, size_t val_size){
    size_t initial_idx = idx;
    uint16_t curr_power = 1;    
    Ht_Item* itm = ht->buckets[idx];

    while(ht->buckets[idx]){
        if(itm->is_tombstone){
            handle_tombstones(itm, key, val, val_size);

            ht->size++;
            return;
        }

        ht->collisions++;

        if(strcmp((char*)(ht->buckets[idx])->key, (char*)key) == 0){
            handle_collision(ht->buckets[idx], val, val_size);            
            return;
        }

        idx = (initial_idx + (size_t)(curr_power * curr_power)) % ht->capacity;
        curr_power++;

        itm = ht->buckets[idx];
    }

    ht->buckets[idx] = ht_item_create(key, val, val_size);
    ht->size++;
}
// End of Collosion Handling

void ht_insert(HashTable* ht, const char* key, const void* val, size_t val_size){
    if(key == NULL || val == NULL){
        printf("The Key and/or the Value you're trying to insert is/are NULL.\n");
        return;
    }
    
    size_t idx = ht->hash_func(key, ht->capacity);
    
    /*  
        * Resizing, the threshold being 0.7 and 0.45
        * When working with linear probing, there will be a lot more clustering
        * so the THRESHOLD for linear-probing HashTable will be a little lower 
    */
    if(ht->coll_resolution == CHAINING && ht->load_factor > CHAINING_THRESHOLD){
        ht_resize(ht, generateNextGreaterPrimeNumber(ht->capacity * INCREMENTAL_RESIZING + ht->capacity));
        idx = ht->hash_func(key, ht->capacity);
    }
    else if(ht->coll_resolution >= LINEAR_PROBING && ht->load_factor >= OA_THRESHOLD){
        ht_resize(ht, generateNextGreaterPrimeNumber(ht->capacity * INCREMENTAL_RESIZING + ht->capacity));
        idx = ht->hash_func(key, ht->capacity);
    }
    ht->load_factor = (float)ht->size / (float)ht->capacity;
    
    if(ht->coll_resolution == CHAINING){
        handle_collision_chaining(ht, idx, key, val, val_size);
    }
    else if(ht->coll_resolution == LINEAR_PROBING){
        handle_collision_lp(ht, idx, key, val, val_size);
    }
    else if(ht->coll_resolution == QUADRATIC_PROBING){
        handle_collision_qp(ht, idx, key, val, val_size);
    }
}

/*
    * NEW! Bulk Insert
    * User is asked nicely to either provide a SENTINEL as the last element
    * of the array, either pass in the correct length as the argument!
*/
void ht_bulk_insert(HashTable* ht, const KeyValuePair* kv, size_t length){
    for(size_t i = 0; i < length; ++i){
        if(IS_SENTINEL(kv[i])){
            return;
        }

        if(!kv[i].key){
            printf("Error: NULL key provided at index %zu.\n", i);
            continue;
        }

        if(!kv[i].value){
            printf("Error: NULL value provided for key %s at index %zu.\n", kv[i].key, i);
            continue;
        }

        ht_insert(ht, kv[i].key, kv[i].value, kv[i].val_size);
    }
}

bool ht_has_key(const HashTable* ht, const char* key){
    if(!key){
        printf("Can't look for a NULL Key.\n");
        return false;
    }

    size_t idx = ht->hash_func(key, ht->capacity);
    
    if(ht->coll_resolution == CHAINING){
        Ht_Item* item = ht->buckets[idx];
        while(item){
            if(strcmp(item->key, key) == 0){
                return true; // Key found
            }
            item = item->next;
        }

        // Key not found
        return false; 
    }else if(ht->coll_resolution >= LINEAR_PROBING){
        while(ht->buckets[idx]){
            Ht_Item* item = ht->buckets[idx];

            if(!item->is_tombstone && strcmp(item->key, key) == 0)
                return true;
            
            idx = (idx + 1) % ht->capacity;
        }

        // No key has been found
        return false;
    }

    return false;
}

/*  
    * UPDATE: The function will now free the memory, it's safer
    * It returns 1 if it was able to find the item and remove it
    * else it returns 0 (no item with that key was found)
*/
/*
    *NEW!! This will now return the index from where it removed or SIZE_MAX
*/
size_t ht_remove(HashTable* ht, const char* key){
    if(!key){
        printf("Can't remove a NULL Key.\n");
        return SIZE_MAX;
    }

    size_t idx = ht->hash_func(key, ht->capacity);
    Ht_Item** curr_item = &ht->buckets[idx];

    if(ht->coll_resolution == CHAINING){
        
        if(!(*curr_item))
            return false;
        
        while(*curr_item){
            // This will act as a PREV pointer
            Ht_Item* curr = *curr_item;
            if(strcmp((char*)curr->key, (char*)key) == 0){
                *curr_item = (*curr_item)->next;
                ht->size--;
                
                free_ht_item(curr);

                // Return, we have deleted the item
                return idx;
            }

            curr_item = &(*curr_item)->next;
        }
    }
    else if(ht->coll_resolution == LINEAR_PROBING){
        size_t start_idx = idx;

        while(ht->buckets[idx]){
            Ht_Item* item = ht->buckets[idx];
            if(!item->is_tombstone && strcmp(item->key, key) == 0){
                free(item->key);
                free(item->val);
                item->is_tombstone = true;
                ht->size--;

                // printf("Am sters de la indexul: %ld\n", idx);

                return idx;
            }

            idx = (idx + 1) % ht->capacity;

            if(idx == start_idx){
                // Reached starting index, item not found
                return SIZE_MAX;
            }
        }

        return SIZE_MAX;
    }
    else if(ht->coll_resolution == QUADRATIC_PROBING){
        size_t initial_idx = idx;
        uint16_t curr_power = 1;

        while(ht->buckets[idx]){
            Ht_Item* item = ht->buckets[idx];

            if(!item->is_tombstone && strcmp((char*)(ht->buckets[idx])->key, key) == 0){
                free(item->key);
                free(item->val);
                item->is_tombstone = true;
                ht->size--;          
                
                return idx;
            }

            idx = (initial_idx + (size_t)(curr_power * curr_power)) % ht->capacity;
            curr_power++;
        }

        return SIZE_MAX;
    }

    return SIZE_MAX;
}

void ht_bulk_remove(HashTable* ht, const char** keys, size_t rows){
    for(size_t row = 0; row < rows; ++row){
        (void)ht_remove(ht, keys[row]);
    }
}

/*
    * Not safe, it borrows the item // pointer to item
    * Assumes user won't free the memory
*/
void* ht_get_item(HashTable* ht, const char* key){
    if(!key){
        printf("Can't lookup a NULL Key.\n");
        return NULL;
    }

    size_t idx = ht->hash_func(key, ht->capacity);
    // ? Look Into This after adding Robin Hood
    if(ht->coll_resolution >= LINEAR_PROBING && ht->buckets[idx])
        if(ht->buckets[idx]->is_tombstone)
            return NULL;

    Ht_Item* curr_item = ht->buckets[idx];
    
    if(ht->coll_resolution == CHAINING){
        while(curr_item){
            if (strcmp(curr_item->key, key) == 0){
                return curr_item->val;
            }

            curr_item = curr_item->next;
        }
    }else if(ht->coll_resolution == LINEAR_PROBING){
        size_t start_idx = idx;

        while(ht->buckets[idx]){
            if(strcmp((char*)curr_item->key, (char*)key) == 0){
                return curr_item->val;
            }

            idx = (idx + 1) % ht->capacity;

            if(idx == start_idx){
                break; // Reached the starting index, no match found
            }

            curr_item = ht->buckets[idx];
        }
    }
    else if(ht->coll_resolution == QUADRATIC_PROBING){
        size_t initial_idx = idx;
        uint16_t curr_power = 0;

        while(ht->buckets[idx]){

            ht->collisions++;

            if(strcmp((char*)(curr_item)->key, (char*)key) == 0){
                return curr_item->val;
            }

            idx = (initial_idx + (size_t)(curr_power * curr_power)) % ht->capacity;
            curr_power++;

            curr_item = ht->buckets[idx];
        }
    }

    return NULL; // Key not found
}

void ht_modify_item(HashTable* ht, const char* key, const void* val, size_t val_size){
    // ht_insert does the same thing
    if(!key || !val){
        printf("The Key and/or the Value you're trying to modify is/are NULL.\n");
        return;
    }

    ht_insert(ht, key, val, val_size);
}

// FREEING!
void free_ht_item(Ht_Item* item){
    if(item){
        if(item->key){
            free(item->key);
            item->key = NULL;
        }
        
        if(item->val){
            free(item->val);
            item->val = NULL;
        }
        
        free(item);
    }
}


void free_ht(HashTable** ht){
    for(size_t i = 0; i < (*ht)->capacity; ++i){
        Ht_Item** item = &(*ht)->buckets[i];

        // Open Addressing Check
        if((*ht)->coll_resolution >= LINEAR_PROBING && 
            (*ht)->buckets[i] && 
            (*item)->is_tombstone){
            free(*item);
            *item = NULL;
            continue;
        }

        while(*item){
            Ht_Item* next = (*item)->next;
            free_ht_item(*item);
            *item = NULL;
            *item = next;
        }
    }

    free((*ht)->buckets);
    (*ht)->buckets = NULL;
    free(*ht);
    *ht = NULL;
}

void ht_clear(HashTable* ht){
    for(size_t i = 0; i < ht->capacity; ++i){
        Ht_Item** item = &ht->buckets[i];

        // Open Addressing Check
        if(*item && (*item)->is_tombstone){
            free(*item);
            *item = NULL;
            continue;
        }

        // We iterate through each bucket(In case there is chaining)
        while(*item){
            Ht_Item* next = (*item)->next;
            free_ht_item(*item);
            *item = NULL;
            *item = next;
        }
    }

    ht->collisions = 0;
    ht->load_factor = 0;
    ht->size = 0;
}

/*
    * Will return true if it was able to purge the slot
    * Will return false otherwise(slot is already NULL)
*/
bool ht_purge_slot(HashTable* ht, size_t idx){
    if(idx >= ht->capacity){
        printf("IDX %zu is out of the HashTable's boundaries.\n", idx);
        return false;
    }

    if(ht->buckets[idx] || (ht->buckets[idx] && !ht->buckets[idx]->is_tombstone)){
        free_ht_item(ht->buckets[idx]);
        ht->buckets[idx] = NULL;
        return true;
    }

    // In case the slot is NULL
    return false;
}