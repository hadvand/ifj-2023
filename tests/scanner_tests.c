#include <stdio.h>
#include "scanner_test_utils.h"

#define TEST_COUNT ((long int) (sizeof(tests) / sizeof(void *)))
int test_passed = 0;

TEST(test_exponent, "Exponent")
    stdin_emulate("1e2");
    test_token->token_type = T_EXPONENT;
    token = next_token(&line_count, &test_error);
    if(assert_token(token, test_token)){
        test_passed++;
    }
ENDTEST

TEST(test_decimal, "Decimal")
    stdin_emulate("1.234");
    test_token->token_type = T_DEMICAL;
    test_token->attribute.decimal = 1.234;
    token = next_token(&line_count, &test_error);
    if(assert_token(token, test_token)){
        test_passed++;
    }
ENDTEST

TEST(test_int, "Int")
    stdin_emulate("120");
    test_token->token_type = T_INT;
    test_token->attribute.integer = 120;
    token = next_token(&line_count, &test_error);
    if(assert_token(token, test_token)){
        test_passed++;
    }
ENDTEST

TEST(test_equals, "Equals")
    stdin_emulate("==");
    test_token->token_type = T_EQUALS;
    token = next_token(&line_count, &test_error);
    if(assert_token(token, test_token)){
        test_passed++;
    }
ENDTEST

TEST(test_assigment, "Assigment")
    stdin_emulate("=");
    test_token->token_type = T_ASSIGMENT;
    token = next_token(&line_count, &test_error);
    if(assert_token(token, test_token)){
        test_passed++;
    }
ENDTEST

TEST(test_underline, "Underline")
    stdin_emulate("_");
    test_token->token_type = T_UNDERLINE;
    token = next_token(&line_count, &test_error);
    if(assert_token(token, test_token)){
        test_passed++;
    }
ENDTEST

TEST(test_keyword, "Keyword")
    stdin_emulate("func");
    test_token->token_type = T_KEYWORD;
    test_token->attribute.keyword = k_func;
    token = next_token(&line_count, &test_error);
    if(assert_token(token, test_token)){
        test_passed++;
    }
ENDTEST

TEST(test_keyword_1, "Keyword 1")
    stdin_emulate("if");
    test_token->token_type = T_KEYWORD;
    test_token->attribute.keyword = k_if;
    token = next_token(&line_count, &test_error);
    if(assert_token(token, test_token)){
        test_passed++;
    }
ENDTEST

TEST(test_keyword_2, "Keyword 2")
    stdin_emulate("else");
    test_token->token_type = T_KEYWORD;
    test_token->attribute.keyword = k_else;
    token = next_token(&line_count, &test_error);
    if(assert_token(token, test_token)){
        test_passed++;
    }
ENDTEST

TEST(test_keyword_3, "Keyword 3")
    stdin_emulate("while");
    test_token->token_type = T_KEYWORD;
    test_token->attribute.keyword = k_while;
    token = next_token(&line_count, &test_error);
    if(assert_token(token, test_token)){
        test_passed++;
    }
ENDTEST

TEST(test_keyword_4, "Keyword 4")
    stdin_emulate("return");
    test_token->token_type = T_KEYWORD;
    test_token->attribute.keyword = k_return;
    token = next_token(&line_count, &test_error);
    if(assert_token(token, test_token)){
        test_passed++;
    }
ENDTEST

TEST(test_keyword_5, "Keyword 5")
    stdin_emulate("var");
    test_token->token_type = T_KEYWORD;
    test_token->attribute.keyword = k_var;
    token = next_token(&line_count, &test_error);
    if(assert_token(token, test_token)){
        test_passed++;
    }
ENDTEST

TEST(test_keyword_6, "Keyword 6")
    stdin_emulate("Double");
    test_token->token_type = T_KEYWORD;
    test_token->attribute.keyword = k_Double;
    token = next_token(&line_count, &test_error);
    if(assert_token(token, test_token)){
        test_passed++;
    }
ENDTEST


TEST(test_keyword_7, "Keyword 7")
    stdin_emulate("Int");
    test_token->token_type = T_KEYWORD;
    test_token->attribute.keyword = k_Int;
    token = next_token(&line_count, &test_error);
    if(assert_token(token, test_token)){
        test_passed++;
    }
ENDTEST


TEST(test_keyword_8, "Keyword 8")
    stdin_emulate("String");
    test_token->token_type = T_KEYWORD;
    test_token->attribute.keyword = k_String;
    token = next_token(&line_count, &test_error);
    if(assert_token(token, test_token)){
        test_passed++;
    }
ENDTEST


TEST(test_keyword_9, "Keyword 9")
    stdin_emulate("let");
    test_token->token_type = T_KEYWORD;
    test_token->attribute.keyword = k_let;
    token = next_token(&line_count, &test_error);
    if(assert_token(token, test_token)){
        test_passed++;
    }
ENDTEST

TEST(test_keyword_10, "Keyword 10")
    stdin_emulate("nil");
    test_token->token_type = T_KEYWORD;
    test_token->attribute.keyword = k_nil;
    token = next_token(&line_count, &test_error);
    if(assert_token(token, test_token)){
        test_passed++;
    }
ENDTEST

//TODO: fix id test
//TEST(test_id, "ID")
//    stdin_emulate("id");
//    test_token->token_type = T_ID;
//    test_token->attribute.string = "test_id";
//    token = next_token(&line_count, &test_error);
//    if(assert_token(token, test_token)){
//        test_passed++;
//    }
//ENDTEST

TEST(keyword_type_id, "Keyword type ID")
    stdin_emulate("Int?");
    test_token->token_type = T_KEYWORD_NIL_POSSIBILITY;
    test_token->attribute.keyword = k_qmark_Int;
    token = next_token(&line_count, &test_error);
    if(assert_token(token, test_token)){
        test_passed++;
    }
ENDTEST

TEST(keyword_type_id_1, "Keyword type ID 1")
    stdin_emulate("Double?");
    test_token->token_type = T_KEYWORD_NIL_POSSIBILITY;
    test_token->attribute.keyword = k_qmark_Double;
    token = next_token(&line_count, &test_error);
    if(assert_token(token, test_token)){
        test_passed++;
    }
ENDTEST

TEST(keyword_type_id_2, "Keyword type ID 2")
    stdin_emulate("String?");
    test_token->token_type = T_KEYWORD_NIL_POSSIBILITY;
    test_token->attribute.keyword = k_qmark_String;
    token = next_token(&line_count, &test_error);
    if(assert_token(token, test_token)){
        test_passed++;
    }
ENDTEST

//TODO: fix string test
//TEST(test_string, "String")
//    stdin_emulate("\"Hello world\"");
//    test_token->token_type = T_STRING;
//    test_token->attribute.string = "Hello world";
//    token = next_token(&line_count, &test_error);
//    if(assert_token(token, test_token)){
//        test_passed++;
//    }
//ENDTEST

TEST(test_more, "More Than")
    stdin_emulate(">");
    test_token->token_type = T_MORE;
    token = next_token(&line_count, &test_error);
    if(assert_token(token, test_token)){
        test_passed++;
    }
ENDTEST

TEST(test_more_equal, "More Than or Equal")
    stdin_emulate(">=");
    test_token->token_type = T_MORE_EQUAL;
    token = next_token(&line_count, &test_error);
    if(assert_token(token, test_token)){
        test_passed++;
    }
ENDTEST

TEST(test_less, "Less Than")
    stdin_emulate("<");
    test_token->token_type = T_LESS;
    token = next_token(&line_count, &test_error);
    if(assert_token(token, test_token)){
        test_passed++;
    }
ENDTEST

TEST(test_less_equal, "Less Than or Equal")
    stdin_emulate("<=");
    test_token->token_type = T_LESS_EQUAL;
    token = next_token(&line_count, &test_error);
    if(assert_token(token, test_token)){
        test_passed++;
    }
ENDTEST

TEST(test_minus, "Minus")
    stdin_emulate("-");
    test_token->token_type = T_MINUS;
    token = next_token(&line_count, &test_error);
    if(assert_token(token, test_token)){
        test_passed++;
    }
ENDTEST

TEST(test_arrow, "Arrow")
    stdin_emulate("->");
    test_token->token_type = T_ARROW;
    token = next_token(&line_count, &test_error);
    if(assert_token(token, test_token)){
        test_passed++;
    }
ENDTEST

TEST(test_tern, "Tern")
    stdin_emulate("??");
    test_token->token_type = T_TERN;
    token = next_token(&line_count, &test_error);
    if(assert_token(token, test_token)){
        test_passed++;
    }
ENDTEST

TEST(test_comment_string, "Comment String")
    stdin_emulate("//Hello world");
    test_token->token_type = T_COMMENT_STRING;
    token = next_token(&line_count, &test_error);
    if(assert_token(token, test_token)){
        test_passed++;
    }
ENDTEST

TEST(test_comment_block, "Comment Block")
    stdin_emulate("/*Hello world*/");
    test_token->token_type = T_COMMENT_BLOCK;
    token = next_token(&line_count, &test_error);
    if(assert_token(token, test_token)){
        test_passed++;
    }
ENDTEST

TEST(test_not_equal, "Not Equal")
    stdin_emulate("!=");
    test_token->token_type = T_NOT_EQUAL;
    token = next_token(&line_count, &test_error);
    if(assert_token(token, test_token)){
        test_passed++;
    }
ENDTEST

TEST(test_division, "Division")
    stdin_emulate("/");
    test_token->token_type = T_DIVISION;
    token = next_token(&line_count, &test_error);
    if(assert_token(token, test_token)){
        test_passed++;
    }
ENDTEST

TEST(test_colon, "Colon")
    stdin_emulate(":");
    test_token->token_type = T_COLON;
    token = next_token(&line_count, &test_error);
    if(assert_token(token, test_token)){
        test_passed++;
    }
ENDTEST

TEST(test_plus, "Plus")
    stdin_emulate("+");
    test_token->token_type = T_PLUS;
    token = next_token(&line_count, &test_error);
    if(assert_token(token, test_token)){
        test_passed++;
    }
ENDTEST

TEST(test_comma, "Comma")
    stdin_emulate(",");
    test_token->token_type = T_COMMA;
    token = next_token(&line_count, &test_error);
    if(assert_token(token, test_token)){
        test_passed++;
    }
ENDTEST

TEST(test_curved_bracket_open, "Curved Bracket Open")
    stdin_emulate("{");
    test_token->token_type = T_CURVED_BRACKET_OPEN;
    token = next_token(&line_count, &test_error);
    if(assert_token(token, test_token)){
        test_passed++;
    }
ENDTEST

TEST(test_curved_bracket_close, "Curved Bracket Close")
    stdin_emulate("}");
    test_token->token_type = T_CURVED_BRACKET_CLOSE;
    token = next_token(&line_count, &test_error);
    if(assert_token(token, test_token)){
        test_passed++;
    }
ENDTEST

TEST(test_square_bracket_open, "Square Bracket Open")
    stdin_emulate("[");
    test_token->token_type = T_SQUARE_BRACKET_OPEN;
    token = next_token(&line_count, &test_error);
    if(assert_token(token, test_token)){
        test_passed++;
    }
ENDTEST

TEST(test_square_bracket_close, "Square Bracket Close")
    stdin_emulate("]");
    test_token->token_type = T_SQUARE_BRACKET_CLOSE;
    token = next_token(&line_count, &test_error);
    if(assert_token(token, test_token)){
        test_passed++;
    }
ENDTEST

TEST(test_bracket_open, "Bracket Open")
    stdin_emulate("(");
    test_token->token_type = T_BRACKET_OPEN;
    token = next_token(&line_count, &test_error);
    if(assert_token(token, test_token)){
        test_passed++;
    }
ENDTEST

TEST(test_bracket_close, "Bracket Close")
    stdin_emulate(")");
    test_token->token_type = T_BRACKET_CLOSE;
    token = next_token(&line_count, &test_error);
    if(assert_token(token, test_token)){
        test_passed++;
    }
ENDTEST

TEST(test_new_line, "New Line")
    stdin_emulate("\n");
    test_token->token_type = T_NEW_LINE;
    token = next_token(&line_count, &test_error);
    if(assert_token(token, test_token)){
        test_passed++;
    }
ENDTEST

TEST(test_eof, "EOF")
    stdin_emulate("");
    test_token->token_type = T_EOF;
    token = next_token(&line_count, &test_error);
    if(assert_token(token, test_token)){
        test_passed++;
    }
ENDTEST

TEST(test_multiplication, "Multiplication")
    stdin_emulate("*");
    test_token->token_type = T_MULTIPLICATION;
    token = next_token(&line_count, &test_error);
    if(assert_token(token, test_token)){
        test_passed++;
    }
ENDTEST

void (*tests[])(void) = {
        test_exponent,
        test_decimal,
        test_int,
        test_equals,
        test_assigment,
        test_underline,
        test_keyword,
        test_keyword_1,
        test_keyword_2,
        test_keyword_3,
        test_keyword_4,
        test_keyword_5,
        test_keyword_6,
        test_keyword_7,
        test_keyword_8,
        test_keyword_9,
        test_keyword_10,
//        test_id,
        keyword_type_id,
        keyword_type_id_1,
        keyword_type_id_2,
//        test_string,
        test_more,
        test_more_equal,
        test_less,
        test_less_equal,
        test_minus,
        test_arrow,
        test_tern,
        test_comment_string,
        test_comment_block,
        test_not_equal,
        test_division,
        test_colon,
        test_plus,
        test_comma,
        test_curved_bracket_open,
        test_curved_bracket_close,
        test_square_bracket_open,
        test_square_bracket_close,
        test_bracket_open,
        test_bracket_close,
        test_new_line,
        test_eof,
        test_multiplication
};


int main() {
    for (int i = 0; i < TEST_COUNT; i++) {
        tests[i]();
    }

    printf("\n\n--------------------RESULTS--------------------\n");
    if (test_passed == TEST_COUNT) {
        printf("\033[0;32m");
        printf("ALL TESTS PASSED\nGOOD JOB:)");
        printf("\033[0m\n\n");
    } else {
        printf("\033[0;31m");
        printf("%d/%ld TESTS PASSED\nNEXT TIME WILL BE BETTER:(", test_passed, TEST_COUNT);
        printf("\033[0m\n\n");
    }
}