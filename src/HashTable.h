#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "./hashtable/hashtable.h"
#include <string.h>
#include <stdlib.h>

typedef hash_table HashTable;

HashTable* hash_init();
void hash_put(HashTable *hash_table, void* key, void* value);
void* hash_get(HashTable *hash_table, void* key);
void* hash_get_with_size(HashTable *hash_table, void* key, size_t *value_size);
void hash_delete(HashTable *hash_table, void* key);
void hash_free(HashTable *hash_table);
void** hash_keys(HashTable *hash_table, unsigned int* num_keys);
int hash_has_key(HashTable *hash_table, void* key);
void hash_dump(HashTable *hash_table);

#endif
