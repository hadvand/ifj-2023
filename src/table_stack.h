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
 * @brief function for initializing table stack
 */
t_table_stack* table_stack_init();

/**
 * @brief function for finding out if table stack is empty
 */
bool table_stack_is_empty(t_table_stack *s);

/**
 * @brief function for pushing new table on top of the table stack
 */
bool table_stack_push(t_table_stack *s, hash_table *table);

/**
 * @brief function for popping table on top of the table stack
 */
bool table_stack_pop(t_table_stack *s);

/**
 * @brief function for getting the table stack top element
 */
t_table_stack_elem *table_stack_top(t_table_stack *s);

/**
 * @brief table stack destructor
 */
void table_stack_free(t_table_stack *s);

/**
 * @brief function for counting elements in the table stack
 */
int table_count_elements_in_stack(t_table_stack *s);

/**
 * @brief function for finding symbol in the whole table stack
 */
symbol *find_symbol_global(t_table_stack *s, char *name, bool is_overlap);

#endif //TABLE_STACK_H
