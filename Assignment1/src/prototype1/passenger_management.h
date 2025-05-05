#ifndef PASSENGER_MANAGEMENT_H
#define PASSENGER_MANAGEMENT_H

// Include airline_types.h first to get all type definitions
#include "../airline_types.h"

// Create a new linked list node for a passenger
LL_Node* create_passenger_node(Passenger passenger);

// Insert a passenger into the linked list
LL_Node* insert_passenger(LL_Node* head, Passenger passenger);

// Find a passenger in the linked list
Passenger* find_passenger(LL_Node* head, int id);

// Print all passengers in the linked list
void print_passengers(LL_Node* head);

// Remove a passenger from the linked list
LL_Node* remove_passenger(LL_Node* head, int id);

// Free linked list memory
void free_list(LL_Node* head);

#endif
