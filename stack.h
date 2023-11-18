#ifndef IFJ_2023_STACK_H
#define IFJ_2023_STACK_H

#include "semantics/semantics.h"
#include "parser/hash.h"

#define BRACKET_STACK_MAX 1000

typedef struct stack_elem
{
    Precedence_table_symbol symbol;
    item_data item;
    struct stack_elem *next;
} t_stack_elem;

typedef struct stack
{
    t_stack_elem *data_array[BRACKET_STACK_MAX];
    int top;
} t_stack;

t_stack *stack_init();

bool stack_is_empty(t_stack *s);

void stack_push(t_stack *s, item_data stack_item);

void stack_pop(t_stack *s);

t_stack_elem *get_top(t_stack *s);

void print_and_pop(t_stack *s);

void stack_dispose(t_stack *s);

#endif //IFJ_2023_STACK_H
