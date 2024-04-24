#include "markov_chain.h"
#include "linked_list.h"
#include <stdlib.h>
#include <string.h>

enum Numbers{
    MAX_SENT_LENGTH = 1000,
    MAX_WORD_LENGTH = 100,
    MAX_TWEET_LENGTH  = 20,
    READ_ALL = (-1),
    TRUE  = 1,
    FALSE = 0,
    READ_ALL_ARG = 4,
    READ_SPECIFIC_NUM = 5,
    BASE = 10
};

#define ALLOCATION_ERROR "Allocation failure: program failed allocating place."
#define FILE_ERROR "ERROR: file not found. "
#define USAGE_ERROR "USAGE: Program gets 3 or 4 arguments."
#define CLEAN_STRING " \n\t\r"
#define READING "r"
#define DOT "."
#define NEW_LINE "\n"
#define TWEETS_FORMAT "Tweet %d:"

/// A function that check if the last char in a given string is a dot.
/// \param word pointer to string.
/// \return true if there is dot,else false.
static bool is_last_tweets(void * word)
{
  char* word_char = (char*) word;
  unsigned int len_word = strlen (word_char);
  char* last_char = &word_char[len_word-1];
  if (!strcmp(last_char,DOT))
  {
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}

/// A function that gets a file and a markov chain and fills the markov chain
/// with the files words avoiding the CLEAN_STRING chars.
/// \param fp
/// \param words_to_read
/// \param markov_chain
/// \return 1 if function fails - allocation fails, 0 on success.
static int fill_database(FILE *fp, int words_to_read,
                         MarkovChain *markov_chain)
{
  MarkovChain ** markov_ptr = &markov_chain;
  Node *prev_node = NULL;
  char sentence[MAX_SENT_LENGTH];
  char *word;
  while (fgets (sentence, MAX_SENT_LENGTH, fp) &&
         (words_to_read == READ_ALL || words_to_read
                                       > markov_chain->database->size))
  {
    word = strtok (sentence, CLEAN_STRING);
    while (word !=NULL && (words_to_read == READ_ALL || words_to_read
    >markov_chain->database->size))
    {
      Node *node = add_to_database (markov_chain, (void *)word);
      if (!node) //checking allocation
      {
        free_database(markov_ptr);
        return 1; //failed allocation.
      }
      if (prev_node && !markov_chain->is_last(prev_node->data->data))
      {
        if (!add_node_to_frequencies_list (prev_node->data,
                                           node->data,markov_chain))
        {
          free_database(markov_ptr);
          return 1;
        }
      }
      prev_node = node;
      word = strtok (NULL, CLEAN_STRING);
    }
    prev_node = NULL;
  }
  return 0; //successes .
}

/// A function that checks if a given number is 4 or 5;
/// \param arg number.
/// \return true if equal to 4 or 5, else false.
static int is_valid_arg(int arg)
{
  if (arg != READ_ALL_ARG && arg != READ_SPECIFIC_NUM)
  {
    printf (USAGE_ERROR);
    return FALSE;
  }
  return TRUE;
}

/// A function that gets a path to a file in a string format and opens it.
/// \param file_arg
/// \return the file if opening succeeded, else false.
static FILE *file_opening(char *file_arg)
{
  FILE *file = fopen (file_arg, READING);
  if (!file)
  {
    printf (FILE_ERROR);
    return NULL;
  }
  return file;
}

static void printing_char(void* str)
{
  char * str_cast = (char*) str;
  printf(" %s", str_cast); //, !is_last_tweets (str_cast) ? "" :"");
}

static void * copy_char (void* data)
{
  char* dynamic_place = malloc(sizeof(char)* MAX_WORD_LENGTH);
  if (!dynamic_place)
  {
    return NULL;
  }
  strcpy(dynamic_place,(char*)data);
  return (void*) dynamic_place;
}

static int char_compare(void *a, void *b)
{
  return strcmp((char*)a,(char*)b);
}

/// A function that allocates and initialized markov chain and a linked list.
/// \return a pointer to the markov chain is all allocations succeeded else
/// returns null.
static MarkovChain *allocate_chain()
{
  MarkovChain *markov_chain = malloc (sizeof (MarkovChain));
  MarkovChain ** p_chain = &markov_chain;
  if(!markov_chain)
  {
    return NULL;
  }
  markov_chain->database = NULL;
  markov_chain->copy_func = copy_char;
  markov_chain->free_data = free;
  markov_chain->is_last = is_last_tweets;
  markov_chain->comp_func = char_compare;
  markov_chain->print_func= printing_char;
  LinkedList * linked_list = malloc (sizeof (LinkedList));
  if(!linked_list)
  {
    free_database(p_chain);
    return NULL;
  }
  markov_chain->database = linked_list;
  linked_list->first = NULL, linked_list->last = NULL, linked_list->size = 0;
  return markov_chain;
}

/// A function that runs the program.
/// \param argc num of arguments from cmd
/// \param argv arguments from amd
/// \return EXIT_SUCCESS on success, EXIT_FAILURE on failure.
int main (int argc, char *argv[])
{
  if (!is_valid_arg (argc))
  {
    return EXIT_FAILURE;
  }
  FILE *file = file_opening (argv[3]);
  if (!file)
  {
    return EXIT_FAILURE;
  }
  unsigned int seed = strtol (argv[1], NULL, BASE);
  srand (seed);
  MarkovChain *markov_chain = allocate_chain();
  MarkovChain **p_chain = &markov_chain;
  int filling_database;
  if (argc == 4) //read all.
  {
    filling_database = fill_database (file, READ_ALL, markov_chain);
  }
  else
  {
    unsigned int words_to_read = strtol (argv[4], NULL, BASE);
    filling_database = fill_database (file, (int)words_to_read, markov_chain);
  }
  if (filling_database)
  {
    free_database (p_chain);
    printf (ALLOCATION_ERROR);
    return EXIT_FAILURE;
  }
  else
  {
    unsigned int counter = 1;
    unsigned int num_of_tweets = strtol (argv[2], NULL, BASE);
    while (counter<= num_of_tweets)
    {
      printf (TWEETS_FORMAT, counter);
      generate_tweet (markov_chain, NULL, MAX_TWEET_LENGTH);
      printf(NEW_LINE);
      counter++;
    }
  }
  free_database (p_chain);
  fclose (file);
  return EXIT_SUCCESS;
}
