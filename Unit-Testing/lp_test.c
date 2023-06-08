#include "../src/hash_iterator.h" // include "hash_map.h" too
#include "../src/hash_map.h"
#include "../src/hash_utils.h"
#include <time.h>

#define ENTRIES_PATH "./Data(input)/entries.txt"
#define OUTPUT_PATH "./Data(output)/lp_out.txt"

int main(int argc, char* argv[]){
    HashTable* hash_t = ht_create(2, hash_func_str, LINEAR_PROBING);
    
    int result = parseFileAndPopulateHashTable(hash_t, ENTRIES_PATH);
    if(result == -1){
        return -1;
    }

     // Saving the Data from the Hash Table in a text file
    FILE* out = fopen(OUTPUT_PATH, "w");
    if(!out){
        (void)printf("Couldn't open the Output file!\n");
        return -1;
    }

    // Only the unique keys will be in here
    for(size_t i = 0; i < hash_t->capacity; ++i){
        Ht_Item* item = hash_t->buckets[i];
        while(item){
            const char* key = (char*)item->key;
            const char* value = (char*)item->val;
            (void)fprintf(out, "%s %s\n", key, value);
            item = item->next;
        }
    }

    (void)fclose(out);
    
    ht_print_perfomance_stats(hash_t, argc, argv);
   
    parseFileAndRemoveEntries(hash_t, ENTRIES_PATH);
    ht_free(&hash_t);
    return 0;
}

