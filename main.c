#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>


#define MAX_WORD_LENGTH     (80)
#define MAX_NUM_WORDS       (1000000)
#define MAX_NUM_CHARS       (10000000)
#define MAX_NUM_TRANSITIONS (26 + 1) //26 letters + end transition

typedef unsigned int uint;

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

#define FINAL_STATE             ((uint)-1)
#define NO_TRANSITION           ((uint)-1)
#define END_TRANSITION          (MAX_NUM_TRANSITIONS-1)
//===========================================================================================
int main(int argc, char const *argv[])
{
    //GET USER INPUT ========================================================================
    uint num_of_dictionaries = 0;
    dictionary_t dictionary_list;
    dictionary_t * current_dictionary = &dictionary_list;

    while(true)
    {            
        uint dictionary_size;
        char buffer[100];
        fgets(buffer, sizeof(buffer), stdin);
        if(sscanf(buffer, "%lu", &dictionary_size) == 1)
        {
            current_dictionary->num_of_words = dictionary_size;
            current_dictionary->list_of_words = malloc(sizeof(char *)*dictionary_size);
            for(uint word_counter = 0; word_counter < dictionary_size;word_counter++)
            {
                char input_word[MAX_WORD_LENGTH];
                scanf(" %s",input_word);
                while ((getchar()) != '\n');
                current_dictionary->list_of_words[word_counter] = malloc(sizeof(char)*MAX_WORD_LENGTH);
                strcpy(current_dictionary->list_of_words[word_counter],input_word);
            }
        }
        else
        {
            break;
        }

        current_dictionary->next = calloc(1,sizeof(dictionary_t));
        current_dictionary = current_dictionary->next;
    }
    
    current_dictionary = &dictionary_list;
    //PRINT USER INPUT ====================================================================
    do
    {
        for(uint word_counter = 0; word_counter < current_dictionary->num_of_words;word_counter++)
        {
            printf("%s\n",current_dictionary->list_of_words[word_counter]);
        }

        current_dictionary = current_dictionary->next;
    } while (current_dictionary->next != NULL);
    

    return 0;
}


//===========================================================================================
//=================================SUPPORT FUNTIONS==========================================
//===========================================================================================
// statemachine_t BuildStateMachine(dictionary_t dictionary)
// {
//     statemachine_t state_machine;
//     state_machine.num_of_states = 1;
//     state_machine.transition_table = (uint **) malloc(sizeof(uint)*MAX_NUM_TRANSITIONS);

//     uint * words_state = malloc(sizeof(uint)*(dictionary.num_of_words));
//     memset(words_state,0,dictionary.num_of_words);

//     bool * states_is_final;
//     //For each possible char id allowed by word
//     for(uint char_id = 0; char_id < MAX_NUM_CHARS; char_id++)
//     {
//         //For each word on the dictionary
//         uint completed_words = 0;
//         uint new_states = 0;
//         for(uint word_id = 0; word_id < dictionary.num_of_words; word_id++ )
//         {
//             //If Word has not completed yet
//             if(words_state[word_id] != FINAL_STATE)
//             {
//                 uint * word_state_transitions = &state_machine.transition_table[words_state[word_id]];
//                 char word_char = dictionary.list_of_words[word_id][char_id];
//                 uint transition_id = (word_char == '\0') ? END_TRANSITION : 'z' - word_char;

//                 //If no transition exists from current state, propose to create a new state
//                 if(word_state_transitions[transition_id] == NO_TRANSITION)
//                 {
//                     state_machine.num_of_states++;
//                     word_state_transitions[transition_id] = state_machine.num_of_states;
                    
//                     uint * new_transition_table = malloc(sizeof(uint)*MAX_NUM_TRANSITIONS*state_machine.num_of_states);
//                     memcpy(new_transition_table,state_machine.transition_table,sizeof(uint)*MAX_NUM_TRANSITIONS*(state_machine.num_of_states-1));
//                     memset(new_transition_table[state_machine.num_of_states-1],NO_TRANSITION,sizeof(uint)*MAX_NUM_TRANSITIONS);
                    
//                     free(state_machine.transition_table);
//                     state_machine.transition_table = new_transition_table;
//                     word_state_transitions = &state_machine.transition_table[words_state[word_id]];

//                     new_states++;
//                 }

//                 //Update the current state of the word
//                 words_state[word_id] = word_state_transitions[transition_id];

//                 if(transition_id == END_TRANSITION)
//                 {
//                     words_state[word_id] = FINAL_STATE;
//                 }
//             }
//             else
//             {
//                 completed_words++;
//             }
//         }

//         //If any of the new states only have one transition out can be considered as if the word was finished
//         for(uint word_id = 0; word_id < dictionary.num_of_words; word_id++ )
//         {
//             if(words_state[word_id] != FINAL_STATE)
//             {
//                 uint * word_state_transitions = &state_machine.transition_table[words_state[word_id]];
//                 uint number_transitions = 0;
//                 for(uint transition = 0; transition < MAX_NUM_TRANSITIONS; transition++)
//                 {
//                     if(word_state_transitions[transition] != NO_TRANSITION)
//                         number_transitions++;
                    
//                     if(number_transitions > 1)
//                         break;
//                 }

//                 if(number_transitions == 1)
//                     words_state[word_id] = FINAL_STATE;
//             }
//         }


//         //If every word is completed then finish state machine creation
//         if(completed_words == dictionary.num_of_words)
//         {
//             break;
//         }
//     }
// }
