/*
 * Reservation Management BST Implementation (Prototype 2)
 * 
 * Sources used:
 * 1. Introduction to Algorithms by Cormen et al. - BST concepts
 * 2. Data Structures and Algorithm Analysis by Mark Allen Weiss - BST traversal
 * 3. The C Programming Language (K&R) - Memory management
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "reservation_management_bst.h"

// Format date to a readable string
static char* format_reservation_date(time_t timestamp) {
    static char buffer[26];
    struct tm* tm_info = localtime(&timestamp);
    strftime(buffer, 26, "%Y-%m-%d %H:%M", tm_info);
    return buffer;
}

// Initialize reservation BST with improved memory handling for large datasets
ReservationBST* init_reservation_bst() {
    ReservationBST* bst = (ReservationBST*)malloc(sizeof(ReservationBST));
    if (bst == NULL) {
        fprintf(stderr, "Memory allocation failed for reservation BST\n");
        return NULL;  // Return NULL instead of exit for better error handling
    }
    
    // Initialize basic properties
    bst->root = NULL;
    bst->count = 0;
    
    // Initialize index structures as NULL - will be built on demand
    bst->flight_index.by_flight = NULL;
    bst->flight_index.flight_counts = NULL;
    bst->flight_index.max_flight_id = 0;
    
    bst->passenger_index.by_passenger = NULL;
    bst->passenger_index.passenger_counts = NULL;
    bst->passenger_index.max_passenger_id = 0;
    
    bst->index_enabled = 0;
    
    return bst;
}

// Create a new reservation BST node
static ReservationBST_Node* create_reservation_node(ReservationRecord record) {
    ReservationBST_Node* node = (ReservationBST_Node*)malloc(sizeof(ReservationBST_Node));
    if (node == NULL) {
        fprintf(stderr, "Memory allocation failed for reservation BST node\n");
        exit(1);
    }
    
    node->data = record;
    node->left = NULL;
    node->right = NULL;
    
    return node;
}

// Add a reservation record to the BST
void add_reservation_bst(ReservationBST* bst, ReservationRecord record) {
    // Use an iterative approach to prevent stack overflow with large datasets
    if (bst->root == NULL) {
        bst->root = create_reservation_node(record);
        bst->count++;
        return;
    }
    
    ReservationBST_Node* current = bst->root;
    ReservationBST_Node* parent = NULL;
    int go_left = 0;
    
    // Create a composite key using flight ID, passenger ID, and seat number
    // to ensure unique entries for each seat reservation
    
    // Traverse the tree to find the insertion point
    while (current != NULL) {
        parent = current;
        
        // Use flight ID as primary key, passenger ID as secondary, seat number as tertiary
        if (record.flightId < current->data.flightId) {
            current = current->left;
            go_left = 1;
        } else if (record.flightId > current->data.flightId) {
            current = current->right;
            go_left = 0;
        } else {
            // Same flight ID, use passenger ID as secondary key
            if (record.passengerId < current->data.passengerId) {
                current = current->left;
                go_left = 1;
            } else if (record.passengerId > current->data.passengerId) {
                current = current->right;
                go_left = 0;
            } else {
                // Both flight and passenger IDs match, use seat number as tertiary key
                int seat_cmp = strcmp(record.seatNumber, current->data.seatNumber);
                if (seat_cmp < 0) {
                    current = current->left;
                    go_left = 1;
                } else if (seat_cmp > 0) {
                    current = current->right;
                    go_left = 0;
                } else {
                    // All keys match (flight ID, passenger ID, seat number) - update the record
                    current->data = record;
                    return;  // No need to increment count as it's an update
                }
            }
        }
    }
    
    // Create new node and link it to the parent
    ReservationBST_Node* new_node = create_reservation_node(record);
    if (new_node == NULL) return;  // Error handled in create_reservation_node
    
    if (go_left) {
        parent->left = new_node;
    } else {
        parent->right = new_node;
    }
    
    bst->count++;
}

// Find reservations by flight ID (iterative implementation to avoid stack overflow)
static void find_by_flight_id_iterative(ReservationBST_Node* root, int flightId, ReservationBST_Node*** results, int* count, int* capacity) {
    if (root == NULL) return;
    
    // Use a stack for iterative traversal
    #define STACK_SIZE 1024
    ReservationBST_Node* stack[STACK_SIZE];
    int top = -1;
    ReservationBST_Node* current = root;
    
    // Iterative in-order traversal
    while (current != NULL || top >= 0) {
        // Reach the leftmost node of the current subtree
        while (current != NULL) {
            // If current's flight ID is less than target, no need to go left
            if (current->data.flightId < flightId) {
                current = current->right;  // Skip to right subtree
                continue;
            }
            
            // Check stack overflow (very unlikely but just to be safe)
            if (top >= STACK_SIZE - 1) {
                fprintf(stderr, "Stack overflow in iterative traversal\n");
                return;
            }
            
            stack[++top] = current;  // Push to stack
            current = current->left;
        }
        
        // Process one node from the stack
        if (top >= 0) {
            current = stack[top--];  // Pop from stack
            
            // Process current node if it matches the flight ID
            if (current->data.flightId == flightId) {
                // Resize results array if needed
                if (*count >= *capacity) {
                    *capacity *= 2;
                    ReservationBST_Node** new_results = (ReservationBST_Node**)realloc(*results, (*capacity) * sizeof(ReservationBST_Node*));
                    if (new_results == NULL) {
                        fprintf(stderr, "Memory allocation failed when collecting reservations\n");
                        return;  // Return without exit to handle error gracefully
                    }
                    *results = new_results;
                }
                
                // Add node to results
                (*results)[(*count)++] = current;
            }
            
            // If current's flight ID is greater than target, no need to go right
            if (current->data.flightId > flightId) {
                current = NULL;  // Skip right subtree
            } else {
                current = current->right;  // Move to the right subtree
            }
        }
    }
}

// Find reservations by passenger ID (iterative implementation to avoid stack overflow)
static void find_by_passenger_id_iterative(ReservationBST_Node* root, int passengerId, ReservationBST_Node*** results, int* count, int* capacity) {
    if (root == NULL) return;
    
    // Use a stack for iterative in-order traversal
    #define PASS_STACK_SIZE 1024
    ReservationBST_Node* stack[PASS_STACK_SIZE];
    int top = -1;
    ReservationBST_Node* current = root;
    
    // Iterative in-order traversal
    while (current != NULL || top >= 0) {
        // Reach the leftmost node of the current subtree
        while (current != NULL) {
            // Check stack overflow
            if (top >= PASS_STACK_SIZE - 1) {
                fprintf(stderr, "Stack overflow in passenger search traversal\n");
                return;
            }
            
            stack[++top] = current;  // Push to stack
            current = current->left;
        }
        
        // Process one node from the stack
        if (top >= 0) {
            current = stack[top--];  // Pop from stack
            
            // Process current node if it matches the passenger ID
            if (current->data.passengerId == passengerId) {
                // Resize results array if needed
                if (*count >= *capacity) {
                    *capacity *= 2;
                    ReservationBST_Node** new_results = (ReservationBST_Node**)realloc(*results, (*capacity) * sizeof(ReservationBST_Node*));
                    if (new_results == NULL) {
                        fprintf(stderr, "Memory allocation failed when collecting reservations\n");
                        return;  // Return without exit to handle error gracefully
                    }
                    *results = new_results;
                }
                
                // Add node to results
                (*results)[(*count)++] = current;
            }
            
            current = current->right;  // Move to the right subtree
        }
    }
}

// Print all flights booked by a specific passenger
void print_passenger_flights_bst(ReservationBST* bst, AVL_Node* flights_root, int passengerId) {
    if (bst == NULL || bst->root == NULL) {
        printf("No reservations found.\n");
        return;
    }
    
    // Allocate initial results array - start with a larger capacity for large datasets
    int capacity = 32; // Start with more capacity for large datasets
    int count = 0;
    ReservationBST_Node** results = (ReservationBST_Node**)malloc(capacity * sizeof(ReservationBST_Node*));
    if (results == NULL) {
        fprintf(stderr, "Memory allocation failed for results array\n");
        return;
    }
    
    // Use the iterative passenger ID search to avoid stack overflow with large datasets
    find_by_passenger_id_iterative(bst->root, passengerId, &results, &count, &capacity);
    
    if (count == 0) {
        printf("Passenger with ID %d has no flight bookings.\n", passengerId);
    } else {
        printf("Flights booked by passenger ID %d:\n", passengerId);
        for (int i = 0; i < count; i++) {
            Flight* flight = avl_find_flight(flights_root, results[i]->data.flightId);
            if (flight != NULL) {
                printf("Flight ID: %d, Number: %s, From: %s, To: %s, Seat: %s, Booked on: %s\n", 
                       flight->id, flight->flightNumber, flight->origin, flight->destination,
                       results[i]->data.seatNumber, format_reservation_date(results[i]->data.bookingDate));
            }
        }
        printf("Total flights booked: %d\n", count);
    }
    
    free(results);
}

// Print all passengers who booked a specific flight
void print_flight_passengers_bst(ReservationBST* bst, PassengerHashTable* passengers_table, int flightId) {
    if (bst == NULL || bst->root == NULL) {
        printf("No reservations found.\n");
        return;
    }
    
    // Allocate initial results array
    int capacity = 10;
    int count = 0;
    ReservationBST_Node** results = (ReservationBST_Node**)malloc(capacity * sizeof(ReservationBST_Node*));
    if (results == NULL) {
        fprintf(stderr, "Memory allocation failed for results array\n");
        return;
    }
    
    // Find all reservations for this flight
    find_by_flight_id_iterative(bst->root, flightId, &results, &count, &capacity);
    
    if (count == 0) {
        printf("Flight with ID %d has no passenger bookings.\n", flightId);
        free(results);
        return;
    }
    
    // Find maximum passenger ID for tracking array
    int max_passenger_id = 0;
    for (int i = 0; i < count; i++) {
        if (results[i]->data.passengerId > max_passenger_id) {
            max_passenger_id = results[i]->data.passengerId;
        }
    }
    
    // Allocate tracking array for unique passenger IDs
    int* counted_passengers = (int*)calloc(max_passenger_id + 1, sizeof(int));
    if (counted_passengers == NULL) {
        fprintf(stderr, "Memory allocation failed for passenger tracking\n");
        free(results);
        return;
    }
    
    // Count unique passengers
    int unique_count = 0;
    
    printf("Passengers booked on flight ID %d:\n", flightId);
    for (int i = 0; i < count; i++) {
        int passenger_id = results[i]->data.passengerId;
        
        // Check if this passenger has already been counted
        if (counted_passengers[passenger_id] == 0) {
            Passenger* passenger = hash_find_passenger(passengers_table, passenger_id);
            if (passenger != NULL) {
                printf("Passenger ID: %d, Name: %s, Passport: %s, Seat: %s, Booked on: %s\n", 
                       passenger->id, passenger->name, passenger->passportNumber,
                       results[i]->data.seatNumber, format_reservation_date(results[i]->data.bookingDate));
                
                // Mark this passenger as counted
                counted_passengers[passenger_id] = 1;
                unique_count++;
            }
        } else {
            // This is a duplicate booking by the same passenger (multiple seats)
            Passenger* passenger = hash_find_passenger(passengers_table, passenger_id);
            if (passenger != NULL) {
                printf("  Additional seat for Passenger ID: %d, Seat: %s\n", 
                       passenger_id, results[i]->data.seatNumber);
            }
        }
    }
    
    printf("Total unique passengers: %d (Total seats booked: %d)\n", unique_count, count);
    
    // Free memory
    free(counted_passengers);
    free(results);
}

// Count the number of passengers who booked a specific flight
int count_passengers_by_flight(ReservationBST* bst, int flightId) {
    if (bst == NULL || bst->root == NULL) {
        return 0;
    }
    
    // Allocate initial results array
    int capacity = 10;
    int count = 0;
    ReservationBST_Node** results = (ReservationBST_Node**)malloc(capacity * sizeof(ReservationBST_Node*));
    if (results == NULL) {
        fprintf(stderr, "Memory allocation failed for results array\n");
        return 0;
    }
    
    // Find all reservations for this flight
    find_by_flight_id_iterative(bst->root, flightId, &results, &count, &capacity);
    
    // If no results, return 0
    if (count == 0) {
        free(results);
        return 0;
    }
    
    // Find maximum passenger ID for tracking array
    int max_passenger_id = 0;
    for (int i = 0; i < count; i++) {
        if (results[i]->data.passengerId > max_passenger_id) {
            max_passenger_id = results[i]->data.passengerId;
        }
    }
    
    // Allocate tracking array for unique passenger IDs
    int* counted_passengers = (int*)calloc(max_passenger_id + 1, sizeof(int));
    if (counted_passengers == NULL) {
        fprintf(stderr, "Memory allocation failed for passenger tracking\n");
        free(results);
        return 0;
    }
    
    // Count unique passengers
    int unique_count = 0;
    for (int i = 0; i < count; i++) {
        int passenger_id = results[i]->data.passengerId;
        
        // Check if this passenger has already been counted
        if (counted_passengers[passenger_id] == 0) {
            counted_passengers[passenger_id] = 1;
            unique_count++;
        }
    }
    
    // Free memory
    free(counted_passengers);
    free(results);
    
    return unique_count;
}

// Count the number of flights booked by a specific passenger
int count_flights_by_passenger(ReservationBST* bst, int passengerId) {
    if (bst == NULL || bst->root == NULL) {
        return 0;
    }
    
    // Allocate initial results array
    int capacity = 10;
    int count = 0;
    ReservationBST_Node** results = (ReservationBST_Node**)malloc(capacity * sizeof(ReservationBST_Node*));
    if (results == NULL) {
        fprintf(stderr, "Memory allocation failed for results array\n");
        return 0;
    }
    
    // Find all reservations for this passenger
    find_by_passenger_id_iterative(bst->root, passengerId, &results, &count, &capacity);
    
    free(results);
    return count;
}

// Free reservation BST subtree
static void free_reservation_subtree(ReservationBST_Node* node) {
    if (node != NULL) {
        free_reservation_subtree(node->left);
        free_reservation_subtree(node->right);
        free(node);
    }
}

// Free reservation BST memory
void free_reservation_bst(ReservationBST* bst) {
    if (bst != NULL) {
        free_reservation_subtree(bst->root);
        free(bst);
    }
}

// Validate that a flight has exactly its capacity in passengers
int validate_flight_capacity_bst(ReservationBST* bst, AVL_Node* flights_root, int flightId) {
    if (bst == NULL || flights_root == NULL) {
        return 0; // Invalid input parameters
    }
    
    // Find the flight by ID
    Flight* flight = avl_find_flight(flights_root, flightId);
    if (flight == NULL) {
        return 0; // Flight not found
    }
    
    // Count unique passengers for this flight
    int passenger_count = count_passengers_by_flight(bst, flightId);
    
    // Compare with capacity - now check for exact match
    if (passenger_count == flight->capacity) {
        return 1; // Valid: passenger count equals capacity
    } else if (passenger_count < flight->capacity) {
        printf("ERROR: Flight %d has only %d passengers, but should have exactly %d (capacity)\n", 
               flightId, passenger_count, flight->capacity);
        return 0; // Invalid: passenger count is below capacity
    } else {
        printf("ERROR: Flight %d has %d passengers, exceeding capacity of %d\n", 
               flightId, passenger_count, flight->capacity);
        return 0; // Invalid: passenger count exceeds capacity
    }
}

// Add a reservation with capacity validation
int add_reservation_bst_with_validation(ReservationBST* bst, AVL_Node* flights_root, ReservationRecord record) {
    if (bst == NULL || flights_root == NULL) {
        return 0; // Invalid parameters
    }
    
    // Find the flight
    Flight* flight = avl_find_flight(flights_root, record.flightId);
    if (flight == NULL) {
        printf("ERROR: Flight ID %d not found\n", record.flightId);
        return 0;
    }
    
    // Count current unique passengers for this flight
    int current_passengers = count_passengers_by_flight(bst, record.flightId);
    
    // Check if passenger is already booked (doesn't count as new)
    int passenger_already_booked = 0;
    
    // Allocate initial results array
    int capacity = 10;
    int count = 0;
    ReservationBST_Node** results = (ReservationBST_Node**)malloc(capacity * sizeof(ReservationBST_Node*));
    if (results == NULL) {
        fprintf(stderr, "Memory allocation failed for results array\n");
        return 0;
    }
    
    // Find all reservations for this flight
    find_by_flight_id_iterative(bst->root, record.flightId, &results, &count, &capacity);
    
    // If there are existing reservations, check if this passenger is already booked
    if (count > 0) {
        // Find maximum passenger ID for tracking array
        int max_passenger_id = 0;
        for (int i = 0; i < count; i++) {
            if (results[i]->data.passengerId > max_passenger_id) {
                max_passenger_id = results[i]->data.passengerId;
            }
        }
        
        if (max_passenger_id > 0) {
            // Allocate tracking array for unique passenger IDs
            int* counted_passengers = (int*)calloc(max_passenger_id + 1, sizeof(int));
            if (counted_passengers == NULL) {
                fprintf(stderr, "Memory allocation failed for passenger tracking\n");
                free(results);
                return 0;
            }
            
            // Mark existing passengers
            for (int i = 0; i < count; i++) {
                int passenger_id = results[i]->data.passengerId;
                counted_passengers[passenger_id] = 1;
            }
            
            // Check if this passenger is already booked
            if (record.passengerId <= max_passenger_id && counted_passengers[record.passengerId]) {
                passenger_already_booked = 1;
            }
            
            free(counted_passengers);
        }
    }
    
    free(results);
    
    // If this would add a new passenger, check capacity
    if (!passenger_already_booked && current_passengers >= flight->capacity) {
        printf("ERROR: Cannot add reservation. Flight %d has reached capacity of %d passengers.\n", 
               record.flightId, flight->capacity);
        return 0;
    }
    
    // Add the reservation
    add_reservation_bst(bst, record);
    return 1;
}
