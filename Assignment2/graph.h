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

 /**
  * Structure representing a bus stop in the transport network
  */
typedef struct {
    int stop_id;
    char name[MAX_NAME_LENGTH];
    double latitude;
    double longitude;
    char suburb[MAX_SUBURB_LENGTH];
    char stop_type[20];
} BusStop;

/**
 * Basic graph structure - to be expanded
 */
typedef struct {
    BusStop* stops;
    int num_stops;
    int capacity;
} TransportGraph;

// Function prototypes - basic structure
TransportGraph* create_transport_graph(int initial_capacity);
void free_transport_graph(TransportGraph* graph);

#endif
