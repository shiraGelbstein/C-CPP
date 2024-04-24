#ifndef EX2_REPO_SORTBUSLINES_H
#define EX2_REPO_SORTBUSLINES_H
// write only between #define EX2_REPO_SORTBUSLINES_H and #endif
// EX2_REPO_SORTBUSLINES_H
#include <string.h>
#define NAME_LEN 21
/**
 * A struct for the bus line and an enum for an easier way to sort.
 */
typedef struct BusLine
{
    char name[NAME_LEN];
    int distance, duration;
} BusLine;


typedef enum SortType
{
    DISTANCE=1,
    DURATION,
    TEST,
    NAME
} SortType;

/// A function that sorts the given struct with the bubble sort algorithm.
/// \param start pointer to the start of the struct.
/// \param end pointer to the end of the struct.
void bubble_sort (BusLine *start, BusLine *end);

/// A function that sorts the given struct with the quick sort algorithm.
/// \param start a pointer to the start of the struct.
/// \param end a pointer to the end of the struct.
/// \return when getting to the base case of recursion.
void quick_sort (BusLine *start, BusLine *end, SortType sort_type);

/// A function that do the partition part of quick sort.
/// \param start a pointer to the start of the struct.
/// \param end a pointer to the end of the struct.
/// \param sort_type the type of sorting we wand to apply. could be distance
/// or duration
/// \return a pointer to the partition we want to do.
BusLine *partition (BusLine *start, BusLine *end, SortType sort_type);
// write only between #define EX2_REPO_SORTBUSLINES_H and #endif
// EX2_REPO_SORTBUSLINES_H
#endif //EX2_REPO_SORTBUSLINES_H
