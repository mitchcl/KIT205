/*
 * main_gtfs.c - Main program for real GTFS data analysis
 * KIT205 Assignment 2
 * 
 * Academic Integrity Statement:
 * - GTFS data processing workflow based on transit analysis examples from:
 *   OpenTripPlanner documentation and SUMO traffic simulation guides
 * - Performance measurement using C standard library timing functions
 * - Program flow structure influenced by data processing best practices
 * - Output formatting patterns from C programming tutorials
 * - AI assistance used for:
 *   - Debugging file path handling for GTFS data loading
 *   - Optimising large dataset processing performance
 *   - Fixing memory management issues with real-world data sizes
 *   - Improving error messages and user feedback
 */

#include "graph.h"
#include <time.h>

int main()
{
    printf("=======================================================\n");
    printf("Tasmania Public Transport Analysis - GTFS DATA\n");
    printf("   KIT205 Assignment 2 - Graph Algorithms\n");
    printf("=======================================================\n\n");

    clock_t start_time = clock();

    // Load GTFS data
    TransportGraph* graph = load_gtfs_data("data/gtfs");
    if (!graph)
    {
        printf("Failed to load GTFS data\n");
        return 1;
    }

    printf("\nNETWORK STATISTICS\n");
    printf("Total bus stops: %d\n", graph->num_stops);
    printf("Total connections: %d\n", graph->num_connections);
    printf("Network density: %.3f%%\n",
        (double)graph->num_connections / (graph->num_stops * (graph->num_stops - 1)) * 100);

    printf("\nIDENTIFYING KEY LOCATIONS\n");
    int hobart_interchange = -1, hospital = -1, utas = -1, shopping = -1;

    for (int i = 0; i < graph->num_stops; i++)
    {
        if (graph->stops[i].id == -1)
            continue;

        char* name = graph->stops[i].name;
        if (hobart_interchange == -1 && (strstr(name, "Hobart") && strstr(name, "Interchange")))
        {
            hobart_interchange = i;
            printf("Found Hobart Interchange: %s\n", name);
        }
        if (hospital == -1 && strstr(name, "Hospital"))
        {
            hospital = i;
            printf("Found Hospital: %s\n", name);
        }
        if (utas == -1 && (strstr(name, "University") || strstr(name, "UTAS")))
        {
            utas = i;
            printf("Found University: %s\n", name);
        }
        if (shopping == -1 && (strstr(name, "Shopping") || strstr(name, "Mall")))
        {
            shopping = i;
            printf("Found Shopping: %s\n", name);
        }
    }

    if (hobart_interchange != -1)
    {
        printf("\nACCESSIBILITY FROM HOBART INTERCHANGE\n");

        int* distances = (int*)malloc(graph->num_stops * sizeof(int));
        if (!distances)
        {
            printf("Memory allocation failed for distances array\n");
            free_transport_graph(graph);
            return 1;
        }
        int parents[graph->num_stops];
        bfs_shortest_path(graph, hobart_interchange, distances, parents);

        int reachable = 0;
        int total_distance = 0;
        int max_distance = 0;

        for (int i = 0; i < graph->num_stops; i++)
        {
            if (graph->stops[i].id != -1 && distances[i] != INFINITY)
            {
                reachable++;
                total_distance += distances[i];
                if (distances[i] > max_distance)
                    max_distance = distances[i];
            }
        }

        printf("Reachable stops: %d (%.1f%%)\n", reachable,
            (double)reachable / graph->num_stops * 100);
        printf("Average transfers: %.2f\n", (double)total_distance / reachable);
        printf("Maximum transfers: %d\n", max_distance);

        // Show paths to key services
        if (hospital != -1 && distances[hospital] != INFINITY)
        {
            printf("\nRoute to Hospital: %d transfers\n", distances[hospital]);
        }
        if (utas != -1 && distances[utas] != INFINITY)
        {
            printf("Route to University: %d transfers\n", distances[utas]);
        }
        if (shopping != -1 && distances[shopping] != INFINITY)
        {
            printf("Route to Shopping: %d transfers\n", distances[shopping]);
        }
    }

    printf("\nNETWORK CONNECTIVITY ANALYSIS\n");

    int isolated_stops = 0;
    int well_connected_stops = 0; // More than 5 connections
    int hub_stops = 0;            // More than 20 connections

    for (int i = 0; i < graph->num_stops; i++)
    {
        if (graph->stops[i].id == -1)
            continue;

        int connections = 0;
        Edge* edge = graph->stops[i].edges;
        while (edge)
        {
            connections++;
            edge = edge->next;
        }

        if (connections == 0)
            isolated_stops++;
        else if (connections > 20)
            hub_stops++;
        else if (connections > 5)
            well_connected_stops++;
    }

    printf("Isolated stops (0 connections): %d\n", isolated_stops);
    printf("Well-connected stops (6-20 connections): %d\n", well_connected_stops);
    printf("Hub stops (20+ connections): %d\n", hub_stops);

    printf("\nTOP NETWORK HUBS\n");

    typedef struct
    {
        int stop_index;
        int connections;
    } HubInfo;

    HubInfo top_hubs[10] = { 0 };

    for (int i = 0; i < graph->num_stops; i++)
    {
        if (graph->stops[i].id == -1)
            continue;

        int connections = 0;
        Edge* edge = graph->stops[i].edges;
        while (edge)
        {
            connections++;
            edge = edge->next;
        }

        // Insert into top 10
        for (int j = 0; j < 10; j++)
        {
            if (connections > top_hubs[j].connections)
            {
                // Shift others down
                for (int k = 9; k > j; k--)
                {
                    top_hubs[k] = top_hubs[k - 1];
                }
                top_hubs[j].stop_index = i;
                top_hubs[j].connections = connections;
                break;
            }
        }
    }

    for (int i = 0; i < 10; i++)
    {
        if (top_hubs[i].connections > 0)
        {
            int idx = top_hubs[i].stop_index;
            printf("%d. %s (%d connections)\n", i + 1,
                graph->stops[idx].name, top_hubs[i].connections);
        }
    }

    // Performance timing
    clock_t end_time = clock();
    double cpu_time = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;

    printf("\nPERFORMANCE METRICS\n");
    printf("=====================================\n");
    printf("Analysis completed in %.2f seconds\n", cpu_time);
    printf("Memory usage: ~%.1f MB\n",
        (sizeof(TransportGraph) + graph->num_stops * sizeof(BusStop)) / 1024.0 / 1024.0);

    // Generate comprehensive network report
    printf("GENERATING COMPREHENSIVE NETWORK REPORT\n");
    generate_network_report(graph, "data/results/network_analysis_report.txt");

    printf("SERVICE FREQUENCY DISTRIBUTION\n");
    printf("=====================================\n");
    ServiceFrequencyAnalysis freq_analysis = analyse_service_frequency(graph);
    printf("High frequency stops (>10 connections): %d\n", freq_analysis.high_frequency_stops);
    printf("Medium frequency stops (5-10 connections): %d\n", freq_analysis.medium_frequency_stops);
    printf("Low frequency stops (1-4 connections): %d\n", freq_analysis.low_frequency_stops);
    printf("Isolated stops (0 connections): %d\n", freq_analysis.isolated_stops);
    if (freq_analysis.isolated_stops > 0)
    {
        printf("  → Isolated stops: %s\n", freq_analysis.isolated_stop_names);
    }
    printf("Average connections per stop: %.2f\n", freq_analysis.avg_connections_per_stop);
    printf("  (Note: %.1f bidirectional connections, as each route creates 2-way travel)\n",
        freq_analysis.avg_connections_per_stop / 2.0);
    printf("\n");

    printf("INSIGHTS FOR TRANSPORT PLANNING:\n");
    if (freq_analysis.isolated_stops > 0)
    {
        printf("• Network has %d isolated stop%s that may need better connections\n",
            freq_analysis.isolated_stops,
            (freq_analysis.isolated_stops == 1) ? "" : "s");
    }
    printf("• %d hub stops serve as critical interchange points\n", freq_analysis.high_frequency_stops);

    // Calculate network connectivity metrics to assess reachability
    NetworkMetrics metrics = calculate_network_metrics(graph);
    double connectivity_ratio = (double)metrics.total_reachable_pairs /
        (graph->num_stops * (graph->num_stops - 1));
    double isolated_ratio = (double)freq_analysis.isolated_stops / graph->num_stops;

    if (connectivity_ratio > 0.6 && isolated_ratio < 0.1 && freq_analysis.avg_connections_per_stop > 3.0)
    {
        printf("• High network connectivity enables most destinations to be reached efficiently\n");
    }
    else if (connectivity_ratio > 0.4 && isolated_ratio < 0.2)
    {
        printf("• Moderate network connectivity allows many destinations to be reached\n");
    }
    else
    {
        printf("• Limited network connectivity may restrict access to some destinations\n");
    }
    printf("  (%.1f%% of stop pairs connected, %.1f avg bidirectional connections per stop)\n",
        connectivity_ratio * 100, freq_analysis.avg_connections_per_stop / 2.0);

    // Cleanup
    free_transport_graph(graph);

    return 0;
}
