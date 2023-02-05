#ifndef STACK_H
#define STACK_H

#include "ListBuffer.h"

typedef struct ListBuffer Stack;

Stack* stack_init();
void stack_push(Stack* stack, void* value);
void* stack_pop(Stack* stack);
void* stack_peak(Stack* stack);
void* stack_free(Stack* stack);
int stack_empty(Stack* stack);

#endif