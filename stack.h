#ifndef IFJ_2023_STACK_H
#define IFJ_2023_STACK_H

#include "scanner/scanner.h"

#define BRACKET_STACK_MAX 1000

typedef struct stack_elem
{
    token_t_ptr *stack_item;
} stack_elem;

typedef struct stack
{
    stack_elem *data_array[BRACKET_STACK_MAX];
    int top;
} stack;

stack *stack_init();

bool stack_is_empty(stack *s);

void stack_push(stack *s, token_t_ptr *stack_item);

void stack_pop(stack *s);

stack_elem *get_top(stack *s);

void print_and_pop(stack *s);

void stack_dispose(stack *s);

#endif //IFJ_2023_STACK_H
