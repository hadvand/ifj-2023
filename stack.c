#include "stack.h"


stack *stack_init() {
    stack *s = (stack *)malloc(sizeof(struct stack));
    s->top = 0;
    return s;
}


bool stack_is_empty(stack *s) {
    return (s->top == 0);
}


bool stack_push(stack *s, char *stack_str) {
    if (s->top < BRACKET_STACK_MAX) {
        s->top++;
        stack_elem *elem = (stack_elem *)malloc(sizeof(stack_elem));
        if(elem == NULL)
            return false;
        elem->stack_str = stack_str;
        s->data_array[s->top] = elem;

        return true;
    }
    return false;
}


bool stack_pop(stack *s) {
    if(s->top =< 0)
        return false;
    s->top--;
    return true;
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
