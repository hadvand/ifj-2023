//
// Created by dargo on 28.10.2023.
//

#ifndef IFJ_2023_PARSER_H
#define IFJ_2023_PARSER_H

#include "../scanner/scanner.h"
#include "hash.h"
#include <stdbool.h>


typedef struct {
    int line_cnt;               // line counter
    int param_index;            // counter of parameters in a function

    bool is_in_function;        // is in void or non-void function
    bool is_void_function;      // is in void function
    bool is_in_declaration;     // is in ID declaration
    bool is_in_condition;       // is in if/while/then construction
    bool eol_flag;              // met EOL

    HashTable *global_table;
    HashTable *local_table;

    token_t_ptr token_ptr;           // current token

    item_data *id;                   // current id that is being processed
    item_data *id_type;              // left side (variable)
    item_data *exp_type;             // right side (expression)
} parser_data_t;


parser_data_t *init_data();

void free_data(parser_data_t *data);

int analyse();

int program(parser_data_t *data);

int stm(parser_data_t *data);

int stm_not_null(parser_data_t *data);

int condition(parser_data_t *data);

int func(parser_data_t *data);

int func_params(parser_data_t *data);

int func_params_not_null(parser_data_t *data);

int var_type(parser_data_t *data);

int return_rule(parser_data_t *data);

int return_void_rule(parser_data_t *data);

int nil_flag(parser_data_t *data);

int call_params(parser_data_t *data);

int call_params_n(parser_data_t *data);

#endif //IFJ_2023_PARSER_H
