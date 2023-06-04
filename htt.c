#include "hash_iterator.h" // include "hash_map.h" too
#include "hash_map.h"
#include <time.h>

void printItemInfo(Ht_Item* item){
    if(item){
        printf("%s: %s\n", (char*)item->key, (char*)item->val);
    }
}

int main(void){
    HashTable* hash_t = ht_create(CAPACITY, hash_func_str);
    HashIterator iterator = ht_iterator_create(hash_t);
    
    parseFileAndPopulateHashTable(hash_t, "entries.txt");

    // This will retrieve the 100th item in the Hash Table
    Ht_Item* onehundreth = ht_iterator_get_nth_item(&iterator, 100);
    Ht_Item* maxChainNode = hash_t->buckets[maximumChainLength(hash_t).hashOfMaxChain]; 
    
    // To output the chain, traverse the maxChainNode as a simple linked list
    printf("Maximum Length Chain Node: ");
    printItemInfo(maxChainNode);

    // Reset the iterator to point at the beginning of the Hash Table
    ht_iterator_reset(&iterator);

    // let's time going all over the Hash Table, even iterating throught the empty buckets
    clock_t start_time, end_time;
    start_time = clock();

    while(ht_iterator_has_next(&iterator)){
        (void)ht_iterator_get(&iterator);
        // printItemInfo(curr);
        ht_iterator_next(&iterator);
    }    

    end_time = clock();

    double cpu_time_used = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;

    // Time Performances
    (void)printf("Totale time to traverse Hash Table: %f\n", cpu_time_used);

    printHashTableInfo(hash_t);
    parseFileAndRemoveEntries(hash_t, ENTRIES);

    ht_free(&hash_t);
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