#include <stdio.h>
#include "../src/hash_map.h"
#include "../src/hash_utils.h"
#include "limits.h"

#define VAL_BYTES 4
#define INITIAL_FR 1

void print_str_int(size_t hash, const void* key, const void* val){
    hash = 0;
    printf("Key: %c, %ld\n", *(char*)key, *(size_t*)val);
}

void print_keys(HashTable* ht){
    for(size_t i = 0; i < ht->capacity; ++i){
        Ht_Item* curr = ht->buckets[i];
        if(curr){
            printf("%c ", *(char*)curr->key);
        }
    }

    printf("\n");
}

void calculate_char_frequencies(HashTable* ht, const char* query) {
    size_t initial_fr = 1;
    for (size_t i = 0; i < strlen(query); ++i) {
        char key = query[i];
        char tempKey[2];  
        tempKey[0] = key;
        tempKey[1] = '\0';
        if (ht_has_key(ht, tempKey)) {
            Ht_Item* itm = ht_get_item(ht, tempKey);
            if (itm) {
                (*(int*)(itm->val))++;
            }
        } else {
            ht_insert(ht, tempKey, strlen(tempKey), &initial_fr, 1);  // Pass the correct key length
        }

    }
}

typedef struct{
    size_t size;
    int* buff;
}CompressedArray;

CompressedArray zlw_encoding(HashTable* ht, const char* query){
    size_t pos = 0;
    int compressed_ar[256];
    for(int i = 0; i <= 255; ++i){
        char key = (char)i;
        char tempKey[2];
        tempKey[0] = key;
        tempKey[1] = '\0';
        ht_insert(ht, tempKey, strlen(tempKey) + 1, &i, sizeof(size_t));
    }

    char p[256] = "", c[256] = "";
    p[0] = query[0];
    int code = 256;
    for(size_t i = 0; i < strlen(query); ++i){
        if(i != strlen(query) - 1)
            sprintf(c + strlen(c), "%c", query[i + 1]);


        char pc[256]; // p and c concatenated
        (void)strncpy(pc, p, sizeof(pc) + strlen(p));
        (void)strncat(pc, c, strlen(c));

        if(ht_has_key(ht, pc)){
           strcat(p, c);
        }else{
            Ht_Item* itm = ht_get_item(ht, p);
            if(itm)
                compressed_ar[pos++] = *(int*)itm->val;
            
            ht_insert(ht, pc, strlen(pc) + 1, &code, 4);
            code++;
            (void)strncpy(p, c, strlen(c) + 1);
        }

        (void)memset(c, '\0', sizeof(c));
    }

    Ht_Item* itm = ht_get_item(ht, p);
    if(itm)
        compressed_ar[pos++] = *(int*)itm->val;

    CompressedArray arr;
    arr.size = pos;
    arr.buff = (int*)malloc(pos * sizeof(int));

    (void)memcpy(arr.buff, compressed_ar, pos * sizeof(int));
    return arr;
}


int main(){
    HashTable* hash_t = ht_create(100, hash_func_str, LINEAR_PROBING);
    const char* query = "WYS*WYGWYS*WYSWYSG";

    CompressedArray compressed = zlw_encoding(hash_t, query);

    for(size_t i = 0; i < compressed.size; ++i)
        printf("%d ", compressed.buff[i]);

    printf("\nUncompressed Size: %ld\nCompressed size: %ld\n", CHAR_BIT * sizeof(query), CHAR_BIT * sizeof(compressed.buff));

    // print_keys(hash_t);
    // ht_print(hash_t, print_str_int);
    // printHashTableInfo(hash_t);
    ht_free(&hash_t);
    return 0;
}