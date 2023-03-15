#include "HashTable.h"
#include <string.h>
#include <stdlib.h>

HashTable* hash_init() {
	HashTable* hash_table = (HashTable*) malloc(sizeof(HashTable) + 1);
	ht_init(hash_table, HT_KEY_CONST | HT_VALUE_CONST, 0.05);
	return hash_table;
}

void hash_put(HashTable *hash_table, void* key, void* value) {
	ht_insert(hash_table, key, strlen(key)+1, value, sizeof(value));
}

void* hash_get(HashTable *hash_table, void* key) {
	size_t size;
	void* result = ht_get(hash_table, key, strlen(key)+1, &size);
	return result;
}

void* hash_get_with_size(HashTable *hash_table, void* key, size_t *value_size) {
	void* result = ht_get(hash_table, key, strlen(key)+1, value_size);
	return result;
}

void hash_delete(HashTable *hash_table, void* key) {
	ht_remove(hash_table, key, sizeof(key));
}

void hash_free(HashTable *hash_table) {
	ht_destroy(hash_table);
}

int hash_has_key(HashTable *hash_table, void* key) {
	return ht_contains(hash_table, key, strlen(key)+1);
}

void** hash_keys(HashTable *hash_table, unsigned int* num_keys) {
	void** keys = ht_keys(hash_table, num_keys);
	return keys;
}

void hash_dump(HashTable *hash_table) {
	unsigned int num_keys;
	void** keys = ht_keys(hash_table, &num_keys);
	printf("Map(");
	for (int i = 0; i < num_keys; i++) {
		printf("%s -> %s", (char*)keys[i], "");
	}
	printf(")");
}
