#include "../src/hash_map.h"
#include "../src/hash_utils.h"

typedef struct env{
    uint8_t u_char;
    uint16_t short_num;
    float float_num;
    char buffer[20];
}env;

// Writing a maximum of 20 bytes
void write_to_buffer(env* a, const char* message){
    strncpy(a->buffer, message, strlen(message) );
    a->buffer[19] = '\0';
}

env create_env_test_var(uint8_t u_c, uint16_t s_n, float f_n, const char* msg){
    env var = {
        .u_char = u_c,
        .short_num = s_n, //1024
        .float_num = f_n,
    };
    write_to_buffer(&var, msg);

    return var;
}

// Helper Print Function
void print_string_struct(size_t hash, const char* key, const void* val){
    hash = fnv_double_hash_func(key, 197); // HardCoded capacity, we already know it from the main(Just for Testing Purposes)
    (void)printf("Hash: %ld, Key: %s, ", hash, key);
    (void)printf("U_Char: %c, Short_Num: %d, Float_Num: %.2f, Buffer: %s\n",
    ((env*)val)->u_char, ((env*)val)->short_num, ((env*)val)->float_num, ((env*)val)->buffer
    );
}

int main(int argc, char* argv[]){
    // Create the HashTable
    HashTable* hash_t = ht_create(197, fnv_double_hash_func, CHAINING);

    // Create the env array
    env vars[26];
    for(uint8_t i = 0; i < 26; ++i){
        vars[i] = create_env_test_var(0x61 + i, 0x0004 + i, 42.2f + i, "Kitty :3");
        char key[6] = {0};
        (void)sprintf(key, "%s%d", "Key_", i);
        ht_insert(hash_t, key, &vars[i], sizeof(env));
    }

    // Testing Removing
    ht_remove(hash_t, "Key_6");
    ht_remove(hash_t, "Key_2");

    // Modifying
    env mod = create_env_test_var(0x46, 0x0008, 58.9248f, "Kitty >:(");
    ht_modify_item(hash_t, "Key_10", &mod, sizeof(env));

    // Retrieve Item(Angry Kitty >:( )
    env* retrieved = (env*)ht_get_item(hash_t, "Key_10");
    if(retrieved)
        printf("Retrieved: %s\n", retrieved->buffer);

    // clear_ht(hash_t);
    // Printing works even when the HashTable is empty
    ht_print_perfomance_stats(hash_t, argc, argv, print_string_struct);
    
    free_ht(&hash_t);
    return 0;
}