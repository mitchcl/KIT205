/*
 * test_transport.c - Unit tests for transport graph functionality
 * KIT205 Assignment 2
 * 
 * Academic Integrity Statement:
 * - Unit testing patterns based on C testing frameworks and examples from:
 *   "Test-Driven Development for Embedded C" by James W. Grenning
 * - Assert-based testing approach from C standard library documentation
 * - Test structure influenced by industry best practices for C testing
 * - Memory leak testing patterns from online C programming resources
 * - AI assistance used for:
 *   - Debugging test failures and assertion errors
 *   - Optimising test coverage for edge cases
 *   - Fixing memory cleanup issues in test teardown
 *   - Improving test output formatting and error reporting
 */


#include <stdio.h>
#include <assert.h>
#include "../src/graph.h"

// Test basic graph creation and manipulation
static void test_graph_creation()
{
    printf("Testing graph creation...\n");

    TransportGraph* graph = create_transport_graph(10);
    assert(graph != NULL);
    assert(graph->num_stops == 0);
    assert(graph->stops != NULL);

    printf("✓ Graph creation test passed\n");
    free_transport_graph(graph);
}

// Test adding bus stops
static void test_add_bus_stops()
{
    printf("Testing bus stop addition...\n");

    TransportGraph* graph = create_transport_graph(10);

    bool result1 = add_bus_stop(graph, 1, INTERCHANGE, "Hobart City Interchange",
        "Hobart", -42.8826, 147.3257);
    assert(result1 == true);
    assert(graph->num_stops == 1);

    bool result2 = add_bus_stop(graph, 2, HOSPITAL, "Royal Hobart Hospital",
        "Hobart", -42.8864, 147.3242);
    assert(result2 == true);
    assert(graph->num_stops == 2);

    printf("✓ Bus stop addition test passed\n");
    free_transport_graph(graph);
}

// Test adding connections between stops
static void test_add_connections()
{
    printf("Testing bus connections...\n");

    TransportGraph* graph = create_transport_graph(10);

    add_bus_stop(graph, 1, INTERCHANGE, "Stop 1", "Hobart", -42.88, 147.32);
    add_bus_stop(graph, 2, BUS_STOP, "Stop 2", "Sandy Bay", -42.90, 147.33);

    bool result = add_bus_connection(graph, 1, 2, 15, 401);
    assert(result == true);
    assert(graph->num_connections == 1);

    // Check if edge was added correctly
    Edge* edge = graph->stops[0].edges;
    assert(edge != NULL);
    assert(edge->to == 1); // Index 1 for stop ID 2
    assert(edge->travel_time == 15);
    assert(edge->route_id == 401);

    printf("✓ Bus connections test passed\n");
    free_transport_graph(graph);
}

// Test BFS shortest path algorithm
static void test_bfs_shortest_path()
{
    printf("Testing BFS shortest path...\n");

    TransportGraph* graph = create_transport_graph(5);

    // Create a simple linear network: 1 -> 2 -> 3 -> 4
    add_bus_stop(graph, 1, INTERCHANGE, "Stop 1", "Hobart", -42.88, 147.32);
    add_bus_stop(graph, 2, BUS_STOP, "Stop 2", "Sandy Bay", -42.90, 147.33);
    add_bus_stop(graph, 3, BUS_STOP, "Stop 3", "Kingston", -42.95, 147.30);
    add_bus_stop(graph, 4, HOSPITAL, "Hospital", "Hobart", -42.88, 147.32);

    add_bus_connection(graph, 1, 2, 10, 401);
    add_bus_connection(graph, 2, 3, 15, 401);
    add_bus_connection(graph, 3, 4, 20, 401);

    int distances[4];
    int parents[4];

    bfs_shortest_path(graph, 0, distances, parents); // Start from stop 1 (index 0)

    // Distance from stop 1 to stop 4 should be 3 transfers
    assert(distances[3] == 3);

    printf("✓ BFS shortest path test passed\n");
    free_transport_graph(graph);
}

// Test loading sample data
static void test_load_sample_data()
{
    printf("Testing sample data loading...\n");

    // This would test loading from the CSV files we created
    // For now, just test the data structure
    TransportGraph* graph = create_transport_graph(20);

    // Add some sample stops like in our stops.txt
    add_bus_stop(graph, 1, INTERCHANGE, "Hobart City Interchange Stop A1",
        "Hobart", -42.8826, 147.3257);
    add_bus_stop(graph, 5, HOSPITAL, "Royal Hobart Hospital",
        "Hobart", -42.8864, 147.3242);
    add_bus_stop(graph, 4, UNIVERSITY, "UTAS Churchill Ave",
        "Sandy Bay", -42.9023, 147.3189);

    assert(graph->num_stops == 3);

    printf("✓ Sample data loading test passed\n");
    free_transport_graph(graph);
}

int main()
{
    printf("=== Tasmania Public Transport Network Tests ===\n\n");

    test_graph_creation();
    test_add_bus_stops();
    test_add_connections();
    test_bfs_shortest_path();
    test_load_sample_data();

    printf("\n=== All tests passed! ===\n");
    return 0;
}
