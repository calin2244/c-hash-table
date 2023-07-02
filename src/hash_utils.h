// This file includes parsing from files, printing the hash_table, Peformance Stats

#include "hash_map.h"
#include "getline.h"

// int getline(char** lline, size_t* len, FILE* m_file);

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

void print_string_string(size_t hash, const char* key, const void* val){
    printf("Index:%li, Key: %s, Value: %s\n", hash, (char*)key, (char*)val);
}

void ht_print_time_performances(HashTable* ht){
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
int parseFileAndPopulateHashTable(HashTable* ht, const char* file_name){
    FILE* m_file = fopen(file_name, "r");
    if(!m_file){
        (void)printf("%s", "Failed to open the file\n");
        return -1;
    }

    char* line_buff = NULL;
    size_t line_capacity = 0;
    int line_length; 

    while((line_length = getl(&line_buff, &line_capacity, m_file)) != -1){        
        char* key = strtok(line_buff, " ");
        char* val = strtok(NULL, "\n");

        if(key && val){
            //Right-Trim
            size_t val_size = strlen(val);
            while(val_size && (
            val[val_size - 1] == '\n' || 
            val[val_size - 1] == ' ' || 
            val[val_size - 1] == '\r')){
                val[val_size - 1] = '\0';
                val_size--;
            }

            if(strlen(val) > 1)
                ht_insert(ht, key, val, strlen(val) + 1);
        }
    }

    free(line_buff);
    fclose(m_file);

    // Parsing from file succeeded!!
    return 0;
}

void parseFileAndRemoveEntries(HashTable* ht, const char* file_name){
    FILE* m_file = fopen(file_name, "r");
    fseek(m_file, 0, SEEK_SET);

    char line_buffer[108];
    // Removing every entry in the hash table by parsing the file
    while(fgets(line_buffer, sizeof(line_buffer), m_file)){
        const char* key = strtok(line_buffer, " ");
        ht_remove(ht, key); 
    }

    fclose(m_file);
}

void ht_print_perfomance_stats(HashTable* ht, int argc, char* argv[], PrintHelper print_helper){
    if(argc > 1 && strcmp(argv[1], "stats") == 0){
        // To output the chain, traverse the maxChainNode as a simple linked list
        Ht_Item* maxChainNode = ht->buckets[maximumChainLength(ht).hashOfMaxChain]; 
        (void)printf("\nMaximum Length Chain Node: ");
        printf("%s: %s\n", (char*)maxChainNode->key, (char*)maxChainNode->val);

        // Time Performances
        ht_print_time_performances(ht);
    }
    else if(argc > 1 && strcmp(argv[1], "print") == 0){
        ht_print(ht, print_helper);
    }
}