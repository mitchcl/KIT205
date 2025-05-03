#ifndef RESERVATION_MANAGEMENT_BST_H
#define RESERVATION_MANAGEMENT_BST_H

#include "../airline_types.h"
#include "flight_management_avl.h"
#include "passenger_management_hash.h"

// Initialize reservation BST
ReservationBST* init_reservation_bst();

// Insert a reservation record into the BST
void add_reservation_bst(ReservationBST* bst, ReservationRecord record);

// Add a reservation with capacity validation
int add_reservation_bst_with_validation(ReservationBST* bst, AVL_Node* flights_root, ReservationRecord record);

// Print all flights booked by a specific passenger
void print_passenger_flights_bst(ReservationBST* bst, AVL_Node* flights_root, int passengerId);

// Print all passengers who booked a specific flight
void print_flight_passengers_bst(ReservationBST* bst, PassengerHashTable* passengers_table, int flightId);

// Count the number of passengers who booked a specific flight
int count_passengers_by_flight(ReservationBST* bst, int flightId);

// Count the number of flights booked by a specific passenger
int count_flights_by_passenger(ReservationBST* bst, int passengerId);

// Validate that a flight doesn't exceed its passenger capacity
int validate_flight_capacity_bst(ReservationBST* bst, AVL_Node* flights_root, int flightId);

// Free reservation BST memory
void free_reservation_bst(ReservationBST* bst);

#endif
