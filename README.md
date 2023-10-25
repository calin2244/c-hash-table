# 📚 `c-hash-table`

## 🌟 Introduction

Welcome to `c-hash-table`! This is a robust, handcrafted implementation of a Hash Table in C. It's been crafted with love and provides a great foundation for those who are interested in exploring hash tables and their intricacies.

---

## Initialization & Usage Guide
## 1. Setting Up🛠️

Before using the c-hash-table, ensure you include the appropriate header in your source file:
```cpp
#include "hash_map.h"
```
## 2. Creating the Hash Table📦

The core of this library is the HashTable structure. To create a new hash table, use the ht_create function:

```cpp
HashTable* ht = ht_create(
    CAPACITY,          // Desired capacity
    fnv_hash_func,     // Hash function of choice
    LINEAR_PROBING     // Collision resolution method
);
```
Replace `fnv_hash_func` with your desired hash function and `LINEAR_PROBING` with your chosen collision resolution method.

## 3. Inserting Elements➕

To insert an element into your hash table:

```cpp
const char* key = "exampleKey";
const char* value = "exampleValue";
ht_insert(ht, key, value, strlen(value) + 1); // +1 for NULL terminator
```

## 4. Retrieving Elements🔍

To retrieve an element:

```cpp
void* retrieved_value = ht_get_item(ht, "exampleKey");
if(retrieved_value) {
    printf("Value: %s\n", (char*)retrieved_value);
}
```

## 5. Removing Elements❌

For removing an element:

```cpp
size_t removed = ht_remove(ht, "exampleKey");
if(removed) {
    printf("Successfully removed the key!\n");
}
```

## 6. Bulk Operations➕❌

For bulk insertions:
```cpp
// SENTINEL macro is defined in hash_map.h
KeyValuePair items_to_insert[] = {
    {"key3", "value3", sizeof("value3") + 1},
    {"key4", "value4", sizeof("value4") + 1},
    SENTINEL
};

ht_bulk_insert(ht, items_to_insert, 2); // where 2 is the number of key-value pairs
```
---
**🚨Note🚨**: Users are asked to either:
- Use the `SENTINEL` as the last element, **or**
- Provide the correct length of the `KeyValuePair` array.
---

For bulk removals:

```cpp
const char* keys_to_remove[] = {"key1", "key2"};
ht_bulk_remove(ht, keys_to_remove, 2); // where 2 is the number of keys
```

## 7. Cleanup

After you're done using the hash table, ensure you free the memory:

```cpp
free_ht(&ht);
```

## 🔗 Features

**0. Key and Value Constraints:**

Please note that this implementation does not support storing `NULL` for either keys or values. Always ensure that you provide valid, non-null data when interacting with the hash table, 
otherwise you'll get an stdout message.

**1. Collision Resolutions:**  
The heart of any hash table lies in how it addresses collisions. This particular implementation supports three distinct Collision Resolutions:
- **Chaining**
- **Open Addressing**:
  - Linear Probing
  - Quadratic Probing

**2. Prime Capacity:**  
The capacity of the hash table is always set to a prime number, ensuring optimal distribution and minimizing collisions.

**3. Flexible Key-Value Pairs:**  
While the keys are restricted to `char*` for simplicity and robustness, the values are generic (`void*` type). This provides a lot of versatility to the users in what they wish to store in the hash table.

**4. Customizable Hash Functions:**  
You're not tied to a specific hash function. You can provide your own function pointer to a custom hash function. Example: 
```cpp
size_t fnv_hash_func(const char* key, size_t capacity);
```

**5. Advanced Techniques:**  
- **Wrap Around Technique**: In open addressing, if the load factor allows, and the desired bucket is full, the table will wrap around and find the first available bucket starting from the beginning.
- **Tombstone Technique**: For deletions, a 'tombstone' is used to mark the spot of a previously occupied bucket.

**6. Bulk Operations**
- **Bulk Removal**: Supported and ready for use.
- **Bulk Insertion**: Supported and ready for use.


---

## 🎨 SFML Visualization - WIP
# 🚨 IMPORTANT 🚨

- **SFML Dependency**: This project relies on SFML specifically for Windows with a 64-bit architecture.
- **Makefile Configuration**: The Makefile is tailored for `make` installed via Chocolatey (from PowerShell).
- ❗ **Linux Users**: This will **NOT** compile and execute on Linux systems.

Visualization can make understanding and debugging a hash table much more intuitive. This section is still under active development, but here's what we have:

- **Basic Functionality**: You can visualize how items are being stored and how collisions are resolved.
- **Pagination**: While currently you may need to scroll to see all the buckets, a pagination feature is on the horizon.
- **UI Aesthetics**: It's functional, but yes, I admit, it's a bit on the 'ugly' side for now. Aesthetic improvements are underway!
- **Chaining Visualization**: Planning to add a pop-up box feature for better visualization when hovering over chained buckets.
- **Event Management**: Refinements in progress for better user experience.
![image](https://github.com/calin2244/c-hash-table/assets/95591065/de6632ac-8c72-4508-9920-6100b21003aa)


---

## 🌐 Upcoming Feature

**Consistent Hash Ring for Distributed Systems**: This feature is nearing its completion. It leverages the hash table implementation for distributed systems. However, as it's still in the testing phase, it won't be made public just yet.

---

Thank you for exploring `c-hash-table`!

---
