#include "../src/hash_iterator.h" // include "hash_map.h" too
#include "../src/hash_map.h"
#include "../src/hash_utils.h"
#include <time.h>

#define ENTRIES_PATH "./Data(input)/entries.txt"
#define OUTPUT_PATH "./Data(output)/chain_out.txt"

int main(int argc, char* argv[]){
    HashTable* hash_t = ht_create(13, fnv_hash_func, CHAINING);
    HashIterator iterator = ht_iterator_create(hash_t);
    
    int result = parseFileAndPopulateHashTable(hash_t, ENTRIES_PATH);
    // Parsing from File failed
    if(result == -1)
        return -1;
    
    // This will retrieve the 100th item in the Hash Table
    // Ht_Item* onehundreth = ht_iterator_get_nth_item(&iterator, 100);

    // Let's time iterating all over the Hash Table, even iterating through the empty buckets
    clock_t start_time, end_time;
    start_time = clock();

    while(ht_iterator_has_next(&iterator)){
        (void)ht_iterator_get(&iterator);
        // printItemInfo(curr);
        ht_iterator_next(&iterator);
    }    

    // Reset the iterator to point at the beginning of the Hash Table
    ht_iterator_reset(&iterator);

    end_time = clock();

    double cpu_time_used = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    
    // Time Performances and Stats
    (void)printf("Total time to traverse Hash Table: %f\n", cpu_time_used);
    ht_print_perfomance_stats(hash_t, argc, argv, print_string_string);

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
    parseFileAndRemoveEntries(hash_t, ENTRIES_PATH);
    free_ht(&hash_t);
    return 0;
}


    // Ht_Item* c = hash_t->buckets[9258];
    // Ht_Item** p = &c;

    // while(*p){
    //     // printItemInfo(*p);
    //     p = &(*p)->next;
    // }

    // while(c){
    //     printItemInfo(c);
    //     c = c->next;
    // }