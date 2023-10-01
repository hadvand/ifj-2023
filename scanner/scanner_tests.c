#include <stdio.h>
#include "scanner.h"

int main() {
    // printf() displays the string inside quotation
    token_t_ptr token;
    int line_cnt = 0;
    while (1)
    {
        token = next_token(&line_cnt);
        
        if(token->token_type == T_EOF){
            break;
        }
            
    }
    
    return 0;
}