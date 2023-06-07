// This file includes parsing from files, printing the hash_table, Peformance Stats

#include "hash_map.h"

typedef struct{
    size_t maxChainLen;
    size_t hashOfMaxChain;
}pair;

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

typedef void (*PrintHelper)(size_t, const void*, const void*);

void ht_print(HashTable* ht, PrintHelper printHasht){
    for(size_t hash = 0; hash < ht->capacity; ++hash){
        Ht_Item* item = ht->buckets[hash];
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

void print_string_string(size_t hash, const void* key, const void* val){
    printf("Index:%li, Key: %s, Value: %s\n", hash, (char*)key, (char*)val);
}

void printHashTableInfo(HashTable* ht){
    pair stats = maximumChainLength(ht);

    (void)printf("-----------------------\n");
    (void)printf("Total Collisions: %d\n", ht->collisions);

    (void)printf("Maximum Chain Length: %ld\n", stats.maxChainLen);
    (void)printf("Hash For the Maximum Chain: %ld\n", stats.hashOfMaxChain);

    (void)printf("HashTable Size: %ld\n", ht->size);
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
    char* key = NULL;
    char* val = NULL;
    while(fgets(line_buffer, sizeof(line_buffer), m_file)){
        key = strtok(line_buffer, " ");
        val = strtok(NULL, "\n");

        // right-trim the value
        size_t firstAlpha = strlen(val) - 1;
        while(firstAlpha){
            if(val[firstAlpha] == '\n' || val[firstAlpha] == ' ' || val[firstAlpha] == '\r')
                firstAlpha--;
            else break;
        }
        val[firstAlpha + 1] = '\0';


        ht_insert(ht, key, strlen(key) + 1, val, strlen(val) + 1);
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