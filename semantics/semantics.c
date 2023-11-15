#include "semantics.h"
#include 'stack.h'
#include "scanner.h"

#define TABLE_SIZE 16


stack stack;


typedef enum {
    I_PLUS,
    I_MINUS,
    I_MUL,
    I_DIV,
    I_EQUAL,
    I_N_EQUAL,
    I_LESS,
    I_GREATER,
    I_L_EQUAL,
    I_G_EQUAL,
    I_D_QMARK,
    I_EXMARK,
    I_LEFT_BRACKET,
    I_RIGHT_BRACKET,
    I_DATA,
    I_DOLLAR
} Precedence_table_indices;

int precedence_table[TABLE_SIZE][TABLE_SIZE] =
{
    //      | + |  - |  * |  / |  == | != | < |  > | <= | >= | ?? |  ! |  ( |  ) |  i |  $ |

            {'>', '>', '<', '<', '>', '>', '>', '>', '>', '>', '>', '<', '<', '>', '<', '>'},   // |+|
            {'>', '>', '<', '<', '>', '>', '>', '>', '>', '>', '>', '<', '<', '>', '<', '>'},   // |-|
            {'>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '<', '<', '>', '<', '>'},   // |*|
            {'>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '<', '<', '>', '<', '>'},   // |/|
            {'<', '<', '<', '<', '>', '>', '>', '>', '>', '>', '>', '<', '<', '>', '<', '>'},   // |==|
            {'<', '<', '<', '<', '>', '>', '>', '>', '>', '>', '>', '<', '<', '>', '<', '>'},   // |!=|
            {'<', '<', '<', '<', '>', '>', '>', '>', '>', '>', '>', '<', '<', '>', '<', '>'},   // |<|
            {'<', '<', '<', '<', '>', '>', '>', '>', '>', '>', '>', '<', '<', '>', '<', '>'},   // |>|
            {'<', '<', '<', '<', '>', '>', '>', '>', '>', '>', '>', '<', '<', '>', '<', '>'},   // |<=|
            {'<', '<', '<', '<', '>', '>', '>', '>', '>', '>', '>', '<', '<', '>', '<', '>'},   // |>=|
            {'<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '>', '<', '>'},   // |??|
            {'>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', ' ', ' ', '>', ' ', '>'},   // |!|
            {'<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', ' ', '<', '=', '<', ' '},   // |(|
            {'>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', ' ', '>', ' ', '>'},   // |)|
            {'>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', ' ', '>', ' ', '>'},   // |i|
            {'<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', ' ', '<', ' '}   // |$|

};

/*
 *
 * getting the indices of the precedence table
 *
 */
static Precedence_table_indices get_index(Precedence_table_symbol symbol){

    switch(symbol){

        case PLUS:
            return I_PLUS;
        case MINUS:
            return I_MINUS;
        case MUL:
            return I_MUL;
        case DIV:
            return I_DIV;

        case EQUAL:
            return I_EQUAL;
        case N_EQUAL:
            return I_N_EQUAL;
        case LESS:
            return I_LESS;
        case GREATER:
            return I_GREATER;
        case L_EQUAL:
            return I_L_EQUAL;
        case G_EQUAL:
            return G_EQUAL;

        case DQ_MARK:
            return I_D_QMARK;
        case EX_MARK:
            return I_EXMARK;

        case LEFT_BRACKET:
            return I_LEFT_BRACKET;
        case RIGHT_BRACKET:
            return I_RIGHT_BRACKET;

        case IDENTIFIER:
            return I_DATA;
        case INT_NUMBER:
            return I_DATA;
        case DOUBLE_NUMBER:
            return I_DATA;
        case STRING:
            return I_DATA;

        default:
            return I_DOLLAR;
    }
}

static Precedence_table_symbol convert_token_into_symbol(token* token){

    switch(token -> token_type){

    }
}
