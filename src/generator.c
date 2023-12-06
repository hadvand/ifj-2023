/**
 * @file generator.c
 * @author Neonila Mashlai (xmashl00)
 * @author Oleg Borshch (xborsh00)
 * @brief code generation
 */

#include "generator.h"
#include <stdbool.h>
#include "string.h"


#define GENERATE_CODE(...) fprintf(stdout, __VA_ARGS__)

#define EMIT(_text)\
        if (!string_concat(code, (_text))) {\
            return false; } else {}\
        codegen_flush();

#define EMIT_NL(_text)\
            EMIT(_text"\n");

#define EMIT_INT(_number) do {\
            char _str[MAX];\
            sprintf(_str, "%d", (_number));\
            EMIT(_str);\
        } while (0)

#define MAX 64

string_ptr code;

void generator_builtin(void){
    fprintf(stdout, "%s", BUILTIN_LENGTH);
    fprintf(stdout, "%s", BUILTIN_SUBSTR);
    fprintf(stdout, "%s", BUILTIN_INT2DOUBLE);
    fprintf(stdout, "%s", BUILTIN_DOUBLE2INT);
    fprintf(stdout, "%s", BUILTIN_READSTRING);
    fprintf(stdout, "%s", BUILTIN_READINT);
    fprintf(stdout, "%s", BUILTIN_READDOUBLE);
    fprintf(stdout, "%s", BUILTIN_WRITE);
    fprintf(stdout, "%s", BUILTIN_ORD);
    fprintf(stdout, "%s", BUILTIN_CHR);
}

void generator_start(void){
    code = string_init();
    GENERATE_CODE(".IFJcode23\n");
    GENERATE_CODE("JUMP $$main\n");
    GENERATE_CODE("\nLABEL $$main\n");
    GENERATE_CODE("CREATEFRAME\n");
    GENERATE_CODE("PUSHFRAME\n");
}

void generator_end(void){
    GENERATE_CODE("POPFRAME\n");
    GENERATE_CODE("CLEARS\n");
}

bool main_scope_start()
{
    GENERATE_CODE("\n# Main scope starts");

    GENERATE_CODE("LABEL $$main");
    GENERATE_CODE("CREATEFRAME");
    GENERATE_CODE("PUSHFRAME");

    return true;
}


bool main_scope_end()
{
    GENERATE_CODE("# Main scope ends ");

    GENERATE_CODE("POPFRAME");
    GENERATE_CODE("CLEARS");

    return true;
}


void generate_var_declaration(item_data data){
    if(data.global){
        GENERATE_CODE("DEFVAR GF@%s\n", data.id);
    }else{
        GENERATE_CODE("DEFVAR LF@%s\n", data.id);
    }
}

void generate_var_definition(item_data data){
    if(data.global){
        GENERATE_CODE("MOVE GF@%s ", data.id);
    }else{
        GENERATE_CODE("MOVE LF@%s ", data.id);
    }
    switch(data.type){
        case IT_DOUBLE:
            GENERATE_CODE("float@0.0\n");
            break;
        case IT_INT:
            GENERATE_CODE("int@0\n");
            break;
        case IT_STRING:
            GENERATE_CODE("string@\n");
            break;
        case IT_NIL:
            GENERATE_CODE("nil@nil\n");
            break;
        default:
            break;
    }
}

bool gen_function_before_params()
{
    EMIT_NL("CREATEFRAME")

    return true;
}

bool gen_function_pass_param_push(token_t_ptr token, bool local_frame) {
    EMIT("PUSHS ");
    if (!gen_value_from_token(token, local_frame)) return false;
    EMIT("\n");

    return true;
}

bool gen_value_from_token(token_t_ptr token, bool local_frame) {
    char term[MAX];
    unsigned char c;
    string_ptr tmp;
    if (!(tmp = string_init())) return false;
    switch (token->token_type) {
        case T_INT:
            snprintf(term, MAX, "%d", token->attribute.integer);
            EMIT("int@");
            EMIT(term);
            break;

        case T_DECIMAL:
            snprintf(term, MAX, "%a", token->attribute.decimal);
            EMIT("float@");
            EMIT(term);
            break;

        case T_STRING:
            for (int i = 0; (c = (unsigned char) (token->attribute.string)[i]) != '\0'; i++) {
                if (c == '#' || c == '\\' || c <= 32 || !isprint(c)) {
                    string_append(tmp, '\\');
                    sprintf(term, "%03d", c);
                    string_concat(tmp, term);
                } else {
                    string_append(tmp, c);
                }
            }
            EMIT("string@");
            EMIT(tmp->string);
            break;
        case T_KEYWORD:
            if (token->attribute.keyword == k_nil) {
                EMIT("nil@nil");
            }
            break;

        case T_ID:
        EMIT(local_frame ? "LF@" : "GF@");
            EMIT(token->attribute.string);
            break;

        default:
            string_free(tmp);
            return false;
    }
    string_free(tmp);
    return true;
}

bool gen_function_pass_param_count(int count)
{
    EMIT("DEFVAR TF@arg_count\n");
    EMIT("MOVE TF@arg_count int@");
    EMIT_INT(count);
    EMIT("\n");

    return true;
}

bool gen_function_call(const char* name)
{
    EMIT("CALL !")
    EMIT(name)
    EMIT("\n")

    return true;
}

bool generate_stack_operation(Precedence_rules rule)
{
    switch (rule){
        case NT_EQ_NT:
            EMIT("EQS\n");
            break;
        case NT_NEQ_NT:
            EMIT("EQS\n");
            EMIT("NOTS\n");
            break;
        case NT_LEQ_NT:
            EMIT("GTS\n");
            EMIT("NOTS\n");
            break;
        case NT_LTN_NT:
            EMIT("LTS\n");
            break;
        case NT_MEQ_NT:
            EMIT("LTS\n");
            EMIT("NOTS\n");
            break;
        case NT_MTN_NT:
            EMIT("GTS\n");
            break;
        case NT_PLUS_NT:
            EMIT("ADDS\n");
            break;
        case NT_MINUS_NT:
            EMIT("SUBS\n");
            break;
        case NT_MUL_NT:
            EMIT("MULS\n");
            break;
        case NT_DIV_NT:
            EMIT("DIVS\n");
            break;
        default:
            break;
    }

    return true;
}

bool generate_stack_push(token_t_ptr token) {
    switch (token->token_type) {
        case T_INT:
        EMIT("PUSHS int@");
            EMIT_INT(token->attribute.integer);
            EMIT("\n");
            break;
        case T_DECIMAL:
        EMIT("PUSHS float@");
            EMIT_INT(token->attribute.decimal);
            EMIT("\n");
            break;
        case T_STRING:
        EMIT("PUSHS string@");
            EMIT(token->attribute.string);
            EMIT("\n");
            break;
        case T_ID:
        EMIT("PUSHS LF@");
            EMIT(token->attribute.string);
            EMIT("\n");
            break;
        default:
            break;
    }

    return true;
}

void codegen_flush()
{
    fprintf(stdout, "%s", code->string);
    string_free(code);
}
