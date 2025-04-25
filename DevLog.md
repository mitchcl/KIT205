# Development Log for KIT205 Assignment 1
## April 15, 2025 - Project Planning and Domain Selection
- Chose airline reservation system as the problem domain (flights, passengers, reservations)
- Identified many-to-many relationship between flights and passengers via reservations
- Decided to compare two alternative data structure approaches for performance
- Outlined initial requirements and planned data structure selection

## April 18, 2025 - Data Structure Design

- Designed data models for Flight, Passenger, and ReservationRecord
- Planned Prototype 1: BST for flights, Linked List for passengers, Array for reservations
- Planned Prototype 2: AVL Tree for flights, Hash Table for passengers, BST for reservations
- Sketched out header files and function prototypes for each module

## April 22, 2025 - Initial Implementation

- Implemented BST for flights (Prototype 1)
- Implemented linked list for passengers (Prototype 1)
- Implemented array for reservations (Prototype 1)
- Wrote basic insert, search, and print functions for each structure

## April 25, 2025 - Advanced Data Structures

- Implemented AVL tree for flights (Prototype 2)
- Implemented hash table for passengers (Prototype 2)
- Implemented BST for reservations (Prototype 2)
- Added collision handling for hash table
- Added balancing logic for AVL tree
