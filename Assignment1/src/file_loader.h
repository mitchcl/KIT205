#ifndef FILE_LOADER_H
#define FILE_LOADER_H

#include "airline_types.h"

// Load flights from a CSV file
Flight* load_flights(const char* filename, int* count);

// Load passengers from a CSV file
Passenger* load_passengers(const char* filename, int* count);

// Load reservation records from a CSV file
ReservationRecord* load_reservations(const char* filename, int* count);

// Estimate record count in a CSV file (without loading the whole file)
int estimate_csv_record_count(const char* filename);

// Memory-optimized loading for huge datasets
// Returns 1 on success, 0 on failure
int load_data_optimized(const char* flights_file, const char* passengers_file,
                       const char* reservations_file,
                       Flight** flights, int* flight_count,
                       Passenger** passengers, int* passenger_count,
                       ReservationRecord** reservations, int* reservation_count);

#endif
