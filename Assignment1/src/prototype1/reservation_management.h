#ifndef RESERVATION_MANAGEMENT_H
#define RESERVATION_MANAGEMENT_H

#include "../airline_types.h"
#include "flight_management.h"
#include "passenger_management.h"

// Initialize reservations array with a given capacity
ReservationArray* init_reservations(int capacity);

// Add a reservation record to the array
void add_reservation(ReservationArray* array, ReservationRecord record);

// Add a reservation with capacity validation
int add_reservation_with_validation(ReservationArray* array, BST_Node* flights_root, ReservationRecord record);

// Print all flights booked by a specific passenger
void print_passenger_flights(ReservationArray* array, BST_Node* flights_root, int passengerId);

// Print all passengers who booked a specific flight
void print_flight_passengers(ReservationArray* array, LL_Node* passengers_head, int flightId);

// Count the number of unique passengers for a flight
int count_passengers_by_flight_array(ReservationArray* array, int flightId);

// Validate that a flight doesn't exceed its passenger capacity
int validate_flight_capacity_array(ReservationArray* array, BST_Node* flights_root, int flightId);

// Free reservations array memory
void free_reservations(ReservationArray* array);

#endif
