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

char *tokens[] = {"T_ITS_NOT_A_TOKEN", "T_EXPONENT", "T_DEMICAL", "T_INT", "T_EQUALS",
                  "T_ASSIGMENT", "T_UNDERLINE", "T_KEYWORD", "T_ID",
                  "T_KEYWORD_TYPE_ID", "T_STRING", "T_MORE", "T_MORE_EQUAL",
                  "T_LESS", "T_LESS_EQUAL", "T_MINUS", "T_ARROW",
                  "T_TERN", "T_COMMENT_STRING", "T_COMMENT_BLOCK",
                  "T_NOT_EQUAL", "T_DIVISION", "T_COLON",
                  "T_PLUS", "T_COMMA", "T_CURVED_BRACKET_OPEN",
                  "T_CURVED_BRACKET_CLOSE", "T_SQUARE_BRACKET_OPEN",
                  "T_SQUARE_BRACKET_CLOSE", "T_BRACKET_OPEN", "T_BRACKET_CLOSE",
                  "T_NEW_LINE"};

void single_token(token_t_ptr token ,int line_cnt, token_type_t token_type){
    token->token_type = token_type;
    printf("Token %s found in line %i\n", tokens[token->token_type], line_cnt);
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