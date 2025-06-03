/**
 * Main program for transport analysis
 * Basic functionality test
 */

#include "graph.h"

int load_stops_from_file(TransportGraph* graph, const char* filename);

int main() {
    printf("Tasmania Transport Analysis\n");
    
    // Create transport graph
    TransportGraph* graph = create_transport_graph(100);
    if (!graph) {
        fprintf(stderr, "Failed to create transport graph\n");
        return 1;
    }
    
    // Load sample data
    int loaded = load_stops_from_file(graph, "data/gtfs/stops.txt");
    if (loaded <= 0) {
        fprintf(stderr, "Failed to load stops data\n");
        free_transport_graph(graph);
        return 1;
    }
    
    // Print basic statistics
    print_graph_stats(graph);
    
    // Clean up
    free_transport_graph(graph);
    
    printf("\functionality test completed.\n");
    return 0;
}
