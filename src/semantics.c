#include "semantics.h"
#include "stack.h"
#include "error.h"

#define TABLE_SIZE 16


t_stack stack;

static Precedence_rules check_rule(int number, t_stack_elem* operand_1, t_stack_elem* operand_2, t_stack_elem* operand_3);

static int check_semantics(Precedence_rules rule, t_stack_elem* operand_1, t_stack_elem* operand_2, t_stack_elem* operand_3,
                           item_data *type_final);

#define GET_TOKEN() \
        if ((data->token_ptr = next_token(&(data->line_cnt), &ret_code, &(data->eol_flag))) == NULL) {\
            return ret_code;                                               \
        }           \

#define VERIFY_TOKEN(t_token)  \
    GET_TOKEN()                \
    if (data->token_ptr->token_type != t_token) return ER_SYNTAX;\

#define FREE(_error_code) \
    do{                   \
        stack_free(&stack); \
        \
        return _error_code;   \
        }\
    while(0) \

#define NIL_POSSIBILITY_CHECK() \
                if(operand_1->item.nil_possibility != operand_3->item.nil_possibility) \
                    return ER_TYPE_COMP;                                               \
                type_final->nil_possibility = operand_1->item.nil_possibility\

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
            {'>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', ' ', '>', '>', '>'},   // |)|
            {'>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', '>', ' ', '>', '>', '>'},   // |i|
            {'<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', '<', ' ', '<', ' '}   // |$|

};


Precedence_table_symbol convert_token_into_symbol(parser_data_t *data, bool last_action_is_reduce ){

    switch(data->token_ptr->token_type){

        case T_PLUS:
            return PLUS;
        case T_MINUS:
            return MINUS;
        case T_MULTIPLICATION:
            return MUL;
        case T_DIVISION:
            return DIV;
        case T_EQUALS:
            return EQUAL;
        case T_NOT_EQUAL:
            return N_EQUAL;
        case T_LESS:
            return LESS;
        case T_MORE:
            return GREATER;
        case T_LESS_EQUAL:
            return L_EQUAL;
        case T_MORE_EQUAL:
            return G_EQUAL;
        case T_KEYWORD_NIL_POSSIBILITY:
            return DQ_MARK;
        case T_EXCLAMATION_MARK:
            return EX_MARK;
        case T_BRACKET_OPEN:
            return LEFT_BRACKET;
        case T_BRACKET_CLOSE:
            return RIGHT_BRACKET;
        case T_ID:
            if(last_action_is_reduce)
                return DOLLAR;
            return IDENTIFIER;
        case T_INT:
            return INT_NUMBER;
        case T_DEMICAL:
            return DOUBLE_NUMBER;
        case T_STRING:
            return STRING;
        case T_KEYWORD:
            if(data->token_ptr->attribute.keyword == k_nil)
                return NIL;
            return DOLLAR;
        default:
            return DOLLAR;
    }
}

/*
 *
 * getting the indices of the precedence table
 *
 */
Precedence_table_indices get_index(Precedence_table_symbol symbol){

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
            return I_G_EQUAL;

        case DQ_MARK:
            return I_D_QMARK;
        case EX_MARK:
            return I_EXMARK;

        case LEFT_BRACKET:
            return I_LEFT_BRACKET;
        case RIGHT_BRACKET:
            return I_RIGHT_BRACKET;

        case IDENTIFIER: // INT DOUBLE STRING
            return I_DATA;

        default:
            return I_DOLLAR;
    }
}

int func_call(parser_data_t* data) {
    (void)(data);
    return ER_NONE;
}


item_type get_type_from_params(item_data *data,int position){
    if(data->params == NULL)
        return IT_UNDEF;
    if(position > data->params->last_index)
        return IT_UNDEF;
    switch (data->params->string[position]) {
        case 's':
        case 'S':
            return IT_STRING;
        case 'd':
        case 'D':
            return IT_DOUBLE;
        case 'i':
        case 'I':
            return IT_INT;
    }
    return IT_UNDEF;
}



int number_of_symbols_after_stop(bool* found_stop){

    t_stack_elem* item = get_top(&stack);
    int counter = 0;
    while (item != NULL){

        if (item->symbol != STOP){
            *found_stop = false;
            counter += 1;
        }

        else {
            *found_stop = true;
            break;
        }
        item = item->next;
    }

    return counter;
}

int reduce(){

    int ret_code;

    t_stack_elem *op1 = NULL;
    t_stack_elem *op2 = NULL;
    t_stack_elem *op3 = NULL;
    item_data final_item;
    Precedence_rules rule;
    bool stop_is_founded;

    int count_symbols_before_stop = number_of_symbols_after_stop(&stop_is_founded);

    if(count_symbols_before_stop == 1 && stop_is_founded){
        op1 = stack.top;
        rule = check_rule(count_symbols_before_stop,op1,op2,op3);
    }
    else if(count_symbols_before_stop == 3 && stop_is_founded){
        op1 = stack.top->next->next;
        op2 = stack.top->next;
        op3 = stack.top;
        rule = check_rule(count_symbols_before_stop,op1,op2,op3);
    }
    else
        return ER_SYNTAX;

    if(rule == NOT_A_RULE){
#ifdef SEM_DEBUG
        printf("not a rule\n");
#endif
        return ER_SYNTAX;
    }
    else{
        //todo check semantics

        if ((ret_code = check_semantics(rule, op1, op2, op3, &final_item))){
#ifdef SEM_DEBUG
            printf("wrong types\n");
#endif
            return ret_code;
        }
        for (int i = count_symbols_before_stop + 1; i > 0 ; i--) {
            stack_pop(&stack);
        }
        stack_push(&stack,final_item,N_TERMINAL);
    }

    return ER_NONE;
}

int expression(parser_data_t* data){
    int ret_code = ER_SYNTAX;

#ifdef SEM_DEBUG
    printf("semantic analysis starts\n");
#endif

    stack_init(&stack);
    //TODO init item_data
    item_data tmp_item;
    tmp_item.type = IT_UNDEF;
    tmp_item.nil_possibility = false;
    stack_push(&stack, tmp_item, DOLLAR);

    bool success = false;

    bool last_action_is_reduce = false;
    t_stack_elem* top_terminal;
    Precedence_table_symbol actual_symbol;


    do {
        top_terminal = stack_top_terminal(&stack);
        if(top_terminal->symbol == IDENTIFIER && data->id_type != NULL && data->id_type->is_function)
            call_params(data);
        actual_symbol = convert_token_into_symbol(data,last_action_is_reduce);
//        if(data->token_ptr->token_type == T_KEYWORD && data->token_ptr->attribute.keyword == k_let)
//            return ret_code;
        if(top_terminal == NULL){
#ifdef SEM_DEBUG
            printf("semantic analysis finish with error\n");
#endif
            FREE(ER_INTERNAL);
        }

        switch (precedence_table[get_index(top_terminal->symbol)][get_index(actual_symbol)]) {
            case '=':
                tmp_item.type = get_type(data->token_ptr,data,&(tmp_item.nil_possibility),&(tmp_item.defined));
                if(!stack_push(&stack, tmp_item,actual_symbol))
                {
#ifdef SEM_DEBUG
                    printf("semantic analysis finish with error\n");
#endif
                    FREE(ER_INTERNAL);
                }

#ifdef SEM_DEBUG
                stack_print_all_symbols(&stack);
#endif

                data->token_ptr = next_token(&(data->line_cnt),&ret_code, &(data->eol_flag));
                if(ret_code != ER_NONE)
                {
#ifdef SEM_DEBUG
                    printf("semantic analysis finish with error\n");
#endif
                    FREE(ER_INTERNAL);
                }

                last_action_is_reduce = false;
                break;
            case '<':
                if(!stack_push_after_top_term(&stack,tmp_item,STOP))
                {
#ifdef SEM_DEBUG
                    printf("semantic analysis finish with error\n");
#endif
                    FREE(ER_INTERNAL);
                }
                tmp_item.type = get_type(data->token_ptr,data,&(tmp_item.nil_possibility),&(tmp_item.defined));
                if(!stack_push(&stack, tmp_item,actual_symbol))
                {
#ifdef SEM_DEBUG
                    printf("semantic analysis finish with error\n");
#endif
                    FREE(ER_INTERNAL);
                }
                //TODO generatecode

#ifdef SEM_DEBUG
                stack_print_all_symbols(&stack);
#endif

                data->token_ptr = next_token(&(data->line_cnt),&ret_code, &(data->eol_flag));
                if(ret_code != ER_NONE)
                {
#ifdef SEM_DEBUG
                    printf("semantic analysis finish with error\n");
#endif
                    FREE(ret_code);
                }
                last_action_is_reduce = false;
                break;
            case '>':
                if((ret_code = reduce()))
                {
#ifdef SEM_DEBUG
                    printf("semantic analysis finish with error\n");
#endif
                    FREE(ret_code);
                }

#ifdef SEM_DEBUG
                stack_print_all_symbols(&stack);
#endif
                last_action_is_reduce = true;
                break;
            default:
                if(actual_symbol == DOLLAR && top_terminal->symbol == DOLLAR)
                    success = true;
                else
                {
#ifdef SEM_DEBUG
                    printf("semantic analysis finish with error\n");
#endif
                    FREE(ER_SYNTAX);
                }
                last_action_is_reduce = false;
                break;
        }

    }while(!success);


    t_stack_elem op1;
    op1.symbol = N_TERMINAL;
    op1.item = *(data->id);
    t_stack_elem op2;
    op2.symbol = EQUAL;
    t_stack_elem op3;
    op3 = *(stack.top);
    item_data final_type;
    if((ret_code =check_semantics(NT_EQ_NT,&op1,&op2,&op3,&final_type))){
#ifdef SEM_DEBUG
        printf("semantic analysis finish with error\n");
#endif
        FREE(ret_code);
    }


#ifdef SEM_DEBUG
    printf("semantic analysis finish\n");
#endif

    return ret_code;
}


static Precedence_rules check_rule(int number, t_stack_elem* operand_1, t_stack_elem* operand_2, t_stack_elem* operand_3){

    switch (number){

        case(1):

            if (operand_1->symbol == IDENTIFIER || operand_1->symbol == INT_NUMBER || operand_1->symbol == DOUBLE_NUMBER ||
                operand_1->symbol == STRING){
                return OPERAND;
            }

            return NOT_A_RULE;

        case(3):

            if (operand_1->symbol == LEFT_BRACKET && operand_2->symbol == N_TERMINAL
                && operand_3->symbol == RIGHT_BRACKET) {
                return LBR_NT_RBR;
            }

            if (operand_1->symbol == N_TERMINAL && operand_3->symbol == N_TERMINAL){

                switch(operand_2->symbol){

                    case PLUS:
                        return NT_PLUS_NT;

                    case MINUS:
                        return NT_MINUS_NT;

                    case MUL:
                        return NT_MUL_NT;

                    case DIV:
                        return NT_DIV_NT;

                    case EQUAL:
                        return NT_EQ_NT;

                    case N_EQUAL:
                        return NT_NEQ_NT;

                    case LESS:
                        return NT_LTN_NT;

                    case GREATER:
                        return NT_MTN_NT;

                    case L_EQUAL:
                        return NT_LEQ_NT;

                    case G_EQUAL:
                        return NT_MTN_NT;

                    case DQ_MARK:
                        return NT_COALESCE_NT;

                    case EX_MARK:
                        return NT_F_UNWRAP;

                    default:
                        return NOT_A_RULE;

                }

            }

            return NOT_A_RULE;
    }

    return NOT_A_RULE;
}

static int check_semantics(Precedence_rules rule, t_stack_elem* operand_1, t_stack_elem* operand_2, t_stack_elem* operand_3,
                    item_data *type_final){

    //bool operand_1_to_int = false;
    //bool operand_3_to_int = false;
    bool operand_1_to_double = false;
    bool operand_3_to_double = false;

    if (rule == OPERAND){
        if (operand_1->item.type == IT_UNDEF){
            return ER_UNDEF_VAR;
        }
    }

    if (rule == LBR_NT_RBR){
        if (operand_2->item.type == IT_UNDEF){
            return ER_UNDEF_VAR;
        }
    }

    if (rule != OPERAND && rule != LBR_NT_RBR && rule != NT_EQ_NT){
        if (operand_1->item.type == IT_UNDEF || operand_3->item.type == IT_UNDEF){
            return ER_UNDEF_VAR;
        }
    }

    switch(rule){
        case OPERAND:
            type_final->type = operand_1->item.type;
            type_final->nil_possibility = operand_1->item.nil_possibility;
            type_final->defined = operand_1->item.defined;
            break;

        case LBR_NT_RBR:
            type_final->type = operand_2->item.type;
            type_final->nil_possibility = operand_2->item.nil_possibility;
            type_final->defined = true;
            break;

        case NT_DIV_NT:
        case NT_PLUS_NT:
        case NT_MINUS_NT:
        case NT_MUL_NT:

            // concatenation
            if (operand_1->item.type == IT_STRING && operand_3->item.type == IT_STRING && rule == NT_PLUS_NT){
                type_final->type = IT_STRING;
                NIL_POSSIBILITY_CHECK();
                type_final->nil_possibility = operand_1->item.nil_possibility;
                type_final->defined = true;
                break;
            }
            else if (operand_1->item.type == IT_INT && operand_3->item.type == IT_INT){
                type_final->type = IT_INT;
                NIL_POSSIBILITY_CHECK();
                type_final->nil_possibility = operand_1->item.nil_possibility;
                type_final->defined = true;
                break;
            }
            else if (operand_1->item.type == IT_DOUBLE && operand_3->item.type == IT_DOUBLE){
                type_final->type = IT_DOUBLE;
                NIL_POSSIBILITY_CHECK();
                type_final->nil_possibility = operand_1->item.nil_possibility;
                type_final->defined = true;
                break;
            }
            else if (operand_1->item.type == IT_STRING || operand_3->item.type == IT_STRING){
                return ER_TYPE_COMP;
            }
            else if(operand_1->item.type != operand_3->item.type){
                if(rule == NT_DIV_NT)
                    return ER_TYPE_COMP;
                if((operand_1->item.type == IT_INT && operand_3->item.type == IT_DOUBLE && !operand_1->item.defined) ||
                   (operand_3->item.type == IT_INT && operand_1->item.type == IT_DOUBLE && !operand_3->item.defined))
                    ; //todo translate Int2Double
                else
                    return ER_TYPE_COMP;
            }

            type_final->type = IT_DOUBLE;
            NIL_POSSIBILITY_CHECK();
            type_final->nil_possibility = operand_1->item.nil_possibility;

            if (operand_1->item.type == IT_INT){
                operand_1_to_double = true;
            }
            if (operand_3->item.type == IT_INT){
                operand_3_to_double = true;
            }

            break;
            // dealing with ?? monster
        case NT_COALESCE_NT:

            if (operand_1->item.type == IT_UNDEF && operand_3->item.type == IT_NIL) {
                // Operand_1 is undefined, but Operand_3 is nil
                type_final->type = IT_NIL;
            } else if (operand_1->item.type == IT_UNDEF) {
                // Operand_1 is undefined
                type_final->type = operand_3->item.type;
            } else {
                // Operand_1 is defined
                type_final->type = operand_1->item.type;
            }
            break;
            //dealing with ! monster
        case NT_F_UNWRAP:

            if (operand_1->item.type == IT_UNDEF || operand_1->item.type == IT_NIL) {
                return ER_UNDEF_VAR;
            }
            type_final->type = operand_1->item.type;
            break;
        case NT_NEQ_NT:
        case NT_LEQ_NT:
        case NT_LTN_NT:
        case NT_MEQ_NT:
        case NT_MTN_NT:
            // Type check
            if ((operand_1->item.type != operand_3->item.type && operand_1->item.type != IT_UNDEF) ||
                (!operand_1->item.nil_possibility && operand_3->item.type == IT_NIL)) {
                return ER_TYPE_COMP;
            }

            if ((operand_1->item.type == IT_NIL || operand_3->item.type == IT_NIL) &&
                    (!operand_1->item.nil_possibility && operand_3->item.type == IT_NIL)) {
                return ER_TYPE_COMP;
            }

            if (operand_1->item.type == IT_STRING) {
                if (rule == NT_LEQ_NT || rule == NT_MEQ_NT) {
                    // <= and >= are not supported for strings for some reason
                    return ER_TYPE_COMP;
                }
            }

            type_final->type = operand_3->item.type;
            break;
        case NT_EQ_NT:

            if(operand_3->item.type == IT_UNDEF)
                return ER_OTHER_SEM;

            if(operand_1->item.type != operand_3->item.type && operand_1->item.type != IT_UNDEF){
                return ER_TYPE_COMP;
            }
            if(!operand_1->item.nil_possibility && operand_3->item.nil_possibility)
                return ER_TYPE_COMP;

            type_final->type = operand_3->item.type;
            if(operand_1->item.type == IT_UNDEF){
                operand_1->item.type = type_final->type;
                operand_1->item.nil_possibility = operand_3->item.nil_possibility;
                operand_1->item.defined = true;
            }

            break;

        default:
            //todo change error;
            return ER_OTHER_SEM;

    }

    if (operand_1_to_double){
        ;
    }

    if (operand_3_to_double){
        ;
    }

    return ER_NONE;

}

int check_param(parser_data_t* data, int position){
    if(data->token_ptr->token_type == T_ID){
        Symbol* sym = NULL;
        if(table_count_elements_in_stack(data->tableStack) == 0)
            return ER_INTERNAL;
        if((sym = findSymbol(data->tableStack->top->table,data->token_ptr->attribute.string)) != NULL){
            data->id_type = &(sym->data);
            if(data->id_type->type != get_type_from_params(data->id,position) && data->id->type != IT_ANY){
                //TODO check error code
                return ER_PARAMS;
            }
            return ER_NONE;
        }

    } else{
        item_type type = get_type(data->token_ptr,data,false,false);
        if(type != IT_UNDEF && type != get_type_from_params(data->id,position))
            //TODO check error code
            return ER_PARAMS;
        return ER_NONE;
    }
    return ER_SYNTAX;
}

int check_func_call(parser_data_t *data, int position){
    int ret_code;
    GET_TOKEN()
    if(data->token_ptr->token_type != T_BRACKET_CLOSE && data->id->params->string == NULL)
        return ER_SEMAN;
    else if(data->token_ptr->token_type == T_ID){

//        printf("id_name[position]: %s AND %s\n",data->id->id_names[position],data->token_ptr->attribute.string);

        if(data->id->id_names && !strcmp(data->id->id_names[position],data->token_ptr->attribute.string)){
            //name_id : id/const
            VERIFY_TOKEN(T_COLON)
            GET_TOKEN()
            return check_param(data,position);

        }
        else{
            return check_param(data,position);
        }
    }
    
    return ER_NONE;
}
