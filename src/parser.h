/**
 * @file parser.h
 * @author Nikita Vetluzhskikh (xvetlu00)
 * @author Murad Mikogaziev (xmikog00)
 * @brief syntax and semantic analysis
 */

#ifndef IFJ_2023_PARSER_H
#define IFJ_2023_PARSER_H

#include "scanner.h"
#include "table_stack.h"
#include <stdbool.h>

/**
 * @brief
 */
typedef struct {
    int line_cnt;               // line counter
    int param_index;            // counter of parameters in a function

    bool is_in_function;        // is in void or non-void function
    bool is_void_function;      // is in void function
    bool is_in_params;          // is in func_params rule
    bool is_in_declaration;     // is in ID declaration
    bool is_in_condition;       // is in if/while/then construction
    bool eol_flag;              // met EOL

    t_table_stack *tableStack;

    token_t_ptr token_ptr;           // current token

    item_data *id;                   // current id that is being processed
    item_data *id_type;              // left side (variable)
    item_data *exp_type;             // right side (expression)
    item_data *func_id;              // current func
} parser_data_t;

/**
 * @brief
 */
parser_data_t *init_data();

/**
 * @brief
 */
void free_data(parser_data_t *data);

/**
 * @brief
 */
int analyse();

/**
 * @brief
 */
int program(parser_data_t *data);

/**
 * @brief
 */
int stm(parser_data_t *data);

/**
 * @brief
 */
int stm_not_null(parser_data_t *data);

/**
 * @brief
 */
int condition(parser_data_t *data);

/**
 * @brief
 */
int func(parser_data_t *data);

/**
 * @brief
 */
int func_params(parser_data_t *data);

/**
 * @brief
 */
int func_params_not_null(parser_data_t *data);

/**
 * @brief
 */
int var_type(parser_data_t *data);

/**
 * @brief
 */
int return_rule(parser_data_t *data);

/**
 * @brief
 */
int return_void_rule(parser_data_t *data);

/**
 * @brief
 */
int nil_flag(parser_data_t *data);

/**
 * @brief
 */
int call_params(parser_data_t *data);

/**
 * @brief
 */
int call_params_n(parser_data_t *data);

/**
 * @brief
 */
item_type get_type(struct token* token, parser_data_t * data, item_data* item);
#endif //IFJ_2023_PARSER_H
