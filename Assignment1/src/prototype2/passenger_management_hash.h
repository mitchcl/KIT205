#ifndef PASSENGER_MANAGEMENT_HASH_H
#define PASSENGER_MANAGEMENT_HASH_H

#include "../airline_types.h"

// Find the next prime number greater than or equal to n
int find_next_prime(int n);

// Initialize hash table
PassengerHashTable* init_hash_table(int size);

// Hash function for passenger IDs
int hash_function(int id, int table_size);

// Insert a passenger into the hash table
void hash_insert_passenger(PassengerHashTable* table, Passenger passenger);

// Find a passenger in the hash table
Passenger* hash_find_passenger(PassengerHashTable* table, int id);

// Print all passengers in the hash table
void print_hash_passengers(PassengerHashTable* table);

// Free hash table memory
void free_hash_table(PassengerHashTable* table);

#endif
