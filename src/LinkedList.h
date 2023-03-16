#ifndef LINKED_LIST_H
#define LINKED_LIST_H
#include "./qlibc/include/qlibc/containers/qlist.h"

typedef qlist_t LinkedList;
typedef qlist_obj_t LinkedListNode;

LinkedList* list_init();
void* list_find(LinkedList* list, int index);
void list_add_first(LinkedList* list, void* value);
void list_add_last(LinkedList* list, void* value);
void* list_pop_last(LinkedList* list);
int list_get_next(LinkedList* list, LinkedListNode* iterate_object);
void list_reverse(LinkedList* list);
void list_free(LinkedList* list);
void list_dump(LinkedList* list);

#endif
