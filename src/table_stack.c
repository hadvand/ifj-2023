/**
 * @file table_stack.c
 * @author Neonila Mashlai (xmashl00)
 * @author Nikita Vetluzhskikh (xvetlu00)
 * @brief syntax and semantic analysis
 */

#include "table_stack.h"

t_table_stack* table_stack_init()
{
    t_table_stack* s = (t_table_stack*)malloc(sizeof (t_table_stack));
    if(s == NULL)
        return NULL;
    s->top = NULL;
    return s;
}

bool table_stack_is_empty(t_table_stack *s)
{
    return s->top == NULL;
}

bool table_stack_push(t_table_stack *s, hash_table *table)
{
    t_table_stack_elem *new_elem = (t_table_stack_elem *)malloc(sizeof(t_table_stack_elem));

    if (new_elem == NULL)
        return false;

    new_elem->table = table;
    new_elem->next = s->top;

    s->top = new_elem;

    return true;
}

bool table_stack_pop(t_table_stack *s)
{
    if (s->top != NULL)
    {
        t_table_stack_elem *tmp = s->top;
        s->top = tmp->next;
        free(tmp);

        return true;
    }
    return false;
}

t_table_stack_elem *table_stack_top(t_table_stack *s)
{
    return s->top;
}


void table_stack_free(t_table_stack *s)
{
    while (s->top != NULL)
    {
        table_stack_pop(s);
    }
}

int table_count_elements_in_stack(t_table_stack *s){
    int count = 0;
    t_table_stack_elem *tmp = s->top;
    while (tmp != NULL){
        count++;
        tmp = tmp->next;
    }
    return count;
}

symbol *find_symbol_global(t_table_stack *s, char *name)
{
    for (t_table_stack_elem *tmp = s->top; tmp != NULL; tmp = tmp->next)
    {
        symbol *symbol = find_symbol(tmp->table, name);
        if(symbol != NULL)
            return symbol;
    }
    return NULL;
}
