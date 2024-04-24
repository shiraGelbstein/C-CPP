#include <string.h> // For strlen(), strcmp(), strcpy()
#include "markov_chain.h"

#define MAX(X, Y) (((X) < (Y)) ? (Y) : (X))

#define EMPTY -1
#define BOARD_SIZE 100
#define MAX_GENERATION_LENGTH 60

#define DICE_MAX 6
#define NUM_OF_TRANSITIONS 20
#define BASE 10
#define TRUE 1
#define FALSE 0
#define NUM_OF_ARGS 3
#define USAGE_ERROR "USAGE: program gets 2 args only."
#define NEW_LINE "\n"
#define PRINT_LAST_CELL "[%d]"
#define LADDER_PRINT_FORMAT "[%d]-ladder to %d -> "
#define SNAKE_PRINT_FORMAT "[%d]-snake to %d -> "
#define CELL_PRINT_FORMAT "[%d] -> "
/**
 * represents the transitions by ladders and snakes in the game
 * each tuple (x,y) represents a ladder from x to if x<y or a snake otherwise
 */
const int transitions[][2] = {{13, 4},
                              {85, 17},
                              {95, 67},
                              {97, 58},
                              {66, 89},
                              {87, 31},
                              {57, 83},
                              {91, 25},
                              {28, 50},
                              {35, 11},
                              {8,  30},
                              {41, 62},
                              {81, 43},
                              {69, 32},
                              {20, 39},
                              {33, 70},
                              {79, 99},
                              {23, 76},
                              {15, 47},
                              {61, 14}};

/**
 * struct represents a Cell in the game board
 */
typedef struct Cell {
    int number; // Cell number 1-100
    int ladder_to;  // ladder_to represents the jump of
    // the ladder in case there is one from this square
    int snake_to;  // snake_to represents the jump of the
    // snake in case there is one from this square
    //both ladder_to and snake_to should be -1 if the Cell doesn't have them
} Cell;

/** Error handler **/
static int handle_error(char *error_msg, MarkovChain **database)
{
    printf("%s", error_msg);
    if (database != NULL)
    {
        free_database(database);
    }
    return EXIT_FAILURE;
}


static int create_board(Cell *cells[BOARD_SIZE])
{
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        cells[i] = malloc(sizeof(Cell));
        if (cells[i] == NULL)
        {
            for (int j = 0; j < i; j++) {
                free(cells[j]);
            }
            handle_error(ALLOCATION_ERROR_MASSAGE,NULL);
            return EXIT_FAILURE;
        }
        *(cells[i]) = (Cell) {i + 1, EMPTY, EMPTY};
    }

    for (int i = 0; i < NUM_OF_TRANSITIONS; i++)
    {
        int from = transitions[i][0];
        int to = transitions[i][1];
        if (from < to)
        {
            cells[from - 1]->ladder_to = to;
        }
        else
        {
            cells[from - 1]->snake_to = to;
        }
    }
    return EXIT_SUCCESS;
}

/**
 * fills database
 * @param markov_chain
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
static int fill_database(MarkovChain *markov_chain)
{
    Cell* cells[BOARD_SIZE];
    if(create_board(cells) == EXIT_FAILURE)
    {
        return EXIT_FAILURE;
    }
    MarkovNode *from_node = NULL, *to_node = NULL;
    size_t index_to;
    for (size_t i = 0; i < BOARD_SIZE; i++)
    {
        add_to_database(markov_chain, cells[i]);
    }

    for (size_t i = 0; i < BOARD_SIZE; i++)
    {
        from_node = get_node_from_database(markov_chain,cells[i])->data;

        if (cells[i]->snake_to != EMPTY || cells[i]->ladder_to != EMPTY)
        {
            index_to = MAX(cells[i]->snake_to,cells[i]->ladder_to) - 1;
            to_node = get_node_from_database(markov_chain, cells[index_to])
                    ->data;
            add_node_to_frequencies_list (from_node, to_node, markov_chain);
        }
        else
        {
            for (int j = 1; j <= DICE_MAX; j++)
            {
                index_to = ((Cell*) (from_node->data))->number + j - 1;
                if (index_to >= BOARD_SIZE)
                {
                    break;
                }
                to_node = get_node_from_database(markov_chain, cells[index_to])
                        ->data;
                add_node_to_frequencies_list (from_node,
                                              to_node, markov_chain);
            }
        }
    }
    // free temp arr
    for (size_t i = 0; i < BOARD_SIZE; i++)
    {
        free(cells[i]);
    }
    return EXIT_SUCCESS;
}

/// A function that prints the markov node.
/// \param markov_node
static void printing(MarkovNode *markov_node)
{
  Cell * cell = (Cell*) (markov_node->data);
  if((cell->number) == BOARD_SIZE)
  {
    printf (PRINT_LAST_CELL,cell->number);
    return;
  }
  if((cell->ladder_to) != EMPTY)
  {
    printf (LADDER_PRINT_FORMAT,cell->number,cell->ladder_to);
  }
  else if((cell->snake_to) != EMPTY)
  {
    printf (SNAKE_PRINT_FORMAT,cell->number,cell->snake_to);
  }
  else
  {
    printf (CELL_PRINT_FORMAT,cell->number);
  }
}

/// A function that generates the given number paths randomly using the markov
/// library random func.
/// \param markov_chain
/// \param max_path_num the number of paths we want to print.
static void generate_print_path(MarkovChain * markov_chain,
                                unsigned int max_path_num)
{
    unsigned int path_counter = 1;
    while(path_counter<max_path_num+1)
    {
      MarkovNode* cur_node = markov_chain->database->first->data;
      int counter = 1; //including the first one.
      printf("Random Walk %d: ",path_counter);
      while(counter <MAX_GENERATION_LENGTH && !(markov_chain->is_last
      (cur_node->data)))
      {
        printing (cur_node);
        cur_node = get_next_random_node (cur_node);
        counter++;
      }
      printing (cur_node);
      printf(NEW_LINE);
      path_counter++;
  }
}

/// A function that checks if the cell is the last cell in board.
/// \param void_cell
/// \return true if it is else false.
static bool is_last_cell(void* void_cell)
{
  Cell * cell = (Cell *) void_cell;
  if((cell->number)==BOARD_SIZE)
  {
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}

/// A function that compare the cells by their number.
/// \param cell1
/// \param cell2
/// \return 0 if equal negative number if cell1>cell2 and positive number else.
static int cell_compare(void* cell1, void* cell2)
{
  return  ((Cell *) cell1)->number - ((Cell *) cell2)->number;
}

/// A function that prints the cell - the num cell.
/// \param cell
static void printing_cell(void* cell)
{
  printf("%d", ((Cell*) cell)->number);
}

/// A function that gets a cell and allocate dynamic place and initialised
/// the cell.
/// \param void_cell
/// \return returns a void pointer to the allocated place.
static void* copy_cell(void* void_cell)
{
  Cell * cell = malloc(sizeof(Cell));
  if (!cell){
    return NULL;
  }
  memcpy (cell, void_cell, sizeof(Cell));
  return (void*) cell;
}

/// A function that frees a cell.
/// \param void_cell
static void free_cell(void*void_cell)
{
  Cell * cell = (Cell*) void_cell;
  free(cell);
}

/// A function that allocates the markov chain and its linked list.
/// \return NUll if failed to allocate else returns a pointer to the markov
/// chain.
static MarkovChain *allocate_chain()
{
  MarkovChain *markov_chain = malloc (sizeof (MarkovChain));
  MarkovChain ** p_chain = &markov_chain;
  if(!markov_chain)
  {
    return NULL;
  }
  markov_chain->database = NULL;
  markov_chain->copy_func = copy_cell;
  markov_chain->free_data = free_cell;
  markov_chain->is_last = is_last_cell;
  markov_chain->comp_func = cell_compare;
  markov_chain->print_func = printing_cell;
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

/**
 * @param argc num of arguments
 * @param argv 1) Seed
 *             2) Number of sentences to generate
 * @return EXIT_SUCCESS or EXIT_FAILURE
 */
int main(int argc, char *argv[])
{
  if(argc != NUM_OF_ARGS)
  {
    printf(USAGE_ERROR);
    return EXIT_FAILURE;
  }
  unsigned int seed = strtol (argv[1], NULL, BASE);
  srand (seed);
  MarkovChain * markov_chain = allocate_chain();
  if(!markov_chain)
  {
    printf(ALLOCATION_ERROR_MASSAGE);
    return EXIT_FAILURE;
  }
  int fill = fill_database(markov_chain);
  if(fill)
  {
    printf(ALLOCATION_ERROR_MASSAGE);
    return EXIT_FAILURE;
  }
  else
  {
    unsigned int num_paths = strtol (argv[2], NULL, BASE);
    generate_print_path (markov_chain,num_paths);
  }
  free_database(&markov_chain);
  return EXIT_SUCCESS;
}
