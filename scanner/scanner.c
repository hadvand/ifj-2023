#include "scanner.h"

token_t_ptr create_token(){
    token_t_ptr token;
    token = (token_t_ptr) malloc(sizeof(struct token));
    token->token_type = T_ITS_NOT_A_TOKEN;
    return token;
}

void delete_token(token_t_ptr token){
    //todo
    if(token != NULL){
        if(token->token_type == T_STRING)
            free(token->attribute.string);
        free(token);
    }
        
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
                  "T_NEW_LINE","T_EOF"};

void single_token(token_t_ptr token ,int line_cnt, token_type_t token_type){
    token->token_type = token_type;
    token->line = line_cnt;
    printf("Token %s found in line %i\n", tokens[token->token_type], line_cnt);
}

void cleaning(token_t_ptr token, string_ptr additional_string){
    delete_token(token);
    string_free(additional_string);
}

token_t_ptr next_token(int *line_cnt){


    int c;

    //актуальное состояние конечного автомата
    state_t state = S_START;

    string_ptr additional_string;
    additional_string = NULL;

    token_t_ptr token = create_token();

    while((c = getc(stdin)) != EOF){
        switch(state){
            case(S_START):
                //printf("curent sym %c\n",c);
                if(c == ':'){
                    single_token(token,*line_cnt, T_COLON);
                }
                else if(c == '+'){
                    single_token(token,*line_cnt, T_PLUS);
                }
                else if(c == ','){
                    single_token(token, *line_cnt, T_COMMA);
                }
                else if(c == '{'){
                    single_token(token, *line_cnt, T_CURVED_BRACKET_OPEN);
                }
                else if(c == '}'){
                    single_token(token, *line_cnt, T_CURVED_BRACKET_CLOSE);
                }
                else if(c == '['){
                    single_token(token, *line_cnt, T_SQUARE_BRACKET_OPEN);
                }
                else if(c == ']'){
                    single_token(token, *line_cnt, T_SQUARE_BRACKET_CLOSE);
                }
                else if(c == '('){
                    single_token(token, *line_cnt, T_BRACKET_OPEN);
                }
                else if(c == ')'){
                    single_token(token, *line_cnt, T_BRACKET_CLOSE);
                }
                else if(c == '\n'){
                    (*line_cnt)++;
                    single_token(token,*line_cnt, T_NEW_LINE);
                }
                else if(c == '"'){
                    state = S_STRING_START;
                    continue;
                }
                else{
                    continue;
                }
                return token;
                break;
            case(S_STRING_START):
                if((additional_string = string_init()) == NULL){
                    cleaning(token,additional_string);
                    return NULL;
                }
                if(c == '"'){
                    single_token(token,*line_cnt,T_STRING);
                    token->attribute.string = additional_string->string;

                    string_free(additional_string);

                    return token;
                }
                else if(c == '\\')
                    state = S_STRING_SPEC_SYMBOL;
                else if(c >= PRINTABLE_MIN 
                    && c <= PRINTABLE_MAX){
                        if(!string_append(additional_string,c)){
                            cleaning(token,additional_string);
                            return NULL;
                        }
                    }
                break;
            default:
                break;
                
        }
    }
    token->token_type = T_EOF;
    token->line = *line_cnt;
    return token;
}