#include "hash_map.h"

typedef struct HashIterator{
    HashTable* ht;
    size_t curr_bucket;
    Ht_Item* curr_item;
}HashIterator;

HashIterator ht_iterator_create(HashTable* ht){
    HashIterator hiter;
    hiter.ht = ht;
    hiter.curr_bucket = 0;
    hiter.curr_item = ht->buckets[0];
    return hiter;
}

// practically places the cursor / curr_bucket at the next available bucket if there is one
bool ht_iterator_has_next(HashIterator* iterator){
    while(iterator->curr_bucket < iterator->ht->capacity){
        if(iterator->curr_item)
            return true;
    
        iterator->curr_bucket++;
        iterator->curr_item = iterator->ht->buckets[iterator->curr_bucket];
    }

    return false;
}

Ht_Item* ht_iterator_get(HashIterator* iterator){
    Ht_Item* item = iterator->curr_item;
    if(item)
        return item;
    
    iterator->curr_bucket++;
    while(iterator->curr_bucket < iterator->ht->capacity){
        iterator->curr_item = iterator->ht->buckets[iterator->curr_bucket];
        if(iterator->curr_item)
            return iterator->curr_item;
        
        iterator->curr_bucket++;
    }

    return NULL;
}

void ht_iterator_next(HashIterator* iterator){
    iterator->curr_item = iterator->curr_item->next;
    if(!iterator->curr_item){
        iterator->curr_bucket++;
        while(iterator->curr_bucket < iterator->ht->capacity){
            iterator->curr_item = iterator->ht->buckets[iterator->curr_bucket];
            if(iterator->curr_item)
                return;
            
            iterator->curr_bucket++;
        }
    }
}

Ht_Item* ht_iterator_get_nth_item(HashIterator* iterator, size_t n){    
    // out of bounds
    if(n > iterator->ht->capacity)
        return NULL;

    while(ht_iterator_has_next(iterator) && n){
        Ht_Item* curr = ht_iterator_get(iterator);
        n--;

        if(n <= 0)
            return curr;

        ht_iterator_next(iterator);
    }

    return NULL;
}

void ht_iterator_reset(HashIterator* iterator){
    iterator->curr_bucket = 0;
    iterator->curr_item = iterator->ht->buckets[0];
}