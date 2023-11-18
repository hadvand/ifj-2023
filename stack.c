#include "stack.h"


t_stack *stack_init() {
    t_stack *s = (t_stack *)malloc(sizeof(struct stack));
    s->top = 0;
    return s;
}


bool stack_is_empty(t_stack *s) {
    return (s->top == 0);
}


void stack_push(t_stack *s, item_data stack_item) {
    if (s->top < BRACKET_STACK_MAX) {
        s->top++;
        t_stack_elem *elem = (t_stack_elem *)malloc(sizeof(struct stack_elem));
        elem->item = stack_item;
        s->data_array[s->top] = elem;
    }
}


void stack_pop(t_stack *s) {
    s->top--;
}


t_stack_elem *get_top(t_stack *s) {
    return s->data_array[s->top];
}


void stack_dispose(t_stack *s) {
    while (!stack_is_empty(s)) {
        free(get_top(s));
        stack_pop(s);
    }
    free(s);
}
