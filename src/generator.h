#ifndef GENERATOR_H
#define GENERATOR_H

#include <stdio.h>
#include <stdlib.h>


#include "hash.h"
#include "parser.h"
#include "stack.h"


// Built-in functions
#define BUILTIN_LENGTH                                                      \
	"\n LABEL $len"														    \
	"\n PUSHFRAME"															\
	"\n DEFVAR LF@%return"													\
	"\n STRLEN LF@%return LF@%0"											\
	"\n POPFRAME"															\
	"\n RETURN"                                                             \

// TODO
#define BUILTIN_SUBSTR \
    "\n # Built-in function substr" \
    "LABEL !substring\n"                            \
	"PUSHFRAME\n"                                   \
	"DEFVAR LF@res\n"                               \
	"MOVE LF@res string@\n"                         \
	"DEFVAR LF@length\n"                            \
	"CREATEFRAME\n"                                 \
	"DEFVAR TF@-0\n"                                \
	"MOVE TF@-0 LF@-0\n"                            \
	"CALL !strlen\n"                                \
	"MOVE LF@length TF@res\n"                       \
	"DEFVAR LF@cond\n"                              \
	"LT LF@cond LF@-1 int@0\n"                      \
	"JUMPIFEQ !substring_end_null LF@cond bool@true\n"   \
	"LT LF@cond LF@-2 int@0\n"                      \
	"JUMPIFEQ !substring_end_null LF@cond bool@true\n"   \
	"GT LF@cond LF@-1 LF@-2\n"                      \
	"JUMPIFEQ !substring_end_null LF@cond bool@true\n"   \
	"LT LF@cond LF@-1 LF@length\n"                  \
	"JUMPIFEQ !substring_end_null LF@cond bool@false\n"  \
	"GT LF@cond LF@-2 LF@length\n"                  \
	"JUMPIFEQ !substring_end_null LF@cond bool@true\n"   \
	"DEFVAR LF@index\n"                             \
	"MOVE LF@index LF@-1\n"                         \
	"DEFVAR LF@char\n"                              \
	"LABEL !substring_loop\n"                       \
	"LT LF@cond LF@index LF@-2\n"                   \
	"JUMPIFEQ !substring_end LF@cond bool@false\n"  \
	"GETCHAR LF@char LF@-0 LF@index\n"              \
	"CONCAT LF@res LF@res LF@char\n"                \
	"ADD LF@index LF@index int@1\n"                 \
	"JUMP !substring_loop\n"                         \
	"LABEL !substring_end_null\n"\
	"MOVE LF@res nil@nil\n"                         \
	"LABEL !substring_end\n"                        \
	"POPFRAME\n"                                    \
	"RETURN\n"


#define BUILTIN_INT2DOUBLE \
    "\n LABEL $int2double" \
    "\n PUSHFRAME" \
    "\n DEFVAR LF@%return" \
    "\n INT2FLOAT LF@%return LF@%0" \
    "\n POPFRAME" \
    "\n RETURN"

#define BUILTIN_DOUBLE2INT \
    "\n LABEL $double2int" \
    "\n PUSHFRAME" \
    "\n DEFVAR LF@%return" \
    "\n FLOAT2INT LF@%return LF@%0" \
    "\n POPFRAME" \
    "\n RETURN"

#define BUILTIN_READSTRING                                                  \
    "\n LABEL $readString"													\
    "\n PUSHFRAME"															\
    "\n DEFVAR LF@%return"													\
    "\n READ LF@%return string"												\
    "\n POPFRAME"															\
    "\n RETURN"                                                             \

#define BUILTIN_READINT                                                     \
    "\n LABEL $readInt"														\
    "\n PUSHFRAME"															\
    "\n DEFVAR LF@%return"													\
    "\n READ LF@%return int"												\
    "\n POPFRAME"															\
    "\n RETURN"                                                             \

#define BUILTIN_READDOUBLE                                                  \
    "\n LABEL $readDouble"													\
    "\n PUSHFRAME"															\
    "\n DEFVAR LF@%return"													\
    "\n READ LF@%return float"												\
    "\n POPFRAME"															\
    "\n RETURN"                                                             \

#define BUILTIN_WRITE                                                       \
    "\n LABEL $write"                                                        \
    "\n PUSHFRAME"      \
    "\n WRITE LF@%0"    \
    "\n POPFRAME"       \
    "\n RETURN"         \

#define BUILTIN_ORD \
    "\n LABEL $ord" \
    "\n PUSHFRAME"  \
    "\n DEFVAR LF@%return"  \
    "\n STRI2INT LF@%return LF@%0 LF@%1" \
    "\n POPFRAME"   \
    "\n RETURN"     \

#define BUILTIN_CHR \
	"\n LABEL $chr"															\
	"\n PUSHFRAME"															\
	"\n DEFVAR LF@%return"													\
	"\n MOVE LF@%return string@"											\
	"\n DEFVAR LF@condition"												\
	"\n LT LF@condition LF@%0 int@0"										\
	"\n JUMPIFEQ $ret LF@condition bool@true"						\
	"\n GT LF@condition LF@%0 int@255"										\
	"\n JUMPIFEQ $ret LF@condition bool@true"						\
	"\n INT2CHAR LF@%return LF@%0"											\
	"\n LABEL $ret"													\
	"\n POPFRAME"															\
	"\n RETURN"




#endif //GENERATOR_H
