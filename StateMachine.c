#include "StateMachine.h"

statemachine_t StateMachine__BuildURI1824StateMachine(dictionary_t dictionary)
{
    //Initializing state machine with only initial state
    statemachine_t state_machine;
    state_machine.num_of_states = 1;
    state_machine.transition_table = (uint **) malloc(sizeof(uint)*MAX_NUM_TRANSITIONS);

    //Each word is started at state 0 (initial state)
    uint * words_state = malloc(sizeof(uint)*(dictionary.num_of_words));
    memset(words_state,INITIAL_STATE,dictionary.num_of_words);

    bool * states_is_final;
    //For each possible char id allowed by word
    for(uint char_id = 0; char_id < MAX_NUM_CHARS; char_id++)
    {
        //For each word on the dictionary
        uint completed_words = 0;
        uint new_states = 0;
        for(uint word_id = 0; word_id < dictionary.num_of_words; word_id++ )
        {
            //If Word was not completed yet and is a valid state
            uint current_state = words_state[word_id];
            if(current_state != FINAL_STATE && current_state < state_machine.num_of_states)
            {
                uint * current_state_transitions = (uint *) &state_machine.transition_table[current_state];
                char word_char = dictionary.list_of_words[word_id][char_id];
                uint transition_id = (word_char == '\0') ? END_TRANSITION : 'z' - word_char;

                //If no transition exists from current state, then create a new state
                if(current_state_transitions[transition_id] == NO_TRANSITION)
                {
                    state_machine.num_of_states++;
                    current_state_transitions[transition_id] = state_machine.num_of_states;
                    
                    //Recreate the transition table by increasing one more state to the table of transitions
                    uint ** new_transition_table = malloc(sizeof(uint)*MAX_NUM_TRANSITIONS*state_machine.num_of_states);
                    memcpy(new_transition_table,state_machine.transition_table,sizeof(uint)*MAX_NUM_TRANSITIONS*(state_machine.num_of_states-1));
                    memset(new_transition_table[state_machine.num_of_states-1],NO_TRANSITION,sizeof(uint)*MAX_NUM_TRANSITIONS);
                    free(state_machine.transition_table);
                    state_machine.transition_table = new_transition_table;
                    current_state_transitions = (uint *) &state_machine.transition_table[current_state];

                    new_states++;
                }

                //Update the current state of the word
                words_state[word_id] = current_state_transitions[transition_id];

                if(transition_id == END_TRANSITION)
                {
                    current_state = FINAL_STATE;
                }
            }
            else
            {
                completed_words++;
            }
        }

        //If any of the new states only have one transition out can be considered as if the word was finished
        for(uint word_id = 0; word_id < dictionary.num_of_words; word_id++ )
        {
            if(words_state[word_id] != FINAL_STATE)
            {
                uint * current_state_transitions = (uint *) &state_machine.transition_table[words_state[word_id]];
                uint number_transitions = 0;
                for(uint transition = 0; transition < MAX_NUM_TRANSITIONS; transition++)
                {
                    if(current_state_transitions[transition] != NO_TRANSITION)
                        number_transitions++;
                    
                    if(number_transitions > 1)
                        break;
                }

                if(number_transitions == 1)
                    words_state[word_id] = FINAL_STATE;
            }
        }


        //If every word is completed then finish state machine creation
        if(completed_words == dictionary.num_of_words)
        {
            break;
        }
    }

    return(state_machine);
}