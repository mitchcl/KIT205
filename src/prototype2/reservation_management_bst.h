#ifndef RESERVATION_MANAGEMENT_BST_H
#define RESERVATION_MANAGEMENT_BST_H

#include "../airline_types.h"
#include "flight_management_avl.h"
#include "passenger_management_hash.h"

// Initialize reservation BST
ReservationBST* init_reservation_bst();

// Insert a reservation record into the BST
void add_reservation_bst(ReservationBST* bst, ReservationRecord record);

// Print all flights booked by a specific passenger
void print_passenger_flights_bst(ReservationBST* bst, AVL_Node* flights_root, int passengerId);

// Print all passengers who booked a specific flight
void print_flight_passengers_bst(ReservationBST* bst, PassengerHashTable* passengers_table, int flightId);

// Free reservation BST memory
void free_reservation_bst(ReservationBST* bst);

#endif
