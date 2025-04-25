#ifndef FLIGHT_MANAGEMENT_AVL_H
#define FLIGHT_MANAGEMENT_AVL_H

#include "../airline_types.h"

// Create a new AVL node
AVL_Node* avl_create_node(Flight flight);

// Get height of an AVL node
int avl_height(AVL_Node* node);

// Get balance factor of an AVL node
int avl_get_balance(AVL_Node* node);

// Right rotation
AVL_Node* avl_right_rotate(AVL_Node* y);

// Left rotation
AVL_Node* avl_left_rotate(AVL_Node* x);

AVL_Node* avl_insert(AVL_Node* root, Flight flight);
Flight* avl_find_flight(AVL_Node* root, int id);
void avl_print_flights(AVL_Node* root);
void free_avl_tree(AVL_Node* root);

#endif
