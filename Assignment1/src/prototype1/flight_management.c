/*
 * Flight Management BST Implementation (Prototype 1)
 * 
 * Sources used:
 * 1. Introduction to Algorithms by Cormen et al. - BST concepts
 * 2. Data Structures and Algorithm Analysis in C by Mark Allen Weiss - Tree traversals
 */

#include <stdio.h>
#include <stdlib.h>
#include "flight_management.h"

// Create a new BST node
BST_Node* create_node(Flight flight) {
    BST_Node* node = (BST_Node*)malloc(sizeof(BST_Node));
    if (node == NULL) {
        fprintf(stderr, "Memory allocation failed for BST node\n");
        exit(1);
    }
    
    node->data = flight;
    node->left = NULL;
    node->right = NULL;
    
    return node;
}

// Insert a flight into the BST
BST_Node* insert(BST_Node* root, Flight flight) {
    // Insert at this position if empty
    if (root == NULL) {
        return create_node(flight);
    }
    
    // Decide where to insert based on ID
    if (flight.id < root->data.id) {
        root->left = insert(root->left, flight);
    } else if (flight.id > root->data.id) {
        root->right = insert(root->right, flight);
    } else {
        // ID already exists, update the flight data
        root->data = flight;
    }
    
    return root;
}

// Find a flight in the BST
Flight* find_flight(BST_Node* root, int id) {
    // Base cases: root is NULL or flight is at root
    if (root == NULL || root->data.id == id) {
        return root == NULL ? NULL : &(root->data);
    }
    
    // Recursive search in the appropriate subtree
    if (id < root->data.id) {
        return find_flight(root->left, id);
    } else {
        return find_flight(root->right, id);
    }
}

// Print all flights in the BST (in-order traversal)
void print_flights(BST_Node* root) {
    if (root != NULL) {
        print_flights(root->left);
        printf("Flight ID: %d, Number: %s, From: %s, To: %s\n", 
               root->data.id, root->data.flightNumber, 
               root->data.origin, root->data.destination);
        print_flights(root->right);
    }
}

// Free BST memory
void free_tree(BST_Node* root) {
    if (root != NULL) {
        free_tree(root->left);
        free_tree(root->right);
        free(root);
    }
}
