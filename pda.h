#ifndef PDA_H_INCLUDED
#define PDA_H_INCLUDED
typedef struct _PDA_Transition PDA_Transition;
FAIL_ELEMENT = '\a';

typedef struct{
    int n_states;
    int n_input_alphabet;
    int n_stack_alphabet;
    int n_accepts;

    char *input_alphabet;
    char *stack_alphabet;
    pda_stack_node *stack_head;

    PDA_State *states;
    PDA_State *curr_state;

    PDA_State *start_state;
    PDA_State *accept_states;
    char *name;
}PDA;

typedef struct CFG{
    char *name;
    int n_vars;
    NonTerminal *var_list;
}CFG;

typedef struct TokenKind{
    //int start_line, start_index, end_line, end_index;
    char *image;    /* string image of the token */
}TokenKind;

typedef struct NonTerminalRHS{
    TokenKind *tok;
    NonTerminalRHS *next;
}NonTerminalRHS;

typedef struct ProductionNode{
    NonTerminalRHS *head;
}ProductionNode;

typedef struct NonTerminal{
    char *indentifier;         /* key (string is WITHIN the structure) */
    int p_num, p_capacity;
    ProductionNode *p_list;    /* array of the NonTerminal's productions */
    UT_hash_handle hh;         /* makes this structure hashable */
}NonTerminal;

typedef struct{
    char *name;
    int t_num;
    PDA_Transition *t_list;
}PDA_State;

struct _PDA_Transition{
    PDA_State *to_state;
    /*
    * 0 - push
    * 1 - pop
    * 2 - nothing
    */
    short int operation;
    char input_char;
    char stack_char;
};

#endif // PDA_H_INCLUDED
