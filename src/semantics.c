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

#define UNUSED(x) (void)(x)

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

            {'>', '>', '<', '<', ' ', ' ', ' ', ' ', ' ', ' ', '>', '<', '<', '>', '<', '>'},   // |+|
            {'>', '>', '<', '<', ' ', ' ', ' ', ' ', ' ', ' ', '>', '<', '<', '>', '<', '>'},   // |-|
            {'>', '>', '>', '>', ' ', ' ', ' ', ' ', ' ', ' ', '>', '<', '<', '>', '<', '>'},   // |*|
            {'>', '>', '>', '>', ' ', ' ', ' ', ' ', ' ', ' ', '>', '<', '<', '>', '<', '>'},   // |/|
            {'<', '<', '<', '<', ' ', ' ', ' ', ' ', ' ', ' ', '>', '<', '<', '>', '<', '>'},   // |==|
            {'<', '<', '<', '<', ' ', ' ', ' ', ' ', ' ', ' ', '>', '<', '<', '>', '<', '>'},   // |!=|
            {'<', '<', '<', '<', ' ', ' ', ' ', ' ', ' ', ' ', '>', '<', '<', '>', '<', '>'},   // |<|
            {'<', '<', '<', '<', ' ', ' ', ' ', ' ', ' ', ' ', '>', '<', '<', '>', '<', '>'},   // |>|
            {'<', '<', '<', '<', ' ', ' ', ' ', ' ', ' ', ' ', '>', '<', '<', '>', '<', '>'},   // |<=|
            {'<', '<', '<', '<', ' ', ' ', ' ', ' ', ' ', ' ', '>', '<', '<', '>', '<', '>'},   // |>=|
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
        case T_TERN:
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


item_type get_type_from_params(item_data *data,int position, bool *nil_possibility){
    UNUSED(nil_possibility);
    *nil_possibility = false;
    if(!strcmp(data->id,"write"))
        return IT_ANY;
    if(data->params == NULL)
        return IT_UNDEF;
    if(position > data->params->last_index)
        return IT_UNDEF;
    switch (data->params->string[position]) {
        case 's':
            return IT_STRING;
        case 'S':
            *nil_possibility = true;
            return IT_STRING;
        case 'd':
            return IT_DOUBLE;
        case 'D':
            *nil_possibility = true;
            return IT_DOUBLE;
        case 'i':
            return IT_INT;
        case 'I':
            *nil_possibility = true;
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
    else if(count_symbols_before_stop == 2 && stop_is_founded){
        op1 = stack.top->next;
        op2 = stack.top;
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
    //init item_data
    item_data tmp_item;

    //tmp_item.it_is_nil = false;
    stack_push(&stack, tmp_item, DOLLAR);

    data->id_type = NULL;
    bool success = false;

    bool last_action_is_reduce = false;
    t_stack_elem* top_terminal;
    Precedence_table_symbol actual_symbol;


    do {
        tmp_item.type = IT_UNDEF;
        tmp_item.nil_possibility = false;
        tmp_item.defined = false;
        top_terminal = stack_top_terminal(&stack);
        if(top_terminal->symbol == IDENTIFIER && data->id_type != NULL && data->id_type->is_function) {

            if (data->token_ptr->token_type == T_BRACKET_OPEN) {

                data->param_index = 0;

                if ((ret_code = call_params(data))) return ret_code;

                if (data->token_ptr->token_type != T_BRACKET_CLOSE) return ER_SYNTAX;

                GET_TOKEN()
            }
            else
                return ER_SYNTAX;
        }
        actual_symbol = convert_token_into_symbol(data,last_action_is_reduce);
        if(data->token_ptr->token_type == T_KEYWORD && data->token_ptr->attribute.keyword == k_let && !data->eol_flag)
            return ret_code;
        if(top_terminal == NULL){
#ifdef SEM_DEBUG
            printf("semantic analysis finish with error\n");
#endif
            FREE(ER_INTERNAL);
        }

        switch (precedence_table[get_index(top_terminal->symbol)][get_index(actual_symbol)]) {
            case '=':
                tmp_item.type = get_type(data->token_ptr,data,&(tmp_item.nil_possibility),&(tmp_item.defined));
                if(actual_symbol == IDENTIFIER && data->token_ptr->token_type == T_ID && !tmp_item.defined)
                    return ER_UNDEF_VAR;
                //tmp_item.it_is_nil = data->token_ptr->attribute.keyword == k_nil;
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
                if(actual_symbol == IDENTIFIER && data->token_ptr->token_type == T_ID && !tmp_item.defined)
                    return ER_UNDEF_VAR;
                //tmp_item.it_is_nil = (data->token_ptr->attribute.keyword == k_nil || (data->id_type != NULL && data->id_type->it_is_nil));
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
                if(actual_symbol == EX_MARK)
                    last_action_is_reduce = true;
                else
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
                    FREE(ER_SEMAN);
                }
                last_action_is_reduce = false;
                break;
        }

    }while(!success);

    if(data->id == NULL)
        return ER_OTHER_SEM_2;

    t_stack_elem op1;
    op1.symbol = N_TERMINAL;
    op1.item = *(data->id);
    t_stack_elem op2;
    op2.symbol = ASSIGMENT;
    t_stack_elem op3;
    op3 = *(stack.top);
    item_data final_type;
    if((ret_code =check_semantics(NT_AS_NT,&op1,&op2,&op3,&final_type))){
#ifdef SEM_DEBUG
        printf("semantic analysis finish with error\n");
#endif
        FREE(ret_code);
    }
    *(data->id) = op1.item;
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
        case(2):
            if(operand_1->symbol == N_TERMINAL && operand_2->symbol == EX_MARK){
                return NT_F_UNWRAP;
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
                    case ASSIGMENT:
                        return NT_AS_NT;

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

    type_final->type = IT_UNDEF;
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

    if (rule != OPERAND && rule != LBR_NT_RBR && rule != NT_AS_NT){
        if (operand_1->item.type == IT_UNDEF || ( operand_3 != NULL && operand_3->item.type == IT_UNDEF)){
            return ER_UNDEF_VAR;
        }
    }

    switch(rule){
        case OPERAND:
            type_final->type = operand_1->item.type;
            type_final->nil_possibility = operand_1->item.nil_possibility;
            type_final->defined = operand_1->item.defined;
            //type_final->it_is_nil = operand_1->item.it_is_nil;
            break;

        case LBR_NT_RBR:
            type_final->type = operand_2->item.type;
            type_final->nil_possibility = operand_2->item.nil_possibility;
            type_final->defined = true;
            //type_final->it_is_nil = operand_2->item.it_is_nil;
            break;

        case NT_DIV_NT:
        case NT_PLUS_NT:
        case NT_MINUS_NT:
        case NT_MUL_NT:

//            if(operand_1->item.type == IT_NIL || operand_3->item.type == IT_NIL)
//                return ER_OTHER_SEM;
            //type_final->it_is_nil = false;
            if(operand_1->item.nil_possibility || operand_3->item.nil_possibility)
                return ER_TYPE_COMP;
            // concatenation
            if (operand_1->item.type == IT_STRING && operand_3->item.type == IT_STRING && rule == NT_PLUS_NT){
                type_final->type = IT_STRING;
                type_final->nil_possibility = operand_1->item.nil_possibility;
                type_final->defined = true;
                break;
            }
            else if (operand_1->item.type == IT_INT && operand_3->item.type == IT_INT){
                type_final->type = IT_INT;
                type_final->nil_possibility = operand_1->item.nil_possibility;
                type_final->defined = true;
                break;
            }
            else if (operand_1->item.type == IT_DOUBLE && operand_3->item.type == IT_DOUBLE){
                type_final->type = IT_DOUBLE;
                type_final->nil_possibility = operand_1->item.nil_possibility;
                type_final->defined = true;
                break;
            }
            else if (operand_1->item.type == IT_STRING || operand_3->item.type == IT_STRING){
                return ER_TYPE_COMP;
            }
            else if(operand_1->item.type != operand_3->item.type){
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

            if(operand_1->item.type != operand_3->item.type){
                if(((operand_1_to_double = operand_1->item.type == IT_INT) && operand_3->item.type == IT_DOUBLE && !operand_1->item.defined) ||
                   ((operand_3_to_double = operand_3->item.type == IT_INT) && operand_1->item.type == IT_DOUBLE && !operand_3->item.defined)){
                    //todo translate Int2Double


                    type_final->type = IT_DOUBLE;
                }
                else if(operand_1->item.type == IT_NIL){
                    //if(operand_3->item.type == IT_NIL)
                    type_final->type = operand_3->item.type;
                }
                else
                    return ER_TYPE_COMP;
            }

            if(type_final->type == IT_UNDEF){
                if((operand_1->item.nil_possibility || !operand_1->item.defined) && (operand_3->item.nil_possibility || !operand_3->item.defined)){
                    type_final->type = operand_1->item.type;
                } else
                    return ER_TYPE_COMP;
            }

//            if (operand_1->item.type == IT_UNDEF && operand_3->item.type == IT_NIL) {
//                // Operand_1 is undefined, but Operand_3 is nil
//                type_final->type = IT_NIL;
//            } else if (operand_1->item.type == IT_UNDEF) {
//                // Operand_1 is undefined
//                type_final->type = operand_3->item.type;
//            } else {
//                // Operand_1 is defined
//                type_final->type = operand_1->item.type;
//            }
            break;
            //dealing with ! monster
        case NT_F_UNWRAP:

            if (operand_1->item.type == IT_UNDEF) {
                return ER_UNDEF_VAR;
            }
            if(operand_1->item.type == IT_NIL)
                return ER_TYPE_COMP;
            //NIL_POSSIBILITY_CHECK();
            type_final->nil_possibility = false;
            type_final->defined = true;
            type_final->type = operand_1->item.type;

            //type_final->it_is_nil = operand_2->item.it_is_nil;
            break;
        case NT_EQ_NT:
        case NT_NEQ_NT:
            if(operand_1->item.type != operand_3->item.type){
                if((operand_1->item.type == IT_INT && operand_3->item.type == IT_DOUBLE && !operand_1->item.defined) ||
                   (operand_3->item.type == IT_INT && operand_1->item.type == IT_DOUBLE && !operand_3->item.defined))
                    ; //todo translate Int2Double
                else
                    return ER_TYPE_COMP;
            }

            type_final->type = operand_3->item.type;
            type_final->nil_possibility = false;
            //type_final->it_is_nil = false;
            type_final->defined = true;
            break;
        case NT_LEQ_NT:
        case NT_LTN_NT:
        case NT_MEQ_NT:
        case NT_MTN_NT:
            // Type check
            if(operand_1->item.type != operand_3->item.type){
                if((operand_1->item.type == IT_INT && operand_3->item.type == IT_DOUBLE && !operand_1->item.defined) ||
                   (operand_3->item.type == IT_INT && operand_1->item.type == IT_DOUBLE && !operand_3->item.defined))
                    ; //todo translate Int2Double
                else
                    return ER_TYPE_COMP;
            }

            if(operand_1->item.nil_possibility || operand_3->item.nil_possibility)
                return ER_TYPE_COMP;

            if ((operand_1->item.type == IT_NIL || operand_3->item.type == IT_NIL)) {
                return ER_TYPE_COMP;
            }

//            if (operand_1->item.type == IT_STRING) {
//                if (rule == NT_LEQ_NT || rule == NT_MEQ_NT) {
//                    // <= and >= are not supported for strings for some reason
//                    return ER_TYPE_COMP;
//                }
//            }

            type_final->type = operand_3->item.type;
            type_final->nil_possibility = false;
            //type_final->it_is_nil = false;
            type_final->defined = true;
            break;
        case NT_AS_NT:

            if(operand_3->item.type == IT_UNDEF)
                return ER_OTHER_SEM;

            if(operand_1->item.type != operand_3->item.type && operand_1->item.type != IT_UNDEF && operand_3->item.type != IT_NIL){
                if(operand_1->item.type == IT_DOUBLE && operand_3->item.type == IT_INT && !operand_3->item.defined)
                    ; // TODO Generate int2Double
                else
                    return ER_TYPE_COMP;
            }
            if(operand_1->item.type != IT_UNDEF &&
                !operand_1->item.nil_possibility &&
                    operand_3->item.nil_possibility)
                return ER_TYPE_COMP;

            if(operand_1->item.type == IT_UNDEF && operand_3->item.type == IT_NIL)
                return ER_OTHER_SEM;

//            if(operand_3->item.type == IT_NIL)
//                operand_1->item.it_is_nil = true;
            //type_final->type = operand_3->item.type;
            if(operand_1->item.type == IT_UNDEF){
                operand_1->item.type = operand_3->item.type;
                operand_1->item.nil_possibility = operand_3->item.nil_possibility;
            }
            operand_1->item.defined = true;
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
            bool param_nil_possibility = false;
            if((sym->data.type != get_type_from_params(data->id_type,position, &param_nil_possibility)
                || sym->data.nil_possibility != param_nil_possibility)
                && data->id_type->type != IT_ANY){
                return ER_PARAMS;
            }
            return ER_NONE;
        }

    } else{
        bool param_nil_possibility = false;
        item_type type = get_type(data->token_ptr,data,&param_nil_possibility,false);
        item_type param_type = get_type_from_params(data->id_type,position,&param_nil_possibility);
        if(type != IT_UNDEF
            && type != param_type
            && param_type != IT_ANY) {
            if(type == IT_NIL && param_nil_possibility)
                return ER_NONE;
            return ER_PARAMS;
        }
        return ER_NONE;
    }
    return ER_PARAMS;
}

int check_func_call(parser_data_t *data, int position){
    int ret_code;
//    if(position >= data->id_type->params->last_index)
//        return ER_PARAMS;
    bool its_write = !strcmp(data->id_type->id,"write");
    GET_TOKEN()
    if(data->token_ptr->token_type != T_BRACKET_CLOSE && data->id_type->params->string == NULL)
        return ER_PARAMS;
    else if(data->token_ptr->token_type == T_ID){

//        printf("id_name[position]: %s AND %s\n",data->id->id_names[position],data->token_ptr->attribute.string);
        if(data->id_type->id_names && !strcmp(data->id_type->id_names[position],data->token_ptr->attribute.string)){
            //name_id : id/const
            VERIFY_TOKEN(T_COLON)
            GET_TOKEN()
            return check_param(data,position);

        }
        else if ((data->id_type->id_names && !strcmp(data->id_type->id_names[position],"_")) || its_write){
            return check_param(data,position);
        } else
            return ER_PARAMS;
    }
    else if(data->token_ptr->token_type == T_INT
            || data->token_ptr->token_type == T_DEMICAL
            || data->token_ptr->token_type == T_STRING
            || (data->token_ptr->token_type == T_KEYWORD && data->token_ptr->attribute.keyword == k_nil)){
        if(position+1 > data->id_type->params->last_index && !its_write)
            return ER_PARAMS;
        else if(data->id_type->id_names && strcmp(data->id_type->id_names[position],"_"))
            return ER_OTHER_SEM_2;
        return check_param(data,position);
    }
    else if(data->token_ptr->token_type == T_BRACKET_CLOSE && (data->id_type->params == NULL || data->id_type->params->last_index == 0))
        return ER_NONE;
    return ER_PARAMS;
}

