#ifndef HASHTABLE_H
#define HASHTABLE_H

#include "LinkedList.h"
#include "ListBuffer.h"

typedef struct HashTableNode {
    char* key;
    void* value;
} HashTableNode;

typedef struct HashTable {
    int count;
    int pool_size;
    LinkedList** container;
    ListBuffer* name_list;
} HashTable;

int hash_key(char* key, int factor);

HashTable* hash_init(int pool_size);
void hash_put(HashTable *hash_table, char* key, void* value);
void* hash_get(HashTable *hash_table, char* key);
void hash_delete(HashTable *hash_table, char* key);
void hash_free(HashTable *hash_table);
int hash_has_key(HashTable *hash_table, char* key);

void hash_dump(HashTable *hash_table);

#endif