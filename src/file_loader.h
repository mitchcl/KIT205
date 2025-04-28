#ifndef FILE_LOADER_H
#define FILE_LOADER_H

#include "airline_types.h"

// Load flights from a CSV file
Flight* load_flights(const char* filename, int* count);

// Load passengers from a CSV file
Passenger* load_passengers(const char* filename, int* count);

// Load reservation records from a CSV file
ReservationRecord* load_reservations(const char* filename, int* count);

#endif
