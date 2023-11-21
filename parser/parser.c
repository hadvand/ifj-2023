#include "../structures/string.h"
#include "../structures/error.h"
#include "hash.h"
#include "../semantics/semantics.h"
#include "parser.h"

#define UNUSED(x) (void)(x)

#define GET_TOKEN() \
        if ((data->token_ptr = next_token(&(data->line_cnt), &ret_code, &(data->eol_flag))) == NULL) {\
            return ret_code;                                               \
        }           \

#define CHECK_RULE(_rule)           \
    if ((ret_code = _rule(data))) { \
        return ret_code;            \
    }                              \

#define VERIFY_TOKEN(t_token)  \
    GET_TOKEN()                \
    if (data->token_ptr->token_type != t_token) return ER_SYNTAX;\
    \

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
    parser_data->eol_flag = false;

    parser_data->param_index = 0;
    parser_data->line_cnt = 0;

    // predefined functions

    item_data *tmp;
    bool internal_error;

    // readString() -> str?

    if ((tmp = insertSymbol(parser_data->global_table, "readString", &internal_error)) == NULL) return NULL;
    tmp->defined = true;
    tmp->type = IT_STRING;
    tmp->nil_possibility = true;

    // readInt() -> int?
    tmp = insertSymbol(parser_data->global_table, "readInt", &internal_error);
    tmp->defined = true;
    tmp->type = IT_INT;
    tmp->nil_possibility = true;

    // readDouble() -> double?
    tmp = insertSymbol(parser_data->global_table, "readDouble", &internal_error);
    tmp->defined = true;
    tmp->type = IT_DOUBLE;
    tmp->nil_possibility = true;

    // write(...)
    tmp = insertSymbol(parser_data->global_table, "write", &internal_error);
    tmp->defined = true;
    tmp->type = IT_ANY;
    tmp->nil_possibility = false;

    if (!string_append(tmp->params, 'a')) {
        return NULL;
    }

    // Int2Double(int) -> double
    tmp = insertSymbol(parser_data->global_table, "Int2Double", &internal_error);
    tmp->defined = true;
    tmp->type = IT_DOUBLE;
    tmp->nil_possibility = false;

    if (!string_append(tmp->params, 'i')) {
        return NULL;
    }

    // Double2Int(double) -> int
    tmp = insertSymbol(parser_data->global_table, "Double2Int", &internal_error);
    tmp->defined = true;
    tmp->type = IT_INT;
    tmp->nil_possibility = false;

    if (!string_append(tmp->params, 'd')) {
        return NULL;
    }

    // ord(str) -> int
    tmp = insertSymbol(parser_data->global_table, "ord", &internal_error);
    tmp->defined = true;
    tmp->type = IT_INT;
    tmp->nil_possibility = false;

    if (!string_append(tmp->params, 's')) {
        return NULL;
    }

    // chr(int) -> str
    tmp = insertSymbol(parser_data->global_table, "chr", &internal_error);
    tmp->defined = true;
    tmp->type = IT_STRING;
    tmp->nil_possibility = false;

    if (!string_append(tmp->params, 'i')) {
        return NULL;
    }

    // length(str) -> int
    tmp = insertSymbol(parser_data->global_table, "length", &internal_error);
    tmp->defined = true;
    tmp->type = IT_INT;
    tmp->nil_possibility = false;

    if (!string_append(tmp->params, 's')) {
        return NULL;
    }

    // substring(str, int, int) -> str?
    tmp = insertSymbol(parser_data->global_table, "substring", &internal_error);
    tmp->defined = true;
    tmp->type = IT_STRING;
    tmp->nil_possibility = true;

    if (!string_append(tmp->params, 's')) {
        return NULL;
    }
    if (!string_append(tmp->params, 'i')) {
        return NULL;
    }
    if (!string_append(tmp->params, 'i')) {
        return NULL;
    }


    return parser_data;
}

void free_data(parser_data_t *parser_data) {
    destroyHashTable(parser_data->global_table);
    destroyHashTable(parser_data->local_table);
}

int analyse() {

#ifdef PARS_DEBUG
    printf("Sample of debug");
#endif

    int ret_code = ER_NONE;
    bool flag = false;

    string_ptr string;
    if ((string = string_init()) == NULL) return ER_INTERNAL;

    parser_data_t *parser_data;
    if ((parser_data = init_data()) == NULL)
    {
        string_free(string);
        return ER_INTERNAL;
    }

    parser_data->line_cnt = 1;

    if ((parser_data->token_ptr = next_token(&(parser_data->line_cnt), &ret_code, &flag)) != NULL)
    {
        ret_code = program(parser_data);
    }

    string_free(string);
    free_data(parser_data);

    return ret_code;
}

// <program> -> <stm> EOF
int program(parser_data_t *data) {
    int ret_code = ER_NONE;

    new_stm:
    CHECK_RULE(stm)

    if (data->token_ptr->token_type != T_EOF) goto new_stm;

    return ret_code;
}

int stm(parser_data_t *data) {
    int ret_code = ER_NONE;

    // <stm> -> var + let id : <var_type> = <expression> \n <stm>
    // <stm> -> var + let id : <var_type> \n <stm>
    // <stm> -> var + let id = <expression> \n <stm>
    if (data->token_ptr->token_type == T_KEYWORD && (data->token_ptr->attribute.keyword == k_var || data->token_ptr->attribute.keyword == k_let)) {
        VERIFY_TOKEN(T_ID)

        GET_TOKEN()
        if (data->token_ptr->token_type == T_COLON) {
            GET_TOKEN()
            CHECK_RULE(var_type)

            GET_TOKEN()
            if (data->token_ptr->token_type == T_ASSIGMENT) {
                GET_TOKEN()
                CHECK_RULE(expression)
                // todo: there may be a problem with EOL
                return stm(data);
            }
            else if (data->eol_flag) {
                return stm(data);
            }
            else return ER_SYNTAX;
        }
        else if (data->token_ptr->token_type == T_ASSIGMENT) {
            GET_TOKEN()
            CHECK_RULE(expression)

            return stm(data);
        }
        else return ER_SYNTAX;

        GET_TOKEN()
    }

    // <stm> -> func_id( <func_params> ) \n <stm>
    // <stm> -> id = <expression> \n <stm>
    if (data->token_ptr->token_type == T_ID) {
        GET_TOKEN()
        if (data->token_ptr->token_type == T_BRACKET_OPEN) {
            GET_TOKEN()
            CHECK_RULE(call_params)

            VERIFY_TOKEN(T_BRACKET_CLOSE)

            GET_TOKEN()
            if (!data->eol_flag) return ER_SYNTAX;

            return stm(data);
        }
        else if (data->token_ptr->token_type == T_ASSIGMENT) {
            GET_TOKEN()
            CHECK_RULE(expression)

            if (!data->eol_flag) return ER_SYNTAX;

            return stm(data);
        }
    }

    // <stm> -> func func_id( <func_params> ) -> <var_type> { <stm> <return> } \n <stm>
    // <stm> -> func func_id( <func_params> ) { <stm> <return_void> } \n <stm>
    if (data->token_ptr->token_type == T_KEYWORD && data->token_ptr->attribute.keyword == k_func) {

        VERIFY_TOKEN(T_ID)
        data->is_in_declaration = true;



        bool internal_error;
        data->id = insertSymbol(data->global_table,data->token_ptr->attribute.string,&internal_error);
        if(!data->id){
            if(internal_error) return ER_INTERNAL;
            else return ER_UNDEF_VAR;
        }
        VERIFY_TOKEN(T_BRACKET_OPEN)

        CHECK_RULE(func_params)

        if (data->token_ptr->token_type != T_BRACKET_CLOSE) return ER_SYNTAX;

        GET_TOKEN()
        if (data->token_ptr->token_type == T_ARROW) {
            data->is_void_function = false;

            GET_TOKEN()
            CHECK_RULE(var_type)

            VERIFY_TOKEN(T_CURVED_BRACKET_OPEN)

            GET_TOKEN()
            CHECK_RULE(stm)

            if (data->is_void_function) {
                CHECK_RULE(return_void_rule)
            }
            else {
                CHECK_RULE(return_rule)
            }

            if (data->token_ptr->token_type != T_CURVED_BRACKET_CLOSE) return ER_SYNTAX;

            GET_TOKEN()
            if (!data->eol_flag) return ER_SYNTAX;

            return stm(data);

        }
        else if (data->token_ptr->token_type == T_CURVED_BRACKET_OPEN) {
            data->is_void_function = true;

            GET_TOKEN()
            CHECK_RULE(stm)

            if (data->is_void_function) {
                CHECK_RULE(return_void_rule)
            }
            else {
                CHECK_RULE(return_rule)
            }

            VERIFY_TOKEN(T_CURVED_BRACKET_CLOSE)

            GET_TOKEN()
            if (!data->eol_flag) return ER_SYNTAX;

            return stm(data);
        }
        else return ER_SYNTAX;
    }

    // <stm> -> if ( <condition> ) { <stm> } \n else { <stm> } \n <stm>
    // <stm> -> if ( <condition> ) { <stm> } \n <stm>
    if (data->token_ptr->token_type == T_KEYWORD && data->token_ptr->attribute.keyword == k_if) {
        data->is_in_condition = true;

        CHECK_RULE(condition)

        VERIFY_TOKEN(T_BRACKET_CLOSE)

        VERIFY_TOKEN(T_BRACKET_OPEN)

        GET_TOKEN()
        CHECK_RULE(stm)

        VERIFY_TOKEN(T_CURVED_BRACKET_CLOSE)

        GET_TOKEN()
        if (!data->eol_flag) return ER_SYNTAX;

        if (data->token_ptr->token_type == T_KEYWORD && data->token_ptr->attribute.keyword == k_else) {
            VERIFY_TOKEN(T_CURVED_BRACKET_OPEN)

            GET_TOKEN()
            CHECK_RULE(stm)

            VERIFY_TOKEN(T_CURVED_BRACKET_CLOSE)

            GET_TOKEN()
            if (!data->eol_flag) return ER_SYNTAX;

            return stm(data);
        }
        else {
            GET_TOKEN()
            return stm(data);
        }
    }

    // <stm> -> while ( <condition> ) { <stm> } \n <stm>
    if (data->token_ptr->token_type == T_KEYWORD && data->token_ptr->attribute.keyword == k_while) {
        data->is_in_condition = true;

        VERIFY_TOKEN(T_BRACKET_OPEN)

        GET_TOKEN()
        CHECK_RULE(condition)

        VERIFY_TOKEN(T_BRACKET_CLOSE)

        VERIFY_TOKEN(T_CURVED_BRACKET_OPEN)

        GET_TOKEN()
        CHECK_RULE(stm)

        VERIFY_TOKEN(T_CURVED_BRACKET_CLOSE)

        GET_TOKEN()
        if (!data->eol_flag) return ER_SYNTAX;

        return stm(data);
    }
    // <statement> -> ε

    return ret_code;
}

//<call_params> -> var_name : var_id <call_params_n>
//<call_params> -> var_id <call_params_n>
int call_params(parser_data_t *data) {
    int ret_code = ER_NONE;

    // todo: its not ID, its a NAME
    VERIFY_TOKEN(T_ID)

    GET_TOKEN()
    if (data->token_ptr->token_type == T_COLON) {
        if (!findSymbol(data->local_table, data->token_ptr->attribute.string)) return ER_UNDEF_VAR;
        else {
            CHECK_RULE(call_params_n)
        }
    }
    else if (data->token_ptr->token_type == T_ID) {
        CHECK_RULE(call_params_n)
    }
    else return ER_SYNTAX;

    return ER_NONE;
}

//<call_params> -> var_name : var_id <call_params_n>
//<call_params> -> var_id <call_params_n>
int call_params_n(parser_data_t *data) {
    int ret_code;

    GET_TOKEN()
    if (data->token_ptr->token_type == T_COMMA)
    {
        data->param_index++;

        CHECK_RULE(call_params)
    }
    else if (data->token_ptr->token_type == T_BRACKET_CLOSE) {
        return ER_NONE;
    }
    else {
        return ER_SYNTAX;
    }

    return ER_NONE;
}

//<condition> -> let id
//<condition> -> <expression>
int condition(parser_data_t *data) {
    int ret_code = ER_NONE;

    GET_TOKEN()
    if (data->token_ptr->token_type == T_KEYWORD && data->token_ptr->attribute.keyword == k_let) {
        VERIFY_TOKEN(T_ID)
    }
    else CHECK_RULE(expression)

    return ret_code;
}

//<func_params> -> ε
//<func_params> -> var_name var_id: <var_type> <func_params_not_null>
//<func_params> -> _ var_id: <var_type> <func_params_not_null>
int func_params(parser_data_t *data) {
    int ret_code = ER_NONE;

    data->param_index = 0;

    if ((data->id->id_names = (char**)malloc(sizeof(char*)))==NULL)
        return ER_INTERNAL;
    GET_TOKEN()

    //T_ID its var_name. it is NOOOOT a var_id
    if(data->token_ptr->token_type == T_UNDERLINE || data->token_ptr->token_type == T_ID){

        // if there is function named as parameter
        if (findSymbol(data->global_table, data->token_ptr->attribute.string))
            return ER_UNDEF_VAR;

        if((data->id->id_names[data->param_index] = (char*)realloc(data->id->id_names[data->param_index],strlen(data->token_ptr->attribute.string))) == NULL)
            return ER_INTERNAL;
        if(data->token_ptr->token_type == T_UNDERLINE)
            strcpy(data->id->id_names[data->param_index],"_");
        else
            strcpy(data->id->id_names[data->param_index],data->token_ptr->attribute.string);

        VERIFY_TOKEN(T_ID)

        // if there is function named as parameter
        if (findSymbol(data->global_table, data->token_ptr->attribute.string))
            return ER_UNDEF_VAR;

        // if we are in definition, we need to add parameters to the local symtable
        bool internal_error;
        if (!(data->exp_type = insertSymbol(data->local_table, data->token_ptr->attribute.string, &internal_error))) {
            if (internal_error) return ER_INTERNAL;
            else return ER_UNDEF_VAR;
        }

        VERIFY_TOKEN(T_COLON)

        GET_TOKEN()
        CHECK_RULE(var_type)

        CHECK_RULE(func_params_not_null)
    }
    else {
        return ER_NONE;
    }

    // h?

    if (data->token_ptr->token_type == T_ID) {
        if (data->param_index + 1 != data->id->params->last_index) return ER_UNDEF_VAR;
    }
    else if (!data->is_in_declaration && data->id->params->last_index) return ER_UNDEF_VAR;

    // <func_params> -> ε

    return ER_NONE;
}

int func_params_not_null(parser_data_t *data) {
    int ret_code;

    GET_TOKEN()
    if (data->token_ptr->token_type == T_COMMA)
    {
        data->param_index++;

        CHECK_RULE(func_params)
    }
    else if (data->token_ptr->token_type == T_BRACKET_CLOSE) {
        return ER_NONE;
    }
    else {
        return ER_SYNTAX;
    }

    return ER_NONE;
}

// <nil_flag> -> ! + ε
int nil_flag(parser_data_t *data) {
    if (data->token_ptr->token_type == T_EXCLAMATION_MARK) {
        return ER_NONE;
    }
    else if (data->eol_flag) {
        return ER_NONE;
    }
    else return ER_SYNTAX;
}

// <return> -> return <expression> <nil_flag>
int return_rule(parser_data_t *data) {
    int ret_code;

    if (!(data->token_ptr->token_type == T_KEYWORD && data->token_ptr->attribute.keyword == k_return)) return ER_SYNTAX;
    GET_TOKEN()

    CHECK_RULE(expression)

    return nil_flag(data);
}

// <return_void> -> return
// <return_void> -> ε
int return_void_rule(parser_data_t *data) {
    if (data->token_ptr->token_type == T_KEYWORD && data->token_ptr->attribute.keyword == k_return) {
        return ER_NONE;
    }
    else if (data->eol_flag) {
        return ER_NONE;
    }
    else {
        return ER_SYNTAX;
    }
}

int var_type(parser_data_t* data) {
    if (data->token_ptr->token_type == T_KEYWORD)
    {
        switch (data->token_ptr->attribute.keyword)
        {
            case k_Int:
                if (!data->is_in_declaration && (data->id->params->string[data->param_index] != 'i')) return ER_UNDEF_VAR;

                if (!data->is_in_declaration) data->exp_type->type = IT_INT;
                break;

            case k_Double:
                if (!data->is_in_declaration && (data->id->params->string[data->param_index] != 'd')) return ER_UNDEF_VAR;

                if (!data->is_in_declaration) data->exp_type->type = IT_DOUBLE;
                break;

            case k_String:
                if (!data->is_in_declaration && (data->id->params->string[data->param_index] != 's')) return ER_UNDEF_VAR;

                if (!data->is_in_declaration) data->exp_type->type = IT_STRING;
                break;
            //TODO nil_possibility = true;
            case k_qmark_Int:
                if (!data->is_in_declaration && (data->id->params->string[data->param_index] != 'i')) return ER_UNDEF_VAR;

                if (!data->is_in_declaration) data->exp_type->type = IT_STRING;
                break;
                //TODO nil_possibility = true;
            case k_qmark_Double:
                if (!data->is_in_declaration && (data->id->params->string[data->param_index] != 'd')) return ER_UNDEF_VAR;

                if (!data->is_in_declaration) data->exp_type->type = IT_STRING;
                break;
                //TODO nil_possibility = true;
            case k_qmark_String:
                if (!data->is_in_declaration && (data->id->params->string[data->param_index] != 's')) return ER_UNDEF_VAR;

                if (!data->is_in_declaration) data->exp_type->type = IT_STRING;
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

int var_value(parser_data_t *data) {
    if (data->token_ptr->token_type == T_INT) {
        if (data->id->type != IT_INT) return ER_SEMAN;
    }
    else if (data->token_ptr->token_type == T_DEMICAL) {
        if (data->id->type != IT_DOUBLE) return ER_SEMAN;
    }
    else if (data->token_ptr->token_type == T_STRING) {
        if (data->id->type != IT_STRING) return ER_SEMAN;
    }
    else return ER_SYNTAX;

    return ER_NONE;
}
