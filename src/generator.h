/**
 * @file generator.h
 * @author Neonila Mashlai (xmashl00)
 * @author Oleg Borshch (xborsh00)
 * @brief code generation
 */
#ifndef GENERATOR_H
#define GENERATOR_H

#include <stdio.h>
#include <stdlib.h>

#include "hash.h"
#include "string.h"
#include "parser.h"
#include "semantics.h"

// Built-in functions
#define BUILTIN_LENGTH                                                      \
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
    "\nRETURN\n"                                                            \

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
    "PUSHFRAME\n"                               \
	"DEFVAR LF@to_write\n"                      \
	"DEFVAR LF@type\n"                          \
	"TYPE LF@type LF@arg_count\n"                  \
	"JUMPIFNEQ !write_end LF@type string@int\n" \
	"DEFVAR LF@cond\n"                          \
	"LT LF@cond LF@arg_count int@1\n"              \
	"JUMPIFEQ !write_end LF@cond bool@true\n"   \
	"LABEL !write_loop\n"                       \
	"POPS LF@to_write\n"                          \
	"WRITE LF@to_write\n"                         \
	"SUB LF@arg_count LF@arg_count int@1\n"           \
	"GT LF@cond LF@arg_count int@0\n"              \
	"JUMPIFEQ !write_loop LF@cond bool@true\n"  \
	"LABEL !write_end\n"                        \
	"POPFRAME\n"                                \
	"RETURN\n"                                  \

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
	"\nMOVE LF@%return string@"											    \
	"\nDEFVAR LF@condition"												    \
	"\nLT LF@condition LF@%0 int@0"										    \
	"\nJUMPIFEQ $ret LF@condition bool@true"						        \
	"\nGT LF@condition LF@%0 int@255"										\
	"\nJUMPIFEQ $ret LF@condition bool@true"						        \
	"\nINT2CHAR LF@%return LF@%0"											\
	"\nLABEL $ret"													        \
	"\nPOPFRAME"															\
	"\nRETURN\n\n"

/**
 * @brief
 */
void generator_start();

/**
 * @brief
 */
void generator_end();

/**
 * @brief
 */
void generator_builtin();

/**
 * @brief
 */
void generate_var_declaration(item_data data);

/**
 * @brief
 */
void generate_var_definition(item_data data);

/**
 * @brief
 */
void codegen_flush();

/**
 * @brief
 */
bool gen_function_before_params();

/**
 * @brief
 */
bool gen_value_from_token(token_t_ptr token, bool local_frame);

/**
 * @brief
 */
bool gen_function_pass_param_push(token_t_ptr token, bool local_frame);

/**
 * @brief
 */
bool gen_function_pass_param_count(int count);

/**
 * @brief
 */
bool gen_function_call(const char* name);

/**
 * @brief
 */
bool generate_stack_operation(Precedence_rules rule);

/**
 * @brief
 */
bool generate_stack_push(token_t_ptr token);

#endif //GENERATOR_H
