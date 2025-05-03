#define _GNU_SOURCE  // For strcasestr function
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../airline_types.h"
#include "passenger_management_hash.h"

// Search for a passenger by name in hash table
Passenger* hash_find_passenger_by_name(PassengerHashTable* table, const char* name) {
    if (table == NULL) {
        return NULL;
    }
    
    // Linear search through the table for name matches
    for (int i = 0; i < table->size; i++) {
        if (table->table[i].occupied == 1) {
            // Case-insensitive substring search
            if (strcasestr(table->table[i].data.name, name) != NULL) {
                return &(table->table[i].data);
            }
            
            // Check chained entries
            HashEntry* current = table->table[i].next;
            while (current != NULL) {
                if (strcasestr(current->data.name, name) != NULL) {
                    return &(current->data);
                }
                current = current->next;
            }
        }
    }
    
    return NULL;
}
