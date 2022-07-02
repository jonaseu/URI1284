#ifndef STATEMACHINE_H_
#define STATEMACHINE_H_

#include "URI1824_defs.h"

#define INITIAL_STATE           ((uint)0)              
#define FINAL_STATE             ((uint)-1)              
#define MAX_NUM_TRANSITIONS     (NUM_CHARS_ALLOWED + 1) //NUM_CHARS_ALLOWED + end transition
#define NO_TRANSITION           ((uint)-1)
#define END_TRANSITION          (MAX_NUM_TRANSITIONS-1) //Transition that indicates end of word

typedef struct dictionary_t
{
    uint num_of_words;
    char ** list_of_words;
    struct dictionary_t * next;
}dictionary_t;

typedef struct 
{
    uint num_of_states;
    uint ** transition_table;
}statemachine_t;


statemachine_t StateMachine__BuildURI1824StateMachine(dictionary_t dictionary);


#endif // STATEMACHINE_H_
