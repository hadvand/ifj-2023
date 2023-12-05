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

void generator_builtin(void){
    fprintf(stdout, "%s", BUILTIN_LENGHT);
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
    GENERATE_CODE(".IFJcode23\n");
    GENERATE_CODE("JUMP $$main\n");
    generator_builtin();
    GENERATE_CODE("\nLABEL $$main\n");
    GENERATE_CODE("CREATEFRAME\n");
    GENERATE_CODE("PUSHFRAME\n");
}

void generator_end(void){
    GENERATE_CODE("POPFRAME\n");
    GENERATE_CODE("CLEARS\n");
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



int main(void){
    generator_start();
    // CODE HERE
    generator_end();
}
