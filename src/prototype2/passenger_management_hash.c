/*
 * Passenger Management Hash Table Implementation (Prototype 2)
 * 
 * Sources used:
 * 1. Introduction to Algorithms by Cormen et al. - Hash table concepts
 * 2. The Art of Computer Programming by Donald Knuth - Hashing techniques
 * 3. Algorithms in C by Robert Sedgewick - Collision resolution methods
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "passenger_management_hash.h"

// Check if a number is prime
static int is_prime(int n) {
    if (n <= 1) return 0;
    if (n <= 3) return 1;
    if (n % 2 == 0 || n % 3 == 0) return 0;
    
    for (int i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return 0;
    }
    return 1;
}

// Find the next prime number greater than or equal to n
int find_next_prime(int n) {
    if (n <= 2) return 2;
    
    // Start with odd number
    int prime = n;
    if (prime % 2 == 0) prime++;
    
    // Keep checking until we find a prime
    while (!is_prime(prime)) {
        prime += 2;
    }
    
    return prime;
}

// Initialize hash table with improved memory handling
PassengerHashTable* init_hash_table(int size) {
    // Calculate better size for large datasets
    // Ensure it's a prime number for better hash distribution
    if (size > 1000000) {
        // For very large datasets, use a smaller load factor (0.5)
        size = (int)(size / 0.5);
    } else if (size > 100000) {
        // For large datasets, use a load factor of 0.6
        size = (int)(size / 0.6);
    } else {
        // For smaller datasets, use the standard load factor of 0.7
        size = (int)(size / 0.7);
    }
    
    size = find_next_prime(size);
    
    // Allocate the hash table structure
    PassengerHashTable* table = (PassengerHashTable*)malloc(sizeof(PassengerHashTable));
    if (table == NULL) {
        fprintf(stderr, "Memory allocation failed for hash table\n");
        return NULL;  // Return NULL instead of exit for better error handling
    }
    
    // Initialize table values
    table->size = size;
    table->count = 0;
    
    // Allocate the hash table entries with error handling
    table->table = (HashEntry*)calloc(size, sizeof(HashEntry));
    if (table->table == NULL) {
        fprintf(stderr, "Memory allocation failed for hash table entries (requested size: %d)\n", size);
        free(table);
        return NULL;  // Return NULL instead of exit for better error handling
    }
    
    // All entries are initialized to 0 by calloc, so no need for explicit initialization
    // This is more efficient for large tables
    
    printf("Hash table initialized with size %d for improved performance\n", size);
    return table;
}

// Hash function for passenger IDs
int hash_function(int id, int table_size) {
    return id % table_size;
}

// Insert a passenger into the hash table
void hash_insert_passenger(PassengerHashTable* table, Passenger passenger) {
    int index = hash_function(passenger.id, table->size);
    
    // If the slot is empty, directly insert
    if (table->table[index].occupied == 0) {
        table->table[index].data = passenger;
        table->table[index].occupied = 1;
        table->count++;
        return;
    }
    
    // If the passenger ID already exists in the main slot, update the passenger
    if (table->table[index].occupied == 1 && table->table[index].data.id == passenger.id) {
        table->table[index].data = passenger;
        return;
    }
    
    // Handle collision using chaining
    HashEntry* current = &table->table[index];
    
    // Check if the ID already exists in the chain
    while (current->next != NULL) {
        current = current->next;
        if (current->data.id == passenger.id) {
            current->data = passenger;
            return;
        }
    }
    
    // Insert at the end of the chain
    HashEntry* new_entry = (HashEntry*)malloc(sizeof(HashEntry));
    if (new_entry == NULL) {
        fprintf(stderr, "Memory allocation failed for hash entry\n");
        return;
    }
    
    new_entry->data = passenger;
    new_entry->occupied = 1;
    new_entry->next = NULL;
    current->next = new_entry;
    table->count++;
}

// Find a passenger in the hash table
Passenger* hash_find_passenger(PassengerHashTable* table, int id) {
    if (table == NULL) {
        return NULL;
    }
    
    int index = hash_function(id, table->size);
    
    // Check if the ID is in the main slot
    if (table->table[index].occupied == 1 && table->table[index].data.id == id) {
        return &(table->table[index].data);
    }
    
    // Search in the chain
    HashEntry* current = table->table[index].next;
    while (current != NULL) {
        if (current->occupied == 1 && current->data.id == id) {
            return &(current->data);
        }
        current = current->next;
    }
    
    // Not found
    return NULL;
}

// Print all passengers in the hash table
void print_hash_passengers(PassengerHashTable* table) {
    if (table == NULL) {
        return;
    }
    
    printf("Passengers in the system:\n");
    for (int i = 0; i < table->size; i++) {
        // Print main slot if occupied
        if (table->table[i].occupied == 1) {
            printf("Passenger ID: %d, Name: %s, Passport: %s\n", 
                   table->table[i].data.id, table->table[i].data.name, 
                   table->table[i].data.passportNumber);
        }
        
        // Print chain if exists
        HashEntry* current = table->table[i].next;
        while (current != NULL) {
            if (current->occupied == 1) {
                printf("Passenger ID: %d, Name: %s, Passport: %s\n", 
                       current->data.id, current->data.name, 
                       current->data.passportNumber);
            }
            current = current->next;
        }
    }
    
    printf("Total passengers: %d\n", table->count);
}

// Free hash table memory
void free_hash_table(PassengerHashTable* table) {
    if (table == NULL) {
        return;
    }
    
    // Free all chained entries
    for (int i = 0; i < table->size; i++) {
        HashEntry* current = table->table[i].next;
        while (current != NULL) {
            HashEntry* tmp = current;
            current = current->next;
            free(tmp);
        }
    }
    
    // Free the main table array and the table struct
    free(table->table);
    free(table);
}
