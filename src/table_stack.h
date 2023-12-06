/**
 * @file table_stack.h
 * @author Neonila Mashlai (xmashl00)
 * @author Nikita Vetluzhskikh (xvetlu00)
 * @brief syntax and semantic analysis
 */

#include "hash.h"

#ifndef TABLE_STACK_H
#define TABLE_STACK_H

/**
 * @brief table stack item representation
 */
typedef struct table_stack_elem
{
    hash_table *table;
    struct table_stack_elem *next;

} t_table_stack_elem;

/**
 * @brief table stack structure
 */
typedef struct table_stack
{
    t_table_stack_elem *top;
} t_table_stack;

/**
 * @brief Initializes and returns a new table stack.
 */
t_table_stack* table_stack_init();

/**
 *  @brief Checks if the table stack is empty.
 *  @param s The table stack to check.
 *  @return Returns true if the table stack is empty; otherwise, returns false.
 */
bool table_stack_is_empty(t_table_stack *s);

/**
 *  @brief Pushes a new hash table onto the table stack.
 *  @param s The table stack.
 *  @param table The hash table to push.
 *  @return Returns true if the push operation is successful; otherwise, returns false.
 */
bool table_stack_push(t_table_stack *s, hash_table *table);

/**
 *  @brief Pops the top hash table from the table stack.
 *  @param s The table stack.
 *  @return Returns true if the pop operation is successful; otherwise, returns false.
 */
bool table_stack_pop(t_table_stack *s);

/**
 *  @brief Returns a pointer to the top element of the table stack.
 *  @param s The table stack.
 *  @return Returns a pointer to the top element of the table stack.
 */
t_table_stack_elem *table_stack_top(t_table_stack *s);

/**
 *  @brief Frees the memory allocated for the entire table stack.
 *  @param s The table stack to free.
 */
void table_stack_free(t_table_stack *s);

/**
 *  @brief Counts the number of elements in the table stack.
 *  @param s The table stack.
 *  @return Returns the number of elements in the table stack.
 */
int table_count_elements_in_stack(t_table_stack *s);

/**
 *  @brief Finds and returns a symbol globally in the table stack by name.
 *  @param s The table stack.
 *  @param name The name of the symbol to find.
 *  @return Returns a pointer to the symbol if found; otherwise, returns NULL.
 */
symbol *find_symbol_global(t_table_stack *s, char *name);

#endif //TABLE_STACK_H
