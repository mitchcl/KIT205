/**
 * Transport Graph Implementation
 * Core graph data structure operations
 */

#include "graph.h"

TransportGraph* create_transport_graph(int initial_capacity) {
    TransportGraph* graph = malloc(sizeof(TransportGraph));
    if (!graph) {
        fprintf(stderr, "Error: Unable to allocate memory for graph\n");
        return NULL;
    }
    
    graph->stops = malloc(sizeof(BusStop) * initial_capacity);
    graph->adjacency_list = malloc(sizeof(Connection*) * initial_capacity);
    
    if (!graph->stops || !graph->adjacency_list) {
        fprintf(stderr, "Error: Unable to allocate memory for graph components\n");
        free(graph->stops);
        free(graph->adjacency_list);
        free(graph);
        return NULL;
    }
    
    // Init adjacency list
    for (int i = 0; i < initial_capacity; i++) {
        graph->adjacency_list[i] = NULL;
    }
    
    graph->num_stops = 0;
    graph->capacity = initial_capacity;
    
    return graph;
}

int add_bus_stop(TransportGraph* graph, int stop_id, const char* name, 
                 double lat, double lon, const char* suburb) {
    if (!graph || !name || !suburb) {
        return -1;
    }
    
    // Check if stop already exists
    if (find_stop_index(graph, stop_id) != -1) {
        return -1; // Stop already exists
    }
    
    // Resize if necessary
    if (graph->num_stops >= graph->capacity) {

        return -1;
    }
    
    BusStop* stop = &graph->stops[graph->num_stops];
    stop->stop_id = stop_id;
    strncpy(stop->name, name, MAX_NAME_LENGTH - 1);
    stop->name[MAX_NAME_LENGTH - 1] = '\0';
    stop->latitude = lat;
    stop->longitude = lon;
    strncpy(stop->suburb, suburb, MAX_SUBURB_LENGTH - 1);
    stop->suburb[MAX_SUBURB_LENGTH - 1] = '\0';
    
    graph->num_stops++;
    return graph->num_stops - 1; // Return index
}

int add_connection(TransportGraph* graph, int from_stop, int to_stop, 
                   int route_id, int travel_time) {
    if (!graph) return -1;
    
    int from_index = find_stop_index(graph, from_stop);
    if (from_index == -1) return -1;
    
    Connection* new_connection = malloc(sizeof(Connection));
    if (!new_connection) {
        fprintf(stderr, "Error: Unable to allocate memory for connection\n");
        return -1;
    }
    
    new_connection->destination_stop_id = to_stop;
    new_connection->route_id = route_id;
    new_connection->travel_time = travel_time;
    new_connection->next = graph->adjacency_list[from_index];
    graph->adjacency_list[from_index] = new_connection;
    
    return 0;
}


int find_stop_index(TransportGraph* graph, int stop_id) {
    if (!graph) return -1;
    
    for (int i = 0; i < graph->num_stops; i++) {
        if (graph->stops[i].stop_id == stop_id) {
            return i;
        }
    }
    return -1;
}


void print_graph_stats(TransportGraph* graph) {
    if (!graph) return;
    
    printf("Transport Graph Statistics:\n");
    printf("Number of stops: %d\n", graph->num_stops);
    printf("Graph capacity: %d\n", graph->capacity);
    
    // Count total connections
    int total_connections = 0;
    for (int i = 0; i < graph->num_stops; i++) {
        Connection* conn = graph->adjacency_list[i];
        while (conn) {
            total_connections++;
            conn = conn->next;
        }
    }
    printf("Total connections: %d\n", total_connections);
}


void free_transport_graph(TransportGraph* graph) {
    if (!graph) return;
    
    // Free adjacency list connections
    for (int i = 0; i < graph->capacity; i++) {
        Connection* conn = graph->adjacency_list[i];
        while (conn) {
            Connection* temp = conn;
            conn = conn->next;
            free(temp);
        }
    }
    
    free(graph->stops);
    free(graph->adjacency_list);
    free(graph);
}
