#ifndef FLIGHT_MANAGEMENT_H
#define FLIGHT_MANAGEMENT_H

// Include airline_types.h first to get all type definitions
#include "../airline_types.h"

// Create a new BST node
BST_Node* create_node(Flight flight);

// Insert a flight into the BST
BST_Node* insert(BST_Node* root, Flight flight);

// Find a flight in the BST
Flight* find_flight(BST_Node* root, int id);

// Print all flights in the BST (in-order traversal)
void print_flights(BST_Node* root);

// Free BST memory
void free_tree(BST_Node* root);

#endif
