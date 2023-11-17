//
// Created by oleg on 11/17/23.
//

#include "parser.h"


#ifndef IFJ_NEW_BEGINNING_SEMANTICS_H
#define IFJ_NEW_BEGINNING_SEMANTICS_H

#ifndef IFJ_NEW_BEGINNING_SEMANTICS_H
#define IFJ_NEW_BEGINNING_SEMANTICS_H


typedef enum
{
    PLUS,			// +
    MINUS,			// -
    MUL,			// *
    DIV,			// /
    EQUAL,			// =
    N_EQUAL,		// !=
    L_EQUAL,		// <=
    LESS,		    // <
    G_EQUAL,		// >=
    GREATER,		// >
    DQ_MARK,        // ??
    EX_MARK,        // !
    LEFT_BRACKET,	// (
    RIGHT_BRACKET,	// )
    IDENTIFIER,		// ID
    INT_NUMBER,		// int
    DOUBLE_NUMBER,	// double
    STRING,			// string
    DOLLAR,			// $
    STOP,			// stop symbol used when reducing
    N_TERMINAL		// non-terminal
} Precedence_table_symbol;

typedef enum
{
    NT_EQ_NT,       // E -> E = E
    NT_NEQ_NT,      // E -> E != E
    NT_LEQ_NT,      // E -> E <= E
    NT_LTN_NT,      // E -> E < E
    NT_MEQ_NT,      // E -> E => E
    NT_MTN_NT,      // E -> E > E
    NT_PLUS_NT,     // E -> E + E
    NT_MINUS_NT,    // E -> E - E
    NT_MUL_NT,      // E -> E * E
    NT_DIV_NT,      // E -> E / E
    NT_COALESCE_NT, // E -> E ?? E
    NT_NOT_NT,      // E -> !E
    LBR_NT_RBR,     // E -> (E)
    OPERAND,        // E -> i
    NOT_A_RULE      // rule doesn't exist
} Precedence_rules;


int expression(PData* data);


#endif //IFJ_NEW_BEGINNING_SEMANTICS_H

#endif //IFJ_NEW_BEGINNING_SEMANTICS_H
