#ifndef FLIGHT_SEARCH_H
#define FLIGHT_SEARCH_H

// Include airline_types.h first to get all type definitions
#include "../airline_types.h"

// Search for a flight by flight number in BST
Flight* find_flight_by_number(BST_Node* root, const char* flight_number);

#endif
