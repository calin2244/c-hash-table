#pragma once
// This file includes parsing from files, printing the hash_table, Peformance Stats

#include <errno.h>
struct HashTable; // Forward-Declaration
typedef void (*PrintHelper)(size_t, const char*, const void*);

typedef struct{
    size_t maxChainLen;
    size_t hashOfMaxChain;
}pair;

// Performance stats
pair maximumChainLength(HashTable* ht){
    size_t maxLen = 0;
    size_t posOfMaxLen = -1;
    for(size_t i = 0; i < ht->capacity; ++i){
        Ht_Item* curr_item = ht->buckets[i];
        if(curr_item){
            size_t currLen = 0;
            while(curr_item){
                curr_item = curr_item->next;
                currLen++;
            }

            if(maxLen < currLen){
                maxLen = currLen;
                posOfMaxLen = i;
            }
        }
    }

    pair out;
    out.maxChainLen = maxLen;
    out.hashOfMaxChain = posOfMaxLen;
    return out;
}

void ht_print(HashTable* ht, PrintHelper printHasht){
    for(size_t hash = 0; hash < ht->capacity; ++hash){
        Ht_Item* item = ht->buckets[hash];

        // Open Addressing Check
        if(item && item->is_tombstone)
            continue;

        if(item){
            printHasht(hash, ht->buckets[hash]->key, ht->buckets[hash]->val);
            if(item->next){
                (void)printf("-----------------------\nChained with: \n");
                item = item->next;
                while(item){
                    (void)printf("[%s: %s]", (char*)item->key, (char*)item->val);
                    item = item->next;
                    if(item)
                        (void)printf(", ");
                }
                (void)printf("\n-----------------------\n\n");
            }
        }
    }
}

void print_string_string(size_t hash, const char* key, const void* val){
    printf("Index:%li, Key: %s, Value: %s\n", hash, (char*)key, (char*)val);
}

void ht_print_time_performances(HashTable* ht){
    pair stats = maximumChainLength(ht);

    (void)printf("-----------------------\n");
    (void)printf("Total Collisions: %d\n", ht->collisions);

    (void)printf("Maximum Chain Length: %ld\n", stats.maxChainLen);
    (void)printf("Hash For the Maximum Chain: %ld\n", stats.hashOfMaxChain);

    (void)printf("HashTable Size: %ld\n", ht->size);
    (void)printf("Load Factor: %f\n", ht->load_factor);
    (void)printf("Capacity: %ld\n", ht->capacity);
    (void)printf("-----------------------\n");
}

/* Copyright (C) 1991 Free Software Foundation, Inc.
This file is part of the GNU C Library.

The GNU C Library is free software; you can redistribute it and/or
modify it under the terms of the GNU Library General Public License as
published by the Free Software Foundation; either version 2 of the
License, or (at your option) any later version.

The GNU C Library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Library General Public License for more details.

You should have received a copy of the GNU Library General Public
License along with the GNU C Library; see the file COPYING.LIB.  If
not, write to the Free Software Foundation, Inc., 675 Mass Ave,
Cambridge, MA 02139, USA.  */

/* CHANGED FOR VMS */
int getl(char **lineptr, size_t *n, FILE *stream){
    static char line[256];
    char* ptr;
    unsigned int len;

    if(lineptr == NULL || n == NULL){
        errno = EINVAL;
        return -1;
    }

    if(ferror (stream))
        return -1;

    if(feof(stream))
        return -1;
        
    fgets(line,256,stream);

    ptr = strchr(line,'\n');   
    if(ptr)
        *ptr = '\0';

    len = strlen(line);

    if((len+1) < 256){
        ptr = (char*)realloc(*lineptr, 256);
        if (ptr == NULL)
            return(-1);
        *lineptr = ptr;
        *n = 256;
    }

    strcpy(*lineptr,line); 
    return(len);
}

// Parsing
int parseFileAndPopulateHashTable(HashTable* ht, const char* file_name){
    FILE* m_file = fopen(file_name, "r");
    if(!m_file){
        (void)printf("%s", "Failed to open the file\n");
        return -1;
    }

    char* line_buff = NULL;
    size_t line_capacity = 0;
    int line_length; 

    while((line_length = getl(&line_buff, &line_capacity, m_file)) != -1){        
        char* key = strtok(line_buff, " ");
        char* val = strtok(NULL, "\n");

        if(key && val){
            //Right-Trim
            size_t val_size = strlen(val);
            while(val_size && (
            val[val_size - 1] == '\n' || 
            val[val_size - 1] == ' ' || 
            val[val_size - 1] == '\r')){
                val[val_size - 1] = '\0';
                val_size--;
            }

            if(val_size != 0)
                ht_insert(ht, key, val, val_size + 1);
        }
    }

    free(line_buff);
    fclose(m_file);

    // Parsing from file succeeded!!
    return 0;
}

void parseFileAndRemoveEntries(HashTable* ht, const char* file_name){
    FILE* m_file = fopen(file_name, "r");
    fseek(m_file, 0, SEEK_SET);

    char line_buffer[108];
    // Removing every entry in the hash table by parsing the file
    while(fgets(line_buffer, sizeof(line_buffer), m_file)){
        const char* key = strtok(line_buffer, " ");
        (void)ht_remove(ht, key); 
    }

    fclose(m_file);
}

void ht_print_perfomance_stats(HashTable* ht, int argc, char* argv[], PrintHelper print_helper){
    if(argc > 1 && strcmp(argv[1], "stats") == 0){
        // To output the chain, traverse the maxChainNode as a simple linked list
        Ht_Item* maxChainNode = ht->buckets[maximumChainLength(ht).hashOfMaxChain]; 
        (void)printf("\nMaximum Length Chain Node: ");
        printf("%s: %s\n", (char*)maxChainNode->key, (char*)maxChainNode->val);

        // Time Performances
        ht_print_time_performances(ht);
    }
    else if(argc > 1 && strcmp(argv[1], "print") == 0){
        ht_print(ht, print_helper);
    }
}
