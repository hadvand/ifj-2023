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
                  "T_NEW_LINE","T_EOF","T_MULTIPLICATION"};

void single_token(token_t_ptr token ,int line_cnt, token_type_t token_type){
    token->token_type = token_type;
    token->line = line_cnt;
    printf("Token %s found in line %i\n", tokens[token->token_type], line_cnt);
}

void scanning_finish_with_error(token_t_ptr token, string_ptr additional_string, error_t* err, error_t error_type){
    delete_token(token);
    string_free(additional_string);
    (*err) = error_type;
}

token_t_ptr next_token(int *line_cnt, error_t* err_type){


    int c;
    
    *err_type = ER_NONE;

    //актуальное состояние конечного автомата
    state_t state = S_START;

    string_ptr additional_string;
    additional_string = NULL;

    token_t_ptr token = create_token();

    unsigned hex_count = 0;

    while((c = getc(stdin))){
        switch(state){
            case(S_START):
                //printf("current sym %c\n",c);
                if(c == '\n'){
                    single_token(token,*line_cnt, T_NEW_LINE);
                    (*line_cnt)++;
                }
                else if(isspace(c))
                    continue;
                else if(c == ':'){
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
                else if(c == '*'){
                    single_token(token,*line_cnt,T_MULTIPLICATION);
                }
                else if(c == '"'){
                    state = S_STRING_START;
                    continue;
                }
                else if(c == '='){
                    state = S_ASSINGMENT;
                    continue;
                }
                else if(c == '-'){
                    state = S_MINUS;
                    continue;
                }
                else if(c == '_'){
                    state = S_UNDERLINE;
                    continue;
                }
                else if(c == '?'){
                    state = S_POSSIBLY_TERN;
                    continue;
                }
                else if((c >= 65 && c <= 90) // a..z
                        || (c >= 97 && c <= 122) // A..Z
                        ){
                    state = S_ID;
                    continue;
                }
                else if (c == EOF){
                    single_token(token, *line_cnt,T_EOF);
                }
                else{
                    scanning_finish_with_error(token,additional_string,err_type,ER_SYNTAX);
                    return NULL;
                }
                return token;
                break;
            case(S_POSSIBLY_TERN):
                if(c == '?'){
                    single_token(token,*line_cnt,T_TERN);
                    return token;
                } else{
                    scanning_finish_with_error(token,additional_string,err_type,ER_SYNTAX);
                    return NULL;
                }
            case(S_UNDERLINE):
                if((c >= 48 && c <= 57) // 0..9
                    || (c >= 65 && c <= 90) // a..z
                    || (c >= 97 && c <= 122) // A..Z
                    || c == '_'){
                    state = S_ID;
                    continue;
                } else{
                    ungetc(c, stdin);
                    single_token(token,*line_cnt,T_UNDERLINE);
                    return token;
                }
            case(S_ID):
                if((c >= 48 && c <= 57) // 0..9
                   || (c >= 65 && c <= 90) // a..z
                   || (c >= 97 && c <= 122) // A..Z
                   || c == '_'){
                    //todo add symbol to string
                    continue;
                }
                else{
                    ungetc(c, stdin);
                    //todo controlling ID, is it ID or Keyword
                    single_token(token,*line_cnt,T_ID);
                    return token;
                }
            case(S_MINUS):
                if(c == '>'){
                    single_token(token,*line_cnt,T_ARROW);
                    return token;
                } else{
                    ungetc(c, stdin);
                    single_token(token, *line_cnt, T_MINUS);
                    return token;
                }
                break;
            case(S_ASSINGMENT): //todo ===
                if(c == '='){
                    single_token(token,*line_cnt,T_EQUALS);
                    return token;
                } else{
                    ungetc(c, stdin);
                    single_token(token,*line_cnt,T_ASSIGMENT);
                    return token;
                }
                break;
            case(S_STRING_START):
                if((additional_string = string_init()) == NULL){
                    scanning_finish_with_error(token,additional_string, err_type, ER_INTERNAL);
                    return NULL;
                }
                if(c == '"'){
                    single_token(token,*line_cnt,T_STRING);
                    token->attribute.string = additional_string->string;

                    string_free(additional_string);

                    return token;
                }
                else if(c == '\\'){
                    state = S_STRING_SPEC_SYMBOL;
                    continue;
                }
                else if(c >= PRINTABLE_MIN 
                    && c <= PRINTABLE_MAX){
                        if(!string_append(additional_string,c)){
                            scanning_finish_with_error(token,additional_string,err_type, ER_INTERNAL);
                            return NULL;
                        }
                    }
                break;
            case(S_STRING_SPEC_SYMBOL):
                if(c == 'u'){
                    state = S_STRING_START_HEX;
                    continue;
                }
                //todo else if for special symbols like \n \t \\ etc.
                else{
                    scanning_finish_with_error(token,additional_string,err_type,ER_LEX);
                    return NULL;
                }
                break;
            case(S_STRING_START_HEX):
                if(c == '{'){
                    state = S_STRING_HEX_OPEN;
                    continue;
                }
                else {
                    scanning_finish_with_error(token,additional_string,err_type,ER_SYNTAX);
                    return NULL;
                }

                break;
            case(S_STRING_HEX_OPEN):
                if((c >= 48 && c <= 57) /*numbers 0..9*/
                   || (c >= 65 && c <= 70) /* HEX numbers A..F */
                   || (c >= 97 && c <= 102)) /* HEX numbers a..f */{
                        //todo add number
                        state = S_STRING_HEX_NUMBER;
                        hex_count++;
                        continue;
                    } else {
                    scanning_finish_with_error(token,additional_string,err_type,ER_SYNTAX);
                    return NULL;
                }
                break;
            case(S_STRING_HEX_NUMBER):
                if(hex_count > 8){
                    //todo i dont understand what is it error type
                    scanning_finish_with_error(token,additional_string,err_type,ER_SYNTAX);
                    return NULL;
                }
                else if((c >= 48 && c <= 57) /*numbers 0..9*/
                    || (c >= 65 && c <= 70) /* HEX numbers A..F */
                    || (c >= 97 && c <= 102)) /* HEX numbers a..f */{
                        //todo add number
                        hex_count++;
                        continue;
                    }
                else if(c == '}'){
                    state = S_STRING_START;
                    continue;
                } else {
                    scanning_finish_with_error(token,additional_string,err_type,ER_SYNTAX);
                    return NULL;
                }
                break;
            default:
                break;
                
        }
    }
    single_token(token,*line_cnt,T_EOF);
    return token;
}