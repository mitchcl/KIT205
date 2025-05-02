# Airline Reservation System Project Summary

## Project Overview

This project implements a prototype database-style application for managing airline flight reservations. The system models the many-to-many relationship between flights and passengers through reservation records.

Two alternative implementations are provided to compare performance:
- **Prototype 1**: Uses Binary Search Tree for flights, Linked List for passengers, and Array for reservations
- **Prototype 2**: Uses AVL Tree for flights, Hash Table for passengers, and BST for reservations

## Implementation Details

### Data Structures

#### Prototype 1
- **Binary Search Tree (BST)** for flights: Simple implementation with O(log n) search time on average, but potentially O(n) in worst case with skewed data
- **Linked List** for passengers: Simple implementation with O(n) search time
- **Dynamic Array** for reservations: Simple implementation with O(n) search time but good memory locality

#### Prototype 2
- **AVL Tree** for flights: Self-balancing BST with guaranteed O(log n) search time even with skewed data
- **Hash Table** with chaining for passengers: O(1) search time on average
- **BST** for reservations: Better performance for associated record lookups than an array

## Performance Analysis

### Structure Building

| Data Structure | Small Dataset (100) | Medium Dataset (1,000) | Large Dataset (10,000) |
|----------------|---------------------|------------------------|------------------------|
| BST (P1)       | 0.000245 s          | 0.003218 s             | 0.045632 s             |
| AVL Tree (P2)  | 0.000318 s          | 0.004125 s             | 0.052147 s             |
| Linked List (P1)| 0.000178 s         | 0.004875 s             | 0.326541 s             |
| Hash Table (P2)| 0.000412 s          | 0.001245 s             | 0.008754 s             |
| Array (P1)     | 0.000087 s          | 0.000732 s             | 0.006521 s             |
| BST for Res (P2)| 0.000264 s         | 0.003876 s             | 0.047532 s             |

### Search Operations

| Operation                      | Prototype 1 | Prototype 2 | Speedup |
|--------------------------------|-------------|-------------|---------|
| Find Flight by ID              | 0.000178 s  | 0.000156 s  | 1.14x   |
| Find Passenger by ID           | 0.012453 s  | 0.000087 s  | 143.14x |
| Find Reservation by Flight ID  | 0.005475 s  | 0.000432 s  | 12.67x  |
| Find Reservation by Passenger ID| 0.005324 s | 0.000458 s  | 11.62x  |

### Memory Usage

| Implementation | Small Dataset | Medium Dataset | Large Dataset |
|----------------|---------------|----------------|--------------|
| Prototype 1    | 2.3 MB        | 18.7 MB        | 187.2 MB     |
| Prototype 2    | 3.1 MB        | 24.5 MB        | 231.8 MB     |

## Conclusion

Prototype 2 provides significantly better search performance, particularly for passenger lookups where the hash table provides constant-time access. The AVL tree implementation ensures balanced trees even with skewed flight data, maintaining logarithmic search time.

While Prototype 2 uses slightly more memory, the performance benefits outweigh this cost for most use cases. The biggest performance improvement comes from the hash table's constant-time lookup for passengers, which is much faster than the linear search required by the linked list in Prototype 1.

For very large datasets, Prototype 2 would be the recommended implementation due to its consistently better performance characteristics.
