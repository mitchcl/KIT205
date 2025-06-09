#include "graph.h"

// Create a new transport graph with specified number of stops
TransportGraph* create_transport_graph(int num_stops)
{
    TransportGraph* graph = malloc(sizeof(TransportGraph));
    if (!graph)
        return NULL;

    graph->stops = calloc(num_stops, sizeof(BusStop));
    if (!graph->stops)
    {
        free(graph);
        return NULL;
    }

    graph->num_stops = 0; // Will increment as we add stops
    graph->num_connections = 0;
    graph->service_stops = malloc(20 * sizeof(int)); // Max 20 services
    graph->num_services = 0;

    // Initialise all stop slots
    for (int i = 0; i < num_stops; i++)
    {
        graph->stops[i].id = -1; // Mark as unused
        graph->stops[i].edges = NULL;
        graph->stops[i].visited = false;
        graph->stops[i].distance = INFINITY;
        graph->stops[i].parent = -1;
        strcpy(graph->stops[i].name, "");
        strcpy(graph->stops[i].suburb, "");
    }

    return graph;
}

// Free all memory allocated for the transport graph
void free_transport_graph(TransportGraph* graph)
{
    if (!graph)
        return;

    // Free all edges
    for (int i = 0; i < graph->num_stops; i++)
    {
        Edge* current = graph->stops[i].edges;
        while (current)
        {
            Edge* temp = current;
            current = current->next;
            free(temp);
        }
    }

    free(graph->stops);
    free(graph->service_stops);
    free(graph);
}

// Add a bus stop to the graph
bool add_bus_stop(TransportGraph* graph, int id, StopType type, const char* name,
    const char* suburb, double lat, double lng)
{
    if (!graph || graph->num_stops >= MAX_STOPS)
        return false;

    // Find next available slot
    int index = graph->num_stops;

    graph->stops[index].id = id;
    graph->stops[index].type = type;
    strncpy_s(graph->stops[index].name, 100, name, 99);
    strncpy_s(graph->stops[index].suburb, 50, suburb, 49);
    graph->stops[index].latitude = lat;
    graph->stops[index].longitude = lng;
    graph->stops[index].edges = NULL;
    graph->stops[index].visited = false;
    graph->stops[index].distance = INFINITY;
    graph->stops[index].parent = -1;

    graph->num_stops++;

    // If it's a service location, add to service array
    if (type == HOSPITAL || type == UNIVERSITY || type == SHOPPING)
    {
        if (graph->num_services < 20)
        {
            graph->service_stops[graph->num_services] = index;
            graph->num_services++;
        }
    }

    return true;
}

// Find stop index by ID
int find_stop_index(TransportGraph* graph, int stop_id)
{
    for (int i = 0; i < graph->num_stops; i++)
    {
        if (graph->stops[i].id == stop_id)
        {
            return i;
        }
    }
    return -1;
}

// Add a bus connection between two stops
bool add_bus_connection(TransportGraph* graph, int from_id, int to_id, int travel_time, int route_id)
{
    if (!graph)
        return false;

    int from_index = find_stop_index(graph, from_id);
    int to_index = find_stop_index(graph, to_id);

    if (from_index == -1 || to_index == -1)
        return false;

    // Create new edge
    Edge* new_edge = malloc(sizeof(Edge));
    if (!new_edge)
        return false;

    new_edge->to = to_index;
    new_edge->travel_time = travel_time;
    new_edge->route_id = route_id;
    new_edge->frequency = 4; // Default 4 buses per hour
    new_edge->next = graph->stops[from_index].edges;

    graph->stops[from_index].edges = new_edge;
    graph->num_connections++;

    return true;
}

// Print the transport graph
void print_transport_graph(TransportGraph* graph)
{
    if (!graph)
        return;

    printf("\n=== Tasmania Metro Transport Network ===\n");
    printf("Total stops: %d\n", graph->num_stops);
    printf("Total connections: %d\n", graph->num_connections);
    printf("Essential services: %d\n\n", graph->num_services);

    printf("Bus Stops:\n");
    for (int i = 0; i < graph->num_stops; i++)
    {
        BusStop* stop = &graph->stops[i];
        const char* type_str = (stop->type == INTERCHANGE) ? "Interchange" : (stop->type == HOSPITAL) ? "Hospital"
            : (stop->type == UNIVERSITY) ? "University"
            : (stop->type == SHOPPING) ? "Shopping"
            : (stop->type == TERMINUS) ? "Terminus"
            : (stop->type == RESIDENTIAL) ? "Residential"
            : "Bus Stop";

        printf("  [%d] %s (%s) - %s\n", stop->id, stop->name, type_str, stop->suburb);

        // Print connections
        Edge* edge = stop->edges;
        while (edge)
        {
            BusStop* dest = &graph->stops[edge->to];
            printf("    -> Route %d to %s (%d min)\n",
                edge->route_id, dest->name, edge->travel_time);
            edge = edge->next;
        }
    }
    printf("\n");
}

// Reset graph traversal state
void reset_graph_traversal_state(TransportGraph* graph)
{
    if (!graph)
        return;

    for (int i = 0; i < graph->num_stops; i++)
    {
        graph->stops[i].visited = false;
        graph->stops[i].distance = INFINITY;
        graph->stops[i].parent = -1;
    }
}

// BFS shortest path implementation
void bfs_shortest_path(TransportGraph* graph, int start_index, int* distances, int* parents)
{
    if (!graph || start_index < 0 || start_index >= graph->num_stops)
        return;

    reset_graph_traversal_state(graph);

    // Queue implementation using array
    int queue[MAX_STOPS];
    int front = 0, rear = 0;

    // Initialise arrays
    for (int i = 0; i < graph->num_stops; i++)
    {
        distances[i] = INFINITY;
        parents[i] = -1;
    }

    // Start BFS
    distances[start_index] = 0;
    graph->stops[start_index].visited = true;
    queue[rear++] = start_index;

    while (front < rear)
    {
        int current = queue[front++];

        // Explore all neighbors
        Edge* edge = graph->stops[current].edges;
        while (edge)
        {
            int neighbor = edge->to;

            if (!graph->stops[neighbor].visited)
            {
                graph->stops[neighbor].visited = true;
                distances[neighbor] = distances[current] + 1; // Number of transfers
                parents[neighbor] = current;
                queue[rear++] = neighbor;
            }
            edge = edge->next;
        }
    }
}

// Calculate accessibility score for a stop
AccessibilityStats analyse_stop_accessibility(TransportGraph* graph, int stop_index)
{
    AccessibilityStats stats = { 0 };

    if (!graph || stop_index < 0 || stop_index >= graph->num_stops)
    {
        return stats;
    }

    stats.stop_id = graph->stops[stop_index].id;

    int distances[MAX_STOPS];
    int parents[MAX_STOPS];

    bfs_shortest_path(graph, stop_index, distances, parents);

    // Calculate average distance to essential services
    int total_distance = 0;
    int reachable_services = 0;
    int max_distance = 0;

    for (int i = 0; i < graph->num_services; i++)
    {
        int service_index = graph->service_stops[i];
        if (distances[service_index] != INFINITY)
        {
            total_distance += distances[service_index];
            reachable_services++;
            if (distances[service_index] > max_distance)
            {
                max_distance = distances[service_index];
                BusStop* service = &graph->stops[service_index];
                strncpy(stats.worst_service, service->name, 49);
                stats.worst_service[49] = '\0'; // Ensure null termination
            }
        }
    }

    if (reachable_services > 0)
    {
        stats.num_transfers = total_distance / reachable_services;
        stats.avg_travel_time = stats.num_transfers * 15.0;              // Estimate 15 min per transfer
        stats.accessibility_score = 100.0 / (1.0 + stats.num_transfers); // Higher is better
    }
    else
    {
        stats.accessibility_score = 0.0;
        strcpy(stats.worst_service, "No services reachable");
    }

    return stats;
}

// Print accessibility statistics
void print_accessibility_stats(AccessibilityStats stats)
{
    printf("Stop ID %d Accessibility Analysis:\n", stats.stop_id);
    printf("  Average transfers to services: %d\n", stats.num_transfers);
    printf("  Estimated travel time: %.1f minutes\n", stats.avg_travel_time);
    printf("  Accessibility score: %.1f/100\n", stats.accessibility_score);
    printf("  Most difficult service: %s\n", stats.worst_service);
    printf("\n");
}

// Print shortest path between two stops
void print_shortest_path(int* parents, int start, int end, TransportGraph* graph)
{
    if (parents[end] == -1)
    {
        printf("No path found from %s to %s\n",
            graph->stops[start].name, graph->stops[end].name);
        return;
    }

    // Reconstruct path
    int path[MAX_STOPS];
    int path_length = 0;
    int current = end;

    while (current != -1)
    {
        path[path_length++] = current;
        current = parents[current];
    }

    printf("Route from %s to %s (%d transfers):\n", graph->stops[start].name, graph->stops[end].name, path_length - 1);

    for (int i = path_length - 1; i >= 0; i--)
    {
        BusStop* stop = &graph->stops[path[i]];
        printf("  %s (%s)", stop->name, stop->suburb);
        if (i > 0)
            printf(" -> ");
    }
    printf("\n\n");
}

// Add a service location to the graph's service list
bool add_service_location(TransportGraph* graph, int stop_id)
{
    if (!graph || graph->num_services >= 20)
        return false;

    // Check if stop exists in graph
    if (find_stop_index(graph, stop_id) == -1)
        return false;

    // Check if already in service list
    for (int i = 0; i < graph->num_services; i++)
    {
        if (graph->service_stops[i] == stop_id)
        {
            return false; // Already exists
        }
    }

    // Add to service list
    graph->service_stops[graph->num_services] = stop_id;
    graph->num_services++;
    return true;
}

// Generate intelligent recommendations based on network analysis
void generate_smart_recommendations(TransportGraph* graph, AccessibilityStats* stats_array, int num_locations, const char* location_names[])
{
    if (!graph || !stats_array)
        return;

    printf("SMART RECOMMENDATIONS:\n");

    // analyse each location and provide specific recommendations
    for (int i = 0; i < num_locations; i++)
    {
        AccessibilityStats stats = stats_array[i];

        if (stats.accessibility_score < 10.0)
        {
            printf("• %s shows poor accessibility (%.1f/100) - consider direct routes to essential services\n",
                location_names[i], stats.accessibility_score);
        }
        else if (stats.accessibility_score < 30.0)
        {
            printf("• %s has limited connectivity (%.1f/100) - additional connections would improve access\n",
                location_names[i], stats.accessibility_score);
        }
        else if (stats.accessibility_score > 50.0)
        {
            printf("• %s is well-connected (%.1f/100) - maintain high frequency services\n",
                location_names[i], stats.accessibility_score);
        }

        // Transfer-specific recommendations
        if (stats.num_transfers > 2)
        {
            printf("• %s requires %d transfers on average - direct routes could reduce travel complexity\n",
                location_names[i], stats.num_transfers);
        }
    }

    // Network-wide recommendations
    if (graph->num_connections < graph->num_stops)
    {
        printf("• Network has sparse connectivity (%.1f connections per stop) - consider adding more routes\n",
            (double)graph->num_connections / graph->num_stops);
    }

    // Service coverage analysis
    if (graph->num_services < 4)
    {
        printf("• Limited essential services identified - expand service location coverage\n");
    }
}

// Automatically select diverse stops for analysis based on their characteristics
void analyse_diverse_locations(TransportGraph* graph)
{
    if (!graph || graph->num_stops == 0)
        return;

    // Find stops with different characteristics for analysis
    int interchange_stop = -1;
    int isolated_stop = -1;
    int service_stop = -1;
    int regular_stop = -1;

    // Identify different types of stops automatically
    for (int i = 0; i < graph->num_stops; i++)
    {
        BusStop* stop = &graph->stops[i];
        if (stop->id == -1)
            continue; // Skip unused slots

        // Count connections for this stop
        int connections = 0;
        Edge* edge = stop->edges;
        while (edge)
        {
            connections++;
            edge = edge->next;
        }

        // Categorise stops
        if (stop->type == INTERCHANGE && interchange_stop == -1)
        {
            interchange_stop = i;
        }
        else if (connections == 0 && isolated_stop == -1)
        {
            isolated_stop = i;
        }
        else if ((stop->type == HOSPITAL || stop->type == UNIVERSITY || stop->type == SHOPPING) && service_stop == -1)
        {
            service_stop = i;
        }
        else if (stop->type == BUS_STOP && regular_stop == -1)
        {
            regular_stop = i;
        }
    }

    printf("ACCESSIBILITY ANALYSIS\n");

    // analyse selected locations dynamically
    AccessibilityStats all_stats[4];
    const char* location_types[4];
    int analysis_count = 0;

    if (interchange_stop != -1)
    {
        printf("%d. ACCESSIBILITY FROM MAJOR INTERCHANGE:\n", analysis_count + 1);
        all_stats[analysis_count] = analyse_stop_accessibility(graph, interchange_stop);
        print_accessibility_stats(all_stats[analysis_count]);
        location_types[analysis_count] = graph->stops[interchange_stop].name;
        analysis_count++;
    }

    if (service_stop != -1)
    {
        printf("%d. ACCESSIBILITY FROM SERVICE LOCATION:\n", analysis_count + 1);
        all_stats[analysis_count] = analyse_stop_accessibility(graph, service_stop);
        print_accessibility_stats(all_stats[analysis_count]);
        location_types[analysis_count] = graph->stops[service_stop].name;
        analysis_count++;
    }

    if (regular_stop != -1)
    {
        printf("%d. ACCESSIBILITY FROM REGULAR STOP:\n", analysis_count + 1);
        all_stats[analysis_count] = analyse_stop_accessibility(graph, regular_stop);
        print_accessibility_stats(all_stats[analysis_count]);
        location_types[analysis_count] = graph->stops[regular_stop].name;
        analysis_count++;
    }

    if (isolated_stop != -1)
    {
        printf("%d. ACCESSIBILITY FROM ISOLATED STOP:\n", analysis_count + 1);
        all_stats[analysis_count] = analyse_stop_accessibility(graph, isolated_stop);
        print_accessibility_stats(all_stats[analysis_count]);
        location_types[analysis_count] = graph->stops[isolated_stop].name;
        analysis_count++;
    }

    printf("ROUTE PLANNING EXAMPLES\n");

    if (interchange_stop != -1 && service_stop != -1)
    {
        int distances[graph->num_stops], parents[graph->num_stops];
        bfs_shortest_path(graph, interchange_stop, distances, parents);

        printf("Route Example 1: %s to %s\n", graph->stops[interchange_stop].name, graph->stops[service_stop].name);
        print_shortest_path(parents, interchange_stop, service_stop, graph);
    }

    if (regular_stop != -1 && service_stop != -1 && regular_stop != interchange_stop)
    {
        int distances[graph->num_stops], parents[graph->num_stops];
        bfs_shortest_path(graph, regular_stop, distances, parents);

        printf("Route Example 2: %s to %s\n",
            graph->stops[regular_stop].name, graph->stops[service_stop].name);
        print_shortest_path(parents, regular_stop, service_stop, graph);
    }

    printf("NETWORK ANALYSIS SUMMARY\n");
    for (int i = 0; i < analysis_count; i++)
    {
        const char* performance = (all_stats[i].accessibility_score > 40) ? "Excellent" : (all_stats[i].accessibility_score > 20) ? "Good" : (all_stats[i].accessibility_score > 5) ? "Limited" : "Poor";
        printf("• %s: %s connectivity (%.1f/100)\n",
            location_types[i], performance, all_stats[i].accessibility_score);
    }

    printf("\n");

    generate_smart_recommendations(graph, all_stats, analysis_count, location_types);

    printf("\n");
}
