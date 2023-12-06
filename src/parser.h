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
 * @brief internal parser data representation
 */
typedef struct {
    int line_cnt;               // line counter
    int param_index;            // counter of parameters in a function

    bool is_in_function;        // is in void or non-void function
    bool is_void_function;      // is in void function
    bool is_in_params;          // is in func_params rule
    bool is_in_declaration;     // is in ID declaration
    bool is_it_let_condition;       // is in if/while/then construction
    bool eol_flag;              // met EOL

    t_table_stack *table_stack;

    token_t_ptr token_ptr;           // current token

    item_data *id;                   // current id that is being processed
    item_data *id_type;              // left side (variable)
    item_data *exp_type;             // right side (expression)
    item_data *func_id;              // current func
} parser_data_t;

/**
 * @brief Initializes parser data.
 * @return A pointer to the initialized parser data.
 */
parser_data_t *init_data();

/**
 * @brief Frees the memory allocated for parser data.
 * @param data A pointer to the parser data to be freed.
 */
void free_data(parser_data_t *data);

/**
 * @brief Main function that launches the entire compilation process.
 * @return Returns the result of the compilation process.
 */
int analyse();

/**
 * @brief Parses the 'program' rule in the grammar.
 * @param data Pointer to the parser data structure.
 * @return Returns the result code of the parsing process.
 */
int program(parser_data_t *data);

/**
 * @brief Parses the 'stm' rule in the grammar.
 * @param data Pointer to the parser data structure.
 * @return Returns the result code of the parsing process.
 */
int stm(parser_data_t *data);

/**
 * @brief stm_not_null rule
 */
int stm_not_null(parser_data_t *data);

/**
 * @brief Parses the 'condition' rule in the grammar.
 * @param data Pointer to the parser data structure.
 * @return Returns the result code of the parsing process.
 */
int condition(parser_data_t *data);

/**
 * @brief Parses the 'func_params' rule in the grammar.
 * @param data Pointer to the parser data structure.
 * @return Returns the result code of the parsing process.
 */
int func_params(parser_data_t *data);

/**
 * @brief Parses the 'func_params_not_null' rule in the grammar.
 * @param data Pointer to the parser data structure.
 * @return Returns the result code of the parsing process.
 */
int func_params_not_null(parser_data_t *data);

/**
 * @brief var_type rule
 */
int var_type(parser_data_t *data);

/**
 * @brief Parses the 'return_rule' rule in the grammar.
 * @param data Pointer to the parser data structure.
 * @return Returns the result code of the parsing process.
 */
int return_rule(parser_data_t *data);

/**
 * @brief Parses the 'return_void_rule' rule in the grammar.
 * @param data Pointer to the parser data structure.
 * @return Returns the result code of the parsing process.
 */
int return_void_rule(parser_data_t *data);

/**
 * @brief Parses the 'call_params' rule in the grammar.
 * @param data Pointer to the parser data structure.
 * @return Returns the result code of the parsing process.
 */
int call_params(parser_data_t *data);

/**
 * @brief Parses the 'call_params_n' rule in the grammar.
 * @param data Pointer to the parser data structure.
 * @return Returns the result code of the parsing process.
 */
int call_params_n(parser_data_t *data);

/**
 * @brief Gets the item type associated with a token.
 * @param token Pointer to the token.
 * @param data Pointer to the parser data.
 * @param item Pointer to the item_data structure.
 * @return Returns the item type based on the token.
 */
item_type get_type(struct token* token, parser_data_t * data, item_data* item);

#endif //IFJ_2023_PARSER_H
