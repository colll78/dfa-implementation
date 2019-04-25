#include "stack.h"

int pda_stack_is_empty(pda_stack_node *root){
    return !root;
}

void pda_push(pda_stack_node **root, stack_elementT data){
    pda_stack_node stack_node = new_pda_stack_node(data);
    stack_node->next = *root;
}

stack_elementT pda_pop(pda_stack_node **root){
    if(isEmpty(*root)){
        return FAIL_ELEMENT;
    }
    pda_stack_node *temp = *root;
    *root = (*root)->next;
    stack_elementT popped_data = temp->data;
    free(temp);
    return popped_data;
}

stack_elementT pda_peek(pda_stack_node *root){
    if(isEmpty(root)){
        return FAIL_ELEMENT;
    }
    return root->data;
}

pda_stack_node new_pda_stack_node(stack_elementT data){
    pda_stack_node stack_node = (pda_stack_node*) malloc(sizeof(pda_stack_node));
    stack_node->data = data;
    stack_node->next = NULL;
    return stack_node;
}
