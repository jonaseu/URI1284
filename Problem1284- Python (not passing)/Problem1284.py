#import pandas

from logging import root
from math import inf

MAX_WORD_LENGTH = 80
MAX_TRANSITIONS = ord("z") - ord("a") + 1 + 1               #all the possible letters plus end of word indicator

def CalculatedAverageKeyPress(dictionary_words,print_info = False):

    if(len(dictionary_words) > 10000):
        return 0

    #Build a tree of nodes, where each node is a character and each child is a next possible character to be pressed
    nodes = {}
    nodes["root"] = [inf,[],False]
    rootNode = nodes["root"]
    keys_pressed = len(dictionary_words)

    for charIndex in range(0,MAX_WORD_LENGTH):
        wordsCompleted = 0
        nodes_evaluated = []
        for word in dictionary_words:
            word +='*'
            if(charIndex == 0):
                current_node = rootNode
                current_node_name = 'root'
            else:
                current_node = None
                current_node_name = word[0:charIndex]
                if(current_node_name in nodes and charIndex < len(word)):
                    current_node = nodes[current_node_name]

            nodes_evaluated.append(current_node_name)
            if(current_node != None and current_node[0] > 1):
                next_node_name = word[0:(charIndex+1)]
                if(next_node_name not in current_node[1]):
                    current_node[1].append(next_node_name)
                    nodes[next_node_name] = [1,[],False]
                else:
                    nodes[next_node_name][0] += 1

            else:
                wordsCompleted += 1

        for node_name in set(nodes_evaluated):
            if(node_name in nodes):
                node = nodes[node_name]
                if(node != rootNode and node[2] == False):
                    if(len(node[1]) > 1):
                        keys_pressed += node[0]
                        for child_name in node[1]:
                            if(child_name[-1] == '*'):
                                keys_pressed -= 1
                    node[2] = True

        for word in dictionary_words:
            current_node_name = word[0:charIndex]


        if(wordsCompleted == (len(dictionary_words)) ):
            break

    return round(keys_pressed/len(dictionary_words),2)


# CalculatedAverageKeyPress(['hello', 'hell', 'heaven', 'goodbye'])
# CalculatedAverageKeyPress(['hell', 'hello', 'heaven', 'goodbye'])
# CalculatedAverageKeyPress(['hi','he','h'])
# CalculatedAverageKeyPress(['structure','structures','ride','riders','stress','solstice','ridiculous'])

dictionaries = []
if __name__ == "__main__":

    while(True):
        try:
            number_words = input()
        except:
            break
        number_words = number_words.strip()

        if( number_words.isdigit() ):
            number_words = int(number_words)
            dictionary_words = []
            
            while(number_words != 0):
                dictionary_words.append( input() )
                number_words-=1

            dictionaries.append(dictionary_words)
        else:
            #done receiving new inputs
            break

    for dictionary in dictionaries:
        print("{:.2f}".format(CalculatedAverageKeyPress(dictionary))) 