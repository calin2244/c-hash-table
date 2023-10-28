#include "../../src/hash_map.h"
#include "../../src/hash_utils.h"
#include "env_vars.h"
#include <assert.h>

int main(int argc, char* argv[]){
    HashTable* hash_t = NULL;
    
    // User Defined Size
    if(argc > 2){
        size_t capacity = atoi(argv[2]);
        printf("Initial Capacity: %ld\n", capacity);
        hash_t = ht_create(capacity, jenkins_hash_func, QUADRATIC_PROBING);
    }
    else{
        // Fixed Size
        hash_t = ht_create(INITIAL_HT_SIZE, jenkins_hash_func, QUADRATIC_PROBING);
    }
    
    parseFileAndPopulateHashTable(hash_t, ENTRIES_PATH);

    ht_print_perfomance_stats(hash_t, argc, argv, print_str_str);

    parseFileAndRemoveEntries(hash_t, ENTRIES_PATH);

    assert(hash_t->size == 0);

    free_ht(&hash_t);

    return 0;
}