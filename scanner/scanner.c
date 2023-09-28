#include "scanner.h"

token_t_ptr create_token(){
    token_t_ptr token;
    token = (token_t_ptr) malloc(sizeof(struct token));
    token->token_type = T_ITS_NOT_A_TOKEN;
    return token;
}

void delete_token(token_t_ptr token){
    //todo
    if(token != NULL)
        free(token);
}

void single_token(token_t_ptr token ,int line_cnt, token_type_t token_type){
    token->token_type = token_type;
    printf("Token %d finded in line %i\n",token->token_type, line_cnt);
}

token_t_ptr next_token(){


    int c;

    //актуальное состояние конечного автомата
    state_t state = S_START;

    int line_cnt = 0;

    token_t_ptr token = create_token();

    while((c = getc(stdin)) != EOF){
        switch(state){
            case(S_START):
                printf("curent sym %c\n",c);

                if(c == '\n'){
                    single_token(token,line_cnt, T_NEW_LINE);
                    line_cnt++;
                }
                if(c == ':'){
                    single_token(token,line_cnt, T_COLON);
                }
                if(c == '+'){
                    single_token(token,line_cnt, T_PLUS);
                }
                if(c == ','){
                    single_token(token, line_cnt, T_COMMA);
                }
                if(c == '{'){
                    single_token(token, line_cnt, T_CURVED_BRACKET_OPEN);
                }
                if(c == '}'){
                    single_token(token, line_cnt, T_CURVED_BRACKET_CLOSE);
                }
                if(c == '['){
                    single_token(token, line_cnt, T_SQUARE_BRACKET_OPEN);
                }
                if(c == ']'){
                    single_token(token, line_cnt, T_SQUARE_BRACKET_CLOSE);
                }
                if(c == '('){
                    single_token(token, line_cnt, T_BRACKET_OPEN);
                }
                if(c == ')'){
                    single_token(token, line_cnt, T_BRACKET_CLOSE);
                }
                else{
                    continue;
                }
                return token;
                break;
            default:
                break;
                
        }
    }
    return NULL;
}