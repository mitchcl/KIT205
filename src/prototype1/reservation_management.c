#include <stdio.h>
#include <stdlib.h>
#include "reservation_management.h"

// init reservations array with a given capacity
ReservationArray* init_reservations(int capacity) {
    ReservationArray* array = (ReservationArray*)malloc(sizeof(ReservationArray));
    if (array == NULL) {
        fprintf(stderr, "Memory allocation failed for reservations array\n");
        exit(1);
    }
    
    array->records = (ReservationRecord*)malloc(capacity * sizeof(ReservationRecord));
    if (array->records == NULL) {
        fprintf(stderr, "Memory allocation failed for reservation records\n");
        free(array);
        exit(1);
    }
    
    array->capacity = capacity;
    array->count = 0;
    
    return array;
}

// Add a reservation record to the array
void add_reservation(ReservationArray* array, ReservationRecord record) {
    if (array == NULL) {
        return;
    }
    
    // Resize array if necessary
    if (array->count >= array->capacity) {
        int new_capacity = array->capacity * 2;
        ReservationRecord* new_records = (ReservationRecord*)realloc(array->records, 
                                                                    new_capacity * sizeof(ReservationRecord));
        
        if (new_records == NULL) {
            fprintf(stderr, "Memory allocation failed for resizing reservation array\n");
            return;
        }
        
        array->records = new_records;
        array->capacity = new_capacity;
    }
    
    // Add the record at the end
    array->records[array->count] = record;
    array->count++;
}

// Print all flights booked by a specific passenger
void print_passenger_flights(ReservationArray* array, BST_Node* flights_root, int passengerId) {
    if (array == NULL) {
        return;
    }
    
    printf("Flights booked by passenger %d:\n", passengerId);
    
    for (int i = 0; i < array->count; i++) {
        if (array->records[i].passengerId == passengerId) {
            Flight* flight = find_flight(flights_root, array->records[i].flightId);
            if (flight != NULL) {
                printf("- Flight %s: %s to %s\n", 
                       flight->flightNumber, flight->origin, flight->destination);
            }
        }
    }
}

// Print all passengers who booked a specific flight
void print_flight_passengers(ReservationArray* array, LL_Node* passengers_head, int flightId) {
    if (array == NULL) {
        return;
    }
    
    printf("Passengers on flight with ID %d:\n", flightId);
    
    for (int i = 0; i < array->count; i++) {
        if (array->records[i].flightId == flightId) {
            Passenger* passenger = find_passenger(passengers_head, array->records[i].passengerId);
            if (passenger != NULL) {
                printf("- %s (Seat: %s)\n", passenger->name, array->records[i].seatNumber);
            }
        }
    }
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
