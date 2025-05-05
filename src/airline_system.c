#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "airline_types.h"
#include "prototype1/flight_management.h"
#include "prototype1/passenger_management.h"
#include "prototype1/reservation_management.h"
#include "prototype1/flight_search.h"
#include "prototype1/passenger_search.h"
#include "prototype2/flight_management_avl.h"
#include "prototype2/passenger_management_hash.h"
#include "prototype2/reservation_management_bst.h"
#include "prototype2/flight_search_avl.h"
#include "prototype2/passenger_search_hash.h"
#include "file_loader.h"
#include "data_generator.h"
#include "test_framework.h"

// Global variables to store data structures for both prototypes
// Prototype 1
BST_Node* p1_flights_root = NULL;
LL_Node* p1_passengers_head = NULL;
ReservationArray* p1_reservations_array = NULL;

// Prototype 2
AVL_Node* p2_flights_root = NULL;
PassengerHashTable* p2_passengers_table = NULL;
ReservationBST* p2_reservations_bst = NULL;

// Global variables to store loaded data
Flight* flights = NULL;
Passenger* passengers = NULL;
ReservationRecord* reservations = NULL;
int flight_count = 0;
int passenger_count = 0;
int reservation_count = 0;

// Helper function prototypes
void display_menu(int active_prototype);
int check_data_loaded(int data_loaded);
void build_data_structures();
void display_data_summary();

// Format timestamp for display
char* format_timestamp_display(time_t timestamp) {
    struct tm* tm_info = localtime(&timestamp);
    static char buffer[26];
    strftime(buffer, 26, "%Y-%m-%d %H:%M", tm_info);
    return buffer;
}

// Helper function to check if data is loaded
int check_data_loaded(int data_loaded) {
    if (!data_loaded) {
        printf("\nERROR: No data loaded. Please load data first (options 1 or 2).\n");
        return 0;
    }
    return 1;
}

// Function to run prototype 1 (BST for flights, linked list for passengers, array for reservations)
void run_prototype1(Flight* flights, int flight_count, Passenger* passengers, int passenger_count, ReservationRecord* reservations, int reservation_count) {
    printf("\n===== Running Prototype 1 =====\n");
    printf("Data structures: BST for flights, Linked List for passengers, Array for reservations\n\n");
    
    clock_t start, end;
    double execution_time;
    
    // Build data structures
    start = clock();
    
    BST_Node* flights_root = NULL;
    LL_Node* passengers_head = NULL;
    ReservationArray* reservations_array = NULL;
    
    // Flights as BST
    for (int i = 0; i < flight_count; i++) {
        flights_root = insert(flights_root, flights[i]);
    }
    
    // Passengers as linked list
    for (int i = 0; i < passenger_count; i++) {
        passengers_head = insert_passenger(passengers_head, passengers[i]);
    }
    
    // Reservations as array
    reservations_array = init_reservations(reservation_count);
    for (int i = 0; i < reservation_count; i++) {
        add_reservation(reservations_array, reservations[i]);
    }
    
    end = clock();
    execution_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Prototype 1 - Structure Building Time: %f seconds\n", execution_time);
    
    // Select example IDs for demonstration
    int passenger_id = 1;
    int flight_id = 1;
    
    // Demo operations with timing
    printf("\nPrototype 1 - Sample Operations:\n");
    
    // 1. Print all flights (ordered)
    printf("\nAll Flights (ordered by ID):\n");
    start = clock();
    print_flights(flights_root);
    end = clock();
    execution_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("\nTime to print all flights: %f seconds\n", execution_time);
    
    // 2. Print all passengers (ordered)
    printf("\nAll Passengers (ordered by insertion):\n");
    print_passengers(passengers_head);
    
    // 3. Find flights booked by a specific passenger
    printf("\nFlights booked by Passenger ID %d:\n", passenger_id);
    start = clock();
    print_passenger_flights(reservations_array, flights_root, passenger_id);
    end = clock();
    execution_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("\nTime to find flights by passenger: %f seconds\n", execution_time);
    
    // 4. Find passengers who booked a specific flight
    printf("\nPassengers who booked Flight ID %d:\n", flight_id);
    start = clock();
    print_flight_passengers(reservations_array, passengers_head, flight_id);
    end = clock();
    execution_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("\nTime to find passengers by flight: %f seconds\n", execution_time);
    
    // Clean up
    free_tree(flights_root);
    free_list(passengers_head);
    free_reservations(reservations_array);
}

// Function to run prototype 2 (AVL for flights, hash table for passengers, BST for reservations)
void run_prototype2(Flight* flights, int flight_count, Passenger* passengers, int passenger_count, ReservationRecord* reservations, int reservation_count) {
    printf("\n===== Running Prototype 2 =====\n");
    printf("Data structures: AVL Tree for flights, Hash Table for passengers, BST for reservations\n\n");
    
    clock_t start, end;
    double execution_time;
    
    // Build data structures
    start = clock();
    
    AVL_Node* flights_root = NULL;
    PassengerHashTable* passengers_table = NULL;
    ReservationBST* reservations_bst = NULL;
    
    // Flights as AVL tree
    for (int i = 0; i < flight_count; i++) {
        flights_root = avl_insert(flights_root, flights[i]);
    }
    
    // Passengers as hash table
    passengers_table = init_hash_table(passenger_count * 2); // Double size for less collisions
    for (int i = 0; i < passenger_count; i++) {
        hash_insert_passenger(passengers_table, passengers[i]);
    }
    
    // Reservations as BST
    reservations_bst = init_reservation_bst();
    for (int i = 0; i < reservation_count; i++) {
        add_reservation_bst(reservations_bst, reservations[i]);
    }
    
    end = clock();
    execution_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Prototype 2 - Structure Building Time: %f seconds\n", execution_time);
    
    // Select example IDs for demonstration
    int passenger_id = 1;
    int flight_id = 1;
    
    // Demo operations with timing
    printf("\nPrototype 2 - Sample Operations:\n");
    
    // 1. Print all flights (ordered)
    printf("\nAll Flights (ordered by ID):\n");
    start = clock();
    avl_print_flights(flights_root);
    end = clock();
    execution_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("\nTime to print all flights: %f seconds\n", execution_time);
    
    // 2. Print all passengers
    printf("\nAll Passengers:\n");
    print_hash_passengers(passengers_table);
    
    // 3. Find flights booked by a specific passenger
    printf("\nFlights booked by Passenger ID %d:\n", passenger_id);
    start = clock();
    print_passenger_flights_bst(reservations_bst, flights_root, passenger_id);
    end = clock();
    execution_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("\nTime to find flights by passenger: %f seconds\n", execution_time);
    
    // 4. Find passengers who booked a specific flight
    printf("\nPassengers who booked Flight ID %d:\n", flight_id);
    start = clock();
    print_flight_passengers_bst(reservations_bst, passengers_table, flight_id);
    end = clock();
    execution_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("\nTime to find passengers by flight: %f seconds\n", execution_time);
    
    // Clean up
    free_avl_tree(flights_root);
    free_hash_table(passengers_table);
    free_reservation_bst(reservations_bst);
}

// Helper function to build data structures for both prototypes
void build_data_structures() {
    // Free any existing data structures
    if (p1_flights_root) free_tree(p1_flights_root);
    if (p1_passengers_head) free_list(p1_passengers_head);
    if (p1_reservations_array) free_reservations(p1_reservations_array);
    if (p2_flights_root) free_avl_tree(p2_flights_root);
    if (p2_passengers_table) free_hash_table(p2_passengers_table);
    if (p2_reservations_bst) free_reservation_bst(p2_reservations_bst);
    
    // Reset data structures
    p1_flights_root = NULL;
    p1_passengers_head = NULL;
    p1_reservations_array = NULL;
    p2_flights_root = NULL;
    p2_passengers_table = NULL;
    p2_reservations_bst = NULL;
    
    clock_t start, end;
    
    // Prototype 1: BST for flights, Linked List for passengers, Array for reservations
    start = clock();
    
    // Flights as BST
    for (int i = 0; i < flight_count; i++) {
        p1_flights_root = insert(p1_flights_root, flights[i]);
    }
    
    // Passengers as linked list
    for (int i = 0; i < passenger_count; i++) {
        p1_passengers_head = insert_passenger(p1_passengers_head, passengers[i]);
    }
    
    // Reservations as array
    p1_reservations_array = init_reservations(reservation_count);
    for (int i = 0; i < reservation_count; i++) {
        add_reservation(p1_reservations_array, reservations[i]);
    }
    
    end = clock();
    double proto1_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    
    // Prototype 2: AVL Tree for flights, Hash Table for passengers, BST for reservations
    start = clock();
    
    // Flights as AVL tree
    for (int i = 0; i < flight_count; i++) {
        p2_flights_root = avl_insert(p2_flights_root, flights[i]);
    }
    
    // Passengers as hash table
    p2_passengers_table = init_hash_table(passenger_count * 2); // Double size for less collisions
    if (!p2_passengers_table) {
        p2_passengers_table = init_hash_table(100000); // Try with a smaller size if memory allocation fails
    }
    
    if (p2_passengers_table) {
        const int BATCH_SIZE = 10000;
        for (int i = 0; i < passenger_count; i += BATCH_SIZE) {
            int end_idx = i + BATCH_SIZE;
            if (end_idx > passenger_count) end_idx = passenger_count;
            
            for (int j = i; j < end_idx; j++) {
                hash_insert_passenger(p2_passengers_table, passengers[j]);
            }
        }
    }
    
    // Reservations as BST
    p2_reservations_bst = init_reservation_bst();
    if (p2_reservations_bst) {
        const int RES_BATCH_SIZE = 50000;
        for (int i = 0; i < reservation_count; i += RES_BATCH_SIZE) {
            int end_idx = i + RES_BATCH_SIZE;
            if (end_idx > reservation_count) end_idx = reservation_count;
            
            for (int j = i; j < end_idx; j++) {
                add_reservation_bst(p2_reservations_bst, reservations[j]);
            }
        }
    }
    
    end = clock();
    double proto2_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    
    printf("\nData structures built successfully.\n");
    printf("Prototype 1 build time: %f seconds\n", proto1_time);
    printf("Prototype 2 build time: %f seconds\n", proto2_time);
}

// Function to display a summary of the loaded data
void display_data_summary() {
    printf("\n========================================\n");
    printf("Airline Reservation System Data Summary:\n");
    printf("========================================\n");
    printf("Number of flights: %d\n", flight_count);
    printf("Number of passengers: %d\n", passenger_count);
    printf("Number of reservations: %d\n", reservation_count);
    printf("========================================\n");
    
    // Show example data if available
    if (flight_count > 0) {
        Flight example_flight = flights[0];
        printf("\nExample Flight:\n");
        printf("ID: %d\n", example_flight.id);
        printf("Flight Number: %s\n", example_flight.flightNumber);
        printf("Origin: %s\n", example_flight.origin);
        printf("Destination: %s\n", example_flight.destination);
        printf("Departure Time: %s\n", format_timestamp_display(example_flight.departureTime));
        printf("Capacity: %d\n", example_flight.capacity);
    }
    
    if (passenger_count > 0) {
        Passenger example_passenger = passengers[0];
        printf("\nExample Passenger:\n");
        printf("ID: %d\n", example_passenger.id);
        printf("Name: %s\n", example_passenger.name);
        printf("Passport Number: %s\n", example_passenger.passportNumber);
    }
    
    if (reservation_count > 0) {
        ReservationRecord example_reservation = reservations[0];
        printf("\nExample Reservation:\n");
        printf("Flight ID: %d\n", example_reservation.flightId);
        printf("Passenger ID: %d\n", example_reservation.passengerId);
        printf("Booking Date: %s\n", format_timestamp_display(example_reservation.bookingDate));
        printf("Seat Number: %s\n", example_reservation.seatNumber);
    }
}

// Function to display the menu options
void display_menu(int active_prototype) {
    printf("\n==============================================\n");
    printf("      AIRLINE RESERVATION SYSTEM MENU\n");
    printf("==============================================\n");
    printf("Data Operations:\n");
    printf("  1. Load data from CSV files\n");
    printf("  2. Generate synthetic test data\n");
    printf("\nView Records:\n");
    printf("  3. List all flights (ordered)\n");
    printf("  4. List all passengers (ordered)\n");
    printf("\nSearch Operations:\n");
    printf("  5. Search for a flight by ID\n");
    printf("  6. Search for a flight by flight number\n");
    printf("  7. Search for a passenger by ID\n");
    printf("  8. Search for a passenger by name\n");
    printf("\nRelationship Queries:\n");
    printf("  9. Find flights booked by a specific passenger\n");
    printf(" 10. Find passengers who booked a specific flight\n");
    printf("\nPerformance Testing:\n");
    printf(" 11. Run performance comparison between prototypes\n");
    printf(" 12. Switch active prototype (current: Prototype %d)\n", active_prototype);
    printf("\n 13. Exit program\n");
    printf("==============================================\n");
    printf("Enter your choice (1-13): ");
}

// Function to search for a flight by ID
Flight* search_flight_by_id(void* flights_root, int flight_id, int prototype) {
    if (prototype == 1) {
        return find_flight((BST_Node*)flights_root, flight_id);
    } else {
        return avl_find_flight((AVL_Node*)flights_root, flight_id);
    }
}

// Function to search for a passenger by ID
Passenger* search_passenger_by_id(void* passengers_data, int passenger_id, int prototype) {
    if (prototype == 1) {
        return find_passenger((LL_Node*)passengers_data, passenger_id);
    } else {
        return hash_find_passenger((PassengerHashTable*)passengers_data, passenger_id);
    }
}

// Helper function to display flight details
void display_flight_details(Flight* flight) {
    if (flight) {
        printf("\nFlight Found:\n");
        printf("ID: %d\n", flight->id);
        printf("Flight Number: %s\n", flight->flightNumber);
        printf("Origin: %s\n", flight->origin);
        printf("Destination: %s\n", flight->destination);
        printf("Departure Time: %s\n", format_timestamp_display(flight->departureTime));
        printf("Capacity: %d\n", flight->capacity);
    }
}

// Helper function to display passenger details
void display_passenger_details(Passenger* passenger) {
    if (passenger) {
        printf("\nPassenger Found:\n");
        printf("ID: %d\n", passenger->id);
        printf("Name: %s\n", passenger->name);
        printf("Passport Number: %s\n", passenger->passportNumber);
    }
}

// Helper function to free all data structures and loaded data
void cleanup_resources() {
    if (flights) free(flights);
    if (passengers) free(passengers);
    if (reservations) free(reservations);
    if (p1_flights_root) free_tree(p1_flights_root);
    if (p1_passengers_head) free_list(p1_passengers_head);
    if (p1_reservations_array) free_reservations(p1_reservations_array);
    if (p2_flights_root) free_avl_tree(p2_flights_root);
    if (p2_passengers_table) free_hash_table(p2_passengers_table);
    if (p2_reservations_bst) free_reservation_bst(p2_reservations_bst);
}

// Main function
int main(int argc, char* argv[]) {
    // Parse command line arguments
    int skip_tests = 0;
    
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--skip-tests") == 0) {
            skip_tests = 1;
        }
    }
    
    // Run tests first unless skipped
    if (!skip_tests) {
        run_all_tests();
    }
    
    // Default prototype to use (1 = BST/LinkedList/Array, 2 = AVL/HashTable/BST)
    int active_prototype = 1;
    
    // Main menu loop
    int exit_program = 0;
    int data_loaded = 0;
    
    while (!exit_program) {
        // Display active prototype in the menu
        display_menu(active_prototype);
        
        int choice;
        scanf("%d", &choice);
        
        // Clear input buffer
        int c;
        while ((c = getchar()) != '\n' && c != EOF) {}
        
        // Variables for search operations
        int id;
        char search_term[MAX_LINE_LENGTH];
        Flight* flight = NULL;
        Passenger* passenger = NULL;
        
        switch (choice) {
            case 1: // Load data from CSV files
                printf("\n======================================\n");
                printf("Loading Data from CSV Files\n");
                printf("======================================\n");
                
                // Define file paths
                char flights_file[256] = "data/flights.csv";
                char passengers_file[256] = "data/passengers.csv";
                char reservations_file[256] = "data/reservations.csv";
                
                // Check if large dataset files exist and prompt user
                int use_large_files = 0;
                FILE* test_file = fopen("data/flights_large.csv", "r");
                if (test_file != NULL) {
                    fclose(test_file);
                    printf("Large dataset files detected. Use large files? (1 for yes, 0 for no): ");
                    scanf("%d", &use_large_files);
                    
                    if (use_large_files) {
                        strcpy(flights_file, "data/flights_large.csv");
                        strcpy(passengers_file, "data/passengers_large.csv");
                        strcpy(reservations_file, "data/reservations_large.csv");
                    }
                }
                
                // Free any existing data
                if (flights) free(flights);
                if (passengers) free(passengers);
                if (reservations) free(reservations);
                
                // Load the data
                flights = load_flights(flights_file, &flight_count);
                passengers = load_passengers(passengers_file, &passenger_count);
                reservations = load_reservations(reservations_file, &reservation_count);
                
                if (flights == NULL || passengers == NULL || reservations == NULL) {
                    printf("Error loading data files. Please check that the CSV files exist.\n");
                    break;
                }
                
                // Build data structures for both prototypes
                printf("\nBuilding data structures...\n");
                build_data_structures();
                data_loaded = 1;
                
                // Display data summary
                display_data_summary();
                break;
                
            case 2: // Generate synthetic test data
                printf("\n======================================\n");
                printf("Generating Test Data\n");
                printf("======================================\n");
                printf("Select dataset size:\n");
                printf("1. Small (100 flights, 500 passengers, ~1000 reservations)\n");
                printf("2. Medium (1,000 flights, 5,000 passengers, ~10,000 reservations)\n");
                printf("3. Large (10,000 flights, 50,000 passengers, ~100,000 reservations)\n");
                printf("4. Huge (100,000 flights, 500,000 passengers, ~1,000,000 reservations)\n");
                printf("Enter choice (1-4): ");
                
                int data_size_choice;
                scanf("%d", &data_size_choice);
                
                int dataset_size = 100; // Default size
                switch(data_size_choice) {
                    case 1: dataset_size = 100; break;
                    case 2: dataset_size = 1000; break;
                    case 3: dataset_size = 10000; break;
                    case 4: dataset_size = 100000; break;
                    default: printf("Invalid choice, using default (small)\n"); dataset_size = 100;
                }
                
                // Free any existing data
                if (flights) free(flights);
                if (passengers) free(passengers);
                if (reservations) free(reservations);
                
                // Generate the data
                printf("\nGenerating %d flights, %d passengers, and approximately %d reservations...\n", 
                    dataset_size, dataset_size * 5, dataset_size * 10);
                
                flights = generate_flights(dataset_size);
                passengers = generate_passengers(dataset_size * 5);
                reservations = generate_reservations(dataset_size * 10, dataset_size, dataset_size * 5);
                
                flight_count = dataset_size;
                passenger_count = dataset_size * 5;
                reservation_count = dataset_size * 10;
                
                // Build data structures for both prototypes
                printf("\nBuilding data structures...\n");
                build_data_structures();
                data_loaded = 1;
                
                // Display data summary
                display_data_summary();
                break;
                
            case 3: // List all flights (ordered)
                if (!check_data_loaded(data_loaded)) break;
                
                printf("\n======== Using Prototype %d ========\n", active_prototype);
                printf("\nAll Flights (ordered by ID):\n");
                if (active_prototype == 1) {
                    print_flights(p1_flights_root);
                } else {
                    avl_print_flights(p2_flights_root);
                }
                break;
                
            case 4: // List all passengers (ordered)
                if (!check_data_loaded(data_loaded)) break;
                
                printf("\n======== Using Prototype %d ========\n", active_prototype);
                printf("\nAll Passengers:\n");
                if (active_prototype == 1) {
                    print_passengers(p1_passengers_head);
                } else {
                    print_hash_passengers(p2_passengers_table);
                }
                break;
                
            case 5: // Search for a flight by ID
                if (!check_data_loaded(data_loaded)) break;
                
                printf("\n======== Using Prototype %d ========\n", active_prototype);
                printf("Enter flight ID: ");
                scanf("%d", &id);
                
                if (active_prototype == 1) {
                    flight = search_flight_by_id(p1_flights_root, id, 1);
                } else {
                    flight = search_flight_by_id(p2_flights_root, id, 2);
                }
                
                display_flight_details(flight);
                break;
                
            case 6: // Search for a flight by flight number
                if (!check_data_loaded(data_loaded)) break;
                
                printf("\n======== Using Prototype %d ========\n", active_prototype);
                printf("Enter flight number: ");
                fgets(search_term, MAX_LINE_LENGTH, stdin);
                search_term[strcspn(search_term, "\n")] = 0; // Remove newline
                
                if (active_prototype == 1) {
                    flight = find_flight_by_number(p1_flights_root, search_term);
                } else {
                    flight = avl_find_flight_by_number(p2_flights_root, search_term);
                }
                
                display_flight_details(flight);
                break;
                
            case 7: // Search for a passenger by ID
                if (!check_data_loaded(data_loaded)) break;
                
                printf("\n======== Using Prototype %d ========\n", active_prototype);
                printf("Enter passenger ID: ");
                scanf("%d", &id);
                
                if (active_prototype == 1) {
                    passenger = search_passenger_by_id(p1_passengers_head, id, 1);
                } else {
                    passenger = search_passenger_by_id(p2_passengers_table, id, 2);
                }
                
                display_passenger_details(passenger);
                break;
                
            case 8: // Search for a passenger by name
                if (!check_data_loaded(data_loaded)) break;
                
                printf("\n======== Using Prototype %d ========\n", active_prototype);
                printf("Enter passenger name (or part of name): ");
                fgets(search_term, MAX_LINE_LENGTH, stdin);
                search_term[strcspn(search_term, "\n")] = 0; // Remove newline
                
                if (active_prototype == 1) {
                    passenger = find_passenger_by_name(p1_passengers_head, search_term);
                } else {
                    passenger = hash_find_passenger_by_name(p2_passengers_table, search_term);
                }
                
                display_passenger_details(passenger);
                break;
                
            case 9: // Find flights booked by a specific passenger
                if (!check_data_loaded(data_loaded)) break;
                
                printf("\n======== Using Prototype %d ========\n", active_prototype);
                printf("Enter passenger ID: ");
                scanf("%d", &id);
                
                printf("\nFlights booked by Passenger ID %d:\n", id);
                if (active_prototype == 1) {
                    print_passenger_flights(p1_reservations_array, p1_flights_root, id);
                } else {
                    print_passenger_flights_bst(p2_reservations_bst, p2_flights_root, id);
                }
                break;
                
            case 10: // Find passengers who booked a specific flight
                if (!check_data_loaded(data_loaded)) break;
                
                printf("\n======== Using Prototype %d ========\n", active_prototype);
                printf("Enter flight ID: ");
                scanf("%d", &id);
                
                printf("\nPassengers who booked Flight ID %d:\n", id);
                if (active_prototype == 1) {
                    print_flight_passengers(p1_reservations_array, p1_passengers_head, id);
                } else {
                    print_flight_passengers_bst(p2_reservations_bst, p2_passengers_table, id);
                }
                break;
                
            case 11: // Run performance comparison tests
                if (!check_data_loaded(data_loaded)) break;
                
                printf("\nRunning performance comparison tests...\n");
                // Toggle between prototypes to compare them
                printf("\n===== Performance Test: Prototype 1 =====\n");
                run_prototype1(flights, flight_count, passengers, passenger_count, reservations, reservation_count);
                
                printf("\n===== Performance Test: Prototype 2 =====\n");
                run_prototype2(flights, flight_count, passengers, passenger_count, reservations, reservation_count);
                break;
                
            case 12: // Switch active prototype
                active_prototype = active_prototype == 1 ? 2 : 1;
                printf("\nSwitched to Prototype %d\n", active_prototype);
                printf("Prototype 1: BST for flights, Linked List for passengers, Array for reservations\n");
                printf("Prototype 2: AVL Tree for flights, Hash Table for passengers, BST for reservations\n");
                break;
                
            case 13: // Exit program
                exit_program = 1;
                printf("\nExiting program. Cleaning up resources...\n");
                break;
                
            default:
                printf("\nInvalid choice. Please try again.\n");
                break;
        }
        
        if (!exit_program) {
            printf("\nPress Enter to continue...");
            getchar();
        }
    }
    
    // Cleanup
    cleanup_resources();
    
    printf("\n======================================\n");
    printf("Program completed successfully.\n");
    printf("======================================\n");
    
    return 0;
}