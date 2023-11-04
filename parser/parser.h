//
// Created by dargo on 28.10.2023.
//

#ifndef IFJ_2023_PARSER_H
#define IFJ_2023_PARSER_H

#include "../scanner/scanner.h"
#include "hash.h"

typedef struct Data{
    // stack for local (maybe add and delete local variables if needed)
    int line_cnt;
    error_t ret_code;

    bool is_in_function;
    bool is_void_function;

    HashTable *global_table;
    HashTable *local_table;

    token_t_ptr token_ptr;
}TData;


bool init_data();

bool free_data(Data *data);

int analyse()

int program(Data *data);

int stm(Data *data);

int stm_not_null(Data *data);

int condition(Data *data);

int func(Data *data);

int func_params(Data *data);

int func_params_not_null(Data *data);

int var_type(Data *data);

int return_rule(Data *data);

#endif //IFJ_2023_PARSER_H
