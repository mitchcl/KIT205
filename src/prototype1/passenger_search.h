#ifndef PASSENGER_SEARCH_H
#define PASSENGER_SEARCH_H

// Include airline_types.h first to get all type definitions
#include "../airline_types.h"

// Search for a passenger by name in linked list
Passenger* find_passenger_by_name(LL_Node* head, const char* name);

#endif
