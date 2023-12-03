#include "generator.h"
#include <stdbool.h>
#include "string.h"

#define MAX_DIGITS 64

#define EMIT(_inst)														\
	if (!dynamic_string_add_const_str(&code, (_inst "\n"))) {               \
    return false;                                                           \
    }

#define ADD_CODE(_code)														\
	if (!dynamic_string_add_const_str(&code, (_code))){                     \
    return false;                                                            \
    }

#define EMIT_INT(_number)													\
	do {																	\
		char str[MAX_DIGITS];												\
		sprintf(str, "%d", _code);											\
		ADD_CODE(str);														\
	} while (0)

string_ptr string;

/* Function label automatic extension */
#define _FUNCLBEXT ""

bool header_emit()
{
    EMIT_NL(".IFJcode23\n"
            "DEFVAR GF@TMP_REG1\n"
            "DEFVAR GF@TMP_REG2\n"
            "DEFVAR GF@TMP_REG3\n"
            "DEFVAR GF@EXPR_VAL\n"
            "DEFVAR GF@EXPR_TYPE\n"
            "JUMP !program_body"_FLBEXT"\n");
    return true;
}


static bool built_in_functions()

{

    EMIT(BUILTIN_READSTRING);
    EMIT(BUILTIN_READINT);
    EMIT(BUILTIN_READSTRING);
    EMIT(BUILTIN_WRITE);
    EMIT(BUILTIN_INT2DOUBLE);
    EMIT(BUILTIN_DOUBLE2INT);
    EMIT(BUILTIN_LENGTH);
    EMIT(BUILTIN_CHR);
    EMIT(BUILTIN_ORD);
    EMIT(BUILTIN_SUBSTR);

    return true;
}

bool code_generator_start()
{
    //if (!string_ptr string_init) return false;

    if (!header_emit()) {
        return false;
    }

    if (!built_in_functions()) {
        return false;
    }

    return true;
}


void code_generator_clear()
{
    string_free(&code);
}


void codegen_flush(FILE *destination_file)
{
    fputs(code.str, destination_file);
    code_generator_clear();
}


bool generate_main_scope_start()
{
    EMIT("\n# Main scope starts");

    EMIT("LABEL $$main");
    EMIT("CREATEFRAME");
    EMIT("PUSHFRAME");

    return true;
}

bool generate_main_scope_end()
{
    EMIT("# Main scope ends");

    EMIT("POPFRAME");
    EMIT("CLEARS");

    return true;
}


bool generate_function_start(char *function_id)
{
    ADD_CODE("\n# Function starts");
    ADD_CODE(function_id);
    ADD_CODE("\n");
    ADD_CODE("LABEL $");
    ADD_CODE(function_id);
    ADD_CODE("\n");
    EMIT("PUSHFRAME");

    return true;
}


bool generate_function_end(char *function_id)
{
    ADD_CODE("# Function ends ");
    ADD_CODE(function_id);
    ADD_CODE("\n");
    ADD_CODE("LABEL $");
    ADD_CODE(function_id);
    ADD_CODE("%return\n");
    ADD_INST("POPFRAME");
    EMIT("RETURN");

    return true;
}

static bool default_var_value(item_type type)
{
    switch (type)
    {
        case TYPE_INT:
            ADD_CODE("int@0");
            break;

        case TYPE_DOUBLE:
            ADD_CODE("float@0.0");
            break;

        case TYPE_STRING:
            ADD_CODE("string@");
            break;

        default:
            return false;
    }

    return true;
}


bool function_return_value(item_type type)
{
    EMIT("DEFVAR LF@%retval");

    ADD_CODE("MOVE LF@%retval ");
    if (!default_var_value(type))
    {
        return false;
    }
    ADD_CODE("\n");

    return true;
}


bool function_call(char *function_id)
{
    ADD_CODE("CALL $");
    ADD_CODE(function_id);
    ADD_CODE("\n");

    return true;
}
