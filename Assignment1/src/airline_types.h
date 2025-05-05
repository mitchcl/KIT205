/*
 * Airline Reservation System Data Types Header
 * 
 * This file defines all the data structures used in the airline reservation system.
 * It includes two separate prototype implementations with different data structures.
 *
 * Sources used:
 * 1. Introduction to Algorithms by Cormen et al. - Data structure concepts
 * 2. Data Structures and Algorithm Analysis by Mark Allen Weiss - BST and AVL tree concepts
 * 3. The C Programming Language (K&R) - Struct definitions and pointers
 */

#ifndef AIRLINE_TYPES_H
#define AIRLINE_TYPES_H

#include <time.h>

#define MAX_LINE_LENGTH 256
#define MAX_FLIGHT_ID_LENGTH 20
#define MAX_PASSENGER_NAME_LENGTH 100
#define MAX_SEAT_NUMBER_LENGTH 10

// Flight structure
typedef struct {
    int id;
    char flightNumber[20];
    char origin[50];
    char destination[50];
    time_t departureTime;
    int capacity;
} Flight;

// Passenger structure
typedef struct {
    int id;
    char name[100];
    char passportNumber[20];
} Passenger;

// Reservation record
typedef struct {
    int flightId;
    int passengerId;
    time_t bookingDate;
    char seatNumber[10];
} ReservationRecord;

//--- PROTOTYPE 1 DATA STRUCTURES ---//

// Binary Search Tree node for flights
typedef struct BST_Node {
    Flight data;
    struct BST_Node* left;
    struct BST_Node* right;
} BST_Node;

// Linked List implementation for passengers
typedef struct LL_Node {
    Passenger data;
    struct LL_Node* next;
} LL_Node;

// Array implementation for reservation records
typedef struct {
    ReservationRecord* records;
    int count;
    int capacity;
} ReservationArray;

//--- PROTOTYPE 2 DATA STRUCTURES ---//

// AVL Tree node for flights
typedef struct AVL_Node {
    Flight data;
    struct AVL_Node* left;
    struct AVL_Node* right;
    int height; // Height for balancing
} AVL_Node;

// Hash table entry for passengers
typedef struct HashEntry {
    Passenger data;
    int occupied; // 0 = empty, 1 = occupied, -1 = deleted
    struct HashEntry* next; // For collision handling (chaining)
} HashEntry;

// Hash table for passengers
typedef struct {
    HashEntry* table;
    int size;
    int count;
} PassengerHashTable;

// Reservation BST for efficient lookup
typedef struct ReservationBST_Node {
    ReservationRecord data;
    struct ReservationBST_Node* left;
    struct ReservationBST_Node* right;
} ReservationBST_Node;

// BST container for reservation records
typedef struct {
    ReservationBST_Node* root;
    int count;
    // Added indexes for faster lookup by ID
    struct {
        ReservationBST_Node** by_flight;     // Array of pointers to first nodes for each flight ID
        int* flight_counts;                  // Count of reservations for each flight ID
        int max_flight_id;                   // Maximum flight ID for array indexing
    } flight_index;
    struct {
        ReservationBST_Node** by_passenger;  // Array of pointers to first nodes for each passenger ID
        int* passenger_counts;               // Count of reservations for each passenger ID
        int max_passenger_id;                // Maximum passenger ID for array indexing
    } passenger_index;
    int index_enabled;                       // Flag to indicate if indexes are enabled
} ReservationBST;

#endif