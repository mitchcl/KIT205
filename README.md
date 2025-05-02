# Airline Reservation System - KIT205 Assignment 1

This is a database-style application for an airline reservation system, implemented as part of KIT205 Assignment 1. The system demonstrates the use of various data structures for storing and retrieving large amounts of data.

## Overview

The system implements and compares two different prototypes for managing an airline reservation database:

- **Prototype 1:** 
  - Flights: Binary Search Tree (BST)
  - Passengers: Linked List
  - Reservations: Array

- **Prototype 2:** 
  - Flights: AVL Tree (self-balancing BST)
  - Passengers: Hash Table
  - Reservations: Binary Search Tree (BST)

## Project Updates (May 5, 2025)

The project has been improved with the following updates:

1. **Code Modularization**: Search functionality has been separated into dedicated modules for better organization and maintainability:
   - `flight_search.c/h` - BST-based flight search
   - `flight_search_avl.c/h` - AVL-tree-based flight search
   - `passenger_search.c/h` - Linked list-based passenger search
   - `passenger_search_hash.c/h` - Hash table-based passenger search

2. **Improved Build System**: Updated the Makefile to include the new modular search components

3. **Case-Insensitive Search**: Implemented case-insensitive search for passenger names using GNU's `strcasestr` function

Both prototypes implement a many-to-many relationship between flights and passengers, and support the following operations:
- Insert flights and passengers
- Record flight reservations
- Print ordered lists of all flights and passengers
- Find all flights booked by a specific passenger
- Find all passengers who booked a specific flight

## Data Structures

The application demonstrates the use of several data structures and provides performance comparisons:

### Binary Search Tree (BST)
- Used for efficient ordered retrieval of data
- O(log n) search time in average case
- O(n) worst case for skewed trees
- Used in Prototype 1 for flights

### AVL Tree
- Self-balancing binary search tree
- Guarantees O(log n) operations even in worst case
- More efficient than regular BST for large, dynamic datasets
- Used in Prototype 2 for flights

### Linked List
- Simple implementation for ordered traversal
- O(n) search time
- Used in Prototype 1 for passengers

### Hash Table
- Nearly O(1) lookup time
- Used in Prototype 2 for faster passenger retrieval
- Uses chaining for collision resolution

### Array
- Simple sequential storage
- O(n) search time
- Used in Prototype 1 for reservation records

## Building the Project

To compile the project, run:

```bash
make
```

This will create the executable in the `bin` directory.

## Running the Application

For basic usage with small test datasets:

```bash
./bin/airline_system
```

For generating and testing with large datasets:

```bash
./bin/airline_system --generate
```

To skip unit tests and proceed directly to data processing:

```bash
./bin/airline_system --skip-tests
```

You can combine arguments:

```bash
./bin/airline_system --generate --skip-tests
```

## Performance Testing

The program includes comprehensive performance testing that:
1. Compares both prototypes with increasing dataset sizes
2. Measures execution time for key operations
3. Calculates speedup factors

## Project Files and Structure

- `src/`: Source code directory
  - `common.h/c`: Common definitions and utilities
  - `bst.h/c`: Binary Search Tree implementation
  - `avl.h/c`: AVL Tree implementation
  - `linked_list.h/c`: Linked List implementation
  - `hash_table.h/c`: Hash Table implementation
  - `prototype1/`: First prototype implementation
  - `prototype2/`: Second prototype implementation
  - `evaluation.h/c`: Performance evaluation functions
  - `data_loader.h/c`: Functions to load data from CSV files
  - `airline_system.c`: Main application entry point
  
- `data/`: CSV data files
  - `flights.csv`: Flight records
  - `passengers.csv`: Passenger records
  - `reservations.csv`: Reservation records

## References and Resources Used
### Books and Academic Resources
- Tutorial code
- Introduction to Algorithms by Cormen et al. - For BST, AVL, and Hash Table concepts
- Data Structures and Algorithm Analysis by Mark Allen Weiss - For data structure implementation techniques
- The C Programming Language (K&R) - For C syntax and file I/O operations
- The Art of Computer Programming by Donald Knuth (Volume 3: Sorting and Searching) - For code techniques

### Online Resources
- Stack Overflow - For random number generation techniques and memory management practices
- GeeksforGeeks - For BST and AVL tree implementation examples
- Copilot - For code snippets and examples of data structure implementations. Also helped in generating test cases and debugging because this was a HEADACHE.

All code implementations based on these resources are properly documented with source citations in the comments of relevant files.

## Development Log

The development process is documented in the DevLog.md file, including:
- Implementation decisions
- Testing strategies
- Performance optimization
- Challenges encountered and solutions

I didn't realise I had to do this in the normal Github repo, so i've kept it as an md in case.

## Evaluation

The application includes performance evaluation tools to compare the two prototypes. Key operations are measured:
- Flight insertion
- Passenger insertion
- Flight finding
- Passenger finding
- Reservation operations

## Usage

The application provides an interactive menu with the following options:
1. Run unit tests for data structures
2. Run unit tests for Prototype 1
3. Run unit tests for Prototype 2
4. Load data from CSV files
5. Run performance evaluation
6. Interactive mode (Prototype 1)
7. Interactive mode (Prototype 2)
0. Exit
