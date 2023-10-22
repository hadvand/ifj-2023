#ifndef IFJ_2023_SCANNER_TEST_UTILS_H
#define IFJ_2023_SCANNER_TEST_UTILS_H

#include "../scanner/scanner.h"
#include <stdio.h>
#include <stdbool.h>

#define TEST(NAME, DESCRIPTION) \
void NAME(void) { \
    printf("\n[%s] %s\n", #NAME, DESCRIPTION); \
    error_t test_error = ER_NONE;              \
    token_t_ptr test_token = (token_t_ptr) malloc(sizeof(struct token)); \
    token_t_ptr token = (token_t_ptr) malloc(sizeof(struct token));      \
    int line_count = 1;

#define ENDTEST \
    free(test_token); \
    free(token); \
    printf("\n"); \
}

#define FAIL(REASON) printf("\033[1;31m[FAILED] %s\033[0m", REASON)
#define PASS printf("\033[1;32m[PASSED]\033[0m")

void print_token(token_t_ptr token);
bool assert_token(token_t_ptr token, token_t_ptr test_token);
void stdin_emulate(char *test_line);


#endif //IFJ_2023_SCANNER_TEST_UTILS_H
