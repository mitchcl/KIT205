/*
 * Passenger Management Linked List Implementation (Prototype 1)
 * 
 * Sources used:
 * 1. The C Programming Language (K&R) - Linked list concepts
 * 2. Data Structures and Algorithm Analysis by Mark Allen Weiss - List operations
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "passenger_management.h"

// Create a new linked list node for a passenger
LL_Node* create_passenger_node(Passenger passenger) {
    LL_Node* new_node = (LL_Node*)malloc(sizeof(LL_Node));
    if (new_node == NULL) {
        fprintf(stderr, "Memory allocation failed for passenger node\n");
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
    
    // If passenger ID is smaller than the head, insert at beginning
    if (passenger.id < head->data.id) {
        new_node->next = head;
        return new_node;
    }
    
    // Find the correct position to insert
    LL_Node* current = head;
    while (current->next != NULL && current->next->data.id < passenger.id) {
        current = current->next;
    }
    
    // If passenger with same ID exists, update the data
    if (current->next != NULL && current->next->data.id == passenger.id) {
        current->next->data = passenger;
    } else {
        // Insert the new node after the current node
        new_node->next = current->next;
        current->next = new_node;
    }
    
    return head;
}

// Find a passenger in the linked list
Passenger* find_passenger(LL_Node* head, int id) {
    LL_Node* current = head;
    
    // Traverse the list
    while (current != NULL) {
        if (current->data.id == id) {
            return &(current->data);
        }
        current = current->next;
    }
    
    // Not found
    return NULL;
}

// Print all passengers in the linked list
void print_passengers(LL_Node* head) {
    LL_Node* current = head;
    
    while (current != NULL) {
        printf("Passenger ID: %d, Name: %s, Passport: %s\n", 
               current->data.id, current->data.name, current->data.passportNumber);
        current = current->next;
    }
}

// Remove a passenger from the linked list
LL_Node* remove_passenger(LL_Node* head, int id) {
    // If list is empty
    if (head == NULL) {
        return NULL;
    }
    
    // If the head node itself holds the key to be deleted
    if (head->data.id == id) {
        LL_Node* temp = head;
        head = head->next;
        free(temp);
        return head;
    }
    
    // Search for the key to be deleted
    LL_Node* current = head;
    while (current->next != NULL && current->next->data.id != id) {
        current = current->next;
    }
    
    // If the key was not present
    if (current->next == NULL) {
        return head;
    }
    
    // Remove the node
    LL_Node* temp = current->next;
    current->next = temp->next;
    free(temp);
    
    return head;
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
