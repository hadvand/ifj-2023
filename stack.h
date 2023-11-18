#ifndef IFJ_2023_STACK_H
#define IFJ_2023_STACK_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define BRACKET_STACK_MAX 1000

typedef struct stack_elem
{
    char *stack_str;
} stack_elem;

typedef struct stack
{
    stack_elem *data_array[BRACKET_STACK_MAX];
    int top;
} stack;

stack *stack_init();

bool stack_is_empty(stack *s);

bool stack_push(stack *s, char *stack_str);

bool stack_pop(stack *s);

stack_elem *get_top(stack *s);

bool print_and_pop(stack *s);

void stack_dispose(stack *s);

#endif //IFJ_2023_STACK_H
