
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

## April 28, 2025 - Data Loading and Generation

- Implemented CSV file loading for flights, passengers, and reservations
- Developed random data generator for large-scale testing
- Added helper functions for random names, flight numbers, and seat assignments

## May 1, 2025 - Testing and Debugging

- Created unit tests for all data structures and search functions
- Fixed bugs in insert and search logic
- Verified correctness with small and large datasets
- Improved error handling and memory management

## May 3, 2025 - Performance Testing

- Added timing measurements for structure building and search operations
- Implemented a comprehensive performance testing framework
- Created large datasets for benchmarking (10,000+ records)
- Documented performance results and analysis

## May 5, 2025 - Code Modularization and Search Improvements

- Refactored search functionality into separate modules:
  - Prototype 1: `flight_search.c/h` and `passenger_search.c/h`
  - Prototype 2: `flight_search_avl.c/h` and `passenger_search_hash.c/h`
- Updated the Makefile to include the new modules
- Improved search functionality with case-insensitive passenger name searching
- Fixed various bugs and compiler warnings
- Enhanced code organization for better maintainability

## May 5, 2025 - Final Code Cleanup and Documentation

- Performed final code cleanup and removed unused functions
- Fixed inconsistencies in function signatures and comments
- Completed comprehensive testing with different data sizes
- Finalized DevLog and Project Summary documentation
- Prepared project for submission and performance evaluation

## Notable Design Decisions

1. **Data Structures Selection**:
   - BST vs AVL Tree for flights: Chose AVL for Prototype 2 to maintain O(log n) search complexity even with skewed data
   - Linked List vs Hash Table for passengers: Hash Table significantly improves lookup performance
   - Array vs BST for reservations: BST provides better performance for associated record lookups

2. **Performance Optimization**:
   - Implemented proper memory management to handle large datasets
   - Used efficient algorithms for record association lookups
   - Added timing measurements at critical points for accurate comparison

## Resources Used

- Introduction to Algorithms by Cormen et al. - BST, AVL, and Hash Table concepts
- Data Structures in C by Noel Kalicharan - Reference for linked list implementations
- The C Programming Language (K&R) - Reference for C syntax and file I/O
- Stack Overflow - Random number generation techniques for data generator
