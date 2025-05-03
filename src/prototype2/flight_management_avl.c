/*
 * Flight Management AVL Tree Implementation (Prototype 2)
 * 
 * Sources used:
 * 1. Introduction to Algorithms by Cormen et al. - AVL tree concepts
 * 2. Data Structures and Algorithm Analysis by Mark Allen Weiss - Tree rotations
 * 3. The Art of Computer Programming by Donald Knuth - Balance factors
 */

#include <stdio.h>
#include <stdlib.h>
#include "flight_management_avl.h"

// Create a new AVL node
AVL_Node* avl_create_node(Flight flight) {
    AVL_Node* node = (AVL_Node*)malloc(sizeof(AVL_Node));
    if (node == NULL) {
        fprintf(stderr, "Memory allocation failed for AVL node\n");
        exit(1);
    }
    
    node->data = flight;
    node->left = NULL;
    node->right = NULL;
    node->height = 1;  // New node is initially at height 1
    
    return node;
}

// Get height of an AVL node
int avl_height(AVL_Node* node) {
    if (node == NULL)
        return 0;
    return node->height;
}

// Get balance factor of an AVL node
int avl_get_balance(AVL_Node* node) {
    if (node == NULL)
        return 0;
    return avl_height(node->left) - avl_height(node->right);
}

// Get maximum of two integers
static int max(int a, int b) {
    return (a > b) ? a : b;
}

// Right rotation
AVL_Node* avl_right_rotate(AVL_Node* y) {
    AVL_Node* x = y->left;
    AVL_Node* T2 = x->right;
    
    // Perform rotation
    x->right = y;
    y->left = T2;
    
    // Update heights
    y->height = max(avl_height(y->left), avl_height(y->right)) + 1;
    x->height = max(avl_height(x->left), avl_height(x->right)) + 1;
    
    // Return new root
    return x;
}

// Left rotation
AVL_Node* avl_left_rotate(AVL_Node* x) {
    AVL_Node* y = x->right;
    AVL_Node* T2 = y->left;
    
    // Perform rotation
    y->left = x;
    x->right = T2;
    
    // Update heights
    x->height = max(avl_height(x->left), avl_height(x->right)) + 1;
    y->height = max(avl_height(y->left), avl_height(y->right)) + 1;
    
    // Return new root
    return y;
}

// Insert a flight into the AVL tree
AVL_Node* avl_insert(AVL_Node* root, Flight flight) {
    // 1. Perform standard BST insert
    if (root == NULL)
        return avl_create_node(flight);
    
    if (flight.id < root->data.id)
        root->left = avl_insert(root->left, flight);
    else if (flight.id > root->data.id)
        root->right = avl_insert(root->right, flight);
    else { // Equal ids, update flight details
        root->data = flight;
        return root;
    }
    
    // 2. Update height of this ancestor node
    root->height = 1 + max(avl_height(root->left), avl_height(root->right));
    
    // 3. Get the balance factor to check if this node became unbalanced
    int balance = avl_get_balance(root);
    
    // Left Left Case
    if (balance > 1 && flight.id < root->left->data.id)
        return avl_right_rotate(root);
    
    // Right Right Case
    if (balance < -1 && flight.id > root->right->data.id)
        return avl_left_rotate(root);
    
    // Left Right Case
    if (balance > 1 && flight.id > root->left->data.id) {
        root->left = avl_left_rotate(root->left);
        return avl_right_rotate(root);
    }
    
    // Right Left Case
    if (balance < -1 && flight.id < root->right->data.id) {
        root->right = avl_right_rotate(root->right);
        return avl_left_rotate(root);
    }
    
    // No rotation needed
    return root;
}

// Find a flight in the AVL tree
Flight* avl_find_flight(AVL_Node* root, int id) {
    if (root == NULL)
        return NULL;
    
    if (id == root->data.id)
        return &(root->data);
    
    if (id < root->data.id)
        return avl_find_flight(root->left, id);
    else
        return avl_find_flight(root->right, id);
}

// Print all flights in the AVL tree (in-order traversal)
void avl_print_flights(AVL_Node* root) {
    if (root != NULL) {
        avl_print_flights(root->left);
        printf("Flight ID: %d, Number: %s, From: %s, To: %s\n", 
               root->data.id, root->data.flightNumber, 
               root->data.origin, root->data.destination);
        avl_print_flights(root->right);
    }
}

// Free AVL tree memory
void free_avl_tree(AVL_Node* root) {
    if (root != NULL) {
        free_avl_tree(root->left);
        free_avl_tree(root->right);
        free(root);
    }
}
