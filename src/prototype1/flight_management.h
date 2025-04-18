#ifndef FLIGHT_MANAGEMENT_H
#define FLIGHT_MANAGEMENT_H

#include "../airline_types.h"

BST_Node* create_node(Flight flight); // INIT a bst node
BST_Node* insert(BST_Node* root, Flight flight);
Flight* find_flight(BST_Node* root, int id);
void print_flights(BST_Node* root); // print all flights

// free memory
void free_tree(BST_Node* root);

#endif
