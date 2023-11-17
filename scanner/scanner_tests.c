#include <stdio.h>
#include "scanner.h"

int main() {
    // printf() displays the string inside quotation
    token_t_ptr token;
    int error_code;
    error_code = ER_NONE;
    int line_cnt = 1;
    bool flag;

    while (1)
    {
        token = next_token(&line_cnt, &error_code, &flag);
        
        if(error_code != ER_NONE){
            fprintf (stderr, "\n**** MINUS ZOPA WITH ERROR CODE %d ****\n\n**** LINE %d ****\n", error_code, line_cnt);
            break;
        }

        if(token->token_type == T_EOF){
            break;
        }
            
    }
    
    return 0;
}