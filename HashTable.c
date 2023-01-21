#include "LinkedList.h"
#include "HashTable.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int hash_key(char* key, int factor) {
    int key_len = (int)strlen(key);
    int sum = 0;
    for (int i = 0; i < key_len; i++) {
        sum += (int) key[i];
    }
    return sum % factor;
}

HashTable* hash_init(int pool_size) {
    HashTable* hash_table = (HashTable *) malloc(sizeof(HashTable) + 1);
    if (hash_table != NULL) {
        hash_table->count = 0;
        hash_table->pool_size = pool_size;
        hash_table->container = malloc(sizeof(LinkedList) * pool_size + 1);
        for (int i = 0; i < pool_size; i++) {
            hash_table->container[i] = list_init();
        }
        return hash_table;
    }
    return NULL;
}

void hash_put(HashTable* hash_table, char* key, void* value) {
    int location = hash_key(key, hash_table->pool_size);
    LinkedList* list = hash_table->container[location];
    LinkedListNode* pointer_node = list->head;
    HashTableNode* new_node = (HashTableNode *) malloc(sizeof(HashTableNode) + 1);
    new_node->key = (char *) malloc(sizeof(char) * strlen(key) + 1);
    strcpy(new_node->key, key);
    new_node->value = value;
    int exists = 0;
    for (int i = 0; i <= list->count; i++) {
        if (i > 0) {
            if (!strcmp(key, ((HashTableNode *) (pointer_node->value))->key)) {
                exists = 1;
                break;
            }
        }
        pointer_node = pointer_node->next_node;
    }
    if (exists) {
        free(((HashTableNode *) pointer_node)->value);
        ((HashTableNode *) pointer_node->value)->value = NULL;
        ((HashTableNode *) pointer_node->value)->value = value;
    } else {
        list_add(list, new_node);
    }
    hash_table->count++;
}

void* hash_get(HashTable *hash_table, char* key) {
    int location = hash_key(key, hash_table->pool_size);
    LinkedList* list = hash_table->container[location];
    LinkedListNode* pointer_node = list->head;
    for (int i = 0; i <= list->count; i++) {
        if (i > 0) {
            if (!strcmp(key, ((HashTableNode *) (pointer_node->value))->key)) {
                return ((HashTableNode *) (pointer_node->value))->value;
            }
        }
        pointer_node = pointer_node->next_node;
    }
    return NULL;
}

void hash_delete(HashTable *hash_table, char* key) {
    int location = hash_key(key, hash_table->pool_size);
    LinkedList* list = hash_table->container[location];
    LinkedListNode* pointer_node = list->head;
    int index = -1;
    for (int i = 0; i <= list->count; i++) {
        if (i > 0) {
            if (!strcmp(key, ((HashTableNode *) (pointer_node->value))->key)) {
                index = i - 1;
                break;
            }
        }
        pointer_node = pointer_node->next_node;
    }
    if (index != -1) {
        list_delete(list, index);
    }
    hash_table->count--;
}

void hash_free(HashTable *hash_table) {
    for (int i = 0; i < hash_table->pool_size; i++) {
        list_free(hash_table->container[i]);
    }
    free(hash_table->container);
    hash_table->container = NULL;
    hash_table->count = 0;
    free(hash_table);
    hash_table = NULL;
}

void hash_dump(HashTable *hash_table) {
    printf("Map (\n");
    for (int j = 0; j < hash_table->pool_size; j++) {
        LinkedList* list = hash_table->container[j];
        if (list->count > 0) {
            LinkedListNode* pointer_node = list->head;
            for (int i = 0; i <= list->count; i++) {
                if (i > 0) {
                    printf("\t%s -> %s\n",
                           ((HashTableNode *) (pointer_node->value))->key,
                           (char *)((HashTableNode *) (pointer_node->value))->value
                    );
                }
                pointer_node = pointer_node->next_node;
            }
        }
    }
    printf(")\n");
}

int hash_has_key(HashTable *hash_table, char* key) {
    int location = hash_key(key, hash_table->pool_size);
    LinkedList* list = hash_table->container[location];
    LinkedListNode* pointer_node = list->head;
    int exists = 0;
    for (int i = 0; i <= list->count; i++) {
        if (i > 0) {
            if (!strcmp(key, ((HashTableNode *) (pointer_node->value))->key)) {
                exists = 1;
                break;
            }
        }
        pointer_node = pointer_node->next_node;
    }
    return exists;
}