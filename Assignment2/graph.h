/**
 * Transport Graph Data Structures
 * KIT205 Assignment 2 - Tasmania Public Transport Analysis
 */

#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME_LENGTH 100
#define MAX_SUBURB_LENGTH 50

// Structure representing a bus stop in the transport network
typedef struct {
    int stop_id;
    char name[MAX_NAME_LENGTH];
    double latitude;
    double longitude;
    char suburb[MAX_SUBURB_LENGTH];
    char stop_type[20];
} BusStop;


// Structure representing a connection between bus stops
typedef struct Connection {
    int destination_stop_id;
    int route_id;
    int travel_time;
    struct Connection* next;
} Connection;

// Adjacency list representation of transport network
typedef struct {
    BusStop* stops;
    Connection** adjacency_list;
    int num_stops;
    int capacity;
} TransportGraph;

// Core graph operations
TransportGraph* create_transport_graph(int initial_capacity);
int add_bus_stop(TransportGraph* graph, int stop_id, const char* name, double lat, double lon, const char* suburb);
int add_connection(TransportGraph* graph, int from_stop, int to_stop, int route_id, int travel_time);
void free_transport_graph(TransportGraph* graph);

// Utility functions
int find_stop_index(TransportGraph* graph, int stop_id);
void print_graph_stats(TransportGraph* graph);

#endif
