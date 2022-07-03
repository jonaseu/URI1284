#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef unsigned int uint;

//URI1824_defs===============================================================================
#define MAX_WORD_LENGTH     (80)

#define MAX_NUM_WORDS       (1000000)

#define MAX_NUM_CHARS       (10000000)

#define NUM_CHARS_ALLOWED   (26)    //'a' to 'z', only lowercase

//Defines and types =========================================================================

//Define to enable or disable the execution of test cases
#define RUN_TESTS           false

//Define to enable or disable additional log that helps understanding of the code
#define PRINT_LOG           true


typedef struct dictionary_t
{
    uint num_of_words;
    char ** list_of_words;
}dictionary_t;

typedef struct node_t
{
    uint state_id;
    uint transition_id;
    uint num_words;
    struct node_t * next_sibling;
    struct node_t * first_child;
}node_t;

static double StateMachine__CalculateKeysPressedByTree(dictionary_t * dictionary);
void WalkTreeAndCountKeysPressed(node_t* current_node);
void RunTests(void);

//===========================================================================================
int main(int argc, char const *argv[])
{    
    if(RUN_TESTS == true)
    {
        RunTests();
        getchar();
        return 0;
    }

    //GET USER INPUT ============================================================================
    #define MAX_DICTIONARIES 200
    uint num_of_dictionaries = 0;
    dictionary_t dictionary_list[MAX_DICTIONARIES];
    dictionary_t * current_dictionary;

    printf("Write your input in the format of 'N0;word1;word2;...;wordN0;N1;word1;word2;...;wordN1'. Press CTRL+Z when done\n");
    while(true)
    {            
        uint dictionary_size;
        if(scanf("%i", &dictionary_size) != EOF)
        {
            current_dictionary = &dictionary_list[num_of_dictionaries];
            current_dictionary->num_of_words = dictionary_size;
            current_dictionary->list_of_words = (char **) malloc(sizeof(char *)*dictionary_size);
            for(uint word_counter = 0; word_counter < dictionary_size;word_counter++)
            {
                char input_word[MAX_WORD_LENGTH];
                scanf("%s",input_word);
                current_dictionary->list_of_words[word_counter] = (char *) calloc(MAX_WORD_LENGTH,sizeof(char));
                strcpy(current_dictionary->list_of_words[word_counter],input_word);
            }
            num_of_dictionaries++;
        }
        else
        {
            break;
        }

        if(num_of_dictionaries > MAX_DICTIONARIES)
            break;
    }

    //CALCULATE KEYS PRESSED =====================================================================
    for(uint dictId = 0; dictId < num_of_dictionaries;dictId++)
        printf("%.2f\n",StateMachine__CalculateKeysPressedByTree(&dictionary_list[dictId]));

    getchar();
    return 0;
}


//===========================================================================================
//=================================SUPPORT FUNTIONS==========================================
//===========================================================================================
uint keys_pressed;
node_t RootNode = {.num_words = 0,.next_sibling = NULL,.first_child = NULL};

static double StateMachine__CalculateKeysPressedByTree(dictionary_t * dictionary)
{
    #define SIZEOF_DICT_ARRAY        (dictionary->num_of_words*sizeof(uint))
    keys_pressed = dictionary->num_of_words;

    //Initialize tree
    int num_states = 0;
    node_t RootNode = {.num_words = dictionary->num_of_words,.next_sibling = NULL,.first_child = NULL};

    //Array to keep track of currentt state of each word
    node_t ** words_state = (node_t **) calloc(dictionary->num_of_words,sizeof(node_t*));
    for(uint word_id = 0; word_id < dictionary->num_of_words; word_id++ )
        words_state[word_id] = &RootNode;

    //Array to track if a word is completed or not, thus meaning it doesn't need to be further evaluated
    bool * word_complete = (bool *) calloc(dictionary->num_of_words,sizeof(bool));

    //For each possible char on a word
    for(uint char_id = 0; char_id < MAX_NUM_CHARS; char_id++)
    {
        //For each word on the dictionary
        for(uint word_id = 0; word_id < dictionary->num_of_words; word_id++ )
        {
            if(word_complete[word_id] == false)
            {
                char transition_id = dictionary->list_of_words[word_id][char_id];
                bool transition_exists = false;
                node_t * current_node = words_state[word_id];
                node_t * iteration_node = current_node->first_child;

                //Finds if a transition already exists on this state
                if(iteration_node != NULL)
                {
                    do
                    {
                        if(iteration_node->transition_id == transition_id)
                        {
                            transition_exists = true;
                            break;
                        }
                        iteration_node = iteration_node->next_sibling;
                    }while(iteration_node != NULL);
                }

                node_t * next_node;
                if(transition_exists)
                {
                    //If a transition already exists for given state just increase amount of words using that next state
                    iteration_node->num_words++;
                    next_node = iteration_node;
                }
                else
                {
                    //If a transition for given state didn't exist previously then create a new child
                    num_states++;
                    node_t * new_node = calloc(1,sizeof(node_t));
                    new_node->transition_id = transition_id;
                    new_node->num_words = 1;
                    new_node->state_id = num_states;
                    if(current_node->first_child == NULL)
                    {
                        current_node->first_child = new_node;
                    }
                    else
                    {
                        node_t * iteration_node = current_node->first_child;
                        while(iteration_node->next_sibling != NULL) iteration_node = iteration_node->next_sibling;
                        iteration_node->next_sibling = new_node;
                    }
                    next_node = new_node;
                    if(PRINT_LOG) printf("\t%.*s [%i] new transition %c [%i]\n",char_id,dictionary->list_of_words[word_id],current_node->state_id,transition_id,next_node->state_id);
                }

                //If it's end of the word, removes the number of words using this state
                //Needed for cases like "hell" and "hello", where the state that represents chardId 3 ("hell") shall not consider the word "hell"
                if(transition_id == '\0')
                    current_node->num_words--;

                words_state[word_id] = next_node;
            }
        }

        //Verify which words are done. A word is done when it's current state has less than 1 word using it, thus meaning that words is now uniquely determined
        uint completed_words = 0;
        for(uint word_id = 0; word_id < dictionary->num_of_words; word_id++ )
        {
            if(word_complete[word_id] == false)
            {
                if(words_state[word_id]->num_words <= 1)
                {
                    word_complete[word_id] = true;
                    completed_words++;
                }
            }
            else
                completed_words++;
        }

        //If every word has been completed then the tree is done
        if(completed_words == dictionary->num_of_words)
            break;
    }

    //Recursively walk the tree
    WalkTreeAndCountKeysPressed(&RootNode);
    if(PRINT_LOG) 
    {
        printf("\t+ %i keys need pressed at start as %i words on input\n",dictionary->num_of_words,dictionary->num_of_words);
        printf("\t= %i keys pressed\n",keys_pressed);
    }

    return( ((double)(keys_pressed)/dictionary->num_of_words) );
}

void WalkTreeAndCountKeysPressed(node_t* current_node) {
    if(current_node != NULL) 
    {
        if(current_node->first_child != NULL)
        {
            WalkTreeAndCountKeysPressed(current_node->first_child);
            //If the current node has more than 1 child (thus it's child having a sibling), that means a key needs to be pressed to choose 
            //between the childs. The amounts of keys to be pressed will shall be incremented by how many words are using it.
            if( current_node->first_child->next_sibling != NULL &&
                current_node->state_id != 0)
            {
                if(PRINT_LOG) printf("\t+ %i keys need to be pressed on state %i\n",current_node->num_words,current_node->state_id);
                keys_pressed += current_node->num_words;
            }
        }

        if(current_node->next_sibling != NULL)
            WalkTreeAndCountKeysPressed(current_node->next_sibling);
    }
}


void RunTests(void)
{
    #define NUM_INPUTS_TO_TEST  3

    #define TEST1_SIZE  4
    char * test1_words[TEST1_SIZE] ={"hello","hell","heaven","go"};
    #define TEST2_SIZE  3
    char * test2_words[TEST2_SIZE] ={"he","h","hi"};
    #define TEST3_SIZE  7
    char * test3_words[TEST3_SIZE] ={"structure","structures","ride","riders","stress","solstice","ridiculous"};
    
    dictionary_t basic_inputs_to_test[NUM_INPUTS_TO_TEST] =
    {
        {
            .num_of_words = TEST1_SIZE,
            .list_of_words = test1_words,
        },
        {
            .num_of_words = TEST2_SIZE,
            .list_of_words = test2_words,
        },
        {
            .num_of_words = TEST3_SIZE,
            .list_of_words = test3_words,
        },
    };

    for(uint test_id = 0; test_id < NUM_INPUTS_TO_TEST; test_id++)
    {
        if(PRINT_LOG)
        {
            printf("\n\nTest #%i\n",test_id);
            
            dictionary_t * current_dictionary = &basic_inputs_to_test[test_id];
            for(uint word_counter = 0; word_counter < current_dictionary->num_of_words;word_counter++)
                printf("->%s\n",current_dictionary->list_of_words[word_counter]);
        }
        
        printf("%.2f\n",StateMachine__CalculateKeysPressedByTree(&basic_inputs_to_test[test_id]));
    }
}