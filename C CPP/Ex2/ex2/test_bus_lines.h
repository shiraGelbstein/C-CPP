#ifndef EX2_REPO_TESTBUSLINES_H
#define EX2_REPO_TESTBUSLINES_H
#define TRUE 1
#define FALSE 0
// write only between #define EX2_REPO_TESTBUSLINES_H and #endif
// EX2_REPO_TESTBUSLINES_H
#include "sort_bus_lines.h"

/// A function that checks if the given struct is sorted by distance.
/// \param start a pointer to the start of the struct.
/// \param end a pointer to the end+1 of the struct.
/// \return true if its sorted anf false else.
int is_sorted_by_distance (BusLine *start, BusLine *end);

/// A function that checks if the given struct is sorted by duration.
/// \param start a pointer to the start of the struct.
/// \param end a pointer to the end+1 of the struct.
/// \return true if its sorted anf false else.
int is_sorted_by_duration (BusLine *start, BusLine *end);

/// A function that checks if the given struct is sorted by name.
/// \param start a pointer to the start of the struct.
/// \param end a pointer to the end+1 of the struct.
/// \return true if its sorted anf false else.
int is_sorted_by_name (BusLine *start, BusLine *end);

/// A function that gets two structs and checks if they has the same args
/// (by names) and if its the same length.
/// \param start_sorted a pointer to the start of the sorted struct.
/// \param end_sorted a pointer to the end+1 of the sorted struct.
/// \param start_original a pointer to the start of the original struct.
/// \param end_original a pointer to the end+1 of the original struct.
/// \return true if its the same else false.
int is_equal (BusLine *start_sorted,
              BusLine *end_sorted, BusLine *start_original,
              BusLine *end_original);
// write only between #define EX2_REPO_TESTBUSLINES_H and #endif
// EX2_REPO_TESTBUSLINES_H
#endif //EX2_REPO_TESTBUSLINES_H
