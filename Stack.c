#include "Stack.h"
#include <stdlib.h>
#include <string.h>

Stack* stack_init() {
    return list_buffer_init();
}

void stack_push(Stack* stack, void* value) {
    list_buffer_add(stack, value);
}

void* stack_pop(Stack* stack) {
    if (stack->count == 0) {
        return NULL;
    }
    return list_buffer_delete_without_free(stack, stack->count - 1);
}

void* stack_peak(Stack* stack) {
    return list_buffer_get(stack, stack->count - 1);
}

void* stack_free(Stack* stack) {
    list_buffer_free(stack);
}

int stack_empty(Stack* stack) {
    return stack->count == 0;
}