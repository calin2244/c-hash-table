#include "hash_iterator.h" // include "hash_map.h" too
#include "hash_map.h"
#include "time.h"

void printItemInfo(Ht_Item* item){
    if(item){
        printf("%s: %s\n", (char*)item->key, (char*)item->val);
    }
}

int main(int argc, char* argv[]){
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

    while(ht_iterator_has_next(&iterator)){
        Ht_Item* curr = ht_iterator_get(&iterator);
        printItemInfo(curr);
        ht_iterator_next(&iterator);
    }

    printHashTableInfo(hash_t);

    parseFileAndRemoveEntries(hash_t, ENTRIES);
    ht_free(&hash_t);

    return 0;
}