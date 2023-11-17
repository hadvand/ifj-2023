#include "stack.h"


stack *stack_init() {
    stack *s = (stack *)malloc(sizeof(struct stack));
    s->top = 0;
    return s;
}


bool stack_is_empty(stack *s) {
    return (s->top == 0);
}


void stack_push(stack *s, token_t_ptr *stack_item) {
    if (s->top < BRACKET_STACK_MAX) {
        s->top++;
        stack_elem *elem = (stack_elem *)malloc(sizeof(stack_elem));
        elem->stack_item = stack_item;
        s->data_array[s->top] = elem;
    }
}


void stack_pop(stack *s) {
    s->top--;
}


stack_elem *get_top(stack *s) {
    return s->data_array[s->top];
}


void stack_dispose(stack *s) {
    while (!stack_is_empty(s)) {
        free(get_top(s));
        stack_pop(s);
    }
    free(s);
}
