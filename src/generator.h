#ifndef GENERATOR_H
#define GENERATOR_H

#include <stdio.h>
#include <stdlib.h>

//#include "hash.h"
//#include "parser.h"
//#include "stack.h"

// Built-in functions
#define BUILTIN_LENGHT                                                      \
	"\n LABEL $len"														    \
	"\n PUSHFRAME"															\
	"\n DEFVAR LF@%return"													\
	"\n STRLEN LF@%return LF@%0"											\
	"\n POPFRAME"															\
	"\n RETURN"                                                             \

// TODO
#define BUILTIN_SUBSTR

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
