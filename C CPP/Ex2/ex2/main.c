//Including:
#include "sort_bus_lines.h"
#include "test_bus_lines.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
//Args from cmd:
#define DURATION_STR "by_duration"
#define DISTANCE_STR "by_distance"
#define NAME_STR "by_name"
#define TEST_STR "test"
#define INFO_LINE_FORMAT "%[^,],%d,%d\n"
#define PRINTING_FORMAT "%s,%d,%d\n"
//Error Messages to print:
#define ERROR_NAME "ERROR: bus name should has only digits and small case"
#define ERROR_DISTANCE "ERROR: distance should be an int between 0 and 1000\n"
#define ERROR_DURATION "ERROR: duration should be an int between 10 and 100\n"
#define ERROR_NUM_OF_LINES "ERROR: num of lines should be positive integer\n"
#define ERROR_WITH_ARGS "USAGE: command is not valid,could not run the program"
#define ERROR_SSCANF "ERROR: the input is not valid.\n"
//Getting input Messages to print:
#define GET_LINE_INFO "Enter line info. Then enter\n"
#define GET_NUM_OF_LINES "Enter number of lines. Then enter\n"
//Passed tests Messages to print:
#define PASS_TEST_1 "TEST 1 PASSED: sorting by distance succeeded.\n"
#define PASS_TEST_2 "TEST 2 PASSED: sorting by distance succeeded.\n"
#define PASS_TEST_3 "TEST 3 PASSED: sorting by duration succeeded.\n"
#define PASS_TEST_4 "TEST 4 PASSED: sorting by duration succeeded.\n"
#define PASS_TEST_5 "TEST 5 PASSED: sorting by name succeeded.\n"
#define PASS_TEST_6 "TEST 6 PASSED: sorting by name succeeded.\n"
//Failed tests Messages to print:
#define FAILED_TEST_1 "TEST 1 FAILED: quick sorting by distance failed.\n"
#define FAILED_TEST_2 "TEST 2 FAILED: sorting by distance has a problem.\n"
#define FAILED_TEST_3 "TEST 3 FAILED: quick sorting by duration failed.\n"
#define FAILED_TEST_4 "TEST 4 FAILED: sorting by duration has a problem.\n"
#define FAILED_TEST_5 "TEST 5 FAILED: bubble sorting by name failed.\n"
#define FAILED_TEST_6 "TEST 6 FAILED: sorting by name has a problem.\n"

typedef enum MainNumbers{
    DIVIDER_BASIS = 10,
    ASCII_START = 97,
    ASCII_END = 122,
    DISTANCE_START = 0,
    DISTANCE_END = 1000,
    DURATION_START = 10,
    DURATION_END = 100,
    MAX_ONE_ARG = 21,
    MAX_INFO_LINE = 63,
    NUM_OF_INFO_ARGS = 3,
    VALID_NUM_OF_ARGC = 2,
}MainNumbers;




/// A function that gets two strings and check if they are even.
/// \param str_1 a string.
/// \param str_2 a string.
/// \return true if equal else false.
int checking_equality(char* str_1, char* str_2){
  int equ = strcmp(str_1, str_2);
  if (!equ){
    return TRUE;
  }
  else{
     return FALSE;
  }
  }

/// A function that checks if the string got from cmd is valid, if its one
/// of the commands - "by_name","by_duration","by_distance","test".
/// \param argv_str the command from cmd.
/// \return the integer that represents the command if valid,else false.
int is_argv_valid(char* argv_str){
  int valid = FALSE;
  if(checking_equality (argv_str, TEST_STR)){
    valid=TEST;
  }
  if(checking_equality (argv_str, DURATION_STR)){
    valid=DURATION;
  }
  if(checking_equality (argv_str, DISTANCE_STR))
  {
    valid=DISTANCE;
  }
  if(checking_equality (argv_str, NAME_STR))
  {
    valid=NAME;
  }
  return valid;
}


/// A function that checks if the info line is valid, if not if prints an
/// informative error message.
/// \param line_name a pointer to a char.
/// \param distance an int.
/// \param duration an int.
/// \return 1 if valid,0 else.
int is_input_valid(char* line_name,int distance,int duration)
{
  for (long unsigned i = 0; i < strlen(line_name); i++)
  {
    if (isdigit ((line_name[i])) ||
    (ASCII_START <= line_name[i] &&line_name[i] <=ASCII_END))
    {
      continue;
    }
    else
    {
      printf(ERROR_NAME);
      return FALSE;
    }
  }
  if(DISTANCE_START > distance || distance > DISTANCE_END)
  {
    printf(ERROR_DISTANCE);
    return FALSE;
  }
  if (DURATION_START > duration || duration > DURATION_END)
  {
    printf(ERROR_DURATION);
    return FALSE;
  }
  else{
    return TRUE;
  }
}

/// A function that gets the num of the bus lines to sort from the user.
/// \return num of lines as am integer.
int user_num_of_lines()
{
  printf (GET_NUM_OF_LINES);
  char number_of_buses[MAX_ONE_ARG];
  fgets(number_of_buses,MAX_ONE_ARG,stdin);
  long num_of_bus = strtol(number_of_buses, NULL,DIVIDER_BASIS);
  while (0 > num_of_bus || !num_of_bus)
  {
    printf ( ERROR_NUM_OF_LINES);
    printf (GET_NUM_OF_LINES);
    fgets(number_of_buses,MAX_ONE_ARG,stdin);
    num_of_bus = strtol(number_of_buses, NULL,DIVIDER_BASIS);
  }
  return num_of_bus;
}

/// A function that checks if sscanf got all the args from user.
/// \param num_of_args the num that sscanf succeeded.
/// \return true if got all of them,else false.
int sscanf_problem(int num_of_args)
{
  if(num_of_args == NUM_OF_INFO_ARGS)
  {
    return TRUE;
  }
  printf (ERROR_SSCANF);
  return FALSE;
}

/// A function that allocates struct of buses and gets the info of lines from
/// user if valid and store them in the struct.
/// \param number_of_buses an int.
/// \return a pointer to the start of the buses struct.
BusLine* user_lines_info(int number_of_buses)
{
  BusLine *buses = (BusLine *) malloc (number_of_buses * sizeof (BusLine));
  if(buses==NULL){
    return FALSE;
  }
  BusLine *buses_start = buses;
  int counter = 0;
  for (int j = 0; j < number_of_buses; j++)
  {
    int duration;
    int distance;
    char bus_name[MAX_ONE_ARG] ={0};
    char buffer[MAX_INFO_LINE];
    printf (GET_LINE_INFO);
    fgets (buffer, MAX_INFO_LINE, stdin);
    int x = sscanf(buffer, INFO_LINE_FORMAT,
                   bus_name, &distance, &duration);
    while (!sscanf_problem(x) ||
                     !(is_input_valid (bus_name, distance, duration)))
    {
      printf (GET_LINE_INFO);
      fgets (buffer, MAX_INFO_LINE, stdin);
      x = sscanf (buffer, INFO_LINE_FORMAT,
                  bus_name, &distance, &duration);
    }
    strcpy(buses[counter].name, bus_name);
    buses[counter].distance =  distance;
    buses[counter].duration = duration;
    counter++;
  }
  return buses_start;
}

/// A function that sorts the buses list according to the given sorting type.
/// \param buses_list a pointer to the start of the struct.
/// \param sorting_type an int.
/// \param num_of_buses an int (for the end pointer).
void sorting(BusLine*buses_list,int sorting_type,int num_of_buses)
{
  if(sorting_type == NAME)
  {
    bubble_sort (buses_list,buses_list+num_of_buses);
  }
  else
  {
    quick_sort (buses_list,buses_list+num_of_buses
                                                       ,sorting_type);
  }
}

/// A function that gets a pointer to the start of a struct and prints them
/// line after line.
/// \param buses_list
/// \param num_of_buses
void printing(BusLine* buses_list,int num_of_buses)
{
  for(int i =0; i<num_of_buses;i++)
  {
    printf(PRINTING_FORMAT,buses_list->name,buses_list->distance,
           buses_list->duration);
    buses_list++;
  }
}

/// A function that gets a struct of buses sort it by distance and check:
/// 1. "is_sorted_by_distance" if the struct is well sorted.
/// 2. "is_equal" if the sorting didnt changed while sorted.
/// \param copy_buses A pointer to a copy of the original struct.
/// \param buses the original struct the user inserts.
/// \param num_of_buses an int.
void testing_distance(BusLine * copy_buses,BusLine *buses ,int num_of_buses)
{
  quick_sort (copy_buses, copy_buses + num_of_buses, DISTANCE);
  if (!(is_sorted_by_distance (copy_buses, copy_buses + num_of_buses)))
  {
    printf (FAILED_TEST_1);
  }
  else
  {
    printf (PASS_TEST_1);
  }
  if (!(is_equal (copy_buses,
                  copy_buses + num_of_buses, buses, buses + num_of_buses)))
  {
    printf (FAILED_TEST_2);
  }
  else
  {
    printf (PASS_TEST_2);
  }
}

/// A function that gets a struct of buses sort it by duration and check:
/// 1. "is_sorted_by_duration" if the struct is well sorted.
/// 2. "is_equal" if the sorting didnt changed while sorted.
/// \param copy_buses A pointer to a copy of the original struct.
/// \param buses the original struct the user inserts.
/// \param num_of_buses an int.
void testing_duration(BusLine * copy_buses,BusLine *buses, int num_of_buses)
{
  quick_sort(copy_buses,copy_buses+num_of_buses,DURATION);
  if(!(is_sorted_by_duration(copy_buses,copy_buses+num_of_buses)))
  {
    printf(FAILED_TEST_3);
  }
  else
  {
    printf(PASS_TEST_3);
  }
  if(!(is_equal(copy_buses,copy_buses+num_of_buses,buses,buses+num_of_buses)))
  {
    printf(FAILED_TEST_4);
  }
  else
  {
    printf(PASS_TEST_4);
  }
}

/// A function that gets a struct of buses sort it by name and check:
/// 1. "is_sorted_by_name" if the struct is well sorted.
/// 2. "is_equal" if the sorting didnt changed while sorted.
/// \param copy_buses A pointer to a copy of the original struct.
/// \param buses the original struct the user inserts.
/// \param num_of_buses an int.
void testing_name(BusLine * copy_buses,BusLine *buses, int num_of_buses)
{
  bubble_sort(copy_buses,copy_buses+num_of_buses);
  if(!(is_sorted_by_name(copy_buses,copy_buses+num_of_buses)))
  {
    printf(FAILED_TEST_5);
  }
  else
  {
    printf(PASS_TEST_5);
  }
  if(!(is_equal(copy_buses,copy_buses+num_of_buses,buses,buses+num_of_buses)))
  {
    printf(FAILED_TEST_6);
  }
  else
  {
    printf(PASS_TEST_6);
  }
}

/// A function that create a copy of the struct calls all the testing
/// function above and prints the relevant messages.
/// \param buses a pointer to the start of the struct.
/// \param num_of_buses an int.
/// \return true if succeeded ,else false.
void testing(BusLine* buses,BusLine* copy_buses ,int num_of_buses)
{
  memcpy(copy_buses, buses, num_of_buses * sizeof(BusLine));
  testing_distance(copy_buses,buses,num_of_buses);
  testing_duration(copy_buses,buses,num_of_buses);
  testing_name(copy_buses,buses,num_of_buses);
}

/// A function that run the program according to the instructions of ex2.
/// \param argc an int.
/// \param argv a list of the cmd args.
/// \return exit success if succeeded else exit failure.
int main (int argc, char *argv[]){
  if(argc == VALID_NUM_OF_ARGC && is_argv_valid(argv[1]))
  {
    int sorting_type = is_argv_valid(argv[1]);
    if (sorting_type==TEST)
    {
      int num_of_buses = user_num_of_lines();
      BusLine * buses = user_lines_info (num_of_buses);
      if(buses == NULL)
      {
        free(buses);
        return EXIT_FAILURE;
      }
      BusLine * copy_buses = malloc (num_of_buses*sizeof(BusLine));
      if(copy_buses == NULL)
      {
        free(copy_buses);
        free(buses);
        return EXIT_FAILURE;
      }
      else{
        testing(buses,copy_buses,num_of_buses);
        free(copy_buses);
        free(buses);
        return EXIT_SUCCESS;
      }
    }
    else if(sorting_type)
    {
      int num_of_buses = user_num_of_lines();
      BusLine * buses = user_lines_info (num_of_buses);
      if(!(buses))
      {
        return EXIT_FAILURE;
      }
      sorting(buses,sorting_type,num_of_buses);
      printing(buses,num_of_buses);
      free(buses);
      return EXIT_SUCCESS;
    }
  }
  else{
    printf(ERROR_WITH_ARGS);
    return EXIT_FAILURE;
  }
}

