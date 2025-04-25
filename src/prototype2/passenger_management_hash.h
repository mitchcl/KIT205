#ifndef PASSENGER_MANAGEMENT_HASH_H
#define PASSENGER_MANAGEMENT_HASH_H

#include "../airline_types.h"

// Initialize hash table
PassengerHashTable* init_hash_table(int size);

// Hash function for passenger IDs
int hash_function(int id, int table_size);

void hash_insert_passenger(PassengerHashTable* table, Passenger passenger);
Passenger* hash_find_passenger(PassengerHashTable* table, int id);
void print_hash_passengers(PassengerHashTable* table);
void free_hash_table(PassengerHashTable* table);

#endif
