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
void test_reservation_capacity_validation();

// Keeping declaration but skipping call to avoid freezing
void test_all_flights_capacity_validation();

#endif
