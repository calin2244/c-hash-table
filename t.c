#include "./src/hash_map.h"
#include "./src/hash_utils.h"

typedef struct{
    int a;
    int b;
}testin;

int main(int argc, char* argv[]){

    HashTable* ht = ht_create(14, double_hash_func, QUADRATIC_PROBING);
    // testin a = {
    //     .a = 2, .b = 4
    // };
    // testin b = {
    //     .a = 1, .b = 4
    // };

    // ht_insert(ht, "Lol", &a, sizeof(testin));
    // ht_insert(ht, "solsd", &b, sizeof(testin));

    ht_insert(ht, "sall", "abc", 4);
    ht_insert(ht, "sall1", "abc", 4);
    ht_insert(ht, "sall2", "abc", 4);
    ht_insert(ht, "sall3", "abc", 4);
    ht_insert(ht, "Calinn", "abc", 4);
    ht_modify_item(ht, "sall2", "rege", 5);

    printf("%ld\n", double_hash_func("sall3", 14));

    ht_remove(ht, "sall2");

    // parseFileAndPopulateHashTable(ht, "hashing.txt");

    // Silently Ignore
    ht_remove(ht, "ss");

    ht_print(ht, print_string_string);
    ht_print_perfomance_stats(ht, argc, argv);

    free_ht(&ht);
    return 0;
}