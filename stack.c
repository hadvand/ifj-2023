#include "stack.h"

void stack_init(t_stack* s)
{
    s->top = NULL;
}


bool stack_push(t_stack *s, item_data item, Precedence_table_symbol  symbol)
{
    t_stack_elem* new_elem = (t_stack_elem*)malloc(sizeof(t_stack_elem));

    if (new_elem == NULL)
        return false;

    new_elem->symbol = symbol;
    new_elem->item = item;
    new_elem->next = s->top;

    s->top = new_elem;

    return true;
}


bool stack_pop(t_stack *s)
{
    if (s->top != NULL)
    {
        t_stack_elem* tmp = s->top;
        s->top = tmp->next;
        free(tmp);

        return true;
    }
    return false;
}


//void symbol_stack_pop_count(Symbol_stack* s, int count)
//{
//    for (int i = 0; i < count; i++)
//    {
//        symbol_stack_pop(s);
//    }
//}


t_stack_elem* stack_top_terminal(t_stack* s)
{
    for (t_stack_elem* tmp = s->top; tmp != NULL; tmp = tmp->next)
    {
        if (tmp->symbol < STOP)
            return tmp;
    }

    return NULL;
}


bool stack_push_after_top_term(t_stack *s, item_data item, Precedence_table_symbol symbol)
{
    t_stack_elem* prev = NULL;

    for (t_stack_elem* tmp = s->top; tmp != NULL; tmp = tmp->next)
    {
        if (tmp->symbol < STOP)
        {
            t_stack_elem* new_elem = (t_stack_elem*)malloc(sizeof(t_stack_elem));

            if (new_elem == NULL)
                return false;

            new_elem->symbol = symbol;
            new_elem->item = item;

            if (prev == NULL)
            {
                new_elem->next = s->top;
                s->top = new_elem;
            }
            else
            {
                new_elem->next = prev->next;
                prev->next = new_elem;
            }

            return true;
        }

        prev = tmp;
    }

    return false;
}


t_stack_elem *get_top(t_stack *s)
{
    return s->top;
}


void stack_free(t_stack* s)
{
    while (stack_pop(s));
}
