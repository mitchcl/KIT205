#ifndef DATA_GENERATOR_H
#define DATA_GENERATOR_H

#include "airline_types.h"

// Generate flights data with given count
Flight* generate_flights(int count);

// Generate passengers data with given count
Passenger* generate_passengers(int count);

// Generate reservation records with given count
ReservationRecord* generate_reservations(int count, int max_flight_id, int max_passenger_id);

// Save generated data to CSV files
void save_data_to_csv(Flight* flights, int flight_count, 
                      Passenger* passengers, int passenger_count, 
                      ReservationRecord* reservations, int reservation_count,
                      const char* output_dir);

#endif
