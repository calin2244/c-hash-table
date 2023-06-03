#include "hash_iterator.h" // include "hash_map.h" too

typedef void (*PrintHelper)(const void*, const void*);

void printItemInfo(Ht_Item* item, PrintHelper pf){
    pf(item->key, item->val);
}

void print_item(const void* key, const void* val){
    printf("%s: %s\n", (char*)key, (char*)val);
}

int main(int argc, char* argv[]){
    HashTable* hash_t = ht_create(CAPACITY, hash_func_str);
    HashIterator iterator = ht_iterator_create(hash_t);
    
    parseFileAndPopulateHashTable(hash_t, ENTRIES);

    Ht_Item* fourth = ht_iterator_get_nth_item(&iterator, 100);
    printItemInfo(fourth, print_item);
    

    //print_ht(hash_t, print_string_string);
    //printHashTableInfo(hash_t);
    ht_free(&hash_t);

    return 0;
}