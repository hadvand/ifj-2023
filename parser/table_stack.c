#include "table_stack.h"

void table_stack_init(t_table_stack *s)
{
    s->top = NULL;
}

bool table_stack_is_empty(t_table_stack *s)
{
    return s->top == NULL;
}

bool table_stack_push(t_table_stack *s, HashTable *table)
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
