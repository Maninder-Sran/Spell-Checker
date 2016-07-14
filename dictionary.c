/**
 * dictionary.c
 *
 * Computer Science 50
 * Problem Set 5
 *
 * Implements a dictionary's functionality.
 */

#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#include "dictionary.h"
node* root;
int numWords;
/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char* word)
{
    // Determine length of word
    int index = strlen(word); 
    
    // Make a new pointer to the root of the trie
    node* cursor = root;
    
    for(int i = 0; i < index; i++)
    {
        // Initializing local variable position
        int position = 0;
        
        // Calculate the index for the char within the trie array
        if(islower(word[i]) && isalpha(word[i]))
            position = ((int) toupper(word[i])) - ASCII_START;
        else if(word[i] == '\'')
            position = (int) word[i] - 12;
        else
            position = ((int) word[i]) - ASCII_START;
        
        /* If the address at the index position != NULL and the word is not 
        *  complete then go to the child of that index 
        */
        if(cursor->letters[position] != NULL && i != index - 1)
        {
            cursor = cursor->letters[position];
        }
        /* If the address at the index position != NULL and the word is 
        *  complete then go to the child of that index and check if it is a
        *  word in the dictionary
        */
        else if (cursor->letters[position] != NULL && i == index - 1)
        {
            cursor = cursor->letters[position];
            if(cursor->is_word == true)
                return true;
        }
        /* If the address at the index position == NULL and the word is not 
        *  complete, then the inputed word is not in the dictionary 
        */
        else if (cursor->letters[position] == NULL && i < index)
        {
            return false;
        }
    }
    return false;
}

/**
 * Loads dictionary into memory.  Returns true if successful else false.
 */
bool load(const char* dictionary)
{
    // Initialize the global variable root with a node
    root = malloc(sizeof(node));
    // Check if the node was created
    if(root == NULL)
    {
        printf("Out of memory for malloc of root.\n");
        return false;
    }
    // Open the dictionary file
    FILE* dict = fopen(dictionary,"r");
    // Check if the file was opened successfully
    if (dict == NULL)
    {
        printf("Unable to open file: %s\n", dictionary);
        return false;
    }
    // Create an array large enough to store one word
    char word[LENGTH + 1];
    char c;
    
    // Loop while there are still chars in the file to iterate over
    while((c = fgetc(dict)) != EOF)
    {
        int index = 0;
        /* Initialize a temporary pointer to maneuver through the trie starting
        *  at the root
        */ 
        node* cursor = root;
        // Check if the pointer was initialized successfully
        if (cursor == NULL)
        {
            printf("Error in loading words from %s\n", dictionary);
            return false;
        }
        
        // Iterate over chars in the current line until a new line is met
        while(strncmp(&c,"\n",1) != 0)
        {
            // Store the chars in the word array
            word[index] = c;
            index++;
            c = fgetc(dict);
        }
        // Increment global variable count of number of words
        numWords++;
        for(int i = 0; i < index; i++)
        {
            // Initializing local variable position
            int position = 0;
            
            // Calculate the index for the char within the trie array
            if(islower(word[i]) && isalpha(word[i]))
                position = ((int) toupper(word[i])) - ASCII_START;
            else if(word[i] == '\'')
                position = (int) word[i] - 12;
            else
                position = ((int) word[i]) - ASCII_START;
            
            /* If the word has been iterated over then create space if 
            *  necessary. And then finally set the is_word to true indicating
            *  that it is the end of the word
            */
            if (i == index - 1)
            {
                if (cursor->letters[position] == NULL)
                {
                    // Malloc space for one node at the index of the array
                    cursor->letters[position] = malloc(sizeof(node));
                    // Check if the node was created
                    if (cursor == NULL)
                    {
                        printf("Error occurred loading words from %s\n",dictionary);
                        return false;
                    }
                }
                // Go to the address of the newly created node
                cursor = cursor->letters[position];
                // Set the member is_word to true
                cursor->is_word = true;
                break;
            }
            /* If the element at the index position is null but there are more
            *  letters in the word then create a node at that index
            */
            if (cursor->letters[position] == NULL && i < index - 1)
            {
                // Malloc space for one node at the index of the array
                cursor->letters[position] = malloc(sizeof(node));
                // Check if the node was created
                if (cursor == NULL)
                {
                    printf("Error occurred loading words from %s\n",dictionary);
                    return false;
                }
                // Go to the address of the newly created node
                cursor = cursor->letters[position];
            }
            /* If the path was already created and the word is not complete
            *  follow the path
            */
            else
            {
                // Go to the address of the newly created node
                cursor = cursor->letters[position];
            }
        }
    }
    //Close the file
    fclose(dict);
    
    return true;
}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    //Return global variable numWords 
    return numWords;
}

/**
 * Unloads dictionary from memory.  Returns true if successful else false.
 */
bool unload(void)
{
    /* Initialize a temporary pointer to maneuver through the trie starting
    *  at the root
    */ 
    node* cursor = root;
    // If all nodes were freed then return true
    if(release(cursor))
        return true;
        
    return false;
}

bool release(node* trie)
{
    // Iterate through the array of structs in the node 
    for(int i = 0; i < SIZE; i++)
    {
        /* If the index at position i is not null go to the child at that index
        *  doing so recursively until approached by an array that is all null
        */
        if(trie->letters[i] != NULL)
            release(trie->letters[i]);
    }    
    // Free the memory used by the current trie
    free(trie);
    // Return true so that the previous stack frame can resume 
    return true;
}
