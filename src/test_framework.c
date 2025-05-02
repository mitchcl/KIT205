#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include "test_framework.h"
#include "airline_types.h"
#include "data_generator.h" 
#include "prototype1/flight_management.h"
#include "prototype1/passenger_management.h"
#include "prototype1/reservation_management.h"
#include "prototype2/flight_management_avl.h"
#include "prototype2/passenger_management_hash.h"
#include "prototype2/reservation_management_bst.h"

// Helper function to check test results
void report_test_result(const char* test_name, int result) {
    if (result) {
        printf("PASS: %s\n", test_name);
    } else {
        printf("FAIL: %s\n", test_name);
    }
}

// Test BST operations (Prototype 1)
void test_bst_operations() {
    printf("\nTesting BST Operations:\n");
    
    // Create test data
    Flight flights[] = {
        {5, "AA555", "New York", "London", 0, 200},
        {3, "BA333", "London", "Paris", 0, 180},
        {7, "DL777", "Los Angeles", "Tokyo", 0, 300},
        {1, "UA111", "Chicago", "Miami", 0, 220},
        {9, "LH999", "Berlin", "Rome", 0, 250}
    };
    
    // Insert flights into BST
    BST_Node* root = NULL;
    for (int i = 0; i < 5; i++) {
        root = insert(root, flights[i]);
    }
    
    // Test find operation
    Flight* found = find_flight(root, 3);
    assert(found != NULL);
    assert(found->id == 3);
    report_test_result("Find existing flight", 1);
    
    found = find_flight(root, 100);
    assert(found == NULL);
    report_test_result("Find non-existing flight", 1);
    
    // Clean up
    free_tree(root);
}

// Test Linked List operations (Prototype 1)
void test_linked_list_operations() {
    printf("\nTesting Linked List Operations:\n");
    
    // Create test data and run tests
    // Similar to test_bst_operations
}

// Test Reservation Array operations (Prototype 1)
void test_reservation_array_operations() {
    printf("\nTesting Reservation Array Operations:\n");
}

// Test AVL Tree operations (Prototype 2)
void test_avl_operations() {
    printf("\nTesting AVL Tree Operations:\n");
    
    // Create test data
    Flight flights[] = {
        {5, "AA555", "New York", "London", 0, 200},
        {3, "BA333", "London", "Paris", 0, 180},
        {7, "DL777", "Los Angeles", "Tokyo", 0, 300},
        {1, "UA111", "Chicago", "Miami", 0, 220},
        {9, "LH999", "Berlin", "Rome", 0, 250}
    };
    
    // Insert flights into AVL
    AVL_Node* root = NULL;
    for (int i = 0; i < 5; i++) {
        root = avl_insert(root, flights[i]);
    }
    
    // Test balance factor
    assert(abs(avl_get_balance(root)) <= 1);
    report_test_result("AVL balanced after inserts", 1);
    
    // Test find operation
    Flight* found = avl_find_flight(root, 3);
    assert(found != NULL);
    assert(found->id == 3);
    report_test_result("Find existing flight in AVL", 1);
    free_avl_tree(root);
}

// Test Hash Table operations (Prototype 2)
void test_hash_table_operations() {
    printf("\nTesting Hash Table Operations:\n");
}

// Test Reservation BST operations (Prototype 2)
void test_reservation_bst_operations() {
    printf("\nTesting Reservation BST Operations:\n");
}

// Test Prototype 1 Integration
void test_prototype1_integration() {
    printf("\nTesting Prototype 1 Integration:\n");
    
    // Create integrated test case with flights, passengers and reservations
}

// Test Prototype 2 Integration
void test_prototype2_integration() {
    printf("\nTesting Prototype 2 Integration:\n");
    
    // Create integrated test case with flights, passengers and reservations
}

// Test flight capacity validation
void test_flight_capacity_validation() {
    printf("\nTesting Flight Capacity Validation:\n");
    
    // Create test data and run tests
}

// Run all unit tests
void run_all_tests() {
    printf("Running all tests...\n");
    
    // Set random seed
    srand(time(NULL));
    
    test_bst_operations();
    test_linked_list_operations();
    test_reservation_array_operations();
    test_avl_operations();
    test_hash_table_operations();
    test_reservation_bst_operations();
    test_prototype1_integration();
    test_prototype2_integration();
    test_flight_capacity_validation();
    
    printf("\nAll tests completed.\n");
}

// Measure execution time of a function
double measure_execution_time(void (*func)(void*), void* arg) {
    clock_t start, end;
    double cpu_time_used;
    
    start = clock();
    func(arg);
    end = clock();
    
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    return cpu_time_used;
}

// Function to build BST for flights (prototype 1)
void build_bst(void* arg) {
    PerformanceTestArgs* args = (PerformanceTestArgs*)arg;
    Flight* flights = (Flight*)args->p1;
    int flight_count = *((int*)args->p2);
    BST_Node** root_ptr = (BST_Node**)args->p3;
    
    *root_ptr = NULL;
    for (int i = 0; i < flight_count; i++) {
        *root_ptr = insert(*root_ptr, flights[i]);
    }
}

// Function to build AVL tree for flights (prototype 2)
void build_avl(void* arg) {
    PerformanceTestArgs* args = (PerformanceTestArgs*)arg;
    Flight* flights = (Flight*)args->p1;
    int flight_count = *((int*)args->p2);
    AVL_Node** root_ptr = (AVL_Node**)args->p3;
    
    *root_ptr = NULL;
    for (int i = 0; i < flight_count; i++) {
        *root_ptr = avl_insert(*root_ptr, flights[i]);
    }
}

// Run performance tests with given dataset size
void run_performance_tests(int dataset_size) {
    printf("\n===== Performance Tests with %d Records =====\n", dataset_size);
    
    // Generate test data
    printf("Generating test data...\n");
    Flight* flights = generate_flights(dataset_size);
    Passenger* passengers = generate_passengers(dataset_size);
    ReservationRecord* reservations = generate_reservations(dataset_size * 2, dataset_size, dataset_size);
    
    if (flights == NULL || passengers == NULL || reservations == NULL) {
        printf("Failed to generate test data\n");
        return;
    }
    
    // Run performance tests
    test_prototype1_performance(flights, dataset_size, passengers, dataset_size, reservations, dataset_size * 2);
    test_prototype2_performance(flights, dataset_size, passengers, dataset_size, reservations, dataset_size * 2);
    
    // Cleanup
    free(flights);
    free(passengers);
    free(reservations);
}

// Test performance of prototype 1
void test_prototype1_performance(Flight* flights, int flight_count, 
                              Passenger* passengers, int passenger_count,
                              ReservationRecord* reservations, int reservation_count) {
    printf("\n----- Prototype 1 Performance -----\n");
    
    // Test BST build time
    BST_Node* bst_root = NULL;
    PerformanceTestArgs bst_args = { flights, &flight_count, &bst_root };
    double bst_build_time = measure_execution_time(build_bst, &bst_args);
    printf("BST build time: %.6f seconds\n", bst_build_time);
    
    // Test other operations
    // ...
    
    // Cleanup
    free_tree(bst_root);
}

// Test performance of prototype 2
void test_prototype2_performance(Flight* flights, int flight_count, 
                              Passenger* passengers, int passenger_count,
                              ReservationRecord* reservations, int reservation_count) {
    printf("\n----- Prototype 2 Performance -----\n");
    
    // Test AVL build time
    AVL_Node* avl_root = NULL;
    PerformanceTestArgs avl_args = { flights, &flight_count, &avl_root };
    double avl_build_time = measure_execution_time(build_avl, &avl_args);
    printf("AVL build time: %.6f seconds\n", avl_build_time);
    
    // Test other operations
    // ...
    
    // Cleanup
    free_avl_tree(avl_root);
}
