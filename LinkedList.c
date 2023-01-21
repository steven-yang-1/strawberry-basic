#include "LinkedList.h"
#include <stdlib.h>

LinkedList* list_init() {
    LinkedList* ll = (LinkedList *) malloc(sizeof(LinkedList) + 1);

    if (ll != NULL) {
        LinkedListNode* empty_node = (LinkedListNode *) malloc(sizeof(LinkedListNode) + 1);
        empty_node->value = NULL;
        empty_node->next_node = NULL;

        ll->count = 0;
        ll->head = empty_node;
        ll->tail = empty_node;
        return ll;
    }

    return NULL;
}

void* list_find(LinkedList* list, int index) {
    LinkedListNode* pointer = list->head;
    for (int count = 0; count < index; count++) {
        pointer = pointer->next_node;
    }
    return pointer->next_node->value;
}

void list_add(LinkedList* list, void* value) {
    LinkedListNode* tail = list->tail;

    LinkedListNode* new_node = (LinkedListNode *) malloc(sizeof(LinkedList) + 1);
    new_node->value = value;
    new_node->next_node = NULL;

    tail->next_node = new_node;
    list->tail = new_node;
    list->count++;
}

void list_delete(LinkedList* list, int index) {
    if (list->count == 0) {
        return;
    }
    LinkedListNode* pointer = list->head;
    for (int count = 0; count <= index - 1; count++) {
        pointer = pointer->next_node;
    }
    LinkedListNode* current_node = pointer->next_node;
    if (list->count == 1) {
        list->head->next_node = NULL;
        list->tail = list->head;
    } else {
        pointer->next_node = pointer->next_node->next_node;
        if (list->count == index - 1) {
            list->tail = pointer;
        }
    }
    free(current_node->value);
    free(current_node);
    current_node = NULL;
    list->count--;
}

void list_free(LinkedList* list) {
    LinkedListNode* pointer = list->head;
    int node_count = list->count;
    for (int i = 0; i < node_count; i++) {
        LinkedListNode *current_node = pointer;
        pointer = pointer->next_node;
        free(current_node->value);
        free(current_node);
        current_node = NULL;
    }
    list->count = 0;
    free(list);
    list = NULL;
}