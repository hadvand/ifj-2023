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
 * @brief
 */
typedef struct table_stack_elem
{
    hash_table *table;
    struct table_stack_elem *next;

} t_table_stack_elem;

/**
 * @brief
 */
typedef struct table_stack
{
    t_table_stack_elem *top;
} t_table_stack;

/**
 * @brief
 */
t_table_stack* table_stack_init();

/**
 * @brief
 */
bool table_stack_is_empty(t_table_stack *s);

/**
 * @brief
 */
bool table_stack_push(t_table_stack *s, hash_table *table);

/**
 * @brief
 */
bool table_stack_pop(t_table_stack *s);

/**
 * @brief
 */
t_table_stack_elem *table_stack_top(t_table_stack *s);

/**
 * @brief
 */
void table_stack_free(t_table_stack *s);

/**
 * @brief
 */
int table_count_elements_in_stack(t_table_stack *s);

/**
 * @brief
 */
symbol *find_symbol_global(t_table_stack *s, char *name);

#endif //TABLE_STACK_H
