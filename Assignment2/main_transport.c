#include "graph.h"

// Demonstrate the Tasmania public transport analysis system
void demonstrate_transport_analysis()
{
    printf("=== Tasmania Public Transport Accessibility Analysis ===\n\n");

    // Create transport network graph
    TransportGraph* graph = create_transport_graph(20);

    // Add key bus stops from our sample data
    add_bus_stop(graph, 1, INTERCHANGE, "Hobart City Interchange Stop A1", "Hobart", -42.8826, 147.3257);
    add_bus_stop(graph, 5, HOSPITAL, "Royal Hobart Hospital", "Hobart", -42.8864, 147.3242);
    add_bus_stop(graph, 4, UNIVERSITY, "UTAS Churchill Ave", "Sandy Bay", -42.9023, 147.3189);
    add_bus_stop(graph, 7, SHOPPING, "Eastlands Shopping Centre", "Rosny Park", -42.8742, 147.3578);
    add_bus_stop(graph, 6, INTERCHANGE, "Glenorchy Interchange Stop H", "Glenorchy", -42.8355, 147.2763);
    add_bus_stop(graph, 3, BUS_STOP, "Sandy Bay Rd Stop 625", "Sandy Bay", -42.9089, 147.3312);
    add_bus_stop(graph, 8, BUS_STOP, "Kingston Central", "Kingston", -42.9789, 147.3065);
    add_bus_stop(graph, 9, SHOPPING, "Northgate Shopping Centre", "Glenorchy", -42.8226, 147.2741);

    printf("✓ Added %d bus stops\n", graph->num_stops);

    // Add bus connections based on actual Metro Tasmania routes
    add_bus_connection(graph, 1, 5, 5, 429);  // City to Hospital (Route 429)
    add_bus_connection(graph, 1, 3, 8, 401);  // City to Sandy Bay (Route 401)
    add_bus_connection(graph, 3, 4, 5, 401);  // Sandy Bay to UTAS (Route 401)
    add_bus_connection(graph, 1, 7, 20, 446); // City to Eastlands (Route 446)
    add_bus_connection(graph, 6, 4, 18, 501); // Glenorchy to UTAS (Route 501)
    add_bus_connection(graph, 6, 1, 15, 500); // Glenorchy to City (Route 500)
    add_bus_connection(graph, 1, 8, 30, 410); // City to Kingston (Route 410)
    add_bus_connection(graph, 6, 9, 5, 500);  // Glenorchy to Northgate (Route 500)

    printf("✓ Added %d bus connections\n\n", graph->num_connections);

    // Display the network
    print_transport_graph(graph);

    // Perform dynamic analysis based on network characteristics
    analyse_diverse_locations(graph);

    free_transport_graph(graph);
}

int main()
{
    demonstrate_transport_analysis();

    printf("Analysis complete!\n\n");

    return 0;
}
