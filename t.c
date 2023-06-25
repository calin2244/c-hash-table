#include <stdio.h>
#include "./src/hash_map.h"
#include "./src/hash_utils.h"

typedef struct{
    int a;
    int b;
}testin;

int main(int argc, char* argv[]){

    HashTable* ht = ht_create(4, double_hash_func, CHAINING);
    testin a = {
        .a = 2, .b = 4
    };
    testin b = {
        .a = 1, .b = 4
    };

    ht_insert(ht, "Lol", &a, sizeof(testin));
    ht_insert(ht, "solsd", &b, sizeof(testin));

    parseFileAndPopulateHashTable(ht, "hashing.txt");

    // Silently Ignore
    ht_remove(ht, "ss");

    ht_print(ht, print_string_string);
    ht_print_perfomance_stats(ht, argc, argv);

    free_ht(&ht);
    return 0;
}