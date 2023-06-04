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

HashTable* ht_create(size_t capacity, size_t(*hash_func)(const void*, size_t)){
    HashTable* hash_t = (HashTable*)malloc(sizeof(HashTable));

    if(hash_t == NULL){ //!hash_t, malloc failed
        return NULL;
    } 

    hash_t->capacity = capacity;
    hash_t->hash_func = hash_func;
    hash_t->collisions = 0;
    hash_t->load_factor = 0;
    hash_t->buckets = (Ht_Item**)calloc(hash_t->capacity, sizeof(Ht_Item*)); 

    if(hash_t->buckets == NULL){
        free(hash_t);
        return NULL;
    }

    return hash_t;
}

void ht_resize(HashTable* ht, size_t new_capacity){
    // std::cout << "RESIZING" << '\n';
    // Create a new hash table with the new capacity
    HashTable* new_ht = ht_create(new_capacity, hash_func_str);
    if(new_ht == NULL){
        // Failed to create a new hash table
        return;
    }

    // Iterate through the old buckets and rehash the items into the new hash table
    for(size_t i = 0; i < ht->capacity; ++i){
        Ht_Item* item = ht->buckets[i];
        while(item != NULL){
            ht_insert(new_ht, item->key, item->val);
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

Ht_Item* ht_item_create(const void* key, const void* val){
    Ht_Item* new_item = (Ht_Item*)malloc(sizeof(Ht_Item));

    uint32_t key_size_bytes = sizeof(strlen((char*)key));
    new_item->key = malloc(key_size_bytes);
    (void)memcpy(new_item->key, key, key_size_bytes);


    // Or just do strdup for strings
    new_item->val = strdup((const char*)val);

    new_item->next = NULL;
    return new_item;
}

void handle_collision_chaining(HashTable* ht, Ht_Item* item, const void* val){
    if(item->val){
        free(item->val);
        item->val = strdup((char*)val);
        ht->collisions++;
        return;
    }
}

// Key as a string, value as a string
void ht_insert(HashTable* ht, const void* key, const void* val){
    size_t idx = ht->hash_func(key, ht->capacity);
    Ht_Item** curr_item = &ht->buckets[idx];
    Ht_Item* item = *curr_item;

    if(ht->load_factor > 1){
        ht_resize(ht, ht->capacity * 2);
        idx = ht->hash_func(key, ht->capacity);
        curr_item = &ht->buckets[idx];
        item = *curr_item;
    }

    while(item){
        if(strcmp((char*)item->key, (char*)key) == 0){
            handle_collision_chaining(ht, item, val);
            return;
        }

        // Pointing to the next item in the list, not destroying the linkage
        curr_item = &(*curr_item)->next;
        item = *curr_item;
    }

    *curr_item = ht_item_create(key, val);
    ht->size++;
    ht->load_factor = (float)ht->size / (float)ht->capacity;
}

bool ht_has_key(const HashTable* ht, const void* key){
    size_t idx = ht->hash_func(key, ht->capacity);
    Ht_Item* item = ht->buckets[idx];

    while(item){
        if (strcmp((const char*)item->key, (const char*)key) == 0) {
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

Ht_Item* ht_get(HashTable* ht, const void* key){
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
        Ht_Item* to_modify = ht_get(ht, key);
        size_t value_bytes = sizeof((char*)to_modify->val);
        (void)strncpy((char*)to_modify->val, (char*)val, value_bytes);
    }
}

/*
BAD INSERT :////
void ht_insert(HashTable* ht, const void* key, const void* val){
    size_t idx = ht->hash_func(key, ht->capacity);
    Ht_Item** curr_item = &ht->buckets[idx];
    Ht_Item* item = *curr_item;

    if(ht->load_factor >= 0.6){
        ht_resize(&ht, ht->capacity * 2);
        idx = ht->hash_func(key, ht->capacity);
        curr_item = &ht->buckets[idx];
        item = *curr_item;
    }

    while(item){
        if(strcmp((char*)item->key, (char*)key) == 0){
            handle_collision_chaining(ht, curr_item, item, val);
            return;
        }

        curr_item = &(*curr_item)->next;
        item = *curr_item;
    }

    *curr_item = ht_item_create(key, val);
    ht->size++;
    ht->load_factor = (float)ht->size / (float)ht->capacity;
}

*/

size_t get_num_of_buckets(HashTable* ht){
    size_t buckets = 0;
    for(size_t i = 0; i < ht->capacity; ++i){
        if(ht->buckets[i]){
            Ht_Item* curr = ht->buckets[i];
            while(curr){
                buckets++;
                curr = curr->next;
            }
        }
    }

    return buckets;
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

// PRINTING!!

void print_ht(HashTable* ht, PrintHelper printHasht){
    for(size_t hash = 0; hash < ht->capacity; ++hash){
        Ht_Item* item = ht->buckets[hash];
        if(item){
            printHasht(hash, ht->buckets[hash]->key, ht->buckets[hash]->val);
            if(item->next){
                (void)printf("-----------------------\nChained with: \n");
                item = item->next;
                while(item){
                    
                    (void)printf("\t %s: %s ", (char*)item->key, (char*)item->val);
                    item = item->next;
                }
                (void)printf("-----------------------\n\n");
            }
        }
    }
}

void print_string_string(size_t hash, const void* key, const void* val){
    printf("Index:%li, Key: %s, Value: %s\n", hash, (char*)key, (char*)val);
}

void printHashTableInfo(HashTable* ht){
    pair stats = maximumChainLength(ht);

    (void)printf("-----------------------\n");
    (void)printf("Total Collisions: %d\n", ht->collisions);

    (void)printf("Maximum Chain Length: %ld\n", stats.maxChainLen);
    (void)printf("Hash For the Maximum Chain: %ld\n", stats.hashOfMaxChain);

    (void)printf("HashTable Size: %ld\n", get_num_of_buckets(ht));
    (void)printf("Load Factor: %f\n", ht->load_factor);
    (void)printf("Capacity: %ld\n", ht->capacity);
    (void)printf("-----------------------\n");
}

// Parsing

void parseFileAndPopulateHashTable(HashTable* ht, const char* file_name){
    FILE* m_file = fopen(file_name, "r");
    if(!m_file){
        (void)printf("%s", "Failed to open the file\n");
        return;
    }

    char line_buffer[16];
    while(fgets(line_buffer, sizeof(line_buffer), m_file)){
        const char* key = strtok(line_buffer, " ");
        const char* val = strtok(NULL, " ");
        ht_insert(ht, key, val);
    }

    fclose(m_file);
}

void parseFileAndRemoveEntries(HashTable* ht, const char* file_name){
    FILE* m_file = fopen(file_name, "r");
    fseek(m_file, 0, SEEK_SET);

    char line_buffer[16];
    // Removing every entry in the hash table by parsing the file
    while(fgets(line_buffer, sizeof(line_buffer), m_file)){
        const char* key = strtok(line_buffer, " ");
        Ht_Item* remo = ht_remove(ht, key); 
        free_ht_item(&remo);
    }

    fclose(m_file);
}

// Performance stats
pair maximumChainLength(HashTable* ht){
    size_t maxLen = 0;
    size_t posOfMaxLen = -1;
    for(size_t i = 0; i < ht->capacity; ++i){
        Ht_Item* curr_item = ht->buckets[i];
        if(curr_item){
            size_t currLen = 0;
            while(curr_item){
                curr_item = curr_item->next;
                currLen++;
            }

            if(maxLen < currLen){
                maxLen = currLen;
                posOfMaxLen = i;
            }
        }
    }

    pair out;
    out.maxChainLen = maxLen;
    out.hashOfMaxChain = posOfMaxLen;
    return out;
}