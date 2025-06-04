/**
 * Main Transport Analysis Program
 */

#include "graph.h"
#include <getopt.h>

void print_usage()
{
    printf("Tasmania Transport Analysis Tool\n");
    printf("Usage: transport_analysis [OPTIONS]\n");
    printf("Options:\n");
    printf("  -h, --help           Show this help message\n");
    printf("  -d, --data DIR       Specify GTFS data directory\n");
    printf("  -s, --services FILE  Specify essential services file\n");
    printf("  -o, --output DIR     Specify output directory\n");
    printf("  -m, --mode MODE      Analysis mode (basic|accessibility|critical)\n");
    printf("  -v, --verbose        Enable verbose output\n");
}

int main(int argc, char* argv[])
{
    printf("Tasmania Public Transport Accessibility Analysis\n");
    printf("===============================================\n\n");

    // Default parameters
    char* data_dir = "data/gtfs/";
    char* services_file = "data/services/essential_services.csv";
    char* output_dir = "data/results/";
    char* mode = "basic";
    bool verbose = false;

    // Parse command line arguments
    int opt;
    static struct option long_options[] = {
        {"help", no_argument, 0, 'h'},
        {"data", required_argument, 0, 'd'},
        {"services", required_argument, 0, 's'},
        {"output", required_argument, 0, 'o'},
        {"mode", required_argument, 0, 'm'},
        {"verbose", no_argument, 0, 'v'},
        {0, 0, 0, 0} };

    while ((opt = getopt_long(argc, argv, "hd:s:o:m:v", long_options, NULL)) != -1)
    {
        switch (opt)
        {
        case 'h':
            print_usage();
            return 0;
        case 'd':
            data_dir = optarg;
            break;
        case 's':
            services_file = optarg;
            break;
        case 'o':
            output_dir = optarg;
            break;
        case 'm':
            mode = optarg;
            break;
        case 'v':
            verbose = true;
            break;
        default:
            print_usage();
            return 1;
        }
    }

    // Create transport graph
    if (verbose)
        printf("Creating transport graph...\n");
    TransportGraph* graph = create_transport_graph(3000);
    if (!graph)
    {
        fprintf(stderr, "Failed to create transport graph\n");
        return 1;
    }

    // Load GTFS data
    if (verbose)
        printf("Loading GTFS data from %s...\n", data_dir);
    char stops_file[256];
    snprintf(stops_file, sizeof(stops_file), "%s/stops.txt", data_dir);

    int stops_loaded = load_stops_from_file(graph, stops_file);
    if (stops_loaded <= 0)
    {
        fprintf(stderr, "Failed to load stops data\n");
        free_transport_graph(graph);
        return 1;
    }

    // Load essential services
    if (verbose)
        printf("Loading essential services...\n");
    load_services_from_file(graph, services_file);

    // Perform analysis based on mode
    if (strcmp(mode, "basic") == 0)
    {
        printf("=== BASIC NETWORK ANALYSIS ===\n");
        print_graph_stats(graph);
    }
    else if (strcmp(mode, "accessibility") == 0)
    {
        printf("=== ACCESSIBILITY ANALYSIS ===\n");
        analyze_network_accessibility(graph);
    }
    else if (strcmp(mode, "critical") == 0)
    {
        printf("=== CRITICAL INFRASTRUCTURE ANALYSIS ===\n");
        int num_critical;
        int* critical_stops = find_critical_stops(graph, &num_critical);

        printf("Found %d critical stops:\n", num_critical);
        for (int i = 0; i < num_critical; i++)
        {
            int stop_index = find_stop_index(graph, critical_stops[i]);
            if (stop_index != -1)
            {
                printf("- %s (Stop ID: %d)\n",
                    graph->stops[stop_index].name, critical_stops[i]);
            }
        }
        free(critical_stops);
    }
    else
    {
        fprintf(stderr, "Unknown analysis mode: %s\n", mode);
        print_usage();
        free_transport_graph(graph);
        return 1;
    }

    // Clean up
    free_transport_graph(graph);

    printf("\nAnalysis completed successfully.\n");
    return 0;
}
