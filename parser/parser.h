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


TData* init_data();

void free_data(TData *data);

int analyse();

int program(TData *data);

int stm(TData *data);

int stm_not_null(TData *data);

int condition(TData *data);

int func(TData *data);

int func_params(TData *data);

int func_params_not_null(TData *data);

int var_type(TData *data);

int return_rule(TData *data);

#endif //IFJ_2023_PARSER_H
