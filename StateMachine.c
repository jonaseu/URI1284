#include "StateMachine.h"

#define SIZEOF_TRANSITION_ARRAY (sizeof(uint)*MAX_NUM_TRANSITIONS)

statemachine_t StateMachine__BuildURI1824StateMachine(dictionary_t dictionary)
{
    //Initializing state machine with only initial state
    statemachine_t state_machine;
    state_machine.num_of_states = 1;
    state_machine.transition_table = (uint **) malloc(sizeof(uint *));
    state_machine.transition_table[0] = (uint *) malloc(SIZEOF_TRANSITION_ARRAY);
    memset(state_machine.transition_table[0],NO_TRANSITION,SIZEOF_TRANSITION_ARRAY);
    
    uint * words_per_state = malloc(sizeof(uint));
    words_per_state[0] = dictionary.num_of_words;

    //Each word is started at state 0 (initial state)
    uint * words_state = malloc(sizeof(uint)*(dictionary.num_of_words));
    memset(words_state,INITIAL_STATE,dictionary.num_of_words*sizeof(uint));

    bool * states_is_final;
    //For each possible char id allowed by word
    for(uint char_id = 0; char_id < MAX_NUM_CHARS; char_id++)
    {
        //For each word on the dictionary
        uint completed_words = 0;
        for(uint word_id = 0; word_id < dictionary.num_of_words; word_id++ )
        {
            //If Word was not completed yet and is a valid state
            uint current_state = words_state[word_id];
            if(current_state != FINAL_STATE && current_state < state_machine.num_of_states)
            {
                uint * current_state_transitions = (uint *) state_machine.transition_table[current_state];
                char word_char = dictionary.list_of_words[word_id][char_id];
                uint transition_id = (word_char == '\0') ? END_TRANSITION : word_char - 'a';

                //If no transition exists from current state, then create a new state
                if(current_state_transitions[transition_id] == NO_TRANSITION)
                {
                    state_machine.num_of_states++;
                    current_state_transitions[transition_id] = state_machine.num_of_states - 1;
                    
                    //Recreate the transition table by increasing one more state to the table of transitions
                    uint ** new_transition_table = (uint **) malloc(state_machine.num_of_states*sizeof(uint *));
                    for(int i = 0; i < state_machine.num_of_states; i++)
                    {
                        new_transition_table[i] = (uint *)malloc(SIZEOF_TRANSITION_ARRAY);
                        if(i == (state_machine.num_of_states - 1)) 
                            memset(new_transition_table[i], NO_TRANSITION, SIZEOF_TRANSITION_ARRAY);
                        else
                            memcpy(new_transition_table[i],state_machine.transition_table[i],SIZEOF_TRANSITION_ARRAY);
                    }
                    free(state_machine.transition_table);
                    state_machine.transition_table = new_transition_table;
                    current_state_transitions = (uint *) state_machine.transition_table[current_state];

                    //Recreate the list of number of words per state
                    uint * new_words_per_state = malloc(state_machine.num_of_states*sizeof(uint));
                    memcpy(new_words_per_state,words_per_state,(state_machine.num_of_states-1)*sizeof(uint));
                    new_words_per_state[state_machine.num_of_states-1] = 0;
                    words_per_state = new_words_per_state;
                }

                //Update the current state of the word
                words_state[word_id] = current_state_transitions[transition_id];
                words_per_state[words_state[word_id]]++;

                if(transition_id == END_TRANSITION)
                {
                    current_state = FINAL_STATE;
                }
            }
        }

        //If any of the new states only have one transition out can be considered as if the word was finished
        for(uint word_id = 0; word_id < dictionary.num_of_words; word_id++ )
        {
            if(words_state[word_id] != FINAL_STATE)
            {
                if(words_per_state[words_state[word_id]] <= 1)
                {
                    words_state[word_id] = FINAL_STATE;
                    completed_words++;
                }
            }
            else
            {
                completed_words++;
            }
        }

        //If every word is completed then finish state machine creation
        if(completed_words == dictionary.num_of_words)
        {
            break;
        }
    }

    free(words_state);

    return(state_machine);
}


bool StateMachine__URI1824StateMachineToCSV(statemachine_t statemachine, char * filename)
{
    FILE * fp;
    fp = fopen (filename, "w+");

    //Write headers
    fprintf(fp, "%s","State");
    for(uint transition = 0; transition < MAX_NUM_TRANSITIONS;transition++)
    {
        fprintf(fp, ",%c",('a'+transition));
    }

    //Write state machine
    for(uint state = 0; state < statemachine.num_of_states;state++)
    {
        fprintf(fp, "\n%i",state);
        for(uint transition = 0; transition < MAX_NUM_TRANSITIONS;transition++)
        {
            fprintf(fp, ",%i",statemachine.transition_table[state][transition]);
        }
    }

   fclose(fp);
   
   return(true); 
}