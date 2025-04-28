#ifndef AIRLINE_TYPES_H
#define AIRLINE_TYPES_H

#include <time.h>

typedef struct {
    int id;
    char flightNumber[20];
    char origin[50];
    char destination[50];
    time_t departureTime;
    int capacity;
} Flight;

typedef struct {
    int id;
    char name[100];
    char passportNumber[20];
} Passenger;

typedef struct {
    int flightId;
    int passengerId;
    time_t bookingDate;
    char seatNumber[10];
} ReservationRecord;

// prototype 1 binary search tree node for flights
typedef struct BST_Node {
    Flight data;
    struct BST_Node* left;
    struct BST_Node* right;
} BST_Node;

// Linked list implementation for passengers
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

// PROTOTYPE 2
// AVL Tree node for flights
typedef struct AVL_Node {
    Flight data;
    struct AVL_Node* left;
    struct AVL_Node* right;
    int height;
} AVL_Node;

// Hash table entry for passengers
typedef struct HashEntry {
    Passenger data;
    int occupied;
    struct HashEntry* next;
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
} ReservationBST;

#endif
