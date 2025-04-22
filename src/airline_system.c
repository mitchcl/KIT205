#include <stdio.h>
#include <stdlib.h>
#include "airline_types.h"
#include "prototype1/flight_management.h"
#include "prototype1/passenger_management.h"
#include "prototype1/reservation_management.h"

// Global variables to store data structures
BST_Node* flights_root = NULL;
LL_Node* passengers_head = NULL;
ReservationArray* reservations_array = NULL;

// Helper functions
void display_menu() {
    printf("\n===== Airline Reservation System =====\n");
    printf("1. Add Flight\n");
    printf("2. Add Passenger\n");
    printf("3. Make Reservation\n");
    printf("4. Find Flight\n");
    printf("5. Find Passenger\n");
    printf("6. View Passenger's Flights\n");
    printf("7. View Flight's Passengers\n");
    printf("8. Exit\n");
    printf("Enter your choice: ");
}

int main() {
    printf("Airline Reservation System - Prototype 1\n");
    printf("Using BST for flights, Linked List for passengers, Array for reservations\n\n");
    
    // init reservations array
    reservations_array = init_reservations(100);
    
    // For now, just a placeholder for testing
    Flight test_flight = {1, "AA123", "New York", "London", 0, 200};
    flights_root = insert(flights_root, test_flight);
    
    Passenger test_passenger = {1, "John Doe", "AB123456"};
    passengers_head = insert_passenger(passengers_head, test_passenger);
    
    ReservationRecord test_reservation = {1, 1, 0, "12A"};
    add_reservation(reservations_array, test_reservation);
    
    printf("Test data added successfully!\n");
    printf("Printing flights...\n");
    print_flights(flights_root);
    
    printf("Printing passengers...\n");
    print_passengers(passengers_head);
    
    printf("Printing passenger's flights...\n");
    print_passenger_flights(reservations_array, flights_root, 1);
    
    // Cleanup
    free_tree(flights_root);
    free_list(passengers_head);
    free_reservations(reservations_array);
    
    return 0;
}
