// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// Choose number of buckets in hash table
const unsigned int N = 10000;

// Hash table
node *table[N];

// Keep track of number of words loaded
unsigned int word_count = 0;

// Hashes word to a number
unsigned int hash(const char *word)
{
    // Improved djb2 hash function
    unsigned long hash_value = 5381;
    int c;

    while ((c = *word++))
    {
        c = tolower(c); // Case insensitive
        hash_value = ((hash_value << 5) + hash_value) + c; // hash * 33 + c
    }

    return hash_value % N;
}

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // Get hash value for the word
    unsigned int index = hash(word);

    // Traverse linked list at that index
    node *cursor = table[index];
    while (cursor != NULL)
    {
        // Compare case-insensitively
        if (strcasecmp(cursor->word, word) == 0)
        {
            return true;
        }
        cursor = cursor->next;
    }

    return false;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // Initialize hash table
    for (int i = 0; i < N; i++)
    {
        table[i] = NULL;
    }

    // Open dictionary file
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        return false;
    }

    // Buffer for reading words
    char word[LENGTH + 1];

    // Read words from dictionary one at a time
    while (fscanf(file, "%s", word) != EOF)
    {
        // Create new node
        node *new_node = malloc(sizeof(node));
        if (new_node == NULL)
        {
            fclose(file);
            return false;
        }

        // Copy word into node
        strcpy(new_node->word, word);
        new_node->next = NULL;

        // Hash word to get index
        unsigned int index = hash(word);

        // Insert node into hash table at beginning
        new_node->next = table[index];
        table[index] = new_node;

        // Increment word count
        word_count++;
    }

    // Close dictionary file
    fclose(file);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return word_count;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // Iterate through hash table
    for (int i = 0; i < N; i++)
    {
        node *cursor = table[i];

        // Free all nodes in linked list
        while (cursor != NULL)
        {
            node *temp = cursor;
            cursor = cursor->next;
            free(temp);
        }
    }

    return true;
}
