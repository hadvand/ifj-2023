#include "parser.h"

#define GET_TOKEN()                                                     \
    if ((data->token_ptr = next_token(&line_cnt, &ret_code)) == NULL) {\
        return ret_code;                                               \
    }   \

#define CHECK_RULE(_rule)           \
    if (ret_code = (_rule(data))) { \
        return ret_code;            \
    }                               \



TData* init_data()
{
    TData *parser_data;

    if((parser_data = (TData*) malloc(sizeof(struct Data))) == NULL) {
        //alloc failed
        return NULL;
    }

    HashTable *global_table = createHashTable();
    if(global_table == NULL) {
        parser_data->ret_code = ER_INTERNAL;
        return NULL;
    }

    HashTable *local_table = createHashTable();
    if(local_table == NULL) {
        err_type = ER_INTERNAL;
        return NULL;
    }

    parser_data->token_ptr = NULL;

    parser_data->global_table = global_table;
    parser_data->local_table = local_table;

    parser_data->is_in_function = false;
    parser_data->is_void_function = false;

    parser_data->ret_code = ER_NONE;
    parser_data->line_cnt = 0; // todo: macro

    return parser_data;
}

void free_data(TData *data) {
    // todo: func destroy_all_scopes
    // todo: free parser_data

    if (data) {
        free(data);
        data = NULL;
    }
}

int analyse() {
    error_t ret_code = ER_NONE;
    TData *parser_data;

    if((parser_data = init_data()) == NULL){
        return ER_INTERNAL;
    }

    ret_code = program(parser_data);

    free_data(parser_data);

    return ret_code;
}

int program(TData *data) {
    error_t ret_code = ER_NONE;
    GET_TOKEN()

    CHECK_RULE(stm);

    GET_TOKEN()
    else if (data->token_ptr->token_type != T_EOF) {
        return ER_SEMAN;
    }

    return ret_code;
}

int stm(TData *data) {
    error_t ret_code = ER_NONE;


    return ret_code;
}

int stm_not_null(TData *data) {
    error_t ret_code = ER_NONE;


    return ret_code;
}

int assignment_value(TData *data) {
    error_t ret_code = ER_NONE;


    return ret_code;
}

int condition(TData *data) {
    error_t ret_code = ER_NONE;


    return ret_code;
}

int func(TData *data) {
    error_t ret_code = ER_NONE;


    return ret_code;
}

int func_params(TData *data) {
    error_t ret_code = ER_NONE;


    return ret_code;
}

int func_params_not_null(TData *data) {
    error_t ret_code = ER_NONE;


    return ret_code;
}

int return_rule(TData *data) {
    error_t ret_code = ER_NONE;


    return ret_code;
}

int var_type(TData *data) {
    error_t ret_code = ER_NONE;

    GET_TOKEN()

    if (data->token_ptr->token_type == T_KEYWORD) {
        if (data->token_ptr->attribute.keyword == k_Double) {

        }
        else if (data->token_ptr->attribute.keyword == k_qmark_Double) {

        }
        else if (data->token_ptr->attribute.keyword == k_Int) {

        }
        else if (data->token_ptr->attribute.keyword == k_qmark_Int) {

        }
        else if (data->token_ptr->attribute.keyword == k_String) {

        }
        else if (data->token_ptr->attribute.keyword == k_qmark_String) {

        }
        else {
            return ER_SYNTAX;
        }


    }
}