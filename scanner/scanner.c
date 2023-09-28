#include "scanner.h"

token_t_ptr create_token(){
    token_t_ptr token;
    token = (token_t_ptr) malloc(sizeof(struct token));
    return token;
}

void delete_token(token_t_ptr token){
    //todo
    if(token != NULL)
        free(token);
}