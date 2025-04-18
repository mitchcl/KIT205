#ifndef RESERVATION_MANAGEMENT_H
#define RESERVATION_MANAGEMENT_H

#include "../airline_types.h"
#include "flight_management.h"
#include "passenger_management.h"

ReservationArray* init_reservations(int capacity);
void add_reservation(ReservationArray* array, ReservationRecord record);
void print_passenger_flights(ReservationArray* array, BST_Node* flights_root, int passengerId);
void print_flight_passengers(ReservationArray* array, LL_Node* passengers_head, int flightId);
void free_reservations(ReservationArray* array); // Free reservations array memory

#endif
