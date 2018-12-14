#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "uthash.h"
#define TRUE 1
#define FALSE 0

typedef struct _Transition Transition;
typedef struct Node;
typedef struct Queue;

typedef struct Visited {
    char *name;             /* key (string is WITHIN the structure) */
    int is_visited;
    UT_hash_handle hh;         /* makes this structure hashable */
}Visited;

typedef struct{
    char *name;
    int t_num;
    Transition *t_list;
}State;

struct _Transition{
    State *to_state;
    char input;
};

typedef struct{
    int n_states;
    int n_alphabet;
    int n_accepts;

    char *alphabet;

    State *states;
    State *curr_state;

    State *start_state;
    State *accept_states;
    char *name;
}DFA;

typedef struct Node{
    State *state;
    struct Node *next;
}Node;


typedef struct Queue{
    Node *head;
    Node *tail;
    int size;
    int limit;
} Queue;


/* Theory of Computation Functions */
void print_dfa(DFA m);
int eval_input(DFA *m, char *inp);
void add_transition(State *from_s, const State *to_s, char c);
int transition(DFA *m, char c);
void print_transitions(State s);
void print_state(State s);
void add_state(DFA *m, State *s);
void dfa_reset(DFA *m);
int dfa_is_accept(DFA *m, State *s);
int dfa_is_start_state(DFA *m, State *s);
int dfa_emptiness_problem(DFA *m);
void dfa_add_start_state(DFA *m, char *name, int t_num);
void dfa_add_accept_state(DFA *m, char *name, int t_num);
void dfa_add_start_accept_state(DFA *m, char *name, int t_num);
void dfa_add_state(DFA *m, char *name, int t_num);
void dfa_complement_transformation(DFA *m);
State construct_state(const char *name, int t_num);
State *get_state(DFA *m, char* state_name);
DFA construct_DFA(int max_element, int n_accept_states, char *name);
DFA dfa_complement_construction(DFA *m, char *name);
DFA dfa_union_construction(DFA m_1, DFA m_2, char *name);
DFA dfa_intersection_construction(DFA m_1, DFA m_2, char *name);
State *get_state_on_input(State *s, char c);

/* String Functions */
int str_len(char *s1);
int str_cmp(const char* s1, const char* s2);
void str_cpy(char *to, char *from);
void str_cat(char *s, char *t);

/* Queue Functions */
Queue *constructQueue(int limit);
void destructQueue(Queue *queue);
int enqueue(Queue *queue, State *item);
Node *dequeue(Queue *queue);


int main(){
    DFA m = construct_DFA(10, 1, "M");
    //State q_0 = construct_state("q_0", 1);
    m.alphabet = malloc(5);
    str_cpy(m.alphabet, "10\0");
    //m.accept_states = malloc(1 * sizeof(State));
    //State q_1 = construct_state("q_1", 1);
    dfa_add_state(&m, "q_0", 2);
    dfa_add_state(&m, "q_1", 2);
    dfa_add_state(&m, "q_2", 2);
    dfa_add_accept_state(&m, "q_3", 2);
    //printf("%s", m.states[0].name);
    //dfa_add_state(&m, "q_1", 1);
    m.curr_state = &m.states[0];
    m.start_state = &m.states[0];
    //add_state(&m, &q_0);
    //add_state(&m, &q_1);
    add_transition(&m.states[0], &m.states[0], '1');
    add_transition(&m.states[0], &m.states[1], '0');
    add_transition(&m.states[1], &m.states[1], '0');
    add_transition(&m.states[1], &m.states[2], '1');
    add_transition(&m.states[2], &m.states[1], '0');
    add_transition(&m.states[2], &m.states[3], '1');
    add_transition(&m.states[3], &m.states[3], '0');
    add_transition(&m.states[3], &m.states[3], '1');
    //add_transition(&q_0, &q_1, 'a');
    //add_transition(&q_1, &q_0, 'b');
    //print_state(q_1);
    //transition(&m, 'a');
    //transition(&m, 'a');
    print_dfa(m);
    printf("\n");

    eval_input(&m, "01010100110");
    //dfa_complement_construction(&m);
    eval_input(&m, "0100");
    //print_state(q_0);


    DFA g = construct_DFA(10, 1, "G");
    g.alphabet = malloc(5);
    str_cpy(g.alphabet, "10\0");
    dfa_add_start_state(&g, "x_0", 2);
    dfa_add_state(&g, "x_1", 2);
    dfa_add_state(&g, "x_2", 2);
    dfa_add_accept_state(&g, "x_4", 2);

    add_transition(get_state(&g, "x_0"), get_state(&g, "x_1"), '0');
    add_transition(get_state(&g, "x_0"), get_state(&g, "x_2"), '1');

    add_transition(get_state(&g, "x_2"), get_state(&g, "x_2"), '0');
    add_transition(get_state(&g, "x_2"), get_state(&g, "x_2"), '1');

    add_transition(get_state(&g, "x_1"), get_state(&g, "x_4"), '1');
    add_transition(get_state(&g, "x_1"), get_state(&g, "x_4"), '0');

    add_transition(get_state(&g, "x_4"), get_state(&g, "x_1"), '0');
    add_transition(get_state(&g, "x_4"), get_state(&g, "x_1"), '1');

    printf("\n");
    print_dfa(g);

    //char *k = get_state_on_input(&g.states[0], s[0])->name;
    //printf("State x_0 on input %s", k);
    DFA intersect_dfa = dfa_intersection_construction(g, m, "G Union M");
    printf("\n");
    print_dfa(intersect_dfa);

    eval_input(&intersect_dfa, "0010");

    if(dfa_emptiness_problem(&intersect_dfa) == TRUE){
        printf("\nEmpty DFA");
    }
    else{
        printf("\nNot Empty DFA\n");
    }

    return 0;
}

State* get_state(DFA *m, char *state_name){
    int n_states = m->n_states;
    int i;
    for(i = 0; i < n_states; i++){
        if(str_cmp(state_name, (m->states[i]).name) == 0){
            return &m->states[i];
        }
    }
    return NULL;
}

DFA construct_DFA(int max_states, int n_accepts, char *name){
    DFA c = {0, 0, 0, 0, };
    c.states = malloc(max_states * sizeof(State));
    c.accept_states = malloc(n_accepts * sizeof(State));
    c.name = name;
    return c;
}

void print_dfa(DFA m){
    int n_states = m.n_states;
    int n_accept_states = m.n_accepts;
    printf("DFA %s:\n", m.name);
    if(m.curr_state != NULL){
        printf("Current State: %s\n", m.curr_state->name);
    }
    printf("Accept States: \n");
    for(int j = 0; j < n_accept_states; j++){
        printf("\t%s\n", m.accept_states[j].name);
    }

    printf("\nStates:\n");
    for(int i = 0; i < n_states; i++){
        print_state(m.states[i]);
    }
    printf("\n");
    return;
}

void add_state(DFA *m, State *s){
    m->states[m->n_states] = *s;
    m->n_states++;
    return;
}

void dfa_add_start_accept_state(DFA *m, char *name, int t_num){
    State s = construct_state(name, t_num);
    m->states[m->n_states] = s;
    m->curr_state = &m->states[m->n_states];
    m->start_state = &m->states[m->n_states];
    m->accept_states[m->n_accepts] = s;
    m->n_accepts++;
    m->n_states++;
}

void dfa_add_start_state(DFA *m, char *name, int t_num){
    State s = construct_state(name, t_num);
    m->states[m->n_states] = s;
    m->start_state = &m->states[m->n_states];
    m->curr_state = &m->states[m->n_states];
    m->n_states++;
    return;
}

void dfa_add_accept_state(DFA *m, char *name, int t_num){
    State s = construct_state(name, t_num);
    m->states[m->n_states] = s;
    m->accept_states[m->n_accepts] = s;
    m->n_states++;
    m->n_accepts++;
    return;
}

void dfa_add_state(DFA *m, char *name, int t_num){
    State s = construct_state(name, t_num);
    m->states[m->n_states] = s;
    m->n_states++;
    return;
}

State construct_state(const char *name, int t_num){
    State q = {name, 0, };
    q.t_list = malloc (t_num * sizeof(Transition));
    q.name = name;
    return q;
}

void print_state(State s){
    printf("\tState Name: %s\n", s.name);
    print_transitions(s);
    return;
}

void print_transitions(State s){
    for (int i = 0; i < s.t_num; i++){
        printf("\tTo: %s on: %c\n", s.t_list[i].to_state->name, s.t_list[i].input);
    }
    return;
}

void dfa_reset(DFA *m){
    m->curr_state = m->start_state;
}

int eval_input(DFA *m, char *inp){
    printf("\nDFA %s on input: \"%s\":\n", m->name, inp);
    int i;
    for(i = 0; inp[i] != '\0'; i++){
        if(transition(m, inp[i]) == FALSE){
            dfa_reset(m);
            printf("Rejected\n");
            return FALSE;
        }
    }
    if(dfa_is_accept(m, m->curr_state) == TRUE){
       dfa_reset(m);
       printf("Accepted\n");
       return TRUE;
    }
    dfa_reset(m);
    printf("Rejected\n");
    return FALSE;
}

Visited *v_states = NULL;

void set_visited(int visited, char *state_name){
    Visited *v_state;

    HASH_FIND_STR(v_states, state_name, v_state);
    if(v_state == NULL){
        v_state = (Visited *) malloc(sizeof(*v_state));
        v_state->is_visited = TRUE;
        v_state->name = state_name;
        HASH_ADD_KEYPTR(hh, v_states, v_state->name, strlen(v_state->name), v_state);
    }
}

int is_visited(char *state_name){
    Visited *v_state;

    HASH_FIND_STR(v_states, state_name, v_state);
    if(v_state == NULL){
        return FALSE;
    }
    if(v_state->is_visited == FALSE){
        return FALSE;
    }

    return TRUE;
}

void free_table(){
    Visited *curr, *tmp;

    HASH_ITER(hh, v_states, curr, tmp){
        HASH_DEL(v_states, curr);
        free(curr);
    }
}

void print_visited() {
    Visited *s;
    for(s=v_states; s != NULL; s=(Visited*)(s->hh.next)) {
        printf("State name: %s, is visited: %d\n", s->name, s->is_visited);
    }
}

int dfa_emptiness_problem(DFA *m){
    Queue *bfs_queue = constructQueue(m->n_states + 2);

    int i, j;
    State *temp = m->start_state;
    //printf("\nTemp state name: %s, t_num: %d\n", temp->name, temp->t_num);
    enqueue(bfs_queue, temp);

    //printf("Queue Size: %d\n", bfs_queue->size);
    //printf("\nBFS Queue Head Name: %s\n", bfs_queue->tail->state->name);
    while(bfs_queue->size > 0){
        temp = dequeue(bfs_queue)->state;
        //printf("\nTemp State name: %s", temp->name);
        //print_visited();

        /* if temp has not been visited */
        if(is_visited(temp->name) == FALSE){
            /* flag temp as visited */
            set_visited(TRUE, temp->name);

            if(dfa_is_accept(m, temp) == TRUE){
                return FALSE;
            }

            for(i = 0; i < temp->t_num; i++){
                enqueue(bfs_queue, (temp->t_list[i].to_state));
            }
        }

    }

    destructQueue(bfs_queue);
    free_table();
    return TRUE;
}

int dfa_equivalence_problem(DFA *m1, DFA *m2){
    DFA m1_complement = dfa_complement_construction(m1, "c_M1");
    DFA m2_complement = dfa_complement_construction(m2, "c_M2");

    DFA intersection_1 = dfa_intersection_construction(*m1, m2_complement, "i_1");
    DFA intersection_2 = dfa_intersection_construction(*m2, m1_complement, "i_2");

    DFA union_inter1_inter2 = dfa_union_construction(intersection_1, intersection_2, "u1");

    /* TRUE or FALSE */
    return dfa_emptiness_problem(&union_inter1_inter2);
}

int dfa_is_accept(DFA *m, State *s){
    int i;
    for(i = 0; i < m->n_accepts; i++){
        if(str_cmp(s->name, (m->accept_states[i]).name) == 0){
            return TRUE;
        }
    }
    return FALSE;
}

int dfa_is_start_state(DFA *m, State *s){
    if(str_cmp(s->name, m->start_state->name) == 0){
        return TRUE;
    }
    return FALSE;
}

void add_transition(State *from_s, const State *to_s, char c){
    from_s->t_list[(from_s->t_num)] = (Transition){to_s, c};
    from_s->t_num++;
    return;
}

int transition(DFA *m, char c){
    for(int i = 0; i < m->curr_state->t_num; i++){
        if (m->curr_state->t_list[i].input == c){
            printf("%s -> %s\n", m->curr_state->name, m->curr_state->t_list[i].to_state->name);
            m->curr_state = m->curr_state->t_list[i].to_state;
            return TRUE;
        }
    }
    m->curr_state = NULL;
    return FALSE;
}

void dfa_complement_transformation(DFA *m){
    int n_states = m->n_states;
    int n_accept_states = m->n_accepts;
    State *new_accepts = malloc(((m->n_states) - (m->n_accepts)) * sizeof(State));
    for(int i = 0, j = 0; i < n_states; i++){
        if(dfa_is_accept(m, &(m->states[i])) == FALSE){
            new_accepts[j++] =  m->states[i];
        }
    }
    free(m->accept_states);
    m->n_accepts = ((m->n_states) - (m->n_accepts));
    m->accept_states = new_accepts;
    return;
}

DFA dfa_complement_construction(DFA *m, char *name){
    DFA c = {0, 0, 0, 0, };
    int new_n_states = m->n_states;
    int new_n_accept_states = ((m->n_states) - (m->n_accepts));
    c.n_alphabet = str_len(m->alphabet);
    c.states = malloc(new_n_states * sizeof(State));
    c.accept_states = malloc(new_n_accept_states * sizeof(State));
    c.alphabet = malloc((c.n_alphabet) * sizeof(char));
    c.name = name;
    str_cpy(c.alphabet, m->alphabet);

    for(int i = 0; i < new_n_states; i++){
        if(dfa_is_accept(m, &(m->states[i])) == FALSE){
            if(dfa_is_start_state(m, &(m->states[i]) == TRUE)){
                dfa_add_start_accept_state(&c, m->states[i].name, m->states[i].t_num);
            }
            else{
                dfa_add_accept_state(&c, m->states[i].name, m->states[i].t_num);
            }
        }
        else if(dfa_is_start_state(m, &(m->states[i])) == TRUE){
            dfa_add_start_state(m, m->states[i].name, m->states[i].t_num);
        }else{
            dfa_add_state(m, m->states[i].name, m->states[i].t_num);
        }
    }

    int i, j;
    char *t1;
    for(i = 0; i < new_n_states; i++){
        for(j = 0; j < c.n_alphabet; j++){
             t1 = get_state_on_input(&m->states[i], c.alphabet[j])->name;
             add_transition(&c.states[i], get_state_on_input(&m->states[i], c.alphabet[j]), c.alphabet[j]);
        }
    }

    return c;
}

State *get_state_on_input(State *s, char c){
    int i;
    for(i = 0; i < s->t_num; i++){
        if(s->t_list[i].input == c){
            return (s->t_list[i].to_state);
        }
    }
    return NULL;
}

DFA dfa_union_construction(DFA m_1, DFA m_2, char *name){
    DFA c = {0, 0, 0, 0, };
    int new_n_states = (m_1.n_states * m_2.n_states);
    int new_n_accept_states = (m_1.n_accepts * m_2.n_states) + (m_2.n_accepts * m_1.n_states);
    int c_alphabet = str_len(m_1.alphabet);
    c.states = malloc(new_n_states * sizeof(State));
    c.accept_states = malloc(new_n_accept_states * sizeof(State));
    c.alphabet = malloc((c_alphabet+1) * sizeof(char));
    c.n_alphabet = c_alphabet;
    c.name = name;
    str_cpy(c.alphabet, m_1.alphabet);

    int i, j;
    char *new_state_name;
    int new_n_transitions = 0;
    for(i = 0; i < m_1.n_states; i++){
        for(j = 0; j < m_2.n_states; j++){
            new_state_name = malloc((str_len(m_1.states[i].name) + str_len(m_2.states[j].name) + 10) * sizeof(char));
            sprintf(new_state_name, "(%s, %s)", m_1.states[i].name,  m_2.states[j].name);
            new_n_transitions = m_1.states[i].t_num;

            if(dfa_is_accept(&m_1, &(m_1.states[i])) || dfa_is_accept(&m_2, &(m_2.states[j]))){
                if(dfa_is_start_state(&m_1, &(m_1.states[i])) && dfa_is_start_state(&m_2, &(m_2.states[j]))){
                    dfa_add_start_accept_state(&c, new_state_name, new_n_transitions);
                }
                else{
                    dfa_add_accept_state(&c, new_state_name, new_n_transitions);
                }
            }
            else if(dfa_is_start_state(&m_1, &(m_1.states[i])) && dfa_is_start_state(&m_2, &(m_2.states[j]))){
                dfa_add_start_state(&c, new_state_name, new_n_transitions);
            }
            else{
                dfa_add_state(&c, new_state_name, new_n_transitions);
            }
        }
    }

    int k;
    int s_index = 0;
    char *t1;
    char *t2;
    char *to_s = malloc(25 * sizeof(char));
    for(i = 0; i < m_1.n_states; i++){
        for(j = 0; j < m_2.n_states; j++){
            //s_index = (m_2.n_states * i) + j;
            for(k = 0; k < c_alphabet; k++){
                t1 = get_state_on_input(&m_1.states[i], c.alphabet[k])->name;
                t2 = get_state_on_input(&m_2.states[j], c.alphabet[k])->name;
                sprintf(to_s, "(%s, %s)", t1, t2);
                add_transition(&(c.states[s_index]), get_state(&c, to_s), c.alphabet[k]);
            }
            s_index++;
        }
    }
    return c;
}

DFA dfa_intersection_construction(DFA m_1, DFA m_2, char *name){
    DFA c = {0, 0, 0, 0, };
    int new_n_states = (m_1.n_states * m_2.n_states);
    int new_n_accept_states = (m_1.n_accepts * m_2.n_accepts);
    int c_alphabet = str_len(m_1.alphabet);
    c.states = malloc(new_n_states * sizeof(State));
    c.accept_states = malloc(new_n_accept_states * sizeof(State));
    c.alphabet = malloc((c_alphabet+1) * sizeof(char));
    c.name = name;
    str_cpy(c.alphabet, m_1.alphabet);

    int i, j;
    char *new_state_name;
    int new_n_transitions = 0;
    for(i = 0; i < m_1.n_states; i++){
        for(j = 0; j < m_2.n_states; j++){
            new_state_name = malloc((str_len(m_1.states[i].name) + str_len(m_2.states[j].name) + 10) * sizeof(char));
            sprintf(new_state_name, "(%s, %s)", m_1.states[i].name,  m_2.states[j].name);
            new_n_transitions = m_1.states[i].t_num;

            if(dfa_is_accept(&m_1, &(m_1.states[i])) && dfa_is_accept(&m_2, &(m_2.states[j]))){
                if(dfa_is_start_state(&m_1, &(m_1.states[i])) && dfa_is_start_state(&m_2, &(m_2.states[j]))){
                    dfa_add_start_accept_state(&c, new_state_name, new_n_transitions);
                }
                else{
                    dfa_add_accept_state(&c, new_state_name, new_n_transitions);
                }
            }
            else if(dfa_is_start_state(&m_1, &(m_1.states[i])) && dfa_is_start_state(&m_2, &(m_2.states[j]))){
                dfa_add_start_state(&c, new_state_name, new_n_transitions);
            }
            else{
                dfa_add_state(&c, new_state_name, new_n_transitions);
            }
        }
    }

    int k;
    int s_index = 0;
    char *t1;
    char *t2;
    char *to_s = malloc(25 * sizeof(char));
    for(i = 0; i < m_1.n_states; i++){
        for(j = 0; j < m_2.n_states; j++){
            //s_index = (m_2.n_states * i) + j;
            for(k = 0; k < c_alphabet; k++){
                t1 = get_state_on_input(&m_1.states[i], c.alphabet[k])->name;
                t2 = get_state_on_input(&m_2.states[j], c.alphabet[k])->name;
                sprintf(to_s, "(%s, %s)", t1, t2);
                add_transition(&(c.states[s_index]), get_state(&c, to_s), c.alphabet[k]);
            }
            s_index++;
        }
    }
    return c;
}









/* str_len: return the length of the string */
int str_len(char *s1){
    int i;
    for(i = 0; s1[i] != '\0'; i++);
    return i;
}


int str_cmp(const char* s1, const char* s2){
    while(*s1 && (*s1 == *s2))
    {
        s1++;
        s2++;
    }
    return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

void str_cpy(char *to, char *from){
    for(int i = 0; (to[i] = from[i]) != '\0'; i++);
}

/* str_cat: concatenate t to s, expects that s is big enough */
void str_cat(char *s, char *t){
    while(*s){
        s++;
    }
    while((*s=*t)){
        s++, t++;
    }
}




int enqueue(Queue *queue, State *item){
    if(queue->size >= queue->limit){
        return FALSE;
    }

    if((queue == NULL) || (item == NULL)){
        printf("enqueue: bad parameter");
        return FALSE;
    }

    Node *temp = (Node*) malloc(sizeof(Node));
    temp->state = item;

    if(queue->size == 0){
        queue->size++;
        temp->next = NULL;
        queue->head = queue->tail = temp;
        //printf("enqueue item: %s", queue->tail->state->name);
        return TRUE;
    }

    queue->tail->next = temp;
    queue->tail = temp;
    queue->size++;
    return TRUE;
}

Node *dequeue(Queue *queue){
    if(queue->size == 0){
        return NULL;
    }

    Node *temp = queue->head;
    //printf("Dequeue state name: %s", temp->state->name);
    queue->head = queue->head->next;

    if(queue->head == NULL){
        queue->tail = NULL;
    }
    queue->size--;
    return temp;
}

Queue *constructQueue(int limit) {
    Queue *queue = (Queue*) malloc(sizeof (Queue));
    if (queue == NULL) {
        return NULL;
    }
    if (limit <= 0) {
        limit = 65535;
    }
    queue->limit = limit;
    queue->size = 0;
    queue->head = NULL;
    queue->tail = NULL;

    return queue;
}

void destructQueue(Queue *queue){
    Node *tmp;
    while(queue->size > 0){
        tmp = dequeue(queue);
        free(tmp);
    }
    free(queue);
}

