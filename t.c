#include "./src/hash_map.h"
#include "./src/hash_utils.h"

typedef struct{
    int a;
    int b;
}testin;

int main(int argc, char* argv[]){

    HashTable* ht = ht_create(113, fnv_hash_func, LINEAR_PROBING);

    ht_insert(ht, "sall", "abc", 4);
    ht_insert(ht, "sall", "laba", 5);
    ht_insert(ht, "sall0", "cal", 4);
    
    parseFileAndPopulateHashTable(ht, "entries.txt");

    // Silently Ignore
    ht_remove(ht, "ss");

    // clear_ht(ht);

    ht_insert(ht, "Norocel", "Meow", 5);

    ht_print_perfomance_stats(ht, argc, argv, print_string_string);

    free_ht(&ht);
    return 0;
}