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
 *  @brief Initializes a stack.
 *  @param s The stack to initialize.
 */
void stack_init(t_stack* stack);

/**
 * @brief function for finding out if stack is empty
 */
bool stack_is_empty(t_stack *s);

/**
 *  @brief Pushes a new item onto the stack.
 *  @param s The stack.
 *  @param item The item data to push.
 *  @param symbol The precedence table symbol associated with the item.
 *  @return Returns true if the push operation is successful; otherwise, returns false.
 */
bool stack_push(t_stack *s, item_data stack_item, Precedence_table_symbol  symbol);

/**
 *  @brief Pushes a new item after the top terminal in the stack.
 *  @param s The stack.
 *  @param item The item data to push.
 *  @param symbol The precedence table symbol associated with the item.
 *  @return Returns true if the push operation is successful; otherwise, returns false.
 */
bool stack_push_after_top_term(t_stack *s, item_data stack_item, Precedence_table_symbol symbol);

/**
 *  @brief Pops the top item from the stack.
 *  @param s The stack.
 *  @return Returns true if the pop operation is successful; otherwise, returns false.
 */
bool stack_pop(t_stack *s);

/**
 *  @brief Returns a pointer to the top terminal element in the stack.
 *  @param s The stack.
 *  @return Returns a pointer to the top terminal element or NULL if not found.
 */
t_stack_elem* stack_top_terminal(t_stack* s);

/**
 *  @brief Returns a pointer to the top element of the stack.
 *  @param s The stack.
 *  @return Returns a pointer to the top element of the stack.
 */
t_stack_elem *get_top(t_stack *s);

/**
 *  @brief Counts the number of elements in the stack.
 *  @param s The stack.
 *  @return Returns the number of elements in the stack.
 */
int stack_count_elements(t_stack *s);

/**
 *  @brief Prints the string representation of all symbols in the stack.
 *  @param s The stack to print.
 */
void stack_print_all_symbols(t_stack *s);

/**
 *  @brief Frees the memory allocated for the entire stack.
 *  @param s The stack to free.
 */
void stack_free(t_stack* s);

#endif //IFJ_2023_STACK_H
