/**
 * @file generator.h
 * @author Neonila Mashlai (xmashl00)
 * @author Oleg Borsh (xborsh00)
 * @brief code generation
 */
#ifndef GENERATOR_H
#define GENERATOR_H

#include <stdio.h>
#include <stdlib.h>

#include "hash.h"
//#include "parser.h"
//#include "stack.h"

#define GENERATE_CODE(...) fprintf(stdout, __VA_ARGS__)

// Built-in functions
#define BUILTIN_LENGHT                                                      \
    "\nLABEL $len"														    \
	"\nPUSHFRAME"															\
	"\nDEFVAR LF@%return"													\
	"\nSTRLEN LF@%return LF@%0"											\
	"\nPOPFRAME"															\
	"\nRETURN\n"                                                             \

// TODO
#define BUILTIN_SUBSTR \
    "\nLABEL $substr" \
    "\nPUSHFRAME" \
    "\nDEFVAR LF@%return" \
    "\nDEFVAR LF@%end_index" \
    "\nDEFVAR LF@%start_index" \
    "\nDEFVAR LF@%string" \
    "\nMOVE LF@%string LF@%0" \
    "\nMOVE LF@%start_index LF@%1" \
    "\nMOVE LF@%end_index LF@%2" \
    "\nDEFVAR LF@%strlength" \
    "\nSTRLEN LF@%strlength LF@%string" \
    "\nDEFVAR LF@%condition" \
    "\nLT LF@%condition LF@%start_index int@0" \
    "\nJUMPIFEQ $error_ret LF@%condition bool@true" \
    "\nGT LF@%condition LF@%start_index LF@%strlength" \
    "\nJUMPIFEQ $error_ret LF@%condition bool@true" \
    "\nLT LF@%condition LF@%end_index int@0" \
    "\nJUMPIFEQ $error_ret LF@%condition bool@true" \
    "\nGT LF@%condition LF@%end_index LF@%strlength" \
    "\nJUMPIFEQ $error_ret LF@%condition bool@true" \
    "\nDEFVAR LF@%char" \
    "\nDEFVAR LF@%substr" \
    "\nMOVE LF@%substr string@" \
    "\nLABEL $while" \
    "\nLT LF@%condition LF@%start_index LF@%end_index" \
    "\nJUMPIFEQ $endwhile LF@%condition bool@false" \
    "\nGETCHAR LF@%char LF@%string LF@%start_index" \
    "\nCONCAT LF@%substr LF@%substr LF@%char" \
    "\nADD LF@%start_index LF@%start_index int@1" \
    "\nJUMP $while"    \
    "\nLABEL $error_ret"  \
    "\nMOVE LF@%substr nil@nil" \
    "\nLABEL $endwhile" \
    "\nMOVE LF@%return LF@%substr" \
    "\nPOPFRAME" \
    "\nRETURN\n"       \

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
    "\nSTRI2INT LF@%return LF@%0 int@0" \
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

/**
 * @brief
 */
void generator_start(void);

/**
 * @brief
 */
void generator_end(void);

/**
 * @brief
 */
void generator_builtin(void);

/**
 * @brief
 */
void generate_var_declaration(item_data data);

/**
 * @brief
 */
void generate_var_definition(item_data data);

#endif //GENERATOR_H
