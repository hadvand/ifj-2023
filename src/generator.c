#include "generator.h"

void generator_builtin(void){
    GENERATE_CODE(BUILTIN_LENGHT);
    GENERATE_CODE(BUILTIN_INT2DOUBLE);
    GENERATE_CODE(BUILTIN_DOUBLE2INT);
    GENERATE_CODE(BUILTIN_READSTRING);
    GENERATE_CODE(BUILTIN_READINT);
    GENERATE_CODE(BUILTIN_READDOUBLE);
    GENERATE_CODE(BUILTIN_WRITE);
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


int main(void){
    generator_start();
    // CODE HERE
    generator_end();
}