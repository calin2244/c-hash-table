#include "../../src/hash_utils.h"
#include "env_vars.h"

int main(int argc, char* argv[]){
    HashTable* hash_t = NULL;
    
    // User Defined Size
    if(argc > 2){
        int size = atoi(argv[2]);
        printf("Size: %d\n", size);
        hash_t = ht_create((size_t)size, jenkins_hash_func, QUADRATIC_PROBING);
    }
    else{
        // Fixed Size
        hash_t = ht_create(INITIAL_HT_SIZE, jenkins_hash_func, QUADRATIC_PROBING);
    }
    
    parseFileAndPopulateHashTable(hash_t, ENTRIES_PATH);

    ht_print_perfomance_stats(hash_t, argc, argv, print_string_string);

    // parseFileAndRemoveEntries(hash_t, OUTPUT_PATH);

    free_ht(&hash_t);

    return 0;
}