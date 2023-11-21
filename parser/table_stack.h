#include "hash.h"

#ifndef TABLE_STACK_H
#define TABLE_STACK_H

typedef struct table_stack_elem
{
    HashTable *table;
    struct table_stack_elem *next;

} t_table_stack_elem;

typedef struct table_stack
{
    t_table_stack_elem *top;
} t_table_stack;

void table_stack_init(t_table_stack *s);

bool table_stack_is_empty(t_table_stack *s);

bool table_stack_push(t_table_stack *s, HashTable *table);

bool table_stack_pop(t_table_stack *s);

t_table_stack_elem *table_stack_top(t_table_stack *s);

void table_stack_free(t_table_stack *s);

Symbol *findSymbol_global(t_table_stack *s, char *name);

#endif //TABLE_STACK_H
