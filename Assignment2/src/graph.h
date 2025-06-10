/*
 * graph.h - Header file for Tasmania Transport Network Analysis
 * KIT205 Assignment 2
 * 
 * Academic Integrity Statement:
 * - Graph data structure design based on "Introduction to Algorithms" by Cormen et al.
 * - Adjacency list representation referenced from standard CS textbooks
 * - GTFS (General Transit Feed Specification) structures based on:
 *   https://developers.google.com/transit/gtfs/reference
 * - Enum design patterns from C programming best practices
 * - AI assistance used for:
 *   - Debugging memory allocation issues
 *   - Optimising struct definitions for memory alignment
 *   - Error handling improvements
 */
#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <limits.h>

// Maximum values for transport network
#define MAX_STOPS 3000
#define INFINITY INT_MAX

// Stop types in transport network
typedef enum
{
    BUS_STOP,    // Regular bus stop
    INTERCHANGE, // Major interchange/hub
    TERMINUS,    // Route terminus
    HOSPITAL,    // Hospital location
    UNIVERSITY,  // University campus
    SHOPPING,    // Shopping center
    RESIDENTIAL  // Residential area
} StopType;

// Edge represents direct bus connection between stops
typedef struct Edge
{
    int to;            // Destination stop
    int travel_time;   // Travel time (minutes)
    int route_id;      // Bus route number
    int frequency;     // Buses per hour
    struct Edge* next; // Next edge in adjacency list
} Edge;

// Node represents a bus stop or service location
typedef struct
{
    int id;           // Unique identifier
    StopType type;    // Type of stop/location
    char name[100];   // Stop name
    char suburb[50];  // Suburb name
    double latitude;  // GPS coordinates
    double longitude; // GPS coordinates
    bool visited;     // For graph traversal algorithms
    int distance;     // For shortest path algorithms (transfers)
    int parent;       // Parent node in path
    Edge* edges;      // Adjacency list of outgoing connections
} BusStop;

// Graph structure representing transport network
typedef struct
{
    BusStop* stops;      // Array of bus stops
    int num_stops;       // Number of stops
    int num_connections; // Number of direct connections
    int* service_stops;  // Array of essential service stop IDs
    int num_services;    // Number of essential services
} TransportGraph;

// Accessibility analysis results
typedef struct
{
    int stop_id;                // Stop identifier
    double avg_travel_time;     // Average travel time to services
    int num_transfers;          // Average number of transfers needed
    double accessibility_score; // Composite accessibility score
    char worst_service[50];     // Most difficult service to reach
} AccessibilityStats;

// Enhanced network analysis structures
typedef struct
{
    double avg_shortest_path;
    double network_efficiency;
    int diameter;
    int total_reachable_pairs;
} NetworkMetrics;

typedef struct
{
    int within_1_transfer;
    int within_2_transfers;
    int within_3_transfers;
    int within_5_transfers;
    int total_reachable;
} CumulativeOpportunities;

typedef struct
{
    int high_frequency_stops;
    int medium_frequency_stops;
    int low_frequency_stops;
    int isolated_stops;
    double avg_connections_per_stop;
    int max_connections;
    char busiest_stop[100];
    char isolated_stop_names[500]; // Names of isolated stops
} ServiceFrequencyAnalysis;

// Function prototypes
TransportGraph* create_transport_graph(int num_stops);
void free_transport_graph(TransportGraph* graph);
bool add_bus_stop(TransportGraph* graph, int id, StopType type, const char* name, const char* suburb, double lat, double lng);
bool add_bus_connection(TransportGraph* graph, int from, int to, int travel_time, int route_id);
bool add_service_location(TransportGraph* graph, int stop_id);
void print_transport_graph(TransportGraph* graph);

// Algorithm implementations
void bfs_shortest_path(TransportGraph* graph, int start, int* distances, int* parents);

// Accessibility analysis
AccessibilityStats analyse_stop_accessibility(TransportGraph* graph, int stop_id);

// Enhanced analysis functions
ServiceFrequencyAnalysis analyse_service_frequency(TransportGraph* graph);
CumulativeOpportunities calculate_cumulative_opportunities(TransportGraph* graph, int start_stop);
NetworkMetrics calculate_network_metrics(TransportGraph* graph);
void generate_network_report(TransportGraph* graph, const char* output_file);

// GTFS parsing functions
void parse_gtfs_stops(TransportGraph* graph, const char* stops_file);
void parse_gtfs_connections(TransportGraph* graph, const char* stop_times_file);
TransportGraph* load_gtfs_data(const char* gtfs_directory);

// Output and display functions
void print_accessibility_stats(AccessibilityStats stats);
void print_shortest_path(int* parents, int start, int end, TransportGraph* graph);
void generate_smart_recommendations(TransportGraph* graph, AccessibilityStats* stats_array,
    int num_locations, const char* location_names[]);
void analyse_diverse_locations(TransportGraph* graph);

// Utility functions
void reset_graph_traversal_state(TransportGraph* graph);
int find_stop_index(TransportGraph* graph, int stop_id);

#endif