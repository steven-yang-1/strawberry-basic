#include <stdlib.h>

void* make_integer(int value) {
    int* new_val = malloc(sizeof(int) + 1);
    *new_val = value;
    return new_val;
}

void* make_double(double value) {
    double* new_val = malloc(sizeof(double) + 1);
    *new_val = value;
    return new_val;
}