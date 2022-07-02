#include "URI1824_defs.h"
#include "StateMachine.h"

//===========================================================================================
int main(int argc, char const *argv[])
{
    //GET USER INPUT ========================================================================
    dictionary_t test_dictionary;

    // #define TEST_SIZE  4
    // char * input_words[TEST_SIZE] ={"hello","hell","heaven","go"};
    // #define TEST_SIZE  7
    // char * input_words[TEST_SIZE] ={"structure","structures","ride","riders","stress","solstice","ridiculous"};
    #define TEST_SIZE  3
    char * input_words[TEST_SIZE] ={"he","h","hi"};
    test_dictionary.num_of_words = TEST_SIZE;
    test_dictionary.list_of_words = (char **) &input_words;

    StateMachine__URI1824StateMachineToCSV(StateMachine__BuildURI1824StateMachine(test_dictionary), "statemachine.csv");
    return 0;


    // //GET USER INPUT ========================================================================
    // uint num_of_dictionaries = 0;
    // dictionary_t dictionary_list;
    // dictionary_t * current_dictionary = &dictionary_list;

    // while(true)
    // {            
    //     uint dictionary_size;
    //     char buffer[100];
    //     fgets(buffer, sizeof(buffer), stdin);
    //     if(sscanf(buffer, "%lu", &dictionary_size) == 1)
    //     {
    //         current_dictionary->num_of_words = dictionary_size;
    //         current_dictionary->list_of_words = (char **) malloc(sizeof(char *)*dictionary_size);
    //         for(uint word_counter = 0; word_counter < dictionary_size;word_counter++)
    //         {
    //             char input_word[MAX_WORD_LENGTH];
    //             scanf(" %s",input_word);
    //             while ((getchar()) != '\n');
    //             current_dictionary->list_of_words[word_counter] = (char *) malloc(SIZEOF_WORDS);
    //             strcpy(current_dictionary->list_of_words[word_counter],input_word);
    //         }
    //     }
    //     else
    //     {
    //         break;
    //     }

    //     current_dictionary->next = (dictionary_t *) calloc(1,sizeof(dictionary_t));
    //     current_dictionary = current_dictionary->next;
    // }
    
    // //PRINT USER INPUT ====================================================================
    // current_dictionary = &dictionary_list;
    // do
    // {
    //     for(uint word_counter = 0; word_counter < current_dictionary->num_of_words;word_counter++)
    //     {
    //         //TODO: Create state machine
    //         //TODO: Process state machine
    //         printf("%s\n",current_dictionary->list_of_words[word_counter]);
    //     }

    //     current_dictionary = current_dictionary->next;
    // } while (current_dictionary->next != NULL);
    

    // return 0;
}


//===========================================================================================
//=================================SUPPORT FUNTIONS==========================================
//===========================================================================================

