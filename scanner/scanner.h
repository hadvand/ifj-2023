
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "../structures/string.h"
#include "../structures/error.h"


#define PRINTABLE_MIN 32
#define PRINTABLE_MAX 126

// char *keywords[] = {
//     "move", "createframe", "pushframe", "popframe",
//     "defvar", "call", "return", "pushs", "pops",
//     "clears", "add", "sub", "mul", "div", "idiv",
//     "adds", "subs", "muls", "divs", "idivs", "lt",
//     "gt", "eq", "lts", "gts", "eqs", "and","or",
//     "not", "ands", "ors", "nots", "int2float", "float2int",
//     "int2char", "stri2int", "int2floats", "float2ints", 
//     "int2chars", "stri2ints", "read", "write", "concat",
//     "strlen", "getchar", "setchar", "type", "label",
//     "jump", "jumpifeq", "jumpifneq", "jumpifeqs", "jumpifneqs",
//     "exit", "dprint", "break"
// };

typedef enum state{
    S_START,
    S_KEYWORD,
    S_ID,
    S_UNDERLINE,
    S_KEYWORD_TYPE_ID, //чекнуть правильное название 
    S_STRING_START,
    S_STRING_SPEC_SYMBOL,
    S_STRING_START_HEX,
    S_STRING_HEX_OPEN,
    S_STRING_HEX_NUMBER,
    S_INT,
    S_NUMBER_POINT,
    S_DEMICAL,
    S_EXPONENT_POSSIBLY,
    S_EXPONENT_SING,
    S_EXPONENT,
    S_ASSINGMENT,
    S_NOT_EQUELS_START,
    S_DIVISION,
    S_COMMENT_STRING,
    S_COMMENT_BLOCK_START,
    S_COMMENT_BLOCK_BOSSIBLY_FINISHED,
    S_POSSIBLY_TERN,
    S_MINUS,
    S_LESS,
    S_MORE,
} state_t;

typedef enum token_type{
    T_ITS_NOT_A_TOKEN,
    T_EXPONENT,
    T_DEMICAL,
    T_INT,
    T_EQUALS,
    T_ASSIGMENT,
    T_UNDERLINE,
    T_KEYWORD,
    T_ID,
    T_KEYWORD_TYPE_ID,
    T_STRING,
    T_MORE,
    T_MORE_EQUAL,
    T_LESS,
    T_LESS_EQUAL,
    T_MINUS,
    T_ARROW,
    T_TERN,
    T_COMMENT_STRING,
    T_COMMENT_BLOCK,
    T_NOT_EQUAL,
    T_DIVISION,
    //token without state
    T_COLON,
    T_PLUS,
    T_COMMA,
    T_CURVED_BRACKET_OPEN,
    T_CURVED_BRACKET_CLOSE,
    T_SQUARE_BRACKET_OPEN,
    T_SQUARE_BRACKET_CLOSE,
    T_BRACKET_OPEN,
    T_BRACKET_CLOSE,
    T_NEW_LINE,
    T_EOF,
    T_MULTIPLICATION
} token_type_t;

typedef enum keyword{
    k_Double,
    k_else,
    k_func,
    k_if,
    k_Int,
    k_let,
    k_nil,
    k_return,
    k_String,
    k_var,
    k_while,
    k_qmark_Double,
    k_qmark_Int,
    k_qmark_String
} keyword_t;

typedef union attribute{
    int integer;
    double decimal;
    char *string;
    keyword_t keyword;
} attribute_t;

typedef struct token
{
    token_type_t token_type;
    attribute_t attribute;
    unsigned int line;
} *token_t_ptr;


token_t_ptr next_token(int *line_cnt, error_t* error);