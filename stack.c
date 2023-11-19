#include "stack.h"
#include "stdio.h"

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

char* get_string_to_table_symbol(Precedence_table_symbol symbol){
    switch (symbol) {
        case PLUS:
            return "+";
        case MINUS:
            return "-";
        case MUL:
            return "*";
        case DIV:
            return "/";
        case EQUAL:
            return "=";
        case N_EQUAL:
            return "!=";
        case L_EQUAL:
            return "<=";
        case LESS:
            return "<";
        case G_EQUAL:
            return ">=";
        case GREATER:
            return ">";
        case DQ_MARK:
            return "??";
        case EX_MARK:
            return "!";
        case LEFT_BRACKET:
            return "(";
        case RIGHT_BRACKET:
            return ")";
        case IDENTIFIER:
            return "I";
        case INT_NUMBER:
            return "i";
        case DOUBLE_NUMBER:
            return "d";
        case STRING:
            return "s";
        case DOLLAR:
            return "$";
        case STOP:
            return ">";
        case N_TERMINAL:
            return "E";
    }
    return "";
}

void stack_print_all_symbols(t_stack *s){
    t_stack_elem *tmp = s->top;
    while (tmp != NULL){
        printf("%s", get_string_to_table_symbol(tmp->symbol));
        tmp = tmp->next;
    }
    printf("\n");
}
