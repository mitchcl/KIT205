/**
 * Unit Tests for Transport Analysis
 * Comprehensive testing framework
 */

#include <assert.h>
#include "graph.h"

 // Test helper functions
void test_graph_creation()
{
    printf("Testing graph creation...\n");

    TransportGraph* graph = create_transport_graph(10);
    assert(graph != NULL);
    assert(graph->num_stops == 0);
    assert(graph->capacity == 10);

    free_transport_graph(graph);
    printf("✓ Graph creation test passed\n");
}

void test_stop_addition()
{
    printf("Testing stop addition...\n");

    TransportGraph* graph = create_transport_graph(10);
    assert(graph != NULL);

    int result = add_bus_stop(graph, 1, "Test Stop", -42.88, 147.33, "Hobart");
    assert(result == 0);
    assert(graph->num_stops == 1);
    assert(graph->stops[0].stop_id == 1);
    assert(strcmp(graph->stops[0].name, "Test Stop") == 0);

    free_transport_graph(graph);
    printf("✓ Stop addition test passed\n");
}

void test_connection_addition()
{
    printf("Testing connection addition...\n");

    TransportGraph* graph = create_transport_graph(10);
    add_bus_stop(graph, 1, "Stop A", -42.88, 147.33, "Hobart");
    add_bus_stop(graph, 2, "Stop B", -42.89, 147.34, "Hobart");

    int result = add_connection(graph, 1, 2, 401, 10);
    assert(result == 0);

    // Verify connection exists
    Connection* conn = graph->adjacency_list[0];
    assert(conn != NULL);
    assert(conn->destination_stop_id == 2);
    assert(conn->route_id == 401);

    free_transport_graph(graph);
    printf("✓ Connection addition test passed\n");
}

void test_shortest_path()
{
    printf("Testing shortest path algorithm...\n");

    TransportGraph* graph = create_transport_graph(10);

    // Create simple test network: 1 -> 2 -> 3
    add_bus_stop(graph, 1, "Stop A", -42.88, 147.33, "Hobart");
    add_bus_stop(graph, 2, "Stop B", -42.89, 147.34, "Hobart");
    add_bus_stop(graph, 3, "Stop C", -42.90, 147.35, "Hobart");

    add_connection(graph, 1, 2, 401, 10);
    add_connection(graph, 2, 3, 401, 10);

    PathResult* result = find_shortest_path(graph, 1, 3);
    assert(result != NULL);
    assert(result->path_found == true);
    assert(result->path_length == 3);
    assert(result->total_transfers == 2);

    free_path_result(result);
    free_transport_graph(graph);
    printf("✓ Shortest path test passed\n");
}

void test_no_path()
{
    printf("Testing disconnected graph handling...\n");

    TransportGraph* graph = create_transport_graph(10);

    // Create disconnected stops
    add_bus_stop(graph, 1, "Stop A", -42.88, 147.33, "Hobart");
    add_bus_stop(graph, 2, "Stop B", -42.89, 147.34, "Hobart");

    // No connections added

    PathResult* result = find_shortest_path(graph, 1, 2);
    assert(result != NULL);
    assert(result->path_found == false);

    free_path_result(result);
    free_transport_graph(graph);
    printf("✓ No path test passed\n");
}

int main()
{
    printf("Running Transport Analysis Unit Tests\n");
    printf("====================================\n\n");

    test_graph_creation();
    test_stop_addition();
    test_connection_addition();
    test_shortest_path();
    test_no_path();

    printf("\nAll tests passed successfully!\n");
    return 0;
}
