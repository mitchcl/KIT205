/**
 * Transport Graph Data Structures
 * KIT205 Assignment 2 - Tasmania Public Transport Analysis
 */

#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_NAME_LENGTH 100
#define MAX_SUBURB_LENGTH 50
#define MAX_PATH_LENGTH 50
#define MAX_SERVICES 50

 /**
  * Structure representing a bus stop in the transport network
  */
typedef struct
{
    int stop_id;
    char name[MAX_NAME_LENGTH];
    double latitude;
    double longitude;
    char suburb[MAX_SUBURB_LENGTH];
    char stop_type[20];
} BusStop;

/**
 * Structure representing a connection between bus stops
 */
typedef struct Connection
{
    int destination_stop_id;
    int route_id;
    int travel_time;
    struct Connection* next;
} Connection;

/**
 * Structure representing an essential service
 */
typedef struct
{
    int service_id;
    char name[MAX_NAME_LENGTH];
    int stop_id;
    char service_type[50];
    double importance_weight;
} EssentialService;

/**
 * Adjacency list representation of transport network
 */
typedef struct
{
    BusStop* stops;
    Connection** adjacency_list;
    int num_stops;
    int capacity;
    EssentialService* services;
    int num_services;
} TransportGraph;

/**
 * Structure for storing shortest path results
 */
typedef struct
{
    int* path;
    int path_length;
    int total_transfers;
    int total_time;
    bool path_found;
} PathResult;

/**
 * Structure for accessibility analysis results
 */
typedef struct
{
    int stop_id;
    double accessibility_score;
    int avg_transfers_to_services;
    int unreachable_services;
} AccessibilityResult;

// Core graph operations
TransportGraph* create_transport_graph(int initial_capacity);
int add_bus_stop(TransportGraph* graph, int stop_id, const char* name, double lat, double lon, const char* suburb);
int add_connection(TransportGraph* graph, int from_stop, int to_stop, int route_id, int travel_time);
void free_transport_graph(TransportGraph* graph);

// Network analysis functions
PathResult* find_shortest_path(TransportGraph* graph, int start_stop, int end_stop);
void free_path_result(PathResult* result);
void print_path(TransportGraph* graph, PathResult* result);

// Accessibility analysis
AccessibilityResult* calculate_accessibility_score(TransportGraph* graph, int stop_id);
void analyse_network_accessibility(TransportGraph* graph);
int* find_critical_stops(TransportGraph* graph, int* num_critical);

// Services
int add_essential_service(TransportGraph* graph, int service_id, const char* name, int stop_id, const char* service_type, double weight);
int load_services_from_file(TransportGraph* graph, const char* filename);

// Utility functions
int find_stop_index(TransportGraph* graph, int stop_id);
void print_graph_stats(TransportGraph* graph);

// GTFS parsing
int load_stops_from_file(TransportGraph* graph, const char* filename);

#endif
