/**
 * @file stack.h
 * @author Nikita Vetluzhskikh (xvetlu00)
 * @author Murad Mikogaziev (xmikog00)
 * @brief syntax and semantic analysis
 */

#ifndef IFJ_2023_STACK_H
#define IFJ_2023_STACK_H

#include "semantics.h"
#include "hash.h"

#define BRACKET_STACK_MAX 1000

/**
 * @brief
 */
typedef struct stack_elem
{
    Precedence_table_symbol symbol;
    item_data item;
    struct stack_elem *next;
} t_stack_elem;

/**
 * @brief
 */
typedef struct stack
{
    t_stack_elem *top;
} t_stack;

/**
 * @brief
 */
void stack_init(t_stack* stack);

/**
 * @brief
 */
bool stack_is_empty(t_stack *s);

/**
 * @brief
 */
bool stack_push(t_stack *s, item_data stack_item, Precedence_table_symbol  symbol);

/**
 * @brief
 */
bool stack_push_after_top_term(t_stack *s, item_data stack_item, Precedence_table_symbol symbol);

/**
 * @brief
 */
bool stack_pop(t_stack *s);

/**
 * @brief
 */
t_stack_elem* stack_top_terminal(t_stack* s);

/**
 * @brief
 */
t_stack_elem *get_top(t_stack *s);

/**
 * @brief
 */
int stack_count_elements(t_stack *s);

/**
 * @brief
 */
void stack_print_all_symbols(t_stack *s);

/**
 * @brief
 */
void stack_free(t_stack* s);

#endif //IFJ_2023_STACK_H
