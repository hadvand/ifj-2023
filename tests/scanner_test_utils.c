#include <unistd.h>
#include <stdio.h>
#include "scanner_test_utils.h"

void print_token(token_t_ptr token) {
    if (token == NULL) {
        printf("TOKEN: NULL\n");
        return;
    }

    switch (token->token_type) {
        case T_NEW_LINE:
            printf("T_NEW_LINE\n");
            break;
        case T_EOF:
            printf("T_EOF\n");
            break;
        case T_MULTIPLICATION:
            printf("T_MULTIPLICATION\n");
            break;
        case T_COLON:
            printf("T_COLON\n");
            break;
        case T_PLUS:
            printf("T_PLUS\n");
            break;
        case T_COMMA:
            printf("T_COMMA\n");
            break;
        case T_CURVED_BRACKET_OPEN:
            printf("T_CURVED_BRACKET_OPEN\n");
            break;
        case T_CURVED_BRACKET_CLOSE:
            printf("T_CURVED_BRACKET_CLOSE\n");
            break;
        case T_SQUARE_BRACKET_OPEN:
            printf("T_SQUARE_BRACKET_OPEN\n");
            break;
        case T_SQUARE_BRACKET_CLOSE:
            printf("T_SQUARE_BRACKET_CLOSE\n");
            break;
        case T_BRACKET_OPEN:
            printf("T_BRACKET_OPEN\n");
            break;
        case T_BRACKET_CLOSE:
            printf("T_BRACKET_CLOSE\n");
            break;
        case T_ITS_NOT_A_TOKEN:
            printf("T_ITS_NOT_A_TOKEN\n");
            break;
        case T_EXPONENT:
            printf("T_EXPONENT\n");
            break;
        case T_DEMICAL:
            printf("T_DEMICAL\n");
            printf("Attribute: %f\n", token->attribute.decimal);
            break;
        case T_INT:
            printf("T_INT\n");
            printf("Attribute: %d\n", token->attribute.integer);
            break;
        case T_EQUALS:
            printf("T_EQUALS\n");
            break;
        case T_ASSIGMENT:
            printf("T_ASSIGMENT\n");
            break;
        case T_UNDERLINE:
            printf("T_UNDERLINE\n");
            break;
        case T_KEYWORD:
            printf("T_KEYWORD\n");
            switch (token->attribute.keyword) {
                case k_Double:
                    printf("k_Double\n");
                    break;
                case k_else:
                    printf("k_else\n");
                    break;
                case k_func:
                    printf("k_func\n");
                    break;
                case k_if:
                    printf("k_if\n");
                    break;
                case k_Int:
                    printf("k_Int\n");
                    break;
                case k_let:
                    printf("k_let\n");
                    break;
                case k_nil:
                    printf("k_nil\n");
                    break;
                case k_return:
                    printf("k_return\n");
                    break;
                case k_String:
                    printf("k_String\n");
                    break;
                case k_var:
                    printf("k_var\n");
                    break;
                case k_while:
                    printf("k_while\n");
                    break;
                default:
                    printf("NOT THIS KW TYPE\n");
                    break;
            }
            break;
        case T_ID:
            printf("T_ID\n");
            printf("Attribute: %s\n", token->attribute.string);
            break;
        case T_KEYWORD_TYPE_ID:
            printf("T_KEYWORD_TYPE_ID\n");
            switch (token->attribute.keyword) {
                case k_qmark_Double:
                    printf("k_qmark_Double\n");
                    break;
                case k_qmark_Int:
                    printf("k_qmark_Int\n");
                    break;
                case k_qmark_String:
                    printf("k_qmark_String\n");
                    break;
                default:
                    printf("NOT THIS KW TYPE\n");
                    break;
            }
            break;
        case T_STRING:
            printf("T_STRING\n");
            printf("Attribute: %s\n", token->attribute.string);
            break;
        case T_MORE:
            printf("T_MORE\n");
            break;
        case T_MORE_EQUAL:
            printf("T_MORE_EQUAL\n");
            break;
        case T_LESS:
            printf("T_LESS\n");
            break;
        case T_LESS_EQUAL:
            printf("T_LESS_EQUAL\n");
            break;
        case T_MINUS:
            printf("T_MINUS\n");
            break;
        case T_ARROW:
            printf("T_ARROW\n");
            break;
        case T_TERN:
            printf("T_TERN\n");
            break;
        case T_COMMENT_STRING:
            printf("T_COMMENT_STRING\n");
            break;
        case T_COMMENT_BLOCK:
            printf("T_COMMENT_BLOCK\n");
            break;
        case T_NOT_EQUAL:
            printf("T_NOT_EQUAL\n");
            break;
        case T_DIVISION:
            printf("T_DIVISION\n");
            break;
    }
}

bool assert_token(token_t_ptr token, token_t_ptr test_token){
    if (token == NULL && test_token == NULL){
        PASS;
        return true;
    }

    if (token == NULL || test_token == NULL){
        FAIL("TOKEN IS NULL");
        return false;
    }

    if(token->token_type != test_token->token_type){
        printf("Except:\n");
        print_token(test_token);
        printf("Got:\n");
        print_token(token);
        FAIL("TOKEN TYPE IS NOT EQUAL");
        return false;
    }

    if(token->token_type == T_DEMICAL){
        if(token->attribute.decimal != test_token->attribute.decimal){
            printf("Except:\n");
            print_token(test_token);
            printf("Got:\n");
            print_token(token);
            FAIL("TOKEN ATTRIBUTE IS NOT EQUAL");
            return false;
        }
    }

    if(token->token_type == T_INT){
        if(token->attribute.integer != test_token->attribute.integer){
            printf("Except:\n");
            print_token(test_token);
            printf("Got:\n");
            print_token(token);
            FAIL("TOKEN ATTRIBUTE IS NOT EQUAL");
            return false;
        }
    }

    if(token->token_type == T_KEYWORD){
        if(token->attribute.keyword != test_token->attribute.keyword){
            printf("Except:\n");
            print_token(test_token);
            printf("Got:\n");
            print_token(token);
            FAIL("TOKEN ATTRIBUTE IS NOT EQUAL");
            return false;
        }
    }

    if(token->token_type == T_KEYWORD_TYPE_ID){
        if(token->attribute.keyword != test_token->attribute.keyword){
            printf("Except:\n");
            print_token(test_token);
            printf("Got:\n");
            print_token(token);
            FAIL("TOKEN ATTRIBUTE IS NOT EQUAL");
            return false;
        }
    }

    if(token->token_type == T_ID){
        if(strcmp(token->attribute.string, test_token->attribute.string) != 0){
            printf("Except:\n");
            print_token(test_token);
            printf("Got:\n");
            print_token(token);
            FAIL("TOKEN ATTRIBUTE IS NOT EQUAL");
            return false;
        }
    }

    if(token->token_type == T_STRING){
        if(strcmp(token->attribute.string, test_token->attribute.string) != 0){
            printf("Except:\n");
            print_token(test_token);
            printf("Got:\n");
            print_token(token);
            FAIL("TOKEN ATTRIBUTE IS NOT EQUAL");
            return false;
        }
    }

    PASS;
    return true;
}

void stdin_emulate(char *test_line){
    FILE *f = fopen("test.tmp", "w");
    if (f != NULL) {
        fprintf(f, "%s", test_line);
        fclose(f);
        freopen("test.tmp", "r", stdin);
        unlink("test.tmp");
    }
}
