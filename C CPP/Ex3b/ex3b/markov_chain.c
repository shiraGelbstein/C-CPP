#include "markov_chain.h"
#include <stdlib.h>
#include <string.h>
#define TRUE 1
#define FALSE 0

void free_linked_list (LinkedList **linked_list,MarkovChain ** p_markov_chain);
void free_nodes (Node **node,MarkovChain ** p_markov_chain);
void free_markov_node (MarkovNode **markov_node,MarkovChain ** p_markov_chain);
void free_markov_freq_arr (MarkovNodeFrequency **markov_frequency_arr);

Node* add_to_database(MarkovChain *markov_chain,void *data_ptr)
{
  MarkovChain **markov_ptr = &markov_chain;
  Node* temp = get_node_from_database(markov_chain,data_ptr);
  if(temp)
  {
    return temp;
  }
  else
  {
    MarkovNode* new_markov = malloc(sizeof(MarkovNode));
    if(!new_markov)
    {
      free_database(markov_ptr);
      return NULL;
    }
    new_markov->data= markov_chain->copy_func(data_ptr);
    if(new_markov->data == NULL)
    {
      free(new_markov);
      return NULL;
    }
    new_markov->arr = NULL,new_markov->len_of_arr = 0,
    new_markov->all_frequency = 0;
    if (add(markov_chain->database, new_markov))
    {
      free_database(markov_ptr);
      return NULL;
    }
    else
    {
      return markov_chain->database->last ;
    }
  }
}

Node* get_node_from_database(MarkovChain *markov_chain, void *data_ptr)
{
  Node* cur_node = markov_chain->database->first;
  for(int i =0; i< markov_chain->database->size; i++)
  {
    if(!markov_chain->comp_func(cur_node->data->data, data_ptr))
    {
      return cur_node;

    }
    cur_node = cur_node->next;
  }
  return NULL;
}

bool add_node_to_frequencies_list(MarkovNode *first_node,
                 MarkovNode *second_node,MarkovChain *markov_chain)
{
  void* sec_str = second_node->data;
  int len_arr_1 = first_node->len_of_arr;
  for(int i =0; i< len_arr_1;i++)
  {
    if((markov_chain->comp_func(first_node->arr[i].node->data,sec_str))==0)
      //changed 14.5
    {
      first_node->arr[i].frequency ++;
      first_node->all_frequency ++;
      return TRUE;
    }
  }
  if(len_arr_1==0)
  {
    first_node->arr =(MarkovNodeFrequency*)malloc(sizeof
                                                      (MarkovNodeFrequency));
  }
  else{
    first_node->arr = realloc(first_node->arr,
                              (sizeof(MarkovNodeFrequency)*(len_arr_1 +1)));
  }
  if (!first_node->arr)
  {
    return FALSE;
  }
  else
  {
    first_node->arr[len_arr_1].node=  second_node;
    first_node->arr[len_arr_1].frequency = 1;
    first_node->len_of_arr ++;
    first_node->all_frequency ++;
    return TRUE;
  }
}

/// frees the markov chain.
/// \param markov_chain
void free_markov_chain(MarkovChain ** markov_chain)
{
  if (*markov_chain)
  {
    free_linked_list(&((*markov_chain)->database),markov_chain);
  }
  free (*markov_chain);
  *markov_chain = NULL;
}

/// frees the linked list.
/// \param markov_chain
void free_linked_list (LinkedList **linked_list,MarkovChain ** p_markov_chain)
{
  if(*linked_list)
  {
    free_nodes(&((*linked_list)->first), p_markov_chain);
  }
  free(*linked_list);
  *linked_list = NULL;
}

/// frees the node.
/// \param markov_chain
void free_nodes(Node **node,MarkovChain ** p_markov_chain)
{
  if(*node)
  {
    Node* cur_node = *node;
    while(cur_node)
    {
      Node* temp = cur_node;
      cur_node = cur_node->next;
      free_markov_node(&(temp->data), p_markov_chain);
      free(temp);
    }
  }
}

/// frees individual markov node.
/// \param markov_chain
void free_markov_node(MarkovNode **markov_node,MarkovChain ** p_markov_chain)
{
  if(*markov_node)
  {
    free_markov_freq_arr(&((*markov_node)->arr));
    (*p_markov_chain)->free_data((*markov_node)->data);
    (*markov_node)->data = NULL;
  }
  free(*markov_node);
  *markov_node= NULL;
}

/// frees the frequency array of a markov node.
/// \param markov_chain
void free_markov_freq_arr(MarkovNodeFrequency **markov_frequency_arr)
{
  free(*markov_frequency_arr);
  *markov_frequency_arr = NULL;
}

/// documented in the markov_chain.h.
void free_database(MarkovChain ** ptr_chain)
{
  if(*ptr_chain)
  {
    free_markov_chain(ptr_chain);
  }
}

/// documented in ex3 Part A.
int get_random_number(int max_number)
{
  return rand() % max_number;
}

/// A function that gets a number n and a markov chain and finds the nth node
/// in the markov and returns it. assuming that n is <= len of markov.
/// \param markov_chain
/// \param n the node num we want to find.
/// \return a pointer to the node we found.
Node* find_nth_node(MarkovChain *markov_chain,int n)
{
  if(n==0)
  {
    return markov_chain->database->first;
  }
  Node * the_node = markov_chain->database->first;
  for(int i=0; i<n;i++)
  {
    the_node = the_node->next;
  }
  return the_node;
}

/// documented in the markov_chain.h.
MarkovNode* get_first_random_node(MarkovChain *markov_chain) //assuming
// there is a valid node, either ways its an endless loop.
{
  int node_num = get_random_number(markov_chain->database->size);
  Node* chosen_node = find_nth_node(markov_chain,node_num);
  while(markov_chain->is_last(chosen_node->data->data)) //if last its newly
    // generates.
  {
    node_num= get_random_number(markov_chain->database->size);
    chosen_node = find_nth_node(markov_chain,node_num);
  }
  return chosen_node->data;
}

/// documented in the markov_chain.h.
MarkovNode* get_next_random_node(MarkovNode *state_struct_ptr)
{
  int chosen_num = get_random_number(state_struct_ptr->all_frequency);
  int counter = -1;
  MarkovNode* chosen_node = NULL;
  for(int i=0; i< state_struct_ptr->len_of_arr;i++)
  {
    for (int j = 0; j < state_struct_ptr->arr[i].frequency; j++)
    {
      counter++;
      if (counter == chosen_num)
      {
        chosen_node = state_struct_ptr->arr[i].node;
        return chosen_node;
      }
    }
  }
  return NULL;
}

/// documented in the markov_chain.h.
void generate_tweet(MarkovChain *markov_chain, MarkovNode *
first_node, int max_length)
{
  MarkovNode* cur_node = NULL;
  int counter = 1; //including the first one.
  if(!first_node)
  {
    MarkovNode * node = get_first_random_node(markov_chain);
    markov_chain->print_func(node->data);
    cur_node = node;
  }
  else
  {
    cur_node = first_node;
    markov_chain->print_func(first_node->data);
  }
  MarkovNode * noode = get_next_random_node(cur_node);
  cur_node = noode;
  while(counter < max_length -1 && !markov_chain->is_last(cur_node->data))
  {
    markov_chain->print_func(cur_node->data);
    cur_node = get_next_random_node (cur_node);
    counter++;
  }
  markov_chain->print_func(cur_node->data);
}
