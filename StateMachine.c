#include "StateMachine.h"

#define SIZEOF_TRANSITION_ARRAY     (sizeof(uint)*MAX_NUM_TRANSITIONS)
#define ROUND(value)    (  (((double)((int)((value*100)+.5)))/100) )

statemachine_t StateMachine__BuildURI1824StateMachine(dictionary_t dictionary)
{
    #define SIZEOF_DICT_ARRAY  (dictionary.num_of_words*sizeof(uint))
    
    uint clicks_pressed = dictionary.num_of_words;

    //Initializing state machine with only initial state
    statemachine_t state_machine;
    state_machine.num_of_states = 1;
    state_machine.transition_table = (uint **) malloc(sizeof(uint *));
    state_machine.transition_table[0] = (uint *) malloc(SIZEOF_TRANSITION_ARRAY);
    memset(state_machine.transition_table[0],NO_TRANSITION,SIZEOF_TRANSITION_ARRAY);

    //Array to keep track of currentt state of each word
    uint * words_state = (uint *) malloc(SIZEOF_DICT_ARRAY);
    memset(words_state,INITIAL_STATE,SIZEOF_DICT_ARRAY);
    //Array to keep track of the past state of the word, thus facilitating to calculate the amount of keyspressed
    uint * words_previous_state = (uint *) malloc(SIZEOF_DICT_ARRAY);
    memcpy(words_previous_state,words_state,SIZEOF_DICT_ARRAY);

    //List to count the number of words per state,thus facilitating the decision to consider a word as completed
    uint * num_words_per_state = (uint *) malloc(sizeof(uint));
    num_words_per_state[0] = dictionary.num_of_words;

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
                        {
                            memcpy(new_transition_table[i],state_machine.transition_table[i],SIZEOF_TRANSITION_ARRAY);
                            free(state_machine.transition_table[i]);
                        }
                    }
                    free(state_machine.transition_table);
                    state_machine.transition_table = new_transition_table;
                    current_state_transitions = (uint *) state_machine.transition_table[current_state];

                    //Recreate the list of number of words per state
                    uint * new_words_per_state = (uint *) malloc(state_machine.num_of_states*sizeof(uint));
                    memcpy(new_words_per_state,num_words_per_state,(state_machine.num_of_states-1)*sizeof(uint));
                    new_words_per_state[state_machine.num_of_states-1] = 0;
                    num_words_per_state = new_words_per_state;
                }

                //Update the current state of the word
                words_state[word_id] = current_state_transitions[transition_id];
                num_words_per_state[words_state[word_id]]++;

                if(transition_id == END_TRANSITION)
                {
                    words_state[word_id] = FINAL_STATE;
                }
            }
        }

        //If any of the new states only have one transition out can be considered as if the word was finished
        for(uint word_id = 0; word_id < dictionary.num_of_words; word_id++ )
        {
            if(words_state[word_id] != FINAL_STATE)
            {
                if(num_words_per_state[words_state[word_id]] <= 1)
                {
                    words_state[word_id] = FINAL_STATE;
                    completed_words++;
                }
            }
            else
                completed_words++;

            //If the number of words from the previous state is different then the current state that the word is 
            //that implies that some of the words made other transition, thus a key was necessary
            uint current_state_num_words = num_words_per_state[words_state[word_id]];
            uint previous_state_num_words = num_words_per_state[words_previous_state[word_id]];
            if(current_state_num_words != previous_state_num_words)
            {
                char word_char = dictionary.list_of_words[word_id][char_id];
                //Do not count a key press if it's the first state or if the word has ended
                if(words_previous_state[word_id] != INITIAL_STATE && word_char != '\0')
                    clicks_pressed++;
            }

        }

        //If every word is completed then finish state machine creation
        if(completed_words == dictionary.num_of_words)
            break;

        memcpy(words_previous_state,words_state,SIZEOF_DICT_ARRAY);
    }


    // for(int i = 0; i < state_machine.num_of_states; i++)
    //     free(state_machine.transition_table[i]);
    free(state_machine.transition_table);
    free(words_state);
    free(words_previous_state);
    free(num_words_per_state);
    free(num_words_per_state);


    double average =  (double)(clicks_pressed)/dictionary.num_of_words;
    average = ROUND(average);
    return(state_machine);
}


bool StateMachine__URI1824StateMachineToCSV(statemachine_t statemachine, char * filename)
{
    FILE * fp;
    fp = fopen (filename, "w+");

    //Write headers
    fprintf(fp, "%s","State Name");
    fprintf(fp, ",%s","State");
    for(uint transition = 0; transition < MAX_NUM_TRANSITIONS;transition++)
    {
        char transition_char = (transition == (MAX_NUM_TRANSITIONS -1)) ? '*' : 'a'+transition;
        fprintf(fp, ",%c",transition_char);
    }

    //Initialize the names of each state to be printed
    char ** state_names = (char **) malloc(statemachine.num_of_states*sizeof(char *));
    for(int i = 0; i < statemachine.num_of_states; i++)
    {
        state_names[i] = (char *) malloc(SIZEOF_WORDS);
        memset(state_names[i],'\0',SIZEOF_WORDS);
    }

    //Write state machine
    for(uint state = 0; state < statemachine.num_of_states;state++)
    {
        fprintf(fp, "\n%s",state_names[state]);
        fprintf(fp, ",%i",state);
        for(uint transition = 0; transition < MAX_NUM_TRANSITIONS;transition++)
        {
            uint state_to_transition = statemachine.transition_table[state][transition];
            fprintf(fp, ",%i",state_to_transition);
            if(state_to_transition != FINAL_STATE)
            {
                //Copy the name of the current state and make that the next state will be called currentstate name + transition
                memcpy(state_names[state_to_transition],state_names[state],SIZEOF_WORDS);
                uint las_char_id = strlen(state_names[state_to_transition]);
                char transition_char = (transition == (MAX_NUM_TRANSITIONS -1)) ? '*' : 'a'+transition;
                state_names[state_to_transition][las_char_id] = transition_char;
            }
        }
    }

   fclose(fp);
   
   return(true); 
}