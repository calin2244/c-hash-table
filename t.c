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
    ht_remove(ht, "sall");
    ht_insert(ht, "sall", "anc", 4);
    ht_remove(ht, "sall");
    ht_insert(ht, "sall", "anc", 4);

    ht_print_perfomance_stats(ht, argc, argv, print_string_string);

    free_ht(&ht);
    return 0;
}