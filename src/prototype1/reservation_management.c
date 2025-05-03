/*
 * Reservation Management Array Implementation (Prototype 1)
 * 
 * Sources used:
 * 1. The C Programming Language (K&R) - Array and memory concepts
 * 2. Data Structures and Algorithm Analysis by Mark Allen Weiss - Array operations
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "reservation_management.h"

// Initialize reservations array with a given capacity - optimized for large datasets
ReservationArray* init_reservations(int capacity) {
    // Allocate the array structure
    ReservationArray* array = (ReservationArray*)malloc(sizeof(ReservationArray));
    if (array == NULL) {
        fprintf(stderr, "Memory allocation failed for reservations array structure\n");
        return NULL;  // Return NULL instead of exit for better error handling
    }
    
    // For large datasets, ensure we allocate with sufficient initial capacity
    // to avoid costly reallocations later
    if (capacity > 100000) {
        // Add a 10% buffer to avoid frequent reallocations
        capacity = (int)(capacity * 1.1);
        printf("Optimizing reservation array for large dataset with capacity: %d\n", capacity);
    }
    
    // Allocate the records array
    array->records = (ReservationRecord*)malloc(capacity * sizeof(ReservationRecord));
    if (array->records == NULL) {
        fprintf(stderr, "Memory allocation failed for %d reservation records\n", capacity);
        free(array);
        return NULL;  // Return NULL instead of exit for better error handling
    }
    
    array->capacity = capacity;
    array->count = 0;
    
    return array;
}

// Add a reservation record to the array - optimized for large datasets
void add_reservation(ReservationArray* array, ReservationRecord record) {
    if (array == NULL) {
        fprintf(stderr, "Cannot add to NULL reservation array\n");
        return;
    }
    
    // Resize array if necessary
    if (array->count >= array->capacity) {
        // For large arrays, use a more conservative growth strategy
        // to avoid excessive memory usage
        int new_capacity;
        if (array->capacity > 500000) {
            // For very large arrays, add a fixed increment instead of doubling
            new_capacity = array->capacity + 100000;
            printf("Growing large reservation array to %d\n", new_capacity);
        } else if (array->capacity > 100000) {
            // For large arrays, add 50% instead of doubling
            new_capacity = (int)(array->capacity * 1.5);
        } else {
            // For smaller arrays, continue with doubling strategy
            new_capacity = array->capacity * 2;
        }
        
        // Try to reallocate with new capacity
        ReservationRecord* new_records = (ReservationRecord*)realloc(array->records, 
                                                                 new_capacity * sizeof(ReservationRecord));
        if (new_records == NULL) {
            fprintf(stderr, "Memory allocation failed while resizing reservations array to %d elements\n", 
                   new_capacity);
            
            // Try a smaller increment as a fallback
            new_capacity = array->capacity + (array->capacity / 10); // Add 10%
            new_records = (ReservationRecord*)realloc(array->records, 
                                                 new_capacity * sizeof(ReservationRecord));
            if (new_records == NULL) {
                fprintf(stderr, "Critical error: Cannot resize reservation array\n");
                return;
            }
        }
        
        array->records = new_records;
        array->capacity = new_capacity;
    }
    
    // Add the record at the end
    array->records[array->count++] = record;
}

// Format date to a readable string
char* format_reservation_date(time_t timestamp) {
    static char buffer[26];
    struct tm* tm_info = localtime(&timestamp);
    strftime(buffer, 26, "%Y-%m-%d %H:%M", tm_info);
    return buffer;
}

// Print all flights booked by a specific passenger
void print_passenger_flights(ReservationArray* array, BST_Node* flights_root, int passengerId) {
    int count = 0;
    
    for (int i = 0; i < array->count; i++) {
        if (array->records[i].passengerId == passengerId) {
            Flight* flight = find_flight(flights_root, array->records[i].flightId);
            if (flight != NULL) {
                printf("Flight ID: %d, Number: %s, From: %s, To: %s, Seat: %s, Booked on: %s\n", 
                       flight->id, flight->flightNumber, flight->origin, flight->destination,
                       array->records[i].seatNumber, format_reservation_date(array->records[i].bookingDate));
                count++;
            }
        }
    }
    
    if (count == 0) {
        printf("Passenger with ID %d has no flight bookings.\n", passengerId);
    } else {
        printf("Total flights booked by passenger %d: %d\n", passengerId, count);
    }
}

// Print all passengers who booked a specific flight
void print_flight_passengers(ReservationArray* array, LL_Node* passengers_head, int flightId) {
    int count = 0;
    int max_passenger_id = 0;
    int* counted_passengers = NULL;
    
    // First pass: find the maximum passenger ID to allocate tracking array
    for (int i = 0; i < array->count; i++) {
        if (array->records[i].flightId == flightId && array->records[i].passengerId > max_passenger_id) {
            max_passenger_id = array->records[i].passengerId;
        }
    }
    
    // If no passengers found for this flight
    if (max_passenger_id == 0) {
        printf("Flight with ID %d has no passengers.\n", flightId);
        return;
    }
    
    // Allocate tracking array for unique passenger IDs
    counted_passengers = (int*)calloc(max_passenger_id + 1, sizeof(int));
    if (counted_passengers == NULL) {
        fprintf(stderr, "Memory allocation failed for passenger tracking\n");
        return;
    }
    
    // Process reservations and track unique passengers
    for (int i = 0; i < array->count; i++) {
        if (array->records[i].flightId == flightId) {
            int passenger_id = array->records[i].passengerId;
            
            // Check if this passenger has already been counted
            if (counted_passengers[passenger_id] == 0) {
                Passenger* passenger = find_passenger(passengers_head, passenger_id);
                if (passenger != NULL) {
                    printf("Passenger ID: %d, Name: %s, Passport: %s, Seat: %s, Booked on: %s\n", 
                           passenger->id, passenger->name, passenger->passportNumber,
                           array->records[i].seatNumber, format_reservation_date(array->records[i].bookingDate));
                    
                    // Mark this passenger as counted
                    counted_passengers[passenger_id] = 1;
                    count++;
                }
            } else {
                // This is a duplicate booking by the same passenger (multiple seats)
                Passenger* passenger = find_passenger(passengers_head, passenger_id);
                if (passenger != NULL) {
                    printf("  Additional seat for Passenger ID: %d, Seat: %s\n", 
                           passenger_id, array->records[i].seatNumber);
                }
            }
        }
    }
    
    // Free tracking array
    free(counted_passengers);
    
    if (count == 0) {
        printf("Flight with ID %d has no passengers.\n", flightId);
    } else {
        printf("Total unique passengers on flight %d: %d\n", flightId, count);
    }
}

// Count the number of unique passengers for a flight
int count_passengers_by_flight_array(ReservationArray* array, int flightId) {
    if (array == NULL || array->count == 0) {
        return 0;
    }
    
    // Find maximum passenger ID to determine tracking array size
    int max_passenger_id = 0;
    for (int i = 0; i < array->count; i++) {
        if (array->records[i].flightId == flightId && 
            array->records[i].passengerId > max_passenger_id) {
            max_passenger_id = array->records[i].passengerId;
        }
    }
    
    // If no reservations found for this flight
    if (max_passenger_id == 0) {
        return 0;
    }
    
    // Allocate tracking array for unique passenger IDs
    int* counted_passengers = (int*)calloc(max_passenger_id + 1, sizeof(int));
    if (counted_passengers == NULL) {
        fprintf(stderr, "Memory allocation failed for passenger counting\n");
        return 0;
    }
    
    // Count unique passengers
    int unique_count = 0;
    for (int i = 0; i < array->count; i++) {
        if (array->records[i].flightId == flightId) {
            int passenger_id = array->records[i].passengerId;
            
            // Check if this passenger has already been counted
            if (counted_passengers[passenger_id] == 0) {
                counted_passengers[passenger_id] = 1;
                unique_count++;
            }
        }
    }
    
    // Free tracking array
    free(counted_passengers);
    return unique_count;
}

// Validate that a flight has exactly its capacity in passengers
int validate_flight_capacity_array(ReservationArray* array, BST_Node* flights_root, int flightId) {
    if (array == NULL || flights_root == NULL) {
        return 0; // Invalid input parameters
    }
    
    // Find the flight by ID
    Flight* flight = find_flight(flights_root, flightId);
    if (flight == NULL) {
        return 0; // Flight not found
    }
    
    // Count unique passengers for this flight
    int passenger_count = count_passengers_by_flight_array(array, flightId);
    
    // Compare with capacity - now check for exact match
    if (passenger_count == flight->capacity) {
        return 1; // Valid: passenger count equals capacity
    } else if (passenger_count < flight->capacity) {
        printf("ERROR: Flight %d has only %d passengers, but should have exactly %d (capacity)\n", 
               flightId, passenger_count, flight->capacity);
        return 0; // Invalid: passenger count is below capacity
    } else {
        printf("ERROR: Flight %d has %d passengers, exceeding capacity of %d\n", 
               flightId, passenger_count, flight->capacity);
        return 0; // Invalid: passenger count exceeds capacity
    }
}

// Add a reservation with capacity validation
int add_reservation_with_validation(ReservationArray* array, BST_Node* flights_root, ReservationRecord record) {
    if (array == NULL || flights_root == NULL) {
        return 0; // Invalid parameters
    }
    
    // Find the flight
    Flight* flight = find_flight(flights_root, record.flightId);
    if (flight == NULL) {
        printf("ERROR: Flight ID %d not found\n", record.flightId);
        return 0;
    }
    
    // Count current unique passengers for this flight
    int current_passengers = count_passengers_by_flight_array(array, record.flightId);
    
    // Check if passenger is already booked (doesn't count as new)
    int passenger_already_booked = 0;
    int max_passenger_id = 0;
    
    // Find max passenger ID for the tracking array
    for (int i = 0; i < array->count; i++) {
        if (array->records[i].flightId == record.flightId && 
            array->records[i].passengerId > max_passenger_id) {
            max_passenger_id = array->records[i].passengerId;
        }
    }
    
    // Create a tracking array if there are existing passengers
    if (max_passenger_id > 0) {
        int* counted_passengers = (int*)calloc(max_passenger_id + 1, sizeof(int));
        if (counted_passengers == NULL) {
            fprintf(stderr, "Memory allocation failed for passenger counting\n");
            return 0;
        }
        
        // Mark existing passengers
        for (int i = 0; i < array->count; i++) {
            if (array->records[i].flightId == record.flightId) {
                int passenger_id = array->records[i].passengerId;
                counted_passengers[passenger_id] = 1;
            }
        }
        
        // Check if this passenger is already booked
        if (record.passengerId <= max_passenger_id && counted_passengers[record.passengerId]) {
            passenger_already_booked = 1;
        }
        
        free(counted_passengers);
    }
    
    // If this would add a new passenger, check capacity
    if (!passenger_already_booked && current_passengers >= flight->capacity) {
        printf("ERROR: Cannot add reservation. Flight %d has reached capacity of %d passengers.\n", 
               record.flightId, flight->capacity);
        return 0;
    }
    
    // Add the reservation
    add_reservation(array, record);
    return 1;
}

// Free reservations array memory
void free_reservations(ReservationArray* array) {
    if (array != NULL) {
        if (array->records != NULL) {
            free(array->records);
        }
        free(array);
    }
}
