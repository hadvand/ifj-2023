
#include <stdio.h>

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
    S_EXPONNET_SING,
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
    T_CURVED_BRACKET_OPEN,
    T_CURVED_BRACKET_CLOSE,
    T_SQUARE_BRACKET_OPEN,
    T_SQUARE_BRACKET_CLOSE,
    T_BRACKET_OPEN,
    T_BRACKET_CLOSE,
    T_MORE,
    T_MORE_EQUAL,
    T_LESS,
    T_LESS_EQUAL,
    T_COMMA,
    T_MINUS,
    T_ARROW,
    T_PLUS,
    T_EOL,
    T_TERN,
    T_COLON,
    T_DIVISION,
    T_COMMENT_STRING,
    T_COMMENT_BLOCK,
    T_NOT_EQUAL
} token_type_t;

typedef struct token
{
    token_type_t token_type;
} *token_t_ptr;


token_t_ptr next_token();