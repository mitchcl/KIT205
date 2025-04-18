#ifndef PASSENGER_MANAGEMENT_H
#define PASSENGER_MANAGEMENT_H

#include "../airline_types.h"

LL_Node* create_passenger_node(Passenger passenger);
LL_Node* insert_passenger(LL_Node* head, Passenger passenger);
Passenger* find_passenger(LL_Node* head, int id);
void print_passengers(LL_Node* head);
void free_list(LL_Node* head);

#endif
