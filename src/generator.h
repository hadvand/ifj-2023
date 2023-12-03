#ifndef GENERATOR_H
#define GENERATOR_H

#include <stdio.h>
#include <stdlib.h>

#include "hash.h"
//#include "parser.h"
//#include "stack.h"

#define GENERATE_CODE(code) fprintf(stdout, "%s", code)

// Built-in functions
#define BUILTIN_LENGHT                                                      \
    "\nLABEL $len"														    \
	"\nPUSHFRAME"															\
	"\nDEFVAR LF@%return"													\
	"\nSTRLEN LF@%return LF@%0"											\
	"\nPOPFRAME"															\
	"\nRETURN\n"                                                             \

// TODO
#define BUILTIN_SUBSTR

#define BUILTIN_INT2DOUBLE \
    "\nLABEL $int2double" \
    "\nPUSHFRAME" \
    "\nDEFVAR LF@%return" \
    "\nINT2FLOAT LF@%return LF@%0" \
    "\nPOPFRAME" \
    "\nRETURN\n"

#define BUILTIN_DOUBLE2INT \
    "\nLABEL $double2int" \
    "\nPUSHFRAME" \
    "\nDEFVAR LF@%return" \
    "\nFLOAT2INT LF@%return LF@%0" \
    "\nPOPFRAME" \
    "\nRETURN\n"

#define BUILTIN_READSTRING                                                  \
    "\nLABEL $readString"													\
    "\nPUSHFRAME"															\
    "\nDEFVAR LF@%return"													\
    "\nREAD LF@%return string"												\
    "\nPOPFRAME"															\
    "\nRETURN\n"                                                             \

#define BUILTIN_READINT                                                     \
    "\nLABEL $readInt"														\
    "\nPUSHFRAME"															\
    "\nDEFVAR LF@%return"													\
    "\nREAD LF@%return int"												\
    "\nPOPFRAME"															\
    "\nRETURN\n"                                                             \

#define BUILTIN_READDOUBLE                                                  \
    "\nLABEL $readDouble"													\
    "\nPUSHFRAME"															\
    "\nDEFVAR LF@%return"													\
    "\nREAD LF@%return float"												\
    "\nPOPFRAME"															\
    "\nRETURN\n"                                                             \

#define BUILTIN_WRITE                                                       \
    "\nLABEL $write"                                                        \
    "\nPUSHFRAME"      \
    "\nWRITE LF@%0"    \
    "\nPOPFRAME"       \
    "\nRETURN\n"         \

#define BUILTIN_ORD \
    "\nLABEL $ord" \
    "\nPUSHFRAME"  \
    "\nDEFVAR LF@%return"  \
    "\nSTRI2INT LF@%return LF@%0 LF@%1" \
    "\nPOPFRAME"   \
    "\nRETURN\n"     \

#define BUILTIN_CHR \
	"\nLABEL $chr"															\
	"\nPUSHFRAME"															\
	"\nDEFVAR LF@%return"													\
	"\nMOVE LF@%return string@"											\
	"\nDEFVAR LF@condition"												\
	"\nLT LF@condition LF@%0 int@0"										\
	"\nJUMPIFEQ $ret LF@condition bool@true"						\
	"\nGT LF@condition LF@%0 int@255"										\
	"\nJUMPIFEQ $ret LF@condition bool@true"						\
	"\nINT2CHAR LF@%return LF@%0"											\
	"\nLABEL $ret"													\
	"\nPOPFRAME"															\
	"\nRETURN\n\n"

void generator_start(void);
void generator_end(void);
void generator_builtin(void);
#endif //GENERATOR_H
