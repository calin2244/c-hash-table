#include "hash_iterator.h" // include "hash_map.h" too

void printItemInfo(Ht_Item* item){
    if(item){
         printf("%s: %s\n", (char*)item->key, (char*)item->val);
    }
}

int main(int argc, char* argv[]){
    HashTable* hash_t = ht_create(CAPACITY, hash_func_str);
    HashIterator iterator = ht_iterator_create(hash_t);
    
    parseFileAndPopulateHashTable(hash_t, ENTRIES);
    Ht_Item* maxChain = ht_iterator_get_nth_item(&iterator, 1324);
    printItemInfo(maxChain);

    parseFileAndRemoveEntries(hash_t, ENTRIES);
    //print_ht(hash_t, print_string_string);
    printHashTableInfo(hash_t);
    ht_free(&hash_t);

    return 0;
}