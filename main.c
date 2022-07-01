#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>


#define MAX_WORD_LENGTH  80
#define MAX_NUMBER_WORDS 1000000
#define MAX_NUMBER_CHARS 10000000

int main(int argc, char const *argv[])
{
    //GET USER INPUT ========================================================================
    unsigned int number_of_dictionaries = 0;
    unsigned int * dictionary_sizes = malloc(MAX_NUMBER_WORDS/1000 * sizeof (unsigned int));
    char ** dictionary_words = malloc(MAX_NUMBER_WORDS * sizeof (char *));

    memset(dictionary_sizes,0,sizeof(dictionary_sizes));

    while(true)
    {            
        unsigned long dictionary_size;
        char buffer[100];
        fgets(buffer, sizeof(buffer), stdin);
        if(sscanf(buffer, "%lu", &dictionary_size) == 1)
        {
            dictionary_sizes[number_of_dictionaries] = dictionary_size;
            for(unsigned long word_counter = 0; word_counter < dictionary_size;word_counter++)
            {
                dictionary_words[word_counter] = (char *) malloc(MAX_WORD_LENGTH*sizeof(char));
                char input_word[MAX_WORD_LENGTH];
                scanf(" %s",input_word);
                while ((getchar()) != '\n');
                dictionary_words[word_counter] = input_word;
            }
        }
        else
        {
            break;
        }

        number_of_dictionaries++;
    }

    //FOR EACH DICTIONARY BUILD A STATE MACHINE AND EVALUATE IT =============================
    for(unsigned int dictionaryId = 0; dictionaryId < number_of_dictionaries; dictionaryId++)
    {

    }

    return 0;
}
