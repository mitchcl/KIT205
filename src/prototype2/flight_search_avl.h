#ifndef FLIGHT_SEARCH_AVL_H
#define FLIGHT_SEARCH_AVL_H

#include "../airline_types.h"

// Search for a flight by flight number in AVL tree
Flight* avl_find_flight_by_number(AVL_Node* root, const char* flight_number);

#endif
