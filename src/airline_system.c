#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "airline_types.h"
#include "prototype1/flight_management.h"
#include "prototype1/passenger_management.h"
#include "prototype1/reservation_management.h"
#include "prototype2/flight_management_avl.h"
#include "prototype2/passenger_management_hash.h"
#include "prototype2/reservation_management_bst.h"
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

int main() {
    printf("Airline Reservation System\n");
    printf("==========================\n\n");
    
    int choice;
    int active_prototype = 1;  // Default to prototype 1
    int data_loaded = 0;
    
    do {
        display_menu(active_prototype);
        scanf("%d", &choice);
        
        switch (choice) {
            case 1:
                // Switch between prototypes
                active_prototype = (active_prototype == 1) ? 2 : 1;
                printf("Switched to Prototype %d\n", active_prototype);
                break;
                
            case 2:
                // Load data from CSV files
                printf("Loading data from CSV files...\n");
                // Implementation here
                data_loaded = 1;
                break;
                
            case 3:
                // Generate random test data
                printf("Generating random test data...\n");
                // Implementation here
                data_loaded = 1;
                break;
                
            case 4:
                // Run tests
                printf("Running tests...\n");
                printf("Test functionality not implemented yet.\n");
                break;
                
            case 5:
                // Search for a flight
                if (!check_data_loaded(data_loaded)) break;
                printf("Search functionality coming soon\n");
                break;
                
            case 6:
                // Exit
                printf("Exiting...\n");
                break;
                
            default:
                printf("Invalid choice, please try again\n");
        }
    } while (choice != 6);
    
    // Cleanup
    // Implementation here
    
    return 0;
}

// Helper function implementations
void display_menu(int active_prototype) {
    printf("\n===== Airline Reservation System Menu =====\n");
    printf("Current Mode: Prototype %d\n", active_prototype);
    printf("1. Switch Prototype (currently %d)\n", active_prototype);
    printf("2. Load Data from CSV Files\n");
    printf("3. Generate Random Test Data\n");
    printf("4. Run Tests\n");
    printf("5. Search Operations\n");
    printf("6. Exit\n");
    printf("Enter your choice: ");
}

int check_data_loaded(int data_loaded) {
    if (!data_loaded) {
        printf("No data loaded. Please load or generate data first.\n");
        return 0;
    }
    return 1;
}

// Function to run performance tests with user-specified dataset size
void run_custom_performance_tests() {
    int dataset_size;
    printf("Enter dataset size for performance testing: ");
    scanf("%d", &dataset_size);
    
    if (dataset_size <= 0) {
        printf("Invalid dataset size\n");
        return;
    }
    
    run_performance_tests(dataset_size);
}
