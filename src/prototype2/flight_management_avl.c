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
static int max_value(int a, int b) {
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
    y->height = max_value(avl_height(y->left), avl_height(y->right)) + 1;
    x->height = max_value(avl_height(x->left), avl_height(x->right)) + 1;
    
    // Return new root
    return x;
}

// Left rotation
AVL_Node* avl_left_rotate(AVL_Node* x) {
    AVL_Node* y = x->right;
    AVL_Node* T2 = y->left;
    
    y->left = x;
    x->right = T2;
    
    x->height = max(avl_height(x->left), avl_height(x->right)) + 1;
    y->height = max(avl_height(y->left), avl_height(y->right)) + 1;
    
    return y;
}

// Insert a flight into the AVL tree
AVL_Node* avl_insert(AVL_Node* node, Flight flight) {
    // 1. Perform standard BST insert
    if (node == NULL)
        return avl_create_node(flight);
        
    if (flight.id < node->data.id)
        node->left = avl_insert(node->left, flight);
    else if (flight.id > node->data.id)
        node->right = avl_insert(node->right, flight);
    else {
        node->data = flight; // Equal keys not allowed in BST, update data
        return node;
    }
        
    // 2. Update height of this ancestor node
    node->height = 1 + max_value(avl_height(node->left), avl_height(node->right));
    
    // 3. Get the balance factor
    int balance = avl_get_balance(node);
    
    // Left Left Case
    if (balance > 1 && flight.id < node->left->data.id)
        return avl_right_rotate(node);
        
    // Right Right Case
    if (balance < -1 && flight.id > node->right->data.id)
        return avl_left_rotate(node);
        
    // Left Right Case
    if (balance > 1 && flight.id > node->left->data.id) {
        node->left = avl_left_rotate(node->left);
        return avl_right_rotate(node);
    }
    
    // Right Left Case
    if (balance < -1 && flight.id < node->right->data.id) {
        node->right = avl_right_rotate(node->right);
        return avl_left_rotate(node);
    }
    
    // return the (unchanged) node pointer
    return node;
}

// Find a flight in the AVL tree
Flight* avl_find_flight(AVL_Node* root, int id) {
    // Base cases: root is NULL or flight is at root
    if (root == NULL || root->data.id == id) {
        return root == NULL ? NULL : &(root->data);
    }
    
    // Recursive search in the appropriate subtree
    if (id < root->data.id) {
        return avl_find_flight(root->left, id);
    } else {
        return avl_find_flight(root->right, id);
    }
}

// Print all flights in the AVL tree (in-order traversal)
void avl_print_flights(AVL_Node* root) {
    if (root != NULL) {
        avl_print_flights(root->left);
        printf("Flight ID: %d, Number: %s (Balance: %d)\n", 
               root->data.id, root->data.flightNumber, avl_get_balance(root));
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
