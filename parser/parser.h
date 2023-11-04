//
// Created by dargo on 28.10.2023.
//

#ifndef IFJ_2023_PARSER_H
#define IFJ_2023_PARSER_H

#include "../scanner/scanner.h"

typedef struct Data{
  //global tab_sym
  //local tab_sym

  //stack for local
  // flag for void func
  // flag if we can use return (isInsideFunction?)

  token_t_ptr token_ptr;
}TData;

/*
<program> -> <stm> EOF
<stm> -> ε
<stm> -> <stm_not_null>
<stm> -> <stm_not_null> <stm>

<stm_not_null> -> var/let id: <var_type> = <expression>
<stm_not_null> -> var/let id = <expression>
<stm_not_null> -> var/let id: <var_type>
<stm_not_null> -> id = <expression>

<stm_not_null> -> if ( <condition> ) { <stm> }
<stm_not_null> -> if ( <condition> ) { <stm> } else { <stm> }
<stm_not_null> -> while <condition> { <stm> }

<condition> -> let id
<condition> -> <expression>

<func> -> func func_id(<func_params>) -> <var_type> { <stm> <return> }
<func> -> func func_id(<func_params>) { <stm> <return_void> }

<func_params> -> ε
<func_params> -> var_name var_id: <var_type> <func_params_not_null>

<func_params_not_null> -> , <func_params>
<func_params_not_null> -> ε

<var_type> -> String
<var_type> -> Int
<var_type> -> Double
<var_type> -> String?
<var_type> -> Int?
<var_type> -> Double?

<value> -> int_value
<value> -> str_value
<value> -> double_value

<return> -> return <expression>

<return_void> -> return
<return_void> -> ε
 */


bool init_data(Data *data);

bool free_data(Data *data);

int analyse()

int program(Data *data);

int stm(Data *data);

int stm_not_null(Data *data);

int condition(Data *data);

int func(Data *data);

int func_params(Data *data);

int func_params_not_null(Data *data);

int var_type(Data *data);

int return_rule(Data *data);

#endif //IFJ_2023_PARSER_H
