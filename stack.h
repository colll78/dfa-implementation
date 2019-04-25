#ifndef STACK_H_INCLUDED
#define STACK_H_INCLUDED
typdef char stack_elementT;

typedef struct pda_stack_node{
    stack_elementT data;
    struct pda_stack_node* next;
}pda_stack_node;


pda_stack_node new_pda_stack_node(stack_elementT data);

int pda_stack_is_empty(pda_stack_node *root);
stack_elementT pda_push(pda_stack_node **root, stack_elementT data);
stack_elementT pda_pop(pda_stack_node **root);
stack_elementT pda_peek(pda_stack_node *root);


#endif // STACK_H_INCLUDED
