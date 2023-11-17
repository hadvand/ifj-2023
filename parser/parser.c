#include "parser.h"

#define UNUSED(x) (void)(x)

#define GET_TOKEN()                                                     \
    if ((data->token_ptr = next_token(&(data->line_cnt),&ret_code)) == NULL) {\
        return ret_code;                                               \
    }   \

#define CHECK_RULE(rule)           \
    if ((ret_code = rule(data))) { \
        return ret_code;            \
    }                               \

#define IS_VALUE(token)												\
	(token).token_type == TOKEN_TYPE_DOUBLE_NUMBER						\
	|| (token).token_type == TOKEN_TYPE_INT_NUMBER						\
	|| (token).token_type == TOKEN_TYPE_INT_NUMBER						\
	|| (token).token_type == TOKEN_TYPE_STRING							\
	|| (token).token_type == TOKEN_TYPE_IDENTIFIER


#define CHECK_TYPE(_type)											\
	if (data->token_ptr.token_type != (_type)) return SYNTAX_ERR


#define CHECK_KEYWORD(_keyword)										\
	if (															\
		data->token_ptr.token_type != TOKEN_TYPE_KEYWORD						\
		|| data->token_ptr.attribute.keyword != (_keyword)				\
	) return SYNTAX_ERR


parser_data_t *init_data()
{
    parser_data_t *parser_data;
    // init parser data
    if((parser_data = (parser_data_t*) malloc(sizeof(parser_data_t))) == NULL) {
        return NULL;
    }

    // init global table
    HashTable *global_table = createHashTable();
    if(global_table == NULL) {
        return NULL;
    }
    parser_data->global_table = global_table;


    // init local table
    HashTable *local_table = createHashTable();
    if(local_table == NULL) {
        return NULL;
    }
    parser_data->local_table = local_table;

    parser_data->token_ptr = NULL;
    parser_data->id = NULL;
    parser_data->id_type = NULL;
    parser_data->exp_type = NULL;

    parser_data->is_in_function = false;
    parser_data->is_void_function = false;
    parser_data->is_in_declaration = false;
    parser_data->is_in_condition = false;

    parser_data->param_index = 0;
    parser_data->line_cnt = 0; // todo: macro

    // predefined functions

    item_data *tmp;

    // readString() -> str?
    tmp = insertSymbol(parser_data->global_table, "readString");
    tmp->defined = true;
    tmp->type = 's';
    tmp->qmark = true;

    // readInt() -> int?
    tmp = insertSymbol(parser_data->global_table, "readInt");
    tmp->defined = true;
    tmp->type = 'i';
    tmp->qmark = true;

    // readDouble() -> double?
    tmp = insertSymbol(parser_data->global_table, "readDouble");
    tmp->defined = true;
    tmp->type = 'd';
    tmp->qmark = true;

    // write(...)
    tmp = insertSymbol(parser_data->global_table, "write");
    tmp->defined = true;
    tmp->type = 'n';
    tmp->qmark = false;

    if (!string_append(*tmp->params, 'a')) {
        return NULL;
    }

    // Int2Double(int) -> double
    tmp = insertSymbol(parser_data->global_table, "Int2Double");
    tmp->defined = true;
    tmp->type = 'd';
    tmp->qmark = false;

    if (!string_append(*tmp->params, 'i')) {
        return NULL;
    }

    // Double2Int(double) -> int
    tmp = insertSymbol(parser_data->global_table, "Double2Int");
    tmp->defined = true;
    tmp->type = 'i';
    tmp->qmark = false;

    if (!string_append(*tmp->params, 'd')) {
        return NULL;
    }

    // ord(str) -> int
    tmp = insertSymbol(parser_data->global_table, "ord");
    tmp->defined = true;
    tmp->type = 'i';
    tmp->qmark = false;

    if (!string_append(*tmp->params, 's')) {
        return NULL;
    }

    // chr(int) -> str
    tmp = insertSymbol(parser_data->global_table, "chr");
    tmp->defined = true;
    tmp->type = 's';
    tmp->qmark = false;

    if (!string_append(*tmp->params, 'i')) {
        return NULL;
    }

    // length(str) -> int
    tmp = insertSymbol(parser_data->global_table, "length");
    tmp->defined = true;
    tmp->type = 'i';
    tmp->qmark = false;

    if (!string_append(*tmp->params, 's')) {
        return NULL;
    }

    // substring(str, int, int) -> str?
    tmp = insertSymbol(parser_data->global_table, "substring");
    tmp->defined = true;
    tmp->type = 's';
    tmp->qmark = true;

    if (!string_append(*tmp->params, 's')) {
        return NULL;
    }
    if (!string_append(*tmp->params, 'i')) {
        return NULL;
    }
    if (!string_append(*tmp->params, 'i')) {
        return NULL;
    }


    return parser_data;
}

void free_data(parser_data_t *parser_data) {
    destroyHashTable(parser_data->global_table);
    destroyHashTable(parser_data->local_table);
}

int analyse() {
    int ret_code = ER_NONE;

    string_ptr string;
    if ((string = string_init()) == NULL) return ER_INTERNAL;

    parser_data_t *parser_data;
    if ((parser_data = init_data()) == NULL)
    {
        string_free(string);
        return ER_INTERNAL;
    }

    if ((parser_data->token_ptr = next_token(&line_cnt, &ret_code)) != NULL)
    {
        ret_code = program(parser_data);
    }

    string_free(string);
    free_data(parser_data);

    return ret_code;
}


int program(parser_data_t *data) {
    int ret_code = ER_NONE;

    CHECK_RULE(stm)
    GET_TOKEN()

    else if (data->token_ptr->token_type != T_EOF) {
        return ER_SEMAN;
    }

    return ret_code;
}

int stm(parser_data_t *data) {
    int ret_code = ER_NONE;

    if ((data->token_ptr = next_token(&line_cnt, &ret_code)) == NULL) {
        return ret_code;
    }

    if (data->token_ptr->token_type == T_EOF) {}

    if (data->token_ptr->token_type == T_KEYWORD) {
        if (data->token_ptr->attribute.keyword == k_var || data->token_ptr->attribute.keyword == k_let) {
            stm_not_null(data);
        }
    }

    return ret_code;
}

int stm_not_null(parser_data_t *data) {
    int ret_code = ER_NONE;
    UNUSED(data);

    return ret_code;
}

int assignment_value(parser_data_t *data) {
    int ret_code = ER_NONE;
    UNUSED(data);

    return ret_code;
}

int condition(parser_data_t *data) {
    int ret_code = ER_NONE;
    UNUSED(data);

    return ret_code;
}

int func(parser_data_t *data) {
    int ret_code = ER_NONE;
    UNUSED(data);

    return ret_code;
}

int func_params(parser_data_t *data) {
    data->param_index = 0;

    // <params> -> ID AS <type> <param_n>
    if (data->token_ptr->token_type == T_KEYWORD_TYPE_ID)
    {
        // if there is function named as parameter
        if (findSymbol(data->global_table, data->token_ptr->attribute.string))
            return ER_UNDEF_VAR;

        // if we are in defintion, we need to add parameters to the local symbol table
        if (!data->is_in_declaration)
        {
            if (!(data->exp_type = insertSymbol(data->local_table, data->token_ptr->attribute.string)))
            {
                return ER_UNDEF_VAR;
            }
        }

        GET_TOKEN_AND_CHECK_KEYWORD(KEYWORD_AS);
        GET_TOKEN_AND_CHECK_RULE(var_type);
        GET_TOKEN_AND_CHECK_RULE(func_params_not_null);

        if (data->param_index + 1 != (*data->id->params)->last_index)
            return ER_UNDEF_VAR;
    }
    else if (!data->is_in_declaration && (*data->id->params)->last_index)
        return ER_UNDEF_VAR;

    // <params> -> ε

    return ER_NONE;
}

int func_params_not_null(parser_data_t *data) {
    int ret_code;

    if (data->token_ptr->token_type == T_COMMA)
    {
        data->param_index++;

        if (!data->is_in_declaration && data->param_index == (*data->id->params)->last_index)
            return ER_UNDEF_VAR;

        GET_TOKEN_AND_CHECK_TYPE(T_ID);

        if (!data->is_in_declaration)
        {
            bool internal_error;
            if (!(data->exp_type = insertSymbol(data->local_table, data->token_ptr->attribute.string)))
            {
                if (internal_error) return ER_INTERNAL;
                else return ER_UNDEF_VAR;
            }
        }

        GET_TOKEN_AND_CHECK_KEYWORD(KEYWORD_AS);
        GET_TOKEN_AND_CHECK_RULE(var_type);

        GET_TOKEN();
        return func_params_not_null(data);
    }

    // <param_n> -> ε

    return ER_NONE;
}

int return_rule(parser_data_t *data) {
    error_t ret_code = ER_NONE;
    UNUSED(data);

    return ret_code;
}

int var_type(parser_data_t* data) {
    if (data->token_ptr->token_type == T_KEYWORD)
    {
        switch (data->token_ptr->attribute.keyword)
        {
            case k_Int:
                if ((*data->id->params)->string[data->param_index] != 'i') return ER_UNDEF_VAR;

                if (!data->is_in_declaration) data->exp_type->type = 'i';
                break;

            case k_Double:
                if ((*data->id->params)->string[data->param_index] != 'd') return ER_UNDEF_VAR;

                if (!data->is_in_declaration) data->exp_type->type = 'd';
                break;

            case k_String:
                if ((*data->id->params)->string[data->param_index] != 's') return ER_UNDEF_VAR;

                if (!data->is_in_declaration) data->exp_type->type = 's';
                break;

            case k_qmark_Int:
                if ((*data->id->params)->string[data->param_index] != 's') return ER_UNDEF_VAR;

                if (!data->is_in_declaration) data->exp_type->type = 's';
                break;

            case k_qmark_Double:
                if ((*data->id->params)->string[data->param_index] != 's') return ER_UNDEF_VAR;

                if (!data->is_in_declaration) data->exp_type->type = 's';
                break;

            case k_qmark_String:
                if ((*data->id->params)->string[data->param_index] != 's') return ER_UNDEF_VAR;

                if (!data->is_in_declaration) data->exp_type->type = T_STRING;
                break;

            default:
                return ER_SYNTAX;
        }
    }
    else {
        return ER_SYNTAX;
    }
    return ER_NONE;
}
