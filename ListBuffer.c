#include "ListBuffer.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

ListBuffer* list_buffer_init() {
    ListBuffer* list_buffer = (ListBuffer *) malloc(sizeof(ListBuffer) + 1);
    list_buffer->count = 0;
    list_buffer->max_capacity = 10;
    list_buffer->list = malloc(sizeof(void) * list_buffer->max_capacity + 1);
    return list_buffer;
}

void list_buffer_add(ListBuffer* list_buffer, void* value) {
    if (list_buffer->count + 1 > list_buffer->max_capacity) {
        list_buffer->max_capacity = list_buffer->max_capacity + 10;
        void** new_list = malloc(sizeof(void) * list_buffer->max_capacity + 1);
        memcpy(new_list, list_buffer->list, list_buffer->count);
        list_buffer->list = new_list;
    }
    list_buffer->list[list_buffer->count] = value;
    list_buffer->count++;
}

void list_buffer_concat(ListBuffer* list_buffer, ListBuffer* other_buffer) {
    if (list_buffer->count + other_buffer->count > list_buffer->max_capacity) {
        list_buffer->max_capacity = list_buffer->max_capacity + other_buffer->count + 10;
        void** new_list = malloc(sizeof(void) * list_buffer->max_capacity + 1);
        memcpy(new_list, list_buffer->list, list_buffer->count);
        list_buffer->list = new_list;
    }
    for (int i = 0; i < other_buffer->count; i++) {
        list_buffer->list[list_buffer->count + i] = other_buffer->list[i];
    }
    list_buffer->count = list_buffer->count + other_buffer->count;
}

void list_buffer_update(ListBuffer* list_buffer, int index, void* value) {
    free(list_buffer->list[index]);
    list_buffer->list[index] = value;
}

void* list_buffer_get(ListBuffer* list_buffer, int index) {
    return list_buffer->list[index];
}

void list_buffer_delete(ListBuffer* list_buffer, int index) {
    if (list_buffer->count == 0) {
        return;
    }
    free(list_buffer->list[index]);
    for (int i = index; i < list_buffer->count - 1; i++) {
        list_buffer->list[i] = list_buffer->list[i + 1];
    }
    list_buffer->count--;
}

void* list_buffer_delete_without_free(ListBuffer* list_buffer, int index) {
    if (list_buffer->count == 0) {
        return NULL;
    }
    void* tmp = list_buffer->list[index];
    for (int i = index; i < list_buffer->count - 1; i++) {
        list_buffer->list[i] = list_buffer->list[i + 1];
    }
    list_buffer->count--;
    return tmp;
}

void list_buffer_dump(ListBuffer* list_buffer) {
    printf("List(\n");
    for (int i = 0; i < list_buffer->count; i++) {
        printf("\t%d \n", *(int *)(list_buffer->list[i]));
    }
    printf(")\n");
}

void list_buffer_free(ListBuffer* list_buffer) {
    list_buffer->count = 0;
    for (int i = 0; i < list_buffer->count; i++) {
        free(list_buffer->list[i]);
    }
    free(list_buffer->list);
}
