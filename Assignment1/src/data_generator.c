/*
 * Data Generator Implementation for Airline Reservation System
 * 
 * Sources used:
 * 1. https://stackoverflow.com/questions/822323/how-to-generate-a-random-number-in-c - random number generation techniques
 * 2. The C Programming Language (K&R) - file I/O operations
 * 3. "Algorithms in C" by Robert Sedgewick - Random data generation techniques
 */
#define _CRT_SECURE_NO_DEPRECATE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "airline_types.h"
#include "data_generator.h"

// Constants for random generation
#define NUM_AIRLINES 12
#define NUM_CITIES 30
#define MIN_CAPACITY 100
#define MAX_CAPACITY 400
#define DATE_RANGE_DAYS 365

// Sample data for generating realistic flight information
const char* airlines[] = {
    "AA", "UA", "DL", "BA", "LH", "AF", "QF", "EK", "SQ", "CX", "JL", "NH"
};

const char* cities[] = {
    "New York", "London", "Paris", "Tokyo", "Sydney", "Hong Kong", "Singapore", "Dubai",
    "Los Angeles", "Chicago", "Toronto", "Berlin", "Rome", "Madrid", "Beijing", "Shanghai",
    "Mumbai", "Bangkok", "Cairo", "Mexico City", "Cape Town", "Rio de Janeiro", "Istanbul",
    "Moscow", "Amsterdam", "Zurich", "Seoul", "San Francisco", "Miami", "Barcelona"
};

// First names for random passenger generation
const char* first_names[] = {
    "James", "Mary", "John", "Patricia", "Robert", "Jennifer", "Michael", "Linda",
    "William", "Elizabeth", "David", "Barbara", "Richard", "Susan", "Joseph", "Jessica",
    "Thomas", "Sarah", "Charles", "Karen", "Emma", "Noah", "Olivia", "Liam", "Ava",
    "Jacob", "Sophia", "Mason", "Isabella", "Ethan", "Charlotte"
};

// Last names for random passenger generation
const char* last_names[] = {
    "Smith", "Johnson", "Williams", "Jones", "Brown", "Davis", "Miller", "Wilson",
    "Moore", "Taylor", "Anderson", "Thomas", "Jackson", "White", "Harris", "Martin",
    "Thompson", "Garcia", "Martinez", "Robinson", "Clark", "Rodriguez", "Lewis", "Lee",
    "Walker", "Hall", "Allen", "Young", "Hernandez", "King"
};

// Random seat letters (excluding I and O which could be confused with numbers)
const char* seat_letters = "ABCDEFGHJKLMNPQRSTUVWXYZ";

// Generate a random date within the given range
time_t random_date(time_t start, time_t range_seconds) {
    return start + (rand() % (int)range_seconds);
}

// Generate a random flight number
void generate_flight_number(char* flight_number, int max_length) {
    int airline_idx = rand() % NUM_AIRLINES;
    int flight_num = 100 + (rand() % 9000); // Flight numbers between 100 and 9099
    
    snprintf(flight_number, max_length, "%s%d", 
             airlines[airline_idx], flight_num);
}

// Generate a random seat assignment
void generate_seat_number(char* seat_number, int max_length) {
    int row = 1 + (rand() % 50); // Rows 1-50
    char letter = seat_letters[rand() % strlen(seat_letters)];
    
    snprintf(seat_number, max_length, "%d%c", row, letter);
}

// Generate a random person name
void generate_person_name(char* name, int max_length) {
    int first_idx = rand() % (sizeof(first_names) / sizeof(first_names[0]));
    int last_idx = rand() % (sizeof(last_names) / sizeof(last_names[0]));
    
    snprintf(name, max_length, "%s %s", first_names[first_idx], last_names[last_idx]);
}

// Generate flights data with given count
Flight* generate_flights(int count) {
    // Allocate memory for flights
    Flight* flights = (Flight*)malloc(count * sizeof(Flight));
    if (flights == NULL) {
        return NULL;
    }
    
    // Generate flights with incremental IDs and random details
    for (int i = 0; i < count; i++) {
        flights[i].id = 1000 + i;  // Start IDs from 1000
        
        // Generate flight number using the helper function
        generate_flight_number(flights[i].flightNumber, sizeof(flights[i].flightNumber));
        
        // Random origin and destination
        int origin_idx = rand() % NUM_CITIES;
        int dest_idx;
        do {
            dest_idx = rand() % NUM_CITIES;
        } while (dest_idx == origin_idx);  // Make sure origin != destination
        
        strncpy(flights[i].origin, cities[origin_idx], sizeof(flights[i].origin)-1);
        flights[i].origin[sizeof(flights[i].origin)-1] = '\0';
        
        strncpy(flights[i].destination, cities[dest_idx], sizeof(flights[i].destination)-1);
        flights[i].destination[sizeof(flights[i].destination)-1] = '\0';
        
        // Random departure time (within next year)
        time_t now = time(NULL);
        flights[i].departureTime = now + (rand() % (365 * 24 * 60 * 60));
        
        // Random capacity between MIN_CAPACITY and MAX_CAPACITY
        flights[i].capacity = MIN_CAPACITY + (rand() % (MAX_CAPACITY - MIN_CAPACITY + 1));
    }
    
    return flights;
}

// Generate passengers data with given count
Passenger* generate_passengers(int count) {
    // Allocate memory for passengers
    Passenger* passengers = (Passenger*)malloc(count * sizeof(Passenger));
    if (passengers == NULL) {
        return NULL;
    }
    
    // Generate passengers with incremental IDs and random names
    for (int i = 0; i < count; i++) {
                passengers[i].id = 2000 + i;  // Start IDs from 2000
        generate_person_name(passengers[i].name, sizeof(passengers[i].name));
        
        // Generate random passport number (e.g., AB123456)
        char letters[3] = {
            'A' + (rand() % 26),
            'A' + (rand() % 26),
            '\0'
        };
        snprintf(passengers[i].passportNumber, sizeof(passengers[i].passportNumber),
                 "%s%06d", letters, rand() % 1000000);
    }
    
    return passengers;
}

// Generate reservation records with given count and ID ranges
ReservationRecord* generate_reservations(int count, int max_flight_id, int max_passenger_id) {
    // Allocate memory for reservation records
    ReservationRecord* reservations = (ReservationRecord*)malloc(count * sizeof(ReservationRecord));
    if (reservations == NULL) {
        return NULL;
    }
    
    // Create an array to track each flight's capacity and current passenger count
    typedef struct {
        int capacity;
        int current_count;
        int* assigned_passengers;  // Track which passengers are already assigned to this flight
    } FlightBooking;
    
    // Create tracking array for flights - assuming flight IDs start at 1000
    FlightBooking* flight_bookings = (FlightBooking*)calloc(max_flight_id + 1, sizeof(FlightBooking));
    if (flight_bookings == NULL) {
        free(reservations);
        return NULL;
    }
    
    // Initialize capacities for each flight
    // We assume flight IDs start at 1000 and are contiguous
    for (int i = 0; i < max_flight_id; i++) {
        // Generate reasonable capacity for each flight
        flight_bookings[i].capacity = MIN_CAPACITY + (rand() % (MAX_CAPACITY - MIN_CAPACITY + 1));
        
        flight_bookings[i].current_count = 0;
        flight_bookings[i].assigned_passengers = (int*)calloc(max_passenger_id + 1, sizeof(int));
        
        if (flight_bookings[i].assigned_passengers == NULL) {
            // Clean up on allocation failure
            for (int j = 0; j < i; j++) {
                free(flight_bookings[j].assigned_passengers);
            }
            free(flight_bookings);
            free(reservations);
            return NULL;
        }
    }
    
    int reservation_index = 0;
    
    // First pass: Ensure each flight has at least up to its capacity in unique passengers
    for (int i = 0; i < max_flight_id && reservation_index < count; i++) {
        int flight_id = 1000 + i;
        int capacity = flight_bookings[i].capacity;
        
        // Add reservations up to the flight's capacity (or less if we're running out of total count)
        while (flight_bookings[i].current_count < capacity && reservation_index < count) {
            // Find an unassigned passenger for this flight
            int passenger_id;
            do {
                passenger_id = 2000 + (rand() % max_passenger_id);
            } while (flight_bookings[i].assigned_passengers[passenger_id - 2000]);
            
            // Mark this passenger as assigned to this flight
            flight_bookings[i].assigned_passengers[passenger_id - 2000] = 1;
            
            // Create the reservation
            reservations[reservation_index].flightId = flight_id;
            reservations[reservation_index].passengerId = passenger_id;
            
            // Random booking date (within past year)
            time_t now = time(NULL);
            reservations[reservation_index].bookingDate = now - (rand() % (365 * 24 * 60 * 60));
            
            // Generate a random seat number
            generate_seat_number(reservations[reservation_index].seatNumber, 
                               sizeof(reservations[reservation_index].seatNumber));
            
            flight_bookings[i].current_count++;
            reservation_index++;
        }
    }
    
    // Second pass: PRIORITIZE ensuring ALL flights have EXACTLY their capacity in unique passengers
    // This is critical for the capacity validation tests
    
    // IMPROVED APPROACH: Clear all existing reservations and start fresh
    // This ensures we can create exactly the right number of passengers for each flight
    reservation_index = 0;
    
    // Reset counters and passenger assignments
    for (int i = 0; i < max_flight_id; i++) {
        flight_bookings[i].current_count = 0;
        // Reset passenger assignments
        for (int j = 0; j < max_passenger_id; j++) {
            flight_bookings[i].assigned_passengers[j] = 0;
        }
    }
    
    // Calculate how many flights we can fully populate with exact capacity
    int total_capacity = 0;
    for (int i = 0; i < max_flight_id; i++) {
        total_capacity += flight_bookings[i].capacity;
    }
    
    // Ensure we don't exceed available reservation count
    int can_populate = (total_capacity <= count);
    
    // If we can't populate all flights fully, adjust capacities
    if (!can_populate) {
        // Scale down capacities proportionally
        float scale_factor = (float)count / total_capacity;
        for (int i = 0; i < max_flight_id; i++) {
            int new_capacity = (int)(flight_bookings[i].capacity * scale_factor);
            if (new_capacity < 1) new_capacity = 1;  // Minimum capacity of 1
            flight_bookings[i].capacity = new_capacity;
        }
    }
    
    // Now ensure each flight gets EXACTLY its capacity in unique passengers
    for (int i = 0; i < max_flight_id && reservation_index < count; i++) {
        int flight_id = 1000 + i;
        int capacity = flight_bookings[i].capacity;
        
        // If this flight doesn't have exactly its capacity in passengers yet, add more
        while (flight_bookings[i].current_count < capacity && reservation_index < count) {
            // Find an unassigned passenger for this flight
            int passenger_id;
            int found = 0;
            
            // Create a list of available passenger IDs
            int* available_passengers = (int*)malloc(max_passenger_id * sizeof(int));
            int available_count = 0;
            
            if (available_passengers == NULL) {
                fprintf(stderr, "Memory allocation failed for available passengers list\n");
                continue;
            }
            
            // Find all available passengers
            for (int p = 0; p < max_passenger_id; p++) {
                if (!flight_bookings[i].assigned_passengers[p]) {
                    available_passengers[available_count++] = 2000 + p;
                }
            }
            
            if (available_count > 0) {
                // Pick a random available passenger
                passenger_id = available_passengers[rand() % available_count];
                found = 1;
            }
            
            free(available_passengers);
            
            // If we couldn't find any unassigned passenger, create a new one beyond max_passenger_id
            if (!found) {
                passenger_id = 2000 + max_passenger_id + (i * capacity) + flight_bookings[i].current_count;
                // Remove debug messages to avoid terminal output issues
                // printf("Created new passenger ID %d for flight %d to match capacity\n", 
                //       passenger_id, flight_id);
            }
            
            // Mark this passenger as assigned to this flight
            int passenger_index = passenger_id - 2000;
            if (passenger_index >= 0 && passenger_index < max_passenger_id) {
                flight_bookings[i].assigned_passengers[passenger_index] = 1;
            }
            
            // Create the reservation
            reservations[reservation_index].flightId = flight_id;
            reservations[reservation_index].passengerId = passenger_id;
            
            // Random booking date (within past year)
            time_t now = time(NULL);
            reservations[reservation_index].bookingDate = now - (rand() % (365 * 24 * 60 * 60));
            
            // Generate a random seat number
            generate_seat_number(reservations[reservation_index].seatNumber, 
                               sizeof(reservations[reservation_index].seatNumber));
                               
            flight_bookings[i].current_count++;
            reservation_index++;
        }
    }
    
    // All flights should now have exactly their capacity in passengers
    
    // Third pass: If we still have remaining reservations, add them for existing passengers
    while (reservation_index < count) {
        // Pick any flight 
        int flight_index = rand() % max_flight_id;
        int flight_id = 1000 + flight_index;
        
        // Choose a passenger who is already assigned to this flight
        int attempts = 0;
        int passenger_id;
        do {
            passenger_id = 2000 + (rand() % max_passenger_id);
            attempts++;
            // If we can't find a passenger after many attempts, just pick any passenger
            if (attempts > 20) {
                passenger_id = 2000 + (rand() % max_passenger_id);
                break;
            }
        } while (!flight_bookings[flight_index].assigned_passengers[passenger_id - 2000]);
        
        // Random booking date (within past year)
        time_t now = time(NULL);
        reservations[reservation_index].bookingDate = now - (rand() % (365 * 24 * 60 * 60));
        
        // Generate a random seat number
        generate_seat_number(reservations[reservation_index].seatNumber, 
                           sizeof(reservations[reservation_index].seatNumber));
        
        reservations[reservation_index].flightId = flight_id;
        reservations[reservation_index].passengerId = passenger_id;
        
        reservation_index++;
    }
    
    // Clean up tracking arrays
    for (int i = 0; i < max_flight_id; i++) {
        free(flight_bookings[i].assigned_passengers);
    }
    free(flight_bookings);
    
    return reservations;
}

// Format time_t to a date string (YYYY-MM-DD format)
void format_date(time_t time_value, char* buffer, int buffer_size) {
    struct tm* tm_info = localtime(&time_value);
    strftime(buffer, buffer_size, "%Y-%m-%d", tm_info);
}

// Save generated data to CSV files
void save_data_to_csv(Flight* flights, int flight_count, 
                     Passenger* passengers, int passenger_count, 
                     ReservationRecord* reservations, int reservation_count,
                     const char* output_dir) {
    char filepath[256];
    FILE* file;
    
    // Save flights
    snprintf(filepath, sizeof(filepath), "%s/flights.csv", output_dir);
    file = fopen(filepath, "w");
    if (file) {
        // Write header
        fprintf(file, "id,flightNumber,origin,destination,departureTime,capacity\n");
        
        // Write data
        for (int i = 0; i < flight_count; i++) {
            char departure_str[20];
            struct tm* tm_info = localtime(&flights[i].departureTime);
            strftime(departure_str, sizeof(departure_str), "%Y-%m-%d %H:%M:%S", tm_info);
            
            fprintf(file, "%d,%s,%s,%s,%s,%d\n", 
                   flights[i].id, 
                   flights[i].flightNumber,
                   flights[i].origin,
                   flights[i].destination,
                   departure_str,
                   flights[i].capacity);
        }
        
        fclose(file);
        printf("Generated %d flights and saved to %s\n", flight_count, filepath);
    } else {
        fprintf(stderr, "Could not open file %s for writing\n", filepath);
    }
    
    // Save passengers
    snprintf(filepath, sizeof(filepath), "%s/passengers.csv", output_dir);
    file = fopen(filepath, "w");
    if (file) {
        // Write header
        fprintf(file, "id,name,passportNumber\n");
        
        // Write data
        for (int i = 0; i < passenger_count; i++) {
            fprintf(file, "%d,%s,%s\n", 
                   passengers[i].id, 
                   passengers[i].name,
                   passengers[i].passportNumber);
        }
        
        fclose(file);
        printf("Generated %d passengers and saved to %s\n", passenger_count, filepath);
    } else {
        fprintf(stderr, "Could not open file %s for writing\n", filepath);
    }
    
    // Save reservations
    snprintf(filepath, sizeof(filepath), "%s/reservations.csv", output_dir);
    file = fopen(filepath, "w");
    if (file) {
        // Write header
        fprintf(file, "flightId,passengerId,bookingDate,seatNumber\n");
        
        // Write data
        for (int i = 0; i < reservation_count; i++) {
            char booking_date_str[20]; // YYYY-MM-DD HH:MM:SS + null terminator
            
            format_date(reservations[i].bookingDate, booking_date_str, sizeof(booking_date_str));
            
            fprintf(file, "%d,%d,%s,%s\n", 
                   reservations[i].flightId, 
                   reservations[i].passengerId,
                   booking_date_str,
                   reservations[i].seatNumber);
        }
        
        fclose(file);
        printf("Generated %d reservation records and saved to %s\n", reservation_count, filepath);
    } else {
        fprintf(stderr, "Could not open file %s for writing\n", filepath);
    }
}
