#ifndef TEST_FRAMEWORK_H
#define TEST_FRAMEWORK_H

// Function to run all unit tests
void run_all_tests();

// Test functions for prototype 1
void test_bst_operations();
void test_linked_list_operations();
void test_reservation_array_operations();
void test_prototype1_integration();

// Test functions for prototype 2
void test_avl_operations();
void test_hash_table_operations();
void test_reservation_bst_operations();
void test_prototype2_integration();

// Test for capacity validation
void test_flight_capacity_validation();

#endif

// Performance testing functions
void run_performance_tests(int dataset_size);
void test_prototype1_performance(Flight* flights, int flight_count, 
                              Passenger* passengers, int passenger_count,
                              ReservationRecord* reservations, int reservation_count);
void test_prototype2_performance(Flight* flights, int flight_count, 
                              Passenger* passengers, int passenger_count,
                              ReservationRecord* reservations, int reservation_count);
double measure_execution_time(void (*func)(void*), void* arg);

typedef struct {
    void* p1;
    void* p2;
    void* p3;
} PerformanceTestArgs;

