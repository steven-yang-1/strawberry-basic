#include "LinkedList.h"
#include <stdlib.h>

LinkedList* list_init() {
	LinkedList *list = qlist(0);
	return list;
}

void* list_find(LinkedList* list, int index) {
	return list->getat(list, index, NULL, false);
}

void list_add_first(LinkedList* list, void* value) {
	list->addfirst(list, value, sizeof(value));	
}

void list_add_last(LinkedList* list, void* value) {
	list->addlast(list, value, sizeof(value));
}

void* list_pop_first(LinkedList* list) {
	return list->popfirst(list, NULL);
}

void* list_pop_last(LinkedList* list) {
	return list->poplast(list, NULL);
}

int list_get_next(LinkedList* list, LinkedListNode* iterate_object) {
	return list->getnext(list, iterate_object, false);
}

void list_reverse(LinkedList* list) {
	list->reverse(list);
}

void list_free(LinkedList* list) {
	list->free(list);
}

void list_dump(LinkedList* list) {
	list->debug(list, stdout);
}
