#include <stdio.h>
#include "../src/hash_map.h"
#include "../src/hash_utils.h"

#define VAL_BYTES 4
#define BIT 8

// void print_str_int(size_t hash, const void* key, const void* val){
//     hash = 0;
//     printf("Key: %c, %ld\n", *(char*)key, *(size_t*)val);
// }

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

    char p[108] = "", c[108] = "";
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

int main(int argc, char* argv[]){
    HashTable* hash_t = ht_create(100, hash_func, LINEAR_PROBING);
    char query[256];
    if(argc > 1)
        (void)strncpy(query, argv[1], strlen(argv[1]) + 1);
    else{
        (void)printf("Provide an input string!\n");
        return -1;
    }

    CompressedArray compressed = zlw_encoding(hash_t, query);

    for(size_t i = 0; i < compressed.size; ++i)
        printf("%d ", compressed.buff[i]);

    size_t uncompressed_size = BIT * strlen(query);
    size_t compressed_size = compressed.size * BIT * sizeof(int);
    float compression_ratio = ((float)compressed_size / (float)uncompressed_size) * 100;

    (void)printf("\nUncompressed Size: %ld\nCompressed size: %ld\n", uncompressed_size, compressed_size);
    (void)printf("Compression Ratio: %.2f%%\n", compression_ratio);

    free(compressed.buff);
    free_ht(&hash_t);
    return 0;
}