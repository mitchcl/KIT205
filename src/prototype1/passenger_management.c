#include <stdio.h>
#include <stdlib.h>
#include "passenger_management.h"

// Create a new linked list node for a passenger
LL_Node* create_passenger_node(Passenger passenger) {
    LL_Node* new_node = (LL_Node*)malloc(sizeof(LL_Node));
    if (new_node == NULL) {
        fprintf(stderr, "Memory allocation failed for linked list node\n");  // is my pc shit?
        exit(1);
    }
    
    new_node->data = passenger;
    new_node->next = NULL;
    
    return new_node;
}

// Insert a passenger into the linked list
LL_Node* insert_passenger(LL_Node* head, Passenger passenger) {
    // Create a new node for the passenger
    LL_Node* new_node = create_passenger_node(passenger);
    
    // If list is empty, make the new node the head
    if (head == NULL) {
        return new_node;
    }
    
    // Find the correct position to insert (keep sorted by ID)
    LL_Node* current = head;
    LL_Node* prev = NULL;
    
    while (current != NULL && current->data.id < passenger.id) {
        prev = current;
        current = current->next;
    }
    
    // Insert at beginning
    if (prev == NULL) {
        new_node->next = head;
        return new_node;
    }
    
    // Insert in the middle or at the end
    prev->next = new_node;
    new_node->next = current;
    
    return head;
}

// Find a passenger in the linked list
Passenger* find_passenger(LL_Node* head, int id) {
    LL_Node* current = head;
    
    while (current != NULL) {
        if (current->data.id == id) {
            return &(current->data);
        }
        current = current->next;
    }
    
    return NULL;
}

// Print all passengers in the linked list
void print_passengers(LL_Node* head) {
    LL_Node* current = head;
    
    while (current != NULL) {
        printf("Passenger ID: %d, Name: %s\n", current->data.id, current->data.name);
        current = current->next;
    }
}

// Free linked list memory
void free_list(LL_Node* head) {
    LL_Node* current = head;
    LL_Node* next;
    
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
}
