#include "sort_bus_lines.h"
#include <stdbool.h>


/// A function that gets two structs and swap their places in memory.
/// \param bus1 a pointer to bus.
/// \param bus2 a pointer to bus.
void swap(BusLine *bus1, BusLine *bus2)
{
  BusLine temp = *bus1;
  *bus1 = *bus2;
  *bus2 = temp;
}


void bubble_sort (BusLine *start, BusLine *end)
{
  long unsigned num_of_buses = end - start;
  for (long unsigned i = num_of_buses; i > 0; --i)
  {
    bool is_swap = false;
    for (long unsigned j=0; j<i-1; j++)
    {
      int check = strcmp((start+j)->name,(start+(j+1))->name);
      if (check > 0)
      {
        swap (start + j, start + (j + 1));
        is_swap = true;
      }
    }
    if(!is_swap){
      break;
    }
  }
  }


/// A function that partitions by distance.
/// \param start a pointer to the start of the struct.
/// \param end a pointer to the end of the struct.
/// \return a pointer to the partition we want to do.
BusLine *partition_distance(BusLine *start, BusLine *end)
{
    long unsigned  len = end - start;
    int pivot = (start + len-1)-> distance;
    int i = -1;
    long unsigned j = 0;
    while(j<(len-1))
    {
      if ((start + j)->distance <= pivot){
        i++;
        swap(start + j , start + i);
      }
      j++;
    }
    swap(start+i+1,start + len-1);
    return start+i+1;
}


/// A function that partitions by duration.
/// \param start a pointer to the start of the struct.
/// \param end a pointer to the end of the struct.
/// \return a pointer to the partition we want to do.
BusLine *partition_duration(BusLine *start, BusLine *end)
{
  long unsigned  len = end - start;
  int pivot = (start + len-1)-> duration;
  int i = -1;
  long unsigned j = 0;
  while(j<(len-1))
  {
    if ((start + j)->duration <= pivot){
      i++;
      swap(start + j , start + i);
    }
    j++;
  }
  swap(start+i+1,start + len-1);
  return start+i+1;
}


BusLine *partition (BusLine *start, BusLine *end, SortType sort_type)
{
  if(sort_type == DISTANCE)
  {
    return partition_distance(start, end);
  }
  else
  {
    return partition_duration(start, end);
  }
}


void quick_sort(BusLine *start, BusLine *end, SortType sort_type)
{
  if (start>=end-1)
  {
    return;
  }
  BusLine *part = partition(start,end, sort_type);
  quick_sort(start,part, sort_type);
  quick_sort(part+1, end, sort_type);
}

