#include "semantics.h"
#include "../stack.h"
#include "../structures/error.h"

#define TABLE_SIZE 16


t_stack stack;

static Precedence_rules check_rule(int number, t_stack_elem* operand_1, t_stack_elem* operand_2, t_stack_elem* operand_3);

#define FREE(_error_code) \
    do{                   \
        stack_free(&stack); \
        \
        return _error_code;   \
        }\
    while(0)

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


Precedence_table_symbol convert_token_into_symbol(struct token* token){

    switch(token -> token_type){

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
            return IDENTIFIER;
        case T_INT:
            return INT_NUMBER;
        case T_DEMICAL:
            return DOUBLE_NUMBER;
        case T_STRING:
            return STRING;

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

int func_call(parser_data_t* data) {
    (void)(data);
    return ER_NONE;
}


item_type get_type(struct token* token, parser_data_t * data){

    Symbol* symbol;

    switch(token -> token_type){
        case T_ID:
            symbol = findSymbol(data->local_table, token->attribute.string);
            if (symbol == NULL)
                return IT_UNDEF;
            return symbol->data.type;
        case T_INT:
            return IT_INT;
        case T_DEMICAL:
            return IT_DOUBLE;
        case T_STRING:
            return IT_STRING;
        default:
            return IT_UNDEF;
    }
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

    //int error_code;

    t_stack_elem *op1 = NULL;
    t_stack_elem *op2 = NULL;
    t_stack_elem *op3 = NULL;
    item_data item;
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

    if(rule == NOT_A_RULE)
        return ER_SYNTAX;
    else{
        //todo check semantics

        for (int i = count_symbols_before_stop + 1; i > 0 ; i--) {
            stack_pop(&stack);
        }
        stack_push(&stack,item,N_TERMINAL);
    }

    return ER_NONE;
}

int expression(parser_data_t* data){
    int error_code = ER_SYNTAX;

#ifdef SEM_DEBUG
    printf("semantic analysis starts\n");
#endif

    stack_init(&stack);
    //TODO init item_data
    item_data tmp_item;
    tmp_item.type = IT_UNDEF;
    stack_push(&stack, tmp_item, DOLLAR);

    bool success = false;

    t_stack_elem* top_terminal;
    Precedence_table_symbol actual_symbol;

    do {
        actual_symbol = convert_token_into_symbol(data->token_ptr);
        top_terminal = stack_top_terminal(&stack);

        if(top_terminal == NULL){
#ifdef SEM_DEBUG
            printf("semantic analysis finish with error\n");
#endif
            FREE(ER_INTERNAL);
        }

        bool flag;

        switch (precedence_table[get_index(top_terminal->symbol)][get_index(actual_symbol)]) {
            case '=':
                tmp_item.type = get_type(data->token_ptr,data);
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

                data->token_ptr = next_token(&(data->line_cnt),&error_code, &flag);
                if(error_code != ER_NONE)
                {
#ifdef SEM_DEBUG
                    printf("semantic analysis finish with error\n");
#endif
                    FREE(ER_INTERNAL);
                }


                break;
            case '<':
                if(!stack_push_after_top_term(&stack,tmp_item,STOP))
                {
#ifdef SEM_DEBUG
                    printf("semantic analysis finish with error\n");
#endif
                    FREE(ER_INTERNAL);
                }
                tmp_item.type = get_type(data->token_ptr,data);
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

                data->token_ptr = next_token(&(data->line_cnt),&error_code, &flag);
                if(error_code != ER_NONE)
                {
#ifdef SEM_DEBUG
                    printf("semantic analysis finish with error\n");
#endif
                    FREE(ER_INTERNAL);
                }
                break;
            case '>':
                if(reduce())
                {
#ifdef SEM_DEBUG
                    printf("semantic analysis finish with error\n");
#endif
                    FREE(ER_INTERNAL);
                }

#ifdef SEM_DEBUG
                stack_print_all_symbols(&stack);
#endif
                break;
            default:
                if(actual_symbol == DOLLAR && top_terminal->symbol == DOLLAR)
                    success = true;
                else
                {
#ifdef SEM_DEBUG
                    printf("semantic analysis finish with error\n");
#endif
                    FREE(ER_INTERNAL);
                }
                break;
        }

    }while(!success);

#ifdef SEM_DEBUG
    printf("semantic analysis finish\n");
#endif

    return error_code;
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
                        return NT_NOT_NT;

                    default:
                        return NOT_A_RULE;

                }

            }

            return NOT_A_RULE;
    }

    return NOT_A_RULE;
}


