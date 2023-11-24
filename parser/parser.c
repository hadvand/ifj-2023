#include "../structures/string.h"
#include "../structures/error.h"
#include "hash.h"
#include "parser.h"
#include "table_stack.h"
#include "../semantics/semantics.h"

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


#define INSERT_SYMBOL() \
        bool internal_error; \
        if(table_count_elements_in_stack(data->tableStack) == 0)\
            return ER_INTERNAL;\
        data->id = insertSymbol(data->tableStack->top->table,data->token_ptr->attribute.string,&internal_error);\
        if(!data->id){\
            if(internal_error) return ER_INTERNAL;\
        else return ER_UNDEF_VAR;\
}                       \


parser_data_t *init_data()
{

    parser_data_t *parser_data;
    // init parser data
    if((parser_data = (parser_data_t*) malloc(sizeof(parser_data_t))) == NULL) {
        return NULL;
    }


    t_table_stack *tableStack = table_stack_init();

    parser_data->tableStack = tableStack;

    // init global table
    HashTable *global_table = createHashTable();
    if(global_table == NULL) {
        return NULL;
    }
    table_stack_push(parser_data->tableStack,global_table);
    parser_data->tableStack->top->next = NULL;

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
    if(table_count_elements_in_stack(parser_data->tableStack) == 1){
        HashTable *global_table = parser_data->tableStack->top->table;
        if ((tmp = insertSymbol(global_table, "readString", &internal_error)) == NULL) return NULL;
        tmp->defined = true;
        tmp->type = IT_STRING;
        tmp->nil_possibility = true;
        tmp->is_function = true;

        // readInt() -> int?
        tmp = insertSymbol(global_table, "readInt", &internal_error);
        tmp->defined = true;
        tmp->type = IT_INT;
        tmp->nil_possibility = true;
        tmp->is_function = true;

        // readDouble() -> double?
        tmp = insertSymbol(global_table, "readDouble", &internal_error);
        tmp->defined = true;
        tmp->type = IT_DOUBLE;
        tmp->nil_possibility = true;
        tmp->is_function = true;

        // write(...)
        tmp = insertSymbol(global_table, "write", &internal_error);
        tmp->defined = true;
        tmp->type = IT_ANY;
        tmp->nil_possibility = false;
        tmp->is_function = true;
        if (!string_append(tmp->params, 'a')) {
            return NULL;
        }

        // print(...)
        tmp = insertSymbol(global_table, "print", &internal_error);
        tmp->defined = true;
        tmp->type = IT_ANY;
        tmp->nil_possibility = false;
        tmp->is_function = true;
        if (!string_append(tmp->params, 'a')) {
            return NULL;
        }

        // Int2Double(int) -> double
        tmp = insertSymbol(global_table, "Int2Double", &internal_error);
        tmp->defined = true;
        tmp->type = IT_DOUBLE;
        tmp->nil_possibility = false;
        tmp->is_function = true;
        if (!string_append(tmp->params, 'i')) {
            return NULL;
        }
        // Double2Int(double) -> int
        tmp = insertSymbol(global_table, "Double2Int", &internal_error);
        tmp->defined = true;
        tmp->type = IT_INT;
        tmp->nil_possibility = false;
        tmp->is_function = true;
        if (!string_append(tmp->params, 'd')) {
            return NULL;
        }

        // ord(str) -> int
        tmp = insertSymbol(global_table, "ord", &internal_error);
        tmp->defined = true;
        tmp->type = IT_INT;
        tmp->nil_possibility = false;
        tmp->is_function = true;

        if (!string_append(tmp->params, 's')) {
            return NULL;
        }

        // chr(int) -> str
        tmp = insertSymbol(global_table, "chr", &internal_error);
        tmp->defined = true;
        tmp->type = IT_STRING;
        tmp->nil_possibility = false;
        tmp->is_function = true;

        if (!string_append(tmp->params, 'i')) {
            return NULL;
        }

        // length(str) -> int
        tmp = insertSymbol(global_table, "length", &internal_error);
        tmp->defined = true;
        tmp->type = IT_INT;
        tmp->nil_possibility = false;
        tmp->is_function = true;

        if (!string_append(tmp->params, 's')) {
            return NULL;
        }

        // substring(str, int, int) -> str?
        tmp = insertSymbol(global_table, "substring", &internal_error);
        tmp->defined = true;
        tmp->type = IT_STRING;
        tmp->nil_possibility = true;
        tmp->is_function = true;

        if (!string_append(tmp->params, 's')) {
            return NULL;
        }
        if (!string_append(tmp->params, 'i')) {
            return NULL;
        }
        if (!string_append(tmp->params, 'i')) {
            return NULL;
        }
    }



    return parser_data;
}

void free_data(parser_data_t *parser_data) {
    table_stack_free(parser_data->tableStack);
}

int analyse() {

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

    while (data->token_ptr->token_type != T_EOF) {
        CHECK_RULE(stm)
    }

    return ret_code;
}

int stm(parser_data_t *data) {
    int ret_code = ER_NONE;

    // <stm> -> var + let id : <var_type> = <expression> \n <stm>
    // <stm> -> var + let id : <var_type> \n <stm>
    // <stm> -> var + let id = <expression> \n <stm>
    if (data->token_ptr->token_type == T_KEYWORD && (data->token_ptr->attribute.keyword == k_var || data->token_ptr->attribute.keyword == k_let)) {
        data->is_in_declaration = true;
        VERIFY_TOKEN(T_ID)
        INSERT_SYMBOL()
        GET_TOKEN()
        if (data->token_ptr->token_type == T_COLON) {
            GET_TOKEN()
            CHECK_RULE(var_type)

            GET_TOKEN()
            if (data->token_ptr->token_type == T_ASSIGMENT) {
                GET_TOKEN()
                CHECK_RULE(expression)
                data->is_in_declaration = false;
                // todo: there may be a problem with EOL
                return stm(data);
            }
            else if (data->eol_flag) {
                data->is_in_declaration = false;
                return stm(data);
            }
            else return ER_SYNTAX;
        }
        else if (data->token_ptr->token_type == T_ASSIGMENT) {
            GET_TOKEN()
            CHECK_RULE(expression)

            data->is_in_declaration = false;
            return stm(data);
        }
        else return ER_SYNTAX;

        GET_TOKEN()
    }

    // <stm> -> func_id( <func_params> ) \n <stm>
    // <stm> -> id = <expression> \n <stm>
    if (data->token_ptr->token_type == T_ID) {
        if(table_count_elements_in_stack(data->tableStack) == 0)
            return ER_INTERNAL;
        Symbol *idFromTable = NULL;
        if((idFromTable = findSymbol(data->tableStack->top->table,data->token_ptr->attribute.string))== NULL)
            return ER_UNDEF_VAR;
        data->id = &(idFromTable->data);
        GET_TOKEN()
        if (data->token_ptr->token_type == T_BRACKET_OPEN) {

            data->param_index = 0;
            CHECK_RULE(call_params)

            if (data->token_ptr->token_type != T_BRACKET_CLOSE) return ER_SYNTAX;

            GET_TOKEN()
//            if (!data->eol_flag) return ER_SYNTAX;

            return stm(data);
        }
        else if (data->token_ptr->token_type == T_ASSIGMENT) {
            GET_TOKEN()
            CHECK_RULE(expression)

            if (!data->eol_flag && data->token_ptr->token_type != T_EOF) return ER_SYNTAX;

            return stm(data);
        }
    }

    // <stm> -> func func_id( <func_params> ) -> <var_type> { <stm> <return> } \n <stm>
    // <stm> -> func func_id( <func_params> ) { <stm> <return_void> } \n <stm>
    if (data->token_ptr->token_type == T_KEYWORD && data->token_ptr->attribute.keyword == k_func) {
        VERIFY_TOKEN(T_ID)
        data->is_in_declaration = true;

        INSERT_SYMBOL()
        VERIFY_TOKEN(T_BRACKET_OPEN)
        data->is_in_params = true;
        data->param_index = 0;
        data->id->is_function = true;
        data->id->id_names = NULL;
        data->is_in_function = true;
        HashTable *local_table = createHashTable();
        table_stack_push(data->tableStack,local_table);
        CHECK_RULE(func_params)
        data->is_in_params = false;

        if (data->token_ptr->token_type != T_BRACKET_CLOSE) return ER_SYNTAX;

        GET_TOKEN()
        if (data->token_ptr->token_type == T_ARROW) {

            data->is_void_function = false;

            GET_TOKEN()
            CHECK_RULE(var_type)
            data->is_in_function = false;

            VERIFY_TOKEN(T_CURVED_BRACKET_OPEN)

            GET_TOKEN()
            CHECK_RULE(stm)

//            if (data->is_void_function) {
//                CHECK_RULE(return_void_rule)
//            }
//            else {
//                CHECK_RULE(return_rule)
//            }

            if (data->token_ptr->token_type != T_CURVED_BRACKET_CLOSE) return ER_SYNTAX;
            table_stack_pop(data->tableStack);

            GET_TOKEN()

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
            table_stack_pop(data->tableStack);

            GET_TOKEN()
//            if (!data->eol_flag) return ER_SYNTAX;

            return stm(data);
        }
        else return ER_SYNTAX;
    }

    // <stm> -> if ( <condition> ) { <stm> } \n else { <stm> } \n <stm>
    // <stm> -> if ( <condition> ) { <stm> } \n <stm>
    if (data->token_ptr->token_type == T_KEYWORD && data->token_ptr->attribute.keyword == k_if) {
        data->is_in_condition = true;

        GET_TOKEN()
        CHECK_RULE(condition)

        if (data->token_ptr->token_type != T_CURVED_BRACKET_OPEN) return ER_SYNTAX;

        GET_TOKEN()
        CHECK_RULE(stm)

        if (data->token_ptr->token_type != T_CURVED_BRACKET_CLOSE) return ER_SYNTAX;

        GET_TOKEN()

        if (data->token_ptr->token_type == T_KEYWORD && data->token_ptr->attribute.keyword == k_else) {
            VERIFY_TOKEN(T_CURVED_BRACKET_OPEN)

            GET_TOKEN()
            CHECK_RULE(stm)

            if (data->token_ptr->token_type != T_CURVED_BRACKET_CLOSE) return ER_SYNTAX;

            GET_TOKEN()
//            if (!data->eol_flag) return ER_SYNTAX;

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

        GET_TOKEN()
        CHECK_RULE(condition)

        if (data->token_ptr->token_type != T_CURVED_BRACKET_OPEN) return ER_SYNTAX;

        GET_TOKEN()
        CHECK_RULE(stm)

        if (data->token_ptr->token_type != T_CURVED_BRACKET_CLOSE) return ER_SYNTAX;

        GET_TOKEN()
//        if (!data->eol_flag) return ER_SYNTAX;

        return stm(data);
    }
    //return ....
    if (data->token_ptr->token_type == T_KEYWORD && data->token_ptr->attribute.keyword == k_return) {
        if (data->is_void_function) {
            CHECK_RULE(return_void_rule)
        }
        else {
            CHECK_RULE(return_rule)
        }
        return ER_NONE;
    }
    // <statement> -> ε

    return ret_code;
}

//<call_params> -> var_name : var_id <call_params_n>
//<call_params> -> var_id <call_params_n>
int call_params(parser_data_t *data) {
    int ret_code = ER_NONE;

    if((ret_code = check_func_call(data,data->param_index))){
        return ret_code;
    }

    CHECK_RULE(call_params_n)

    return ret_code;
}

//<call_params> -> var_name : var_id <call_params_n>
//<call_params> -> var_id <call_params_n>
int call_params_n(parser_data_t *data) {
    int ret_code;

    if (data->token_ptr->token_type == T_BRACKET_CLOSE) {
        GET_TOKEN()
        return ER_NONE;
    }

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

    if (data->token_ptr->token_type == T_KEYWORD && data->token_ptr->attribute.keyword == k_let) {
        VERIFY_TOKEN(T_ID)
        if(table_count_elements_in_stack(data->tableStack) == 0)
            return ER_INTERNAL;
        if (!findSymbol(data->tableStack->top->table, data->token_ptr->attribute.string)) {
            return ER_UNDEF_VAR;
        }
        GET_TOKEN()
        return ret_code;
    }
    item_data tmp_data;
    tmp_data.type = IT_INT;
    data->id = &tmp_data;
    CHECK_RULE(expression)

    if (data->token_ptr->token_type == T_KEYWORD && data->token_ptr->attribute.keyword == k_let) {
        VERIFY_TOKEN(T_ID)
    }

    return ret_code;
}

//<func_params> -> ε
//<func_params> -> var_name var_id: <var_type> <func_params_not_null>
//<func_params> -> _ var_id: <var_type> <func_params_not_null>
int func_params(parser_data_t *data) {
    int ret_code = ER_NONE;


    if ((data->id->id_names = (char**)realloc(data->id->id_names,data->param_index+1 * sizeof(char*)))==NULL)
        return ER_INTERNAL;
    GET_TOKEN()

    //T_ID its var_name. it is NOOOOT a var_id
    if(data->token_ptr->token_type == T_UNDERLINE || data->token_ptr->token_type == T_ID){
        if((data->id->id_names[data->param_index] = (char*)realloc(data->id->id_names[data->param_index],strlen(data->token_ptr->attribute.string))) == NULL)
            return ER_INTERNAL;
        if(data->token_ptr->token_type == T_UNDERLINE)
            strcpy(data->id->id_names[data->param_index],"_");
        else
            strcpy(data->id->id_names[data->param_index],data->token_ptr->attribute.string);

        VERIFY_TOKEN(T_ID)

        // if there is function named as parameter
        if(table_count_elements_in_stack(data->tableStack) != 2)
            return ER_INTERNAL;
        if (findSymbol(data->tableStack->top->table, data->token_ptr->attribute.string))
            return ER_UNDEF_VAR;

        // if we are in definition, we need to add parameters to the local symtable
        bool internal_error;
        if(table_count_elements_in_stack(data->tableStack) == 0)
            return ER_INTERNAL;
        if (!(data->exp_type = insertSymbol(data->tableStack->top->table, data->token_ptr->attribute.string, &internal_error))) {
            if (internal_error) return ER_INTERNAL;
            else return ER_UNDEF_VAR;
        }

        VERIFY_TOKEN(T_COLON)

        GET_TOKEN()
        CHECK_RULE(var_type)


#ifdef PARS_DEBUG
    for(int i = 0; i < data->id->params->last_index;i++){
            printf("string in data->id->id_names %s, position: %d\n",data->id->id_names[i],i);
        }
#endif

        CHECK_RULE(func_params_not_null)
    }
    else {
        return ER_NONE;
    }

    // <func_params> -> ε

    return ER_NONE;
}

// <func_params_not_null> -> , <func_params>
// <func_params_not_null> -> ε
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
    else if (data->token_ptr->token_type == T_CURVED_BRACKET_CLOSE) {
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

item_type get_type(struct token* token, parser_data_t * data){

    Symbol* symbol;

    switch(token -> token_type){
        case T_ID:
            if(table_count_elements_in_stack(data->tableStack) == 0)
                return IT_UNDEF;
            symbol = findSymbol(data->tableStack->top->table, token->attribute.string);
            if (symbol == NULL)
                return IT_UNDEF;
            data->id_type = &(symbol->data);
            return symbol->data.type;
        case T_INT:
            return IT_INT;
        case T_STRING:
            return IT_STRING;
        case T_DEMICAL:
            return IT_DOUBLE;
        default:
            switch (token->attribute.keyword) {
                case k_Int:
                case k_qmark_Int:
                    return IT_INT;
                case k_String:
                case k_qmark_String:
                    return IT_STRING;
                case k_Double:
                case k_qmark_Double:
                    return IT_DOUBLE;
                case k_nil:
                    return IT_NIL;
                default:
                    return IT_UNDEF;
            }
            return IT_UNDEF;
    }
}

int insert_data_type(parser_data_t *data){
    item_type type  = get_type(data->token_ptr,data);

    //var declaration
    if(data->is_in_declaration && !data->is_in_function && !data->is_in_params){
        data->id->type = type;
        switch (data->token_ptr->token_type) {
            case T_KEYWORD_NIL_POSSIBILITY:
                data->id->nil_possibility = true;
            default:
                break;
        }
    }
    //return func type
    else if(data->is_in_declaration && data->is_in_function && !data->is_in_params){
        data->id->type = type;
    }
    //func params
    else if(data->is_in_declaration && data->is_in_function && data->is_in_params) {
        char data_type_for_func_params = 'a';
        switch (type) {
            case IT_INT:
                data_type_for_func_params = 'i' - ((data->token_ptr->token_type == T_KEYWORD_NIL_POSSIBILITY) * 32);
                break;
            case IT_STRING:
                data_type_for_func_params = 's' - ((data->token_ptr->token_type == T_KEYWORD_NIL_POSSIBILITY) * 32);
                break;
            case IT_DOUBLE:
                data_type_for_func_params = 'd' - ((data->token_ptr->token_type == T_KEYWORD_NIL_POSSIBILITY) * 32);
                break;
            default:
                return ER_INTERNAL;
        }
        if (!string_append(data->id->params, data_type_for_func_params)) {
            return ER_INTERNAL;
        }
        data->exp_type->type = type;
    }

    return ER_NONE;
}

int var_type(parser_data_t* data) {
    if (data->token_ptr->token_type == T_KEYWORD || data->token_ptr->token_type == T_KEYWORD_NIL_POSSIBILITY)
    {
        insert_data_type(data);
    }
    else {
        return ER_SYNTAX;
    }
    return ER_NONE;
}
