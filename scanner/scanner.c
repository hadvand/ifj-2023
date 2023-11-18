#include "../structures/error.h"
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
                  "T_KEYWORD_NIL_POSSIBILITY", "T_EXCLAMATION_MARK", "T_STRING", "T_MORE", "T_MORE_EQUAL",
                  "T_LESS", "T_LESS_EQUAL", "T_MINUS", "T_ARROW",
                  "T_TERN", "T_COMMENT_STRING", "T_COMMENT_BLOCK",
                  "T_NOT_EQUAL", "T_DIVISION", "T_COLON",
                  "T_PLUS", "T_COMMA", "T_CURVED_BRACKET_OPEN",
                  "T_CURVED_BRACKET_CLOSE", "T_SQUARE_BRACKET_OPEN",
                  "T_SQUARE_BRACKET_CLOSE", "T_BRACKET_OPEN", "T_BRACKET_CLOSE",
                  "T_NEW_LINE","T_EOF","T_MULTIPLICATION","S_NESTED_COMMENT"};
// todo: add "by" and "of"
const char *keywords[] = {"Double","else","func","if","Int","let","nil","return","String","var","while","qm_Double","qm_Int","qm_String"};


void single_token(token_t_ptr token ,int line_cnt, token_type_t token_type,string_ptr string){
    token->token_type = token_type;
    token->line = line_cnt;
    printf("Token %s found in line %i", tokens[token->token_type], line_cnt);

    switch (token->token_type) {
        case(T_STRING):
            printf(": content is %s\n",token->attribute.string);
            break;
        case(T_INT):
            printf(": content is %i\n",token->attribute.integer);
            break;
        case(T_DEMICAL):
            printf(": content is %f\n",token->attribute.decimal);
            break;
        case(T_KEYWORD):
            printf(": content is %s\n",keywords[token->attribute.keyword]);
            break;
        case(T_ID):
            printf(": content is %s\n",token->attribute.string);
            break;
        default:
            printf("\n");
            break;
    }

    string_free(string);
    string = NULL;
}

void scanning_finish_with_error(token_t_ptr token, string_ptr additional_string, int* err, int error_type){
    delete_token(token);
    string_free(additional_string);
    (*err) = error_type;
}

bool keyword_control(token_t_ptr token, string_ptr add_string){
    char *keywords[] = {"Double","else","func",
                        "if","Int","let",
                        "nil","return","String",
                        "var","while"};
    char *keywords_with_qmark[] = {"Double?","Int?","String?"};

    for (size_t i = 0; i < sizeof(keywords) / sizeof(keywords[0]); i++) {
        if (strcmp(add_string->string, keywords[i]) == 0) {
            token->attribute.keyword = i;
            return true;
        }
    }
    for (size_t i = 0; i < sizeof(keywords_with_qmark) / sizeof(keywords_with_qmark[0]); i++) {
        if (strcmp(add_string->string, keywords_with_qmark[i]) == 0) {
            token->attribute.keyword = i + COUNT_KEYWORDS_BEFORE_QMARK;
            return true;
        }
    }
    return false;
}

token_t_ptr next_token(int *line_cnt, int* err_type){


    int c;

    int comment_count = 0;

    bool is_multiline = false;
    state_t multiline_state;
    
    *err_type = ER_NONE;

    //current status of fsm
    state_t state = S_START;

    string_ptr additional_string, unicode;
    additional_string = NULL;

    if((unicode = string_init()) == NULL){
        *err_type = ER_INTERNAL;
        return NULL;
    }

    token_t_ptr token = create_token();

    unsigned hex_count = 0;

    while((c = getc(stdin))){
        //todo 2 more tokens missing
        switch(state){
            case(S_START):
                //printf("current sym %c\n",c);
                if(c == '\n'){
                    single_token(token,*line_cnt, T_NEW_LINE,additional_string);
                    (*line_cnt)++;
                }
                else if(isspace(c))
                    continue;
                else if(c == ':'){
                    single_token(token,*line_cnt, T_COLON,additional_string);
                }
                else if(c == '+'){
                    single_token(token,*line_cnt, T_PLUS,additional_string);
                }
                else if(c == ','){
                    single_token(token, *line_cnt, T_COMMA,additional_string);
                }
                else if(c == '{'){
                    single_token(token, *line_cnt, T_CURVED_BRACKET_OPEN,additional_string);
                }
                else if(c == '}'){
                    single_token(token, *line_cnt, T_CURVED_BRACKET_CLOSE,additional_string);
                }
                else if(c == '['){
                    single_token(token, *line_cnt, T_SQUARE_BRACKET_OPEN,additional_string);
                }
                else if(c == ']'){
                    single_token(token, *line_cnt, T_SQUARE_BRACKET_CLOSE,additional_string);
                }
                else if(c == '('){
                    single_token(token, *line_cnt, T_BRACKET_OPEN,additional_string);
                }
                else if(c == ')'){
                    single_token(token, *line_cnt, T_BRACKET_CLOSE,additional_string);
                }
                else if(c == '*'){
                    single_token(token,*line_cnt,T_MULTIPLICATION,additional_string);
                }
                else if(c == '/'){
                    state = S_DIVISION;
                    continue;
                }
                else if(c == '>'){
                    state = S_MORE;
                    continue;
                }
                else if(c >= 48 && c <= 57){
                    if(additional_string == NULL){
                        if((additional_string = string_init()) == NULL){
                            scanning_finish_with_error(token,additional_string, err_type, ER_INTERNAL);
                            return NULL;
                        }
                    }
                    if(!string_append(additional_string,c)){
                        scanning_finish_with_error(token,additional_string,err_type, ER_INTERNAL);
                        return NULL;
                    }
                    state = S_INT;
                    continue;
                }
                else if(c == '<'){
                    state = S_LESS;
                    continue;
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
                else if(c == '!'){
                    state = S_EXCLAMATION_MARK;
                    continue;
                }
                else if(c == '?'){
                    state = S_POSSIBLY_TERN;
                    continue;
                }
                else if((c >= 65 && c <= 90) // a..z
                        || (c >= 97 && c <= 122) // A..Z
                        ){
                    if(additional_string == NULL){
                        if((additional_string = string_init()) == NULL){
                            scanning_finish_with_error(token,additional_string, err_type, ER_INTERNAL);
                            return NULL;
                        }
                    }
                    if(!string_append(additional_string,c)){
                        scanning_finish_with_error(token,additional_string,err_type, ER_INTERNAL);
                        return NULL;
                    }
                    state = S_ID;
                    continue;
                }
                else if (c == EOF){
                    single_token(token, *line_cnt,T_EOF,additional_string);
                }
                else{
                    scanning_finish_with_error(token,additional_string,err_type,ER_SYNTAX);
                    return NULL;
                }
                return token;

            case(S_POSSIBLY_TERN):
                if(c == '?'){
                    single_token(token,*line_cnt,T_TERN,additional_string);
                    return token;
                } else{
                    scanning_finish_with_error(token,additional_string,err_type,ER_SYNTAX);
                    return NULL;
                }
            case(S_DIVISION):
                if(c == '/'){
                    state = S_COMMENT_STRING;
                    continue;
                }
                else if(c == '*'){
                    state = S_COMMENT_BLOCK_START;
                    comment_count++;
                    continue;
                } else{
                    ungetc(c,stdin);
                    single_token(token,*line_cnt,T_DIVISION,additional_string);
                    return token;
                }
            case (S_COMMENT_STRING):
                if(c == '\n' || c == EOF){
                    ungetc(c,stdin);
                    single_token(token,*line_cnt,T_COMMENT_STRING,additional_string);
                    return token;
                }
                continue;
            case(S_COMMENT_BLOCK_START):
                if(c == '*'){
                    state = S_COMMENT_BLOCK_BOSSIBLY_FINISHED;
                }
                else if(c == '/'){
                    state = S_NESTED_COMMENT;
                }
                else if (c == EOF){
                    scanning_finish_with_error(token,additional_string,err_type,ER_SYNTAX);
                    return NULL;
                }
                else if (c == '\n')
                    (*line_cnt)++;
                continue;
            case (S_NESTED_COMMENT):
                if(c == '*'){
                    comment_count++;
                }
                if(c == EOF){
                    scanning_finish_with_error(token,additional_string,err_type,ER_SYNTAX);
                    return NULL;
                }
                state = S_COMMENT_BLOCK_START;
                continue;
            case(S_COMMENT_BLOCK_BOSSIBLY_FINISHED):
                if(c == '/'){
                    comment_count--;
                    if(comment_count == 0){
                        single_token(token,*line_cnt,T_COMMENT_BLOCK,additional_string);
                        return token;
                    } else
                        state = S_COMMENT_BLOCK_START;
                }
                else if (c == EOF){
                    scanning_finish_with_error(token,additional_string,err_type,ER_SYNTAX);
                    return NULL;
                }
                else{
                    state = S_COMMENT_BLOCK_START;
                }
                continue;
            case(S_MORE):
                if(c == '='){
                    single_token(token,*line_cnt,T_MORE_EQUAL,additional_string);
                    return token;
                } else{
                    ungetc(c,stdin);
                    single_token(token,*line_cnt,T_MORE,additional_string);
                    return token;
                }
            case(S_LESS):
                if(c == '='){
                    single_token(token,*line_cnt,T_LESS_EQUAL,additional_string);
                    return token;
                } else{
                    ungetc(c,stdin);
                    single_token(token,*line_cnt,T_LESS,additional_string);
                    return token;
                }
            case(S_EXCLAMATION_MARK):
                if(c == '='){
                    single_token(token,*line_cnt,T_NOT_EQUAL,additional_string);
                    return token;
                } else{
                    ungetc(c,stdin);
                    single_token(token,*line_cnt,T_EXCLAMATION_MARK,additional_string);
                    return token;
                }
            case(S_INT):
                if(c >= 48 && c <= 57){
                    if(!string_append(additional_string,c)){
                        scanning_finish_with_error(token,additional_string,err_type, ER_INTERNAL);
                        return NULL;
                    }
                    state = S_INT;
                    continue;
                }
                else if(c == 69 || c == 101){
                    if(!string_append(additional_string,c)){
                        scanning_finish_with_error(token,additional_string,err_type, ER_INTERNAL);
                        return NULL;
                    }
                    state = S_EXPONENT_POSSIBLY;
                    continue;
                }
                else if(c == '.'){
                    if(!string_append(additional_string,c)){
                        scanning_finish_with_error(token,additional_string,err_type, ER_INTERNAL);
                        return NULL;
                    }
                    state = S_NUMBER_POINT;
                    continue;
                }
                else{
                    ungetc(c, stdin);
                    token->attribute.integer = atoi(additional_string->string);
                    single_token(token,*line_cnt,T_INT,additional_string);
                    return token;
                }
            case(S_NUMBER_POINT):
                if(c >=48 && c<= 57){
                    if(!string_append(additional_string,c)){
                        scanning_finish_with_error(token,additional_string,err_type, ER_INTERNAL);
                        return NULL;
                    }
                    state = S_DEMICAL;
                    continue;
                }
                else{
                    scanning_finish_with_error(token,additional_string,err_type,ER_SYNTAX);
                    return NULL;
                }
            case(S_DEMICAL):
                if(c >=48 && c<= 57){
                    if(!string_append(additional_string,c)){
                        scanning_finish_with_error(token,additional_string,err_type, ER_INTERNAL);
                        return NULL;
                    }
                    state = S_DEMICAL;
                    continue;
                }
                else if(c == 69 || c == 101){
                    if(!string_append(additional_string,c)){
                        scanning_finish_with_error(token,additional_string,err_type, ER_INTERNAL);
                        return NULL;
                    }
                    state = S_EXPONENT_POSSIBLY;
                    continue;
                }
                else{
                    ungetc(c, stdin);
                    token->attribute.decimal = strtod(additional_string->string,NULL);
                    single_token(token,*line_cnt,T_DEMICAL,additional_string);
                    return token;
                }
            case(S_EXPONENT_POSSIBLY):
                if(c >=48 && c<= 57){
                    if(!string_append(additional_string,c)){
                        scanning_finish_with_error(token,additional_string,err_type, ER_INTERNAL);
                        return NULL;
                    }
                    state = S_EXPONENT;
                    continue;
                }
                else if(c == '+' || c == '-'){
                    if(!string_append(additional_string,c)){
                        scanning_finish_with_error(token,additional_string,err_type, ER_INTERNAL);
                        return NULL;
                    }
                    state = S_EXPONENT_SING;
                    continue;
                }
                else{
                    scanning_finish_with_error(token,additional_string,err_type,ER_SYNTAX);
                    return NULL;
                }
            case(S_EXPONENT_SING):
                if(c >=48 && c<= 57){
                    if(!string_append(additional_string,c)){
                        scanning_finish_with_error(token,additional_string,err_type, ER_INTERNAL);
                        return NULL;
                    }
                    state = S_EXPONENT;
                    continue;
                }
                else{
                    scanning_finish_with_error(token,additional_string,err_type,ER_SYNTAX);
                    return NULL;
                }
            case(S_EXPONENT):
                if(c >=48 && c<= 57){
                    if(!string_append(additional_string,c)){
                        scanning_finish_with_error(token,additional_string,err_type, ER_INTERNAL);
                        return NULL;
                    }
                    state = S_EXPONENT;
                    continue;
                }
                else{
                    ungetc(c, stdin);
                    token->attribute.decimal = strtod(additional_string->string,NULL);
                    single_token(token,*line_cnt,T_EXPONENT,additional_string);
                    return token;
                }
            case(S_UNDERLINE):
                if((c >= 48 && c <= 57) // 0..9
                    || (c >= 65 && c <= 90) // a..z
                    || (c >= 97 && c <= 122) // A..Z
                    || c == '_'){

                    if(additional_string == NULL){
                        if((additional_string = string_init()) == NULL){
                            scanning_finish_with_error(token,additional_string, err_type, ER_INTERNAL);
                            return NULL;
                        }
                    }
                    //add '_'
                    if(!string_append(additional_string,'_')){
                        scanning_finish_with_error(token,additional_string,err_type, ER_INTERNAL);
                        return NULL;
                    }
                    if(!string_append(additional_string,c)){
                        scanning_finish_with_error(token,additional_string,err_type, ER_INTERNAL);
                        return NULL;
                    }
                    state = S_ID;
                    continue;
                } else{
                    ungetc(c, stdin);
                    single_token(token,*line_cnt,T_UNDERLINE,additional_string);
                    return token;
                }
            case(S_ID):
                if((c >= 48 && c <= 57) // 0..9
                   || (c >= 65 && c <= 90) // a..z
                   || (c >= 97 && c <= 122) // A..Z
                   || c == '_'){
                    if(!string_append(additional_string,c)){
                        scanning_finish_with_error(token,additional_string,err_type, ER_INTERNAL);
                        return NULL;
                    }
                    continue;
                }
                else if(c == '?'){
                    if(!string_append(additional_string,c)){
                        scanning_finish_with_error(token,additional_string,err_type, ER_INTERNAL);
                        return NULL;
                    }
                    keyword_control(token,additional_string);
                    single_token(token,*line_cnt,T_KEYWORD_NIL_POSSIBILITY,additional_string);
                    return token;
                }
                else{
                    ungetc(c, stdin);
                    if(keyword_control(token,additional_string))
                        single_token(token,*line_cnt,T_KEYWORD,additional_string);
                    else{
                        token->attribute.string = additional_string->string;
                        single_token(token,*line_cnt,T_ID,additional_string);
                    }

                    return token;
                }
            case(S_MINUS):
                if(c == '>'){
                    single_token(token,*line_cnt,T_ARROW,additional_string);
                    return token;
                } else{
                    ungetc(c, stdin);
                    single_token(token, *line_cnt, T_MINUS,additional_string);
                    return token;
                }
            case(S_ASSINGMENT):
                if(c == '='){
                    single_token(token,*line_cnt,T_EQUALS,additional_string);
                    return token;
                } else{
                    ungetc(c, stdin);
                    single_token(token,*line_cnt,T_ASSIGMENT,additional_string);
                    return token;
                }
            case(S_STRING_START):
                if(additional_string == NULL){
                    if((additional_string = string_init()) == NULL){
                        scanning_finish_with_error(token,additional_string, err_type, ER_INTERNAL);
                        return NULL;
                    }
                }
                if(c == '"'){
                    state = S_STRING;
                    continue;
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
                else if(c == '"'){
                    if(!string_append(additional_string,c)){
                        scanning_finish_with_error(token,additional_string,err_type, ER_INTERNAL);
                        return NULL;
                    }
                    else if(is_multiline){
                        state = multiline_state;
                        is_multiline = false;
                    }
                    else {
                        state = S_STRING_START;
                    }
                    continue;
                }
                else if(c == 'n'){
                    if(!string_append(additional_string,'\n')){
                        scanning_finish_with_error(token,additional_string,err_type, ER_INTERNAL);
                        return NULL;
                    }
                    else if(is_multiline){
                        state = multiline_state;
                        is_multiline = false;
                    }
                    else {
                        state = S_STRING_START;
                    }
                    continue;
                }
                else if(c == 'r'){
                    if(!string_append(additional_string,'\r')){
                        scanning_finish_with_error(token,additional_string,err_type, ER_INTERNAL);
                        return NULL;
                    }
                    else if(is_multiline){
                        state = multiline_state;
                        is_multiline = false;
                    }
                    else {
                        state = S_STRING_START;
                    }
                    continue;
                }
                else if(c == 't'){
                    if(!string_append(additional_string,'\t')){
                        scanning_finish_with_error(token,additional_string,err_type, ER_INTERNAL);
                        return NULL;
                    }
                    else if(is_multiline){
                        state = multiline_state;
                        is_multiline = false;
                    }
                    else {
                        state = S_STRING_START;
                    }
                    continue;
                }
                else if(c == '\\'){
                    if(!string_append(additional_string,'\\')){
                        scanning_finish_with_error(token,additional_string,err_type, ER_INTERNAL);
                        return NULL;
                    }
                    else if(is_multiline){
                        state = multiline_state;
                        is_multiline = false;
                    }
                    else {
                        state = S_STRING_START;
                    }
                    continue;
                }
                else{
                    scanning_finish_with_error(token,additional_string,err_type,ER_LEX);
                    return NULL;
                }
            case(S_STRING_START_HEX):
                if(c == '{'){
                    state = S_STRING_HEX_OPEN;
                    continue;
                }
                else {
                    scanning_finish_with_error(token,additional_string,err_type,ER_SYNTAX);
                    return NULL;
                }

            case(S_STRING_HEX_OPEN):
                if((c >= 48 && c <= 57) /*numbers 0..9*/
                   || (c >= 65 && c <= 70) /* HEX numbers A..F */
                   || (c >= 97 && c <= 102)) /* HEX numbers a..f */{
                        //todo add number
                        if(!string_append(unicode,c)){
                            scanning_finish_with_error(token,additional_string,err_type, ER_INTERNAL);
                            return NULL;
                        }
                        printf("test");
                        state = S_STRING_HEX_NUMBER;
                        hex_count++;
                        continue;
                    } else {
                    scanning_finish_with_error(token,additional_string,err_type,ER_SYNTAX);
                    return NULL;
                }
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
                        if(!string_append(unicode,c)){
                            scanning_finish_with_error(token,additional_string,err_type, ER_INTERNAL);
                            return NULL;
                        }
                        hex_count++;
                        continue;
                    }
                else if(c == '}'){
                    int unicodeint = (int) strtol(unicode->string, NULL, 16);
                    if(!string_append(additional_string,(char)unicodeint)){
                        scanning_finish_with_error(token,additional_string,err_type, ER_INTERNAL);
                        return NULL;
                    }
                    if(is_multiline){
                        state = multiline_state;
                        is_multiline = false;
                    }
                    else {
                        state = S_STRING_START;
                    }
                    continue;
                } else {
                    scanning_finish_with_error(token,additional_string,err_type,ER_SYNTAX);
                    return NULL;
                }
            case(S_STRING):
                if(additional_string->string != NULL){
                    token->attribute.string = additional_string->string;
                    single_token(token,*line_cnt,T_STRING,additional_string);
                    return token;
                }
                else if(c == '"'){
                    state = S_MULTILINE_OPEN;
                    continue;
                } else {
                    scanning_finish_with_error(token,additional_string,err_type,ER_SYNTAX);
                    return NULL;
                }
            case(S_MULTILINE_OPEN):
                if(c == '\n'){
                    state = S_MULTILINE_START;
                    (*line_cnt)++;
                    continue;
                } else {
                    scanning_finish_with_error(token,additional_string,err_type,ER_SYNTAX);
                    return NULL;
                }
            case(S_MULTILINE_START):
                if(c == '"') {
                    state = S_MULTILINE_END_1;
                    continue;
                } else if(c == '\\'){
                    multiline_state = S_MULTILINE_START;
                    is_multiline = true;
                    state = S_STRING_SPEC_SYMBOL;
                    continue;
                } else if (c >= PRINTABLE_MIN && c <= PRINTABLE_MAX) {
                    if(!string_append(additional_string,c)){
                        scanning_finish_with_error(token,additional_string,err_type, ER_INTERNAL);
                        return NULL;
                    }
                    state = S_MULTILINE_LINE;
                    continue;
                } else {
                    scanning_finish_with_error(token,additional_string,err_type,ER_SYNTAX);
                    return NULL;
                }
            case(S_MULTILINE_LINE):
                if(c == '\n'){
                    state = S_MULTILINE_START;
                    if(!string_append(additional_string,c)){
                        scanning_finish_with_error(token,additional_string,err_type, ER_INTERNAL);
                        return NULL;
                    }
                    (*line_cnt)++;
                    continue;
                } else if(c == '\\'){
                    multiline_state = S_MULTILINE_LINE;
                    is_multiline = true;
                    state = S_STRING_SPEC_SYMBOL;
                    continue;
                } else if (c >= PRINTABLE_MIN && c <= PRINTABLE_MAX) {
                    if(!string_append(additional_string,c)){
                        scanning_finish_with_error(token,additional_string,err_type, ER_INTERNAL);
                        return NULL;
                    }
                    state = S_MULTILINE_LINE;
                    continue;
                } else {
                    scanning_finish_with_error(token,additional_string,err_type,ER_SYNTAX);
                    return NULL;
                }
            case(S_MULTILINE_END_1):
                if(c == '"'){
                    state = S_MULTILINE_END_2;
                    continue;
                } else if (c == '\\'){
                    is_multiline = true;
                    multiline_state = S_MULTILINE_END_1;
                    state = S_STRING_SPEC_SYMBOL;
                    continue;
                } else if (c >= PRINTABLE_MIN && c <= PRINTABLE_MAX) {
                    if(!string_append(additional_string,c)){
                        scanning_finish_with_error(token,additional_string,err_type, ER_INTERNAL);
                        return NULL;
                    }
                    state = S_MULTILINE_LINE;
                    continue;
                } else {
                    scanning_finish_with_error(token,additional_string,err_type,ER_SYNTAX);
                    return NULL;
                }
            case(S_MULTILINE_END_2):
                if(c == '"'){
                    state = S_MULTILINE_STRING;
                    continue;
                } else if (c == '\\'){
                    is_multiline = true;
                    multiline_state = S_MULTILINE_END_2;
                    state = S_STRING_SPEC_SYMBOL;
                    continue;
                } else if (c >= PRINTABLE_MIN && c <= PRINTABLE_MAX) {
                    if(!string_append(additional_string,c)){
                        scanning_finish_with_error(token,additional_string,err_type, ER_INTERNAL);
                        return NULL;
                    }
                    state = S_MULTILINE_LINE;
                    continue;
                } else {
                    scanning_finish_with_error(token,additional_string,err_type,ER_SYNTAX);
                    return NULL;
                }
            case(S_MULTILINE_STRING):
                if(additional_string->string != NULL){
                    additional_string->string[additional_string->last_index - 1] = '\0';
                    token->attribute.string = additional_string->string;
                    single_token(token,*line_cnt,T_STRING,additional_string);
                    return token;
                } else{
                    scanning_finish_with_error(token,additional_string,err_type,ER_SYNTAX);
                    return NULL;

                }
            default:
                break;
        }
    }
    single_token(token,*line_cnt,T_EOF,additional_string);
    return token;
}
