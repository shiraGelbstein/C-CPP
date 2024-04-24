#include "test_bus_lines.h"


int is_sorted_by_distance (BusLine *start, BusLine *end)
{
  long unsigned len = end-start;
  for(long unsigned j=0;j<len-1;j++)
  {
    if((start+j)->distance > (start+j+1)->distance){
      return FALSE;
    }
  }
  return TRUE;
}


int is_sorted_by_duration (BusLine *start, BusLine *end)
{
  long unsigned len = end-start;
  for(long unsigned j=0;j<(len-1);j++)
  {
    if((start+j)->duration > (start+j+1)->duration){
      return FALSE;
    }
  }
  return TRUE;
}


int is_sorted_by_name (BusLine *start, BusLine *end)
{
  long unsigned len = end-start;
  for(long unsigned j=0;j<len-1;j++)
  {
    int check = strcmp((start+j)->name,(start+j+1)->name);
    if(check>0){
      return FALSE;
    }
  }
  return TRUE;
}


int is_equal (BusLine *start_sorted,
              BusLine *end_sorted, BusLine *start_original,
              BusLine *end_original)
{
  long unsigned len_sorted = end_sorted - start_sorted ;
  long unsigned len_original = end_original- start_original ;
  if (len_original != len_sorted)
  {
    return FALSE;
  }
  for (long unsigned i = 0; i < len_sorted; i++)
  {
    int found_name = 0;
    for(long unsigned j = 0; j < len_sorted; j++)
    {
      int check = strcmp((start_sorted+i)->name,(start_original+j)->name);
      if(check==0)
      {
        found_name = 1;
      }
  }
    if(!found_name)
    {
      return FALSE;
    }
  }
  return TRUE;
}