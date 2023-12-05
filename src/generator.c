/**
 * @file generator.c
 * @author Neonila Mashlai (xmashl00)
 * @author Oleg Borsh (xborsh00)
 * @brief code generation
 */

#include "generator.h"
#include <stdbool.h>
#include "string.h"

#define MAX_DIGITS 64

string_ptr code;


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
    //generator_builtin();
    generator_end();
}
