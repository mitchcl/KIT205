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

#endif
