#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../airline_types.h"
#include "flight_management_avl.h"

// Search for a flight by flight number in AVL tree
Flight* avl_find_flight_by_number(AVL_Node* root, const char* flight_number) {
    if (root == NULL) {
        return NULL;
    }
    
    // Check if current node matches
    if (strcmp(root->data.flightNumber, flight_number) == 0) {
        return &(root->data);
    }
    
    // Recursively search left and right subtrees
    Flight* result = avl_find_flight_by_number(root->left, flight_number);
    if (result != NULL) {
        return result;
    }
    
    return avl_find_flight_by_number(root->right, flight_number);
}
