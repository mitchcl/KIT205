#include "graph.h"
#include <time.h>

// Enhanced network analysis functions

// analyse service frequency distribution
ServiceFrequencyAnalysis analyse_service_frequency(TransportGraph* graph)
{
    ServiceFrequencyAnalysis analysis = { 0 };
    int max_connections = 0;
    int max_stop_index = -1;
    int total_connections = 0;

    // Initialize isolated stop names string
    analysis.isolated_stop_names[0] = '\0';

    for (int i = 0; i < graph->num_stops; i++)
    {
        // Count connections for this stop
        int connections = 0;
        Edge* edge = graph->stops[i].edges;
        while (edge)
        {
            connections++;
            edge = edge->next;
        }

        total_connections += connections;

        if (connections > max_connections)
        {
            max_connections = connections;
            max_stop_index = i;
        }

        if (connections == 0)
        {
            analysis.isolated_stops++;
            // Add this stop's name to the isolated stops list
            if (strlen(analysis.isolated_stop_names) > 0)
            {
                strncat(analysis.isolated_stop_names, "; ", 498 - strlen(analysis.isolated_stop_names));
            }
            strncat(analysis.isolated_stop_names, graph->stops[i].name, 498 - strlen(analysis.isolated_stop_names));
        }
        else if (connections <= 4)
        {
            analysis.low_frequency_stops++;
        }
        else if (connections <= 10)
        {
            analysis.medium_frequency_stops++;
        }
        else
        {
            analysis.high_frequency_stops++;
        }
    }

    analysis.avg_connections_per_stop = (double)total_connections / graph->num_stops;
    analysis.max_connections = max_connections;

    if (max_stop_index >= 0)
    {
        strncpy(analysis.busiest_stop, graph->stops[max_stop_index].name, 99);
        analysis.busiest_stop[99] = '\0';
    }

    return analysis;
}

// Calculate cumulative opportunities from a specific stop
CumulativeOpportunities calculate_cumulative_opportunities(TransportGraph* graph, int start_stop)
{
    CumulativeOpportunities opportunities = { 0 };

    if (start_stop < 0 || start_stop >= graph->num_stops)
    {
        return opportunities;
    }

    int* distances = malloc(graph->num_stops * sizeof(int));
    int* parents = malloc(graph->num_stops * sizeof(int));
    if (!distances || !parents)
    {
        free(distances);
        free(parents);
        return opportunities;
    }

    bfs_shortest_path(graph, start_stop, distances, parents);

    for (int i = 0; i < graph->num_stops; i++)
    {
        if (distances[i] != -1)
        {
            opportunities.total_reachable++;
            if (distances[i] <= 1)
                opportunities.within_1_transfer++;
            if (distances[i] <= 2)
                opportunities.within_2_transfers++;
            if (distances[i] <= 3)
                opportunities.within_3_transfers++;
            if (distances[i] <= 5)
                opportunities.within_5_transfers++;
        }
    }

    free(distances);
    free(parents);
    return opportunities;
}

// Calculate basic network metrics
NetworkMetrics calculate_network_metrics(TransportGraph* graph)
{
    NetworkMetrics metrics = { 0 };
    int total_paths = 0;
    int total_distance = 0;
    int max_distance = 0;

    printf("Calculating network metrics (this may take a moment)...\n");

    // Sample analysis from key hubs to avoid O(n²) complexity
    int sample_size = (graph->num_stops > 100) ? 50 : graph->num_stops;
    int step = graph->num_stops / sample_size;
    if (step < 1)
        step = 1;

    for (int i = 0; i < graph->num_stops; i += step)
    {
        int* distances = malloc(graph->num_stops * sizeof(int));
        int* parents = malloc(graph->num_stops * sizeof(int));
        if (!distances || !parents)
        {
            free(distances);
            free(parents);
            continue;
        }

        bfs_shortest_path(graph, i, distances, parents);

        for (int j = 0; j < graph->num_stops; j++)
        {
            if (distances[j] != -1 && i != j)
            {
                total_paths++;
                total_distance += distances[j];
                if (distances[j] > max_distance)
                {
                    max_distance = distances[j];
                }
            }
        }

        free(distances);
        free(parents);
    }

    if (total_paths > 0)
    {
        metrics.avg_shortest_path = (double)total_distance / total_paths;
        metrics.network_efficiency = 1.0 / metrics.avg_shortest_path;
        metrics.total_reachable_pairs = total_paths;
    }

    metrics.diameter = max_distance;

    return metrics;
}

// Generate a comprehensive network report
void generate_network_report(TransportGraph* graph, const char* output_file)
{
    FILE* report = fopen(output_file, "w");
    if (!report)
    {
        printf("Warning: Could not create network report file\n");
        return;
    }

    time_t now = time(NULL);
    char* date_str = ctime(&now);

    fprintf(report, "=======================================================\n");
    fprintf(report, "TASMANIA TRANSPORT NETWORK ANALYSIS REPORT\n");
    fprintf(report, "Generated: %s", date_str);
    fprintf(report, "=======================================================\n\n");

    // Basic network statistics
    fprintf(report, "NETWORK OVERVIEW\n");
    fprintf(report, "----------------\n");
    fprintf(report, "Total bus stops: %d\n", graph->num_stops);
    fprintf(report, "Total connections: %d\n", graph->num_connections);
    fprintf(report, "Network density: %.3f%%\n",
        (double)(graph->num_connections * 100) / (graph->num_stops * graph->num_stops));
    fprintf(report, "\n");

    // Service frequency analysis
    ServiceFrequencyAnalysis freq_analysis = analyse_service_frequency(graph);
    fprintf(report, "SERVICE FREQUENCY ANALYSIS\n");
    fprintf(report, "-------------------------\n");
    fprintf(report, "High frequency stops (>10 connections): %d\n", freq_analysis.high_frequency_stops);
    fprintf(report, "Medium frequency stops (5-10 connections): %d\n", freq_analysis.medium_frequency_stops);
    fprintf(report, "Low frequency stops (1-4 connections): %d\n", freq_analysis.low_frequency_stops);
    fprintf(report, "Isolated stops (0 connections): %d\n", freq_analysis.isolated_stops);
    fprintf(report, "Average connections per stop: %.2f\n", freq_analysis.avg_connections_per_stop);
    fprintf(report, "Busiest stop: %s (%d connections)\n",
        freq_analysis.busiest_stop, freq_analysis.max_connections);
    fprintf(report, "\n");

    // Find key interchanges for accessibility analysis
    int interchange_indices[3] = { -1, -1, -1 };
    for (int i = 0; i < graph->num_stops && interchange_indices[2] == -1; i++)
    {
        // Count connections for this stop
        int connections = 0;
        Edge* edge = graph->stops[i].edges;
        while (edge)
        {
            connections++;
            edge = edge->next;
        }

        if (strstr(graph->stops[i].name, "Interchange") ||
            strstr(graph->stops[i].name, "interchange") ||
            connections > 20)
        {
            if (interchange_indices[0] == -1)
                interchange_indices[0] = i;
            else if (interchange_indices[1] == -1)
                interchange_indices[1] = i;
            else
                interchange_indices[2] = i;
        }
    }

    // Accessibility analysis from key locations
    fprintf(report, "ACCESSIBILITY ANALYSIS\n");
    fprintf(report, "---------------------\n");

    for (int i = 0; i < 3; i++)
    {
        if (interchange_indices[i] != -1)
        {
            CumulativeOpportunities opp = calculate_cumulative_opportunities(graph, interchange_indices[i]);
            fprintf(report, "From %s:\n", graph->stops[interchange_indices[i]].name);
            fprintf(report, "  Reachable within 1 transfer: %d stops (%.1f%%)\n",
                opp.within_1_transfer, (double)opp.within_1_transfer * 100 / graph->num_stops);
            fprintf(report, "  Reachable within 2 transfers: %d stops (%.1f%%)\n",
                opp.within_2_transfers, (double)opp.within_2_transfers * 100 / graph->num_stops);
            fprintf(report, "  Reachable within 3 transfers: %d stops (%.1f%%)\n",
                opp.within_3_transfers, (double)opp.within_3_transfers * 100 / graph->num_stops);
            fprintf(report, "  Total reachable: %d stops (%.1f%%)\n",
                opp.total_reachable, (double)opp.total_reachable * 100 / graph->num_stops);
            fprintf(report, "\n");
        }
    }

    // Network efficiency metrics
    NetworkMetrics metrics = calculate_network_metrics(graph);
    fprintf(report, "NETWORK EFFICIENCY METRICS\n");
    fprintf(report, "-------------------------\n");
    fprintf(report, "Average shortest path length: %.2f transfers\n", metrics.avg_shortest_path);
    fprintf(report, "Network diameter: %d transfers\n", metrics.diameter);
    fprintf(report, "Network efficiency: %.4f\n", metrics.network_efficiency);
    fprintf(report, "analysed path pairs: %d\n", metrics.total_reachable_pairs);
    fprintf(report, "\n");

    // Recommendations
    fprintf(report, "RECOMMENDATIONS\n");
    fprintf(report, "---------------\n");

    if (freq_analysis.isolated_stops > 0)
    {
        fprintf(report, "• %d isolated stops need connections to the network:\n", freq_analysis.isolated_stops);
        fprintf(report, "  %s\n", freq_analysis.isolated_stop_names);
    }

    if (freq_analysis.low_frequency_stops > freq_analysis.high_frequency_stops)
    {
        fprintf(report, "• Many stops have low service frequency - consider route optimisation\n");
    }

    if (metrics.avg_shortest_path > 3.0)
    {
        fprintf(report, "• Average path length is high - consider express services or direct routes\n");
    }

    if (freq_analysis.avg_connections_per_stop < 5.0)
    {
        fprintf(report, "• Low overall connectivity - consider increasing route density\n");
    }

    fprintf(report, "• Focus on maintaining high-frequency service at hub stops\n");
    fprintf(report, "• Consider accessibility from different geographic areas\n");

    fprintf(report, "\n=======================================================\n");
    fprintf(report, "End of Report\n");
    fprintf(report, "=======================================================\n");

    fclose(report);
    printf("Network analysis report saved to %s\n", output_file);
}
