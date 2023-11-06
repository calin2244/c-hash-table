#include "hash_map.h"

void* dmalloc(size_t size){
    void* p = malloc(size);
    if (!p) {
        perror("Error: malloc failed: ");
        exit(1);
    }
    
    memset(p, 0, size);
    return p;
}

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

void ht_set_second_hash_func(HashTable* ht, size_t (*hash_func2)(const char*, size_t)){
    ht->hash_func2 = hash_func2;
}

void ht_print(HashTable* ht, PrintHelper printHasht){
    for(size_t hash = 0; hash < ht->capacity; ++hash){
        Ht_Item* item = ht->buckets[hash];

        // Open Addressing Check
        if(item && item->is_tombstone)
            continue;

        if(item){
            printHasht(hash, ht->buckets[hash]->key, ht->buckets[hash]->val);
            if(item->next){
                (void)printf("-----------------------\nChained with: \n");
                item = item->next;
                while(item){
                    (void)printf("[%s: %s]", (char*)item->key, (char*)item->val);
                    item = item->next;
                    if(item)
                        (void)printf(", ");
                }
                (void)printf("\n-----------------------\n\n");
            }
        }
    }
}

void print_str_str(size_t hash, const char* key, const void* val){
    printf("Index:%li, Key: %s, Value: %s\n", hash, (char*)key, (char*)val);
}
// END OF HELPER FUNCTIONS

HashTable* /* The above code appears to be written in the C programming language. However, it is
incomplete and does not provide enough information to determine what it is doing. The
code starts with a comment indicating the language, but there is no actual code present.
The text "ht_cre" and " */
ht_create(size_t capacity, size_t(*hash_func)(const char*, size_t), CollisionResolution coll_res){
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
    hash_t->hash_func2 = NULL;
    hash_t->coll_resolution = coll_res;
    hash_t->resize_threshold = (IS_OA(hash_t)) ? OA_THRESHOLD : CHAINING_THRESHOLD;
    hash_t->resize_factor = INCREMENTAL_RESIZING;
    hash_t->num_resize_strategies = 0;
    hash_t->resize_strategies = NULL;

    return hash_t;
}

bool inResizeMode = false;
void ht_resize(HashTable* ht, size_t new_capacity){
    inResizeMode = true;

    // Create a new hash table with the new capacity
    HashTable* new_ht = ht_create(new_capacity, ht->hash_func, ht->coll_resolution);
    if(!new_ht){
        return;
    }

    // Iterate through the old buckets and rehash the items into the new hash table
    for(size_t i = 0; i < ht->capacity; ++i){
        Ht_Item* item = ht->buckets[i];
        while(item && !item->is_tombstone){
            ht_insert(new_ht, item->key, item->val, item->val_size);
            Ht_Item* next = item->next;
            free_ht_item(item);
            item = next;
        }
    }
    inResizeMode = false;

    // Free the old buckets
    free(ht->buckets);

    // Update the hash table properties
    ht->capacity = new_ht->capacity;
    ht->load_factor = 0;

    // Update the buckets pointer to point to the new buckets
    ht->buckets = new_ht->buckets;

    // Free the new hash table structure
    free(new_ht);
}

void ht_set_custom_resize_strategy(HashTable* ht, size_t capacity_threshold, float associated_resize_factor){
    ht->num_resize_strategies++;
    if(!ht->resize_strategies){
        ht->resize_strategies = (ResizeStrategy*)malloc(ht->num_resize_strategies * sizeof(ResizeStrategy));
    }
    else{
        ResizeStrategy* new_strategies = (ResizeStrategy*)malloc(ht->num_resize_strategies * sizeof(ResizeStrategy));
        for(size_t i = 0; i < ht->num_resize_strategies - 1; ++i){
            new_strategies[i] = ht->resize_strategies[i];
        }

        free(ht->resize_strategies);
        ht->resize_strategies = new_strategies;
    }

    // Set the new strategy at the last position
    ht->resize_strategies[ht->num_resize_strategies - 1].capacity_threshold = capacity_threshold;
    ht->resize_strategies[ht->num_resize_strategies - 1].resize_factor = associated_resize_factor;
    update_resize_factor(ht);
}

Ht_Item* ht_item_create(const char* key, const void* val, size_t val_size){ 
    if(!key || !val || val_size == 0){
        return 0;
    }

    Ht_Item* new_item = (Ht_Item*)dmalloc(sizeof(Ht_Item));
    if(!new_item)
        return NULL;

    new_item->key = (char*)dmalloc(strlen(key) + 1);
    if(!new_item->key){
        free(new_item);
        return NULL;
    }

    strcpy(new_item->key, key);

    // Or just do strdup for strings
    new_item->val = dmalloc(val_size + 1);
    if(!new_item->val){
        free(new_item->key);
        free(new_item);
        return NULL;
    }

    memcpy(new_item->val, val, val_size);

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
            
    itm->key = (char*)dmalloc(strlen(key) + 1);
    strcpy(itm->key, key);

    itm->val = dmalloc(val_size + 1);
    (void)memcpy(itm->val, val, val_size);
}

// Normal collision, keys coincide
void handle_collision(Ht_Item* item, const void* val, size_t val_size){    
    if(item->val){
        free(item->val);
    }

    item->val = dmalloc(val_size + 1);
    (void)memcpy(item->val, val, val_size);
}

void handle_insertion_chaining(HashTable* ht, size_t idx, const char* key, const void* val, size_t val_size){
    Ht_Item* curr_item = ht->buckets[idx];
    
    if(curr_item){
        ht->collisions++;
    }

    while(curr_item){        
        if(strcmp(curr_item->key, key) == 0){
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

void handle_insertion_lp(HashTable* ht, size_t idx, const char* key, const void* val, size_t val_size){
    size_t initial_idx = idx;
    Ht_Item* itm = ht->buckets[idx];

    if(itm){
        ht->collisions++;
    }

    while(true){
        itm = ht->buckets[idx];

        if(!itm || itm->is_tombstone){
            if(itm && itm->is_tombstone){
                handle_tombstones(itm, key, val, val_size);
            }else{
                ht->buckets[idx] = ht_item_create(key, val, val_size);
            }

            ht->size++;
            return;
        }else if (strcmp(itm->key, key) == 0){
            handle_collision(itm, val, val_size);
            return;
        }

        idx = (idx + 1) % ht->capacity;
        if (idx == initial_idx) {
            // Table is full or probed everywhere
            return;
        }
    }
}

void handle_insertion_qp(HashTable* ht, size_t idx, const char* key, const void* val, size_t val_size){
    size_t initial_idx = idx;
    size_t curr_power = 1;    
    Ht_Item* itm = ht->buckets[idx];

    if(itm){
        ht->collisions++;
    }

    while(ht->buckets[idx]){

        if(itm->is_tombstone){
            handle_tombstones(itm, key, val, val_size);
            ht->size++;
            
            return;
        }

        if(strcmp(ht->buckets[idx]->key, key) == 0){
            handle_collision(ht->buckets[idx], val, val_size);            
            return;
        }

        idx = (initial_idx + (curr_power * curr_power)) % ht->capacity;
        curr_power++;
        itm = ht->buckets[idx];

        if(idx == initial_idx)
            return;
    }

    ht->buckets[idx] = ht_item_create(key, val, val_size);
    ht->size++;
}

void handle_insertion_double_hashing(HashTable* ht, size_t idx, const char* key, const void* val, size_t val_size){
    size_t i = 0; // Probing starts from 0
    size_t hash1 = ht->hash_func(key, ht->capacity);
    size_t hash2 = (ht->hash_func2) ? ht->hash_func2(key, ht->capacity) : jenkins_hash_func(key, ht->capacity);
    
    if (hash2 == 0) 
        hash2 = 1;
    
    if(ht->buckets[idx])
        ht->collisions++;

    while(ht->buckets[idx]){
        if(strcmp(key, ht->buckets[idx]->key) == 0){
            handle_collision(ht->buckets[idx], val, val_size);
            return;
        }

        i++;
        idx = (hash1 + i * hash2) % ht->capacity; 
    }

    ht->buckets[idx] = ht_item_create(key, val, val_size);
    ht->size++;
}

// TODO
// void handle_insertion_robin_hood(HashTable* ht, size_t idx, const char* key, const void* val, size_t val_size){
//     return;
// }

// End of Collosion Handling

void ht_set_resize_threshold(HashTable* ht, float resize_threshold){
    if(resize_threshold <= 0.0f || resize_threshold >= 1.0f){
        printf("Error: Invalid resize_threshold value. It should be between 0 and 1 (exclusive).\n");
        return;
    }

    ht->resize_threshold = resize_threshold;
}

void update_resize_factor(HashTable* ht){
    if(ht->num_resize_strategies == 0){
        ht->resize_factor = (ht->capacity < SMALL_CAPACITY) 
            ? 1.0f 
            : (ht->capacity < MEDIUM_CAPACITY) 
            ? .6f 
            : .4f;
    }else{
        for(size_t i = 0; i < ht->num_resize_strategies; i++){
            if(ht->capacity < ht->resize_strategies[i].capacity_threshold){
                ht->resize_factor = ht->resize_strategies[i].resize_factor;
                break;
            }
        }
    }
}

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
    if(!inResizeMode){
        // *Update the resize_factor
        update_resize_factor(ht);

        if(IS_CHAINING(ht) && ht->load_factor > ht->resize_threshold){
            ht_resize(ht, generateNextGreaterPrimeNumber((size_t)(ht->capacity * ht->resize_factor) + ht->capacity));
            idx = ht->hash_func(key, ht->capacity);
        }
        // * LP, QP, Robin Hood, Double Hashing
        else if(IS_OA(ht) && ht->load_factor >= ht->resize_threshold){
            ht_resize(ht, generateNextGreaterPrimeNumber((size_t)(ht->capacity * ht->resize_factor) + ht->capacity));
            idx = ht->hash_func(key, ht->capacity);
        }
    }
    ht->load_factor = (float)ht->size / (float)ht->capacity;
    
    switch(ht->coll_resolution){
        case CHAINING:
            handle_insertion_chaining(ht, idx, key, val, val_size);
            break;
        case LINEAR_PROBING:
            handle_insertion_lp(ht, idx, key, val, val_size);
            break;
        case QUADRATIC_PROBING:
            handle_insertion_qp(ht, idx, key, val, val_size);
            break;
        case DOUBLE_HASHING:
            handle_insertion_double_hashing(ht, idx, key, val, val_size);
            break;
        // case ROBIN_HOOD:
        //     handle_insertion_robin_hood(ht, idx, key, val, val_size); // TODO
        //     break;
        default:
            break;
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

void calculate_next_probe_index(const HashTable* ht, size_t* idx, size_t* curr_power, size_t* probes, size_t hash1, size_t hash2){
    size_t capacity = ht->capacity;
    size_t c_p = *curr_power;
    CollisionResolution coll_res = ht->coll_resolution;

    // * Hash 1 is also initial_idx

    switch(coll_res){
        case LINEAR_PROBING:
            *idx = (*idx + 1) % capacity; 
            break;
        case QUADRATIC_PROBING:
            *idx = (hash1 + c_p * c_p) % capacity;
            (*curr_power)++;
            break;
        case DOUBLE_HASHING:
            *idx = (hash1 + *probes * hash2) % capacity;
            (*probes)++;
            break;
        default:
            break;
    }

}

bool ht_has_key(const HashTable* ht, const char* key){
    if(!key){
        printf("Can't look for a NULL Key.\n");
        return false;
    }

    size_t idx = ht->hash_func(key, ht->capacity);
    size_t initial_idx = idx;
    Ht_Item* curr_item = ht->buckets[idx];

    if(ht->coll_resolution == CHAINING){
        while(curr_item){
            if (strcmp(curr_item->key, key) == 0){
                return true;
            }
            curr_item = curr_item->next;
        }   
    }else{
        size_t curr_power = 1; // * Used for Quadratic Probing
        size_t probes = 0; // * For DoubleH
        size_t hash2 = (ht->hash_func2) ? ht->hash_func2(key, ht->capacity) : jenkins_hash_func(key, ht->capacity); // * For DoubleH
        
        if(ht->coll_resolution == DOUBLE_HASHING && hash2 == 0){
            hash2 = 1;
        }

        while(curr_item){
            if(!curr_item->is_tombstone && strcmp(curr_item->key, key) == 0){
                return true;
            }
            
            calculate_next_probe_index(ht, &idx, &curr_power, &probes, initial_idx, hash2);

            if(idx == initial_idx && ht->coll_resolution != DOUBLE_HASHING){
                break;
            }
            
            curr_item = ht->buckets[idx];
        }
    }

    // If we get here, the item was never found.
    return false;
}


Ht_ExitCode handle_tombstones_removal(HashTable* ht, size_t idx, const char* key){
    Ht_Item* item = ht->buckets[idx];

    if(!item->is_tombstone && strcmp(item->key, key) == 0){
        free(item->key);
        free(item->val);
        item->is_tombstone = true;
        ht->size--;;

        return HT_SUCCESS;
    }

    return HT_NO_ACTION;
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
    size_t initial_idx = idx;
    Ht_Item* curr_item = ht->buckets[idx];

    if(ht->coll_resolution == CHAINING){
        Ht_Item** p_curr_item = &ht->buckets[idx];
        while(*p_curr_item){
            curr_item = *p_curr_item;
            if(strcmp(curr_item->key, key) == 0){
                *p_curr_item = curr_item->next;
                free_ht_item(curr_item); // Make sure to implement this function
                ht->size--;
                return idx;
            }

            p_curr_item = &(*p_curr_item)->next;
        }
    }else{
        size_t curr_power = 1; // * Used for Quadratic Probing
        size_t probes = 0; // * For DoubleH
        size_t hash2 = (ht->hash_func2) ? ht->hash_func2(key, ht->capacity) : jenkins_hash_func(key, ht->capacity); // * For DoubleH

        if(ht->coll_resolution == DOUBLE_HASHING && hash2 == 0) {
            hash2 = 1;
        }

        while(curr_item && probes < ht->capacity){
            handle_tombstones_removal(ht, idx, key);
            calculate_next_probe_index(ht, &idx, &curr_power, &probes, initial_idx, hash2);

            if((initial_idx == idx || probes >= ht->capacity) && ht->coll_resolution != DOUBLE_HASHING){
                break;
            }

            curr_item = ht->buckets[idx];
        }
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
    size_t initial_idx = idx;
    Ht_Item* curr_item = ht->buckets[idx];

    if(ht->coll_resolution == CHAINING){
        while(curr_item){
            if (strcmp(curr_item->key, key) == 0){
                return curr_item->val;
            }

            curr_item = curr_item->next;
        }
    }else if(ht->coll_resolution >= LINEAR_PROBING){
        size_t curr_power = 1; // * Used for Quadratic Probing
        size_t probes = 0; // * For DoubleH
        size_t hash1 = initial_idx; // *ForDoubleH
        size_t hash2 = (ht->hash_func2) ? ht->hash_func2(key, ht->capacity) : jenkins_hash_func(key, ht->capacity); // * For DoubleH
        
        if(hash2 == 0 && ht->coll_resolution == DOUBLE_HASHING)
            hash2 = 1;
        
        while(curr_item){
            if(!curr_item->is_tombstone && strcmp(curr_item->key, key) == 0){
                return curr_item->val;
            }

            calculate_next_probe_index(ht, &idx, &curr_power, &probes, hash1, hash2);

            if(idx == initial_idx && ht->coll_resolution != DOUBLE_HASHING){
                break; // Might wanna look into this for DH
            }

            curr_item = ht->buckets[idx];
        }
    }

    return NULL;
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
        if(IS_OA((*ht)) && 
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

    // Free the Resize Strategies
    if((*ht)->resize_strategies){
        free((*ht)->resize_strategies);
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