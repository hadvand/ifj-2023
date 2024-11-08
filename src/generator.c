/**
 * @file generator.c
 * @author Neonila Mashlai (xmashl00)
 * @author Oleg Borshch (xborsh00)
 * @brief code generation
 */

#include "generator.h"
#include <stdbool.h>
#include "string.h"

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

void codegen_flush()
{
    fprintf(stdout, "%s", code->string);
    string_clear(code);
}

bool generator_start(void){
    code = string_init();
    EMIT(".IFJcode23\n")
    EMIT("DEFVAR GF@%tmp_op1\n");
    EMIT("DEFVAR GF@%tmp_op2\n");
    EMIT("DEFVAR GF@%tmp_op3\n");
    EMIT("JUMP $$main\n")

    generator_builtin();

    EMIT("\nLABEL $$main\n")
    EMIT("CREATEFRAME\n")
    EMIT("PUSHFRAME\n")

    return true;
}

bool generator_end(void){
    EMIT("POPFRAME\n")
    EMIT("CLEARS\n")

    codegen_flush();

    return true;
}


void generate_var_declaration(item_data data){
    if(data.global){
        GENERATE_CODE("DEFVAR GF@%s\n", data.id);
    }else{
        GENERATE_CODE("DEFVAR LF@%s\n", data.id);
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

bool gen_concat_stack_strings()
{
    EMIT("POPS GF@%tmp_op3\n");
    EMIT("POPS GF@%tmp_op2\n");
    EMIT("CONCAT GF@%tmp_op1 GF@%tmp_op2 GF@%tmp_op3\n");
    EMIT("PUSHS GF@%tmp_op1\n");

    return true;
}

bool gen_define_var(const char* var, bool is_local)
{
    EMIT("DEFVAR ");
    const char* frame = is_local ? "LF" : "GF";
    EMIT(frame);
    EMIT("@");
    EMIT(var);
    EMIT("\n");

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
    EMIT("CALL $")
    EMIT(name)
    EMIT("\n")

    return true;
}

bool gen_stack_operation(Precedence_rules rule)
{
    switch (rule){
        case NT_AS_NT:

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

bool gen_push_token(token_t_ptr token, bool is_local_scope) {
    EMIT("PUSHS ");
    if (!gen_value_from_token(token, is_local_scope)) return false;
    EMIT("\n");

    return true;
}

bool gen_pop_expr_result(const char* var, const char* scope)
{
    EMIT("POPS ");
    EMIT(scope);
    EMIT("@");
    EMIT(var);
    EMIT("\n");

    return true;
}


