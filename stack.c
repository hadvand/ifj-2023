#include "stack.h"


stack *stack_init() {
    stack *s = (stack *)malloc(sizeof(struct stack));
    s->top = 0;
    return s;
}


bool stack_is_empty(stack *s) {
    return (s->top == 0);
}


void stack_push(stack *s, char *stack_str) {
    if (s->top < BRACKET_STACK_MAX) {
        s->top++;
        stack_elem *elem = (stack_elem *)malloc(sizeof(stack_elem));
        elem->stack_str = stack_str;
        s->dataArray[s->top] = elem;
    }
}


void stack_pop(stack *s) {
    s->top--;
}


stack_elem *get_top(stack *s) {
    return s->dataArray[s->top];
}


void print_and_pop(stack *s) {
    while (!stack_is_empty(s)) {
        printf("%s\n", get_top(s)->stack_str);
        stack_pop(s);
    }
}


void stack_dispose(stack *s) {
    while (!stack_is_empty(s)) {
        free(get_top(s));
        stack_pop(s);
    }
    free(s);
}
