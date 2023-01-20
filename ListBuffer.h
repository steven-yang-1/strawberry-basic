#ifndef LIST_BUFFER_H
#define LIST_BUFFER_H

typedef struct ListBuffer {
    int count;
    int max_capacity;
    void** list;
} ListBuffer;

ListBuffer* list_buffer_init();
void list_buffer_add(ListBuffer* list_buffer, void* value);
void list_buffer_concat(ListBuffer* list_buffer, ListBuffer* other_buffer);
void list_buffer_update(ListBuffer* list_buffer, int index, void* value);
void* list_buffer_get(ListBuffer* list_buffer, int index);
void list_buffer_delete(ListBuffer* list_buffer, int index);
void* list_buffer_delete_without_free(ListBuffer* list_buffer, int index);
void list_buffer_dump(ListBuffer* list_buffer);
void list_buffer_free(ListBuffer* list_buffer);

#endif
