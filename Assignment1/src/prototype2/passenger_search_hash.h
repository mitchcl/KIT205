#ifndef PASSENGER_SEARCH_HASH_H
#define PASSENGER_SEARCH_HASH_H

#include "../airline_types.h"

// Search for a passenger by name in hash table
Passenger* hash_find_passenger_by_name(PassengerHashTable* table, const char* name);

#endif
