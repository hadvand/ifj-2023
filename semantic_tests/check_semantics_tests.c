//
// Created by oleg on 11/21/23.
//


#include "../semantics/semantics.h"
#include "../stack.h"
#include "../structures/error.h"
#include <stdio.h>
#include <assert.h>

void test(char* description, int expectedErrorCode, int actualErrorCode, item_type expectedType, item_type actualType) {
    printf("Test: %s\n", description);
    assert(expectedErrorCode == actualErrorCode);
    assert(expectedType == actualType);
    printf("Passed!\n\n");
}


int main() {
    // Test for OPERAND rule
    t_stack_elem operand1 = {.item.type = IT_INT};  // Set operand_1 type to IT_INT
    t_stack_elem operand2 = {.item.type = IT_UNDEF};  // Placeholder for operand_2
    t_stack_elem operand3 = {.item.type = IT_UNDEF};  // Placeholder for operand_3
    item_type typeFinal;
    int errorCode;

    errorCode = check_semantics(OPERAND, &operand1, &operand2, &operand3, &typeFinal);
    test("OPERAND rule with valid type", ER_NONE, errorCode, IT_INT, typeFinal);

    operand1.item.type = IT_UNDEF;  // Set operand_1 type to IT_UNDEF
    errorCode = check_semantics(OPERAND, &operand1, &operand2, &operand3, &typeFinal);
    test("OPERAND rule with undefined variable", ER_UNDEF_VAR, errorCode, IT_UNDEF, typeFinal);

    // Test for LBR_NT_RBR rule
    operand1.item.type = IT_UNDEF;  // Reset operand_1 type
    operand2.item.type = IT_DOUBLE;  // Set operand_2 type to IT_DOUBLE
    errorCode = check_semantics(LBR_NT_RBR, &operand1, &operand2, &operand3, &typeFinal);
    test("LBR_NT_RBR rule with valid type", ER_NONE, errorCode, IT_DOUBLE, typeFinal);

    operand2.item.type = IT_UNDEF;  // Set operand_2 type to IT_UNDEF
    errorCode = check_semantics(LBR_NT_RBR, &operand1, &operand2, &operand3, &typeFinal);
    test("LBR_NT_RBR rule with undefined variable", ER_UNDEF_VAR, errorCode, IT_UNDEF, typeFinal);

    return 0;
}
