#include "../../src/hash_utils.h"
#include "env_vars.h"
#include <time.h>
#include <assert.h>

int main(int argc, char* argv[]){
    HashTable* hash_t = NULL;

    if(argc > 2){
        int size = atoi(argv[2]);
        printf("Size: %d\n", size);
        hash_t = ht_create((size_t)size, fnv_double_hash_func, LINEAR_PROBING);
    }
    else{
        // Fixed Size
        hash_t = ht_create(INITIAL_HT_SIZE, fnv_double_hash_func, LINEAR_PROBING);
    }
    
    int result = parseFileAndPopulateHashTable(hash_t, ENTRIES_PATH);
    if(result == -1){
        return -1;
    }

    // Saving the Data from the Hash Table in a text file
    FILE* out = fopen(OUTPUT_PATH_LINEAR_PROBING, "w");
    if(!out){
        (void)printf("Couldn't open the Output file!\n");
        return -1;
    }


    (void)fclose(out);
    
    ht_print_perfomance_stats(hash_t, argc, argv, print_string_string);
    parseFileAndRemoveEntries(hash_t, ENTRIES_PATH);

    // Assertions
    assert(hash_t->size == 0);

    clear_ht(hash_t);

    free_ht(&hash_t);
    return 0;
}

