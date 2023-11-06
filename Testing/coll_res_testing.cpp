#include <iostream>
#include "../src/hash_map.h"
#include <string>
#include <fstream>
#include <stdio.h>
#include <assert.h>

#define TEST_SIZE 100'000

// Helper function to generate a random string
char* generate_random_string(size_t length) {
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    char* randomString = NULL;

    if(length){
        randomString = (char*)malloc(length + 1);

        if(randomString){
            for(int n = 0; n < length; n++){
                int key = rand() % (sizeof(charset) -1);
                randomString[n] = charset[key];
            }
            randomString[length] = '\0';
        }
    }

    return randomString;
}

void test_coll_res_large(CollisionResolution coll_res){
    HashTable* ht = ht_create(1000, fnv_hash_func, coll_res);

    ht_set_custom_resize_strategy(ht, 25'000, 2);
    ht_set_custom_resize_strategy(ht, 50'000, 5.f);

    int cnt = 0;

    char* keys[TEST_SIZE];
    char* values[TEST_SIZE]; 
    for(int i = 0; i < TEST_SIZE; i++){
        keys[i] = generate_random_string(10);      // 10 chars long random string for key
        values[i] = generate_random_string(10);    // 20 chars long random string for value

        ht_insert(ht, keys[i], values[i], strlen(values[i]) + 1); 

        // Make sure every item is immediately retrievable after insertion
        assert(ht_has_key(ht, keys[i]));
        assert(strcmp((char*)ht_get_item(ht, keys[i]), values[i]) == 0);
    }

    // Ensure all items are still retrievable
    for(int i = 0; i < TEST_SIZE; i++){
        assert(strcmp((char*)ht_get_item(ht, keys[i]), values[i]) == 0);
        cnt++;
    }

    // Remove half the items
    for(int i = 0; i < TEST_SIZE / 2; i++){
        size_t idx = ht_remove(ht, keys[i]);
        assert(!ht_has_key(ht, keys[i]));
    }

    // Ensure the rest still exist
    for(int i = TEST_SIZE / 2; i < TEST_SIZE; i++){
      assert(strcmp((char*)ht_get_item(ht, keys[i]), values[i]) == 0);
      cnt++;
    }

    for(int i = TEST_SIZE / 2; i < TEST_SIZE; ++i){
      ht_remove(ht, keys[i]);
      assert(ht_get_item(ht, keys[i]) == NULL);
    }

    assert(ht->size == 0);

    // Clean up
    for(int i = 0; i < TEST_SIZE; i++){
        free(keys[i]);
        free(values[i]); 
    }

    free_ht(&ht);

    printf("All extensive tests passed!\n");
}

int main(int argc, char* argv[]){
    srand(time(NULL));  // Initialize random seed

    char ch;
    CollisionResolution coll_res;
    
    switch(argc){
      case 1:
        test_coll_res_large(CHAINING);
        return 0;
      case 2: 
        if(strlen(argv[1]) == 1){
          ch = argv[1][0];
          switch(ch){
            case 'c':
              coll_res = CHAINING;
              break;
            case 'l':
              coll_res = LINEAR_PROBING;
              break;
            case 'q':
              coll_res = QUADRATIC_PROBING;
              break;
            case 'd':
              coll_res = DOUBLE_HASHING;
              break;
            default:
              break;
          }
        }
    }

    test_coll_res_large(coll_res);

    return 0;
}