#include "parser.h"

Data init_data() {
    Data *parser_data;

    if((parser_data = (Data*) malloc(sizeof(struct parser_data))) == NULL) {
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

void free_data(Data *data) {
    // todo: func destroy_all_scopes
    // todo: free parser_data

    if (data) {
        free(data);
        data = NULL;
    }
}

int analyse() {
    Data *parser_data;

    if((parser_data = init_data()) == NULL){
        return ER_INTERNAL;
    }

    ret_code = program(parser_data);

    free_data(parser_data);

    return ret_code;
}

int program(Data *data) {
    if ((data->token_ptr = next_token(&line_cnt, &ret_code)) == NULL) {
        return ret_code;
    }

    if (stm(data) != ER_NONE) {
        return ret_code;
    }

    if ((data->token_ptr = next_token(&line_cnt, &ret_code)) == NULL) {
        return ret_code;
    }
    else if (data->token_ptr->token_type != T_EOF) {
        return ER_SEMAN;
    }

    return ret_code;
}

int stm(Data *data) {
    error_t ret_code = ER_NONE;


    return ret_code;
}

int stm_not_null(Data *data) {
    error_t ret_code = ER_NONE;


    return ret_code;
}

int assignment_value(Data *data) {
    error_t ret_code = ER_NONE;


    return ret_code;
}

int condition(Data *data) {
    error_t ret_code = ER_NONE;


    return ret_code;
}

int func(Data *data) {
    error_t ret_code = ER_NONE;


    return ret_code;
}

int func_params(Data *data) {
    error_t ret_code = ER_NONE;


    return ret_code;
}

int func_params_not_null(Data *data) {
    error_t ret_code = ER_NONE;


    return ret_code;
}

int return_rule(Data *data) {
    error_t ret_code = ER_NONE;


    return ret_code;
}

int var_type(Data *data) {
    if ((data->token_ptr = next_token(&line_cnt, &ret_code)) == NULL) {
        return ret_code;
    }

    if (data->token_ptr->token_type == T_KEYWORD) {
        if (data->token_ptr->attribute == k_Double) {

        }
        else if (data->token_ptr->attribute == k_qmark_Double) {

        }
        else if (data->token_ptr->attribute == k_Int) {

        }
        else if (data->token_ptr->attribute == k_qmark_Int) {

        }
        else if (data->token_ptr->attribute == k_String) {

        }
        else if (data->token_ptr->attribute == k_qmark_String) {

        }
        else {
            return ER_SYNTAX;
        }


    }
}