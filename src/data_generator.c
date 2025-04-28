#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "airline_types.h"
#include "data_generator.h"

// Constants for random generation
#define NUM_AIRLINES 10
#define NUM_CITIES 20
#define MIN_CAPACITY 100
#define MAX_CAPACITY 300

// Sample data for generating realistic flight information
const char* airlines[] = {
    "AA", "UA", "DL", "BA", "LH", "AF", "QF", "EK", "SQ", "CX"
};

const char* cities[] = {
    "New York", "London", "Paris", "Tokyo", "Sydney", "Hong Kong", "Singapore", "Dubai",
    "Los Angeles", "Chicago", "Toronto", "Berlin", "Madrid", "Rome", "Beijing", "Mumbai",
    "Cairo", "Rio de Janeiro", "Mexico City", "Johannesburg"
};

// First and last names for passenger generation
const char* first_names[] = {
    "James", "John", "Robert", "Michael", "William", "David", "Richard", "Joseph", "Thomas", "Charles",
    "Mary", "Patricia", "Jennifer", "Linda", "Elizabeth", "Barbara", "Susan", "Jessica", "Sarah", "Karen"
};

const char* last_names[] = {
    "Smith", "Johnson", "Williams", "Jones", "Brown", "Davis", "Miller", "Wilson", "Moore", "Taylor",
    "Anderson", "Thomas", "Jackson", "White", "Harris", "Martin", "Thompson", "Garcia", "Martinez", "Robinson"
};

// Generate a random date within the given range
time_t random_date(time_t start, time_t range_seconds) {
    return start + (rand() % range_seconds);
}

// Generate a random flight number
void generate_flight_number(char* flight_number, int max_length) {
    snprintf(flight_number, max_length, "%s%d", 
             airlines[rand() % NUM_AIRLINES], 
             100 + rand() % 900);  // Flight numbers between 100 and 999
}

// Generate a random seat assignment
void generate_seat_number(char* seat_number, int max_length) {
    snprintf(seat_number, max_length, "%d%c", 
             1 + rand() % 30,      // Row numbers between 1 and 30
             'A' + rand() % 6);    // Seat letters between A and F
}

// Generate a random person name
void generate_person_name(char* name, int max_length) {
    snprintf(name, max_length, "%s %s", 
             first_names[rand() % (sizeof(first_names) / sizeof(char*))],
             last_names[rand() % (sizeof(last_names) / sizeof(char*))]);
}

// Generate flights data with given count
Flight* generate_flights(int count) {
    Flight* flights = (Flight*)malloc(count * sizeof(Flight));
    if (flights == NULL) {
        return NULL;
    }
    
    time_t now = time(NULL);
    time_t one_year = 365 * 24 * 60 * 60;
    
    for (int i = 0; i < count; i++) {
        flights[i].id = i + 1;
        generate_flight_number(flights[i].flightNumber, MAX_FLIGHT_ID_LENGTH);
        
        // Random origin and destination (ensure they're different)
        int origin_idx = rand() % NUM_CITIES;
        int dest_idx;
        do {
            dest_idx = rand() % NUM_CITIES;
        } while (dest_idx == origin_idx);
        
        strncpy(flights[i].origin, cities[origin_idx], sizeof(flights[i].origin));
        strncpy(flights[i].destination, cities[dest_idx], sizeof(flights[i].destination));
        
        // Random departure time within the next year
        flights[i].departureTime = random_date(now, one_year);
        
        // Random capacity
        flights[i].capacity = MIN_CAPACITY + rand() % (MAX_CAPACITY - MIN_CAPACITY + 1);
    }
    
    return flights;
}

// Generate passengers data with given count, Similar implementation as generate_flights
Passenger* generate_passengers(int count) {
    return NULL;
}

// Generate reservation records with given count
ReservationRecord* generate_reservations(int count, int max_flight_id, int max_passenger_id) {
    return NULL;
}

// Save generated data to CSV files
void save_data_to_csv(Flight* flights, int flight_count, 
                      Passenger* passengers, int passenger_count, 
                      ReservationRecord* reservations, int reservation_count,
                      const char* output_dir) {
}
