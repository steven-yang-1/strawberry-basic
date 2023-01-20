#ifndef LINKED_LIST_H
#define LINKED_LIST_H
#include "Data.h"

typedef struct LinkedListNode {
    void* value;
    struct LinkedListNode* next_node;
} LinkedListNode;

typedef struct LinkedList {
    int count;
    LinkedListNode* head;
    LinkedListNode* tail;
} LinkedList;

LinkedList* list_init();
void* list_find(LinkedList* list, int index);
void list_add(LinkedList* list, void* value);
void list_delete(LinkedList* list, int index);
void list_free(LinkedList* list);

#endif