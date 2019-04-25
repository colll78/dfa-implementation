#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pda.h"
#define TRUE 1
#define FALSE 0

PDA construct_PDA(int max_num_states, int n_accepts, int *name){
    PDA c = {0, 0, 0, 0, };
    c.states = malloc(max_states * sizeof(State));
    c.accept_states = malloc(n_accepts * sizeof(State));
    c.name = name;
    c.start_state = NULL;
    return c;
}

CFG construct_CFG(char *name, int n_vars){
    CFG g;
    g.name = name;
    g.n_vars = n_vars;
    g.var_list = NULL;
}

void pda_add_state(PDA *m, char *name, int t_num, int is_start, int is_accept){
    PDA_State s = pda_construct_state(name, t_num);
    m->states[m->n_states] = s;
    if(is_start == TRUE){
        if(m->start_state != NULL){
            printf("PDA Error: %s", name);
            printf("PDA cannot have more than one start state.")
            exit(1);
        }
        m->start_state = &m->states[m->n_states];
        m->curr_state = &m->states[m->n_states];
    }
    if (is_accept == TRUE){
        m->n_accepts++;
    }
    m->n_states++;
    return;
}

PDA_State pda_construct_state(const char *name, int t_num){
    PDA_State q = {name, 0, };
    q.t_list = malloc (t_num * sizeof(PDA_Transition));
    q.name = name;
    return q;
}

void cfg_add_nonterminal(CFG *cfg, char *var_id, int p_capacity){
    NonTerminal *var;

    HASH_FIND_STR(cfg->var_list, var_id, var);
    if(var == NULL){
        var = (NonTerminal *) malloc(sizeof(*var));
        var->identifier = var_id;
        var->p_capacity = p_capacity;
        var->p_num = 0;
        var->p_list = (ProductionNode*) malloc(p_capacity * sizeof(ProductionNode));
        HASH_ADD_KEYPTR(hh, cfg->var_list, var->identifier, strlen(var->identifier), var);
    }
}

/* str_len: return the length of the string */
int str_len(char *s1){
    int i;
    for(i = 0; s1[i] != '\0'; i++);
    return i;
}

NonTerminal *get_nonterminal(CFG *cfg, char *var_id){
    NonTerminal *var;

    HASH_FIND_STR(cfg->var_list, var_id, var);
    if(var == NULL){
        return NULL;
    } else {
        return var;
    }
}

void cfg_add_production(CFG *cfg, char *var_id, char *token_id, int prod_id){
    NonTerminal *var = get_nonterminal(cfg, var_id);
    if(var == NULL){
        printf("NonTerminal: %s not found in CFG: %s. \n", var_id, cfg->name);
        return;
    } else if(var->p_num >= var->p_capacity){
        printf("Exceeded maximum amount of productions for NonTerminal: %s in CFG: %s.\n", var_id, cfg->name);
        return;
    } else if(var->p_list[prod_id] == NULL){
        ProductionNode *prod_node = (*ProductionNode) malloc(sizeof(ProductionNode));
        NonTerminalRHS *nterm_rhs = (*NonTerminalRHS) malloc(sizeof(NonTerminal));
        TokenKind *tk = (*TokenKind) malloc (sizeof(TokenKind));
        tk->image = token_id;
        nterm_rhs->next = NULL;
        nterm_rhs->tok = tk;
        prod_node->head = nterm_rhs;
        var->p_list[prod_id] = prod_node;
    }
}
