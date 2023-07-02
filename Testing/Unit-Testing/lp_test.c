#include "../../src/hash_map.h"
#include "../../src/hash_utils.h"
#include <time.h>
#include <stdio.h>

#define ENTRIES_PATH "./Data(input)/entries.txt"
#define OUTPUT_PATH "./Data(output)/lp_out.txt"

int main(int argc, char* argv[]){
    HashTable* hash_t = ht_create(13, fnv_hash_func, LINEAR_PROBING);
    
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

    (void)fclose(out);
    
    ht_print_perfomance_stats(hash_t, argc, argv, print_string_string);

    parseFileAndRemoveEntries(hash_t, ENTRIES_PATH);
    free_ht(&hash_t);
    return 0;
}

