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
 * @brief stack element representation
 */
typedef struct stack_elem
{
    Precedence_table_symbol symbol;
    item_data item;
    struct stack_elem *next;
} t_stack_elem;

/**
 * @brief stack structure
 */
typedef struct stack
{
    t_stack_elem *top;
} t_stack;

/**
 * @brief function for stack initialization
 */
void stack_init(t_stack* stack);

/**
 * @brief function for finding out if stack is empty
 */
bool stack_is_empty(t_stack *s);

/**
 * @brief function for pushing element on top of the stack
 */
bool stack_push(t_stack *s, item_data stack_item, Precedence_table_symbol  symbol);

/**
 * @brief
 */
bool stack_push_after_top_term(t_stack *s, item_data stack_item, Precedence_table_symbol symbol);

/**
 * @brief function for popping the top element of the stack
 */
bool stack_pop(t_stack *s);

/**
 * @brief getter for top terminal of the stack
 */
t_stack_elem* stack_top_terminal(t_stack* s);

/**
 * @brief getter for top element of the stack
 */
t_stack_elem *get_top(t_stack *s);

/**
 * @brief function for counting all element in the stack
 */
int stack_count_elements(t_stack *s);

/**
 * @brief function for printing all symbols from stack
 */
void stack_print_all_symbols(t_stack *s);

/**
 * @brief stack destructor
 */
void stack_free(t_stack* s);

#endif //IFJ_2023_STACK_H
