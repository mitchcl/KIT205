/*
 * gtfs_parser.c - GTFS data parser for real transit data
 * KIT205 Assignment 2
 * 
 * Academic Integrity Statement:
 * - GTFS file format specification from Google Transit documentation:
 *   https://developers.google.com/transit/gtfs/reference
 * - CSV parsing techniques referenced from Stack Overflow discussions
 * - File I/O patterns from "The C Programming Language" by Kernighan & Ritchie
 * - String manipulation functions from C standard library documentation
 * - AI assistance used for:
 *   - Debugging CSV parsing edge cases (quoted fields, escaped commas)
 *   - Optimising file reading performance for large datasets
 *   - Fixing buffer overflow issues in string handling
 *   - Error handling for malformed GTFS files
 */

#define _GNU_SOURCE
#include "graph.h"

// GTFS parser for transport network analysis

// CSV parser for GTFS files
int parse_csv_line(char* line, char* tokens[], int max_tokens)
{
    int token_count = 0;
    char* ptr = line;

    while (*ptr && token_count < max_tokens)
    {
        // Skip whitespace
        while (*ptr == ' ' || *ptr == '\t')
            ptr++;

        if (*ptr == '"')
        {
            // Handle quoted field
            ptr++; // Skip opening quote
            tokens[token_count] = ptr;
            while (*ptr && *ptr != '"')
                ptr++;
            if (*ptr)
                *ptr++ = '\0'; // Replace closing quote with null
            if (*ptr == ',')
                ptr++; // Skip comma
        }
        else
        {
            // Handle unquoted field
            tokens[token_count] = ptr;
            while (*ptr && *ptr != ',' && *ptr != '\n' && *ptr != '\r')
                ptr++;
            if (*ptr)
                *ptr++ = '\0'; // Replace delimiter with null
        }
        token_count++;
    }

    return token_count;
}

void parse_gtfs_stops(TransportGraph* graph, const char* stops_file)
{
    FILE* file = fopen(stops_file, "r");
    if (!file)
    {
        printf("Error: Cannot open stops file %s\n", stops_file);
        return;
    }

    char line[512];
    // Skip header line
    if (!fgets(line, sizeof(line), file))
    {
        fclose(file);
        return;
    }

    int stops_added = 0;
    while (fgets(line, sizeof(line), file) && stops_added < MAX_STOPS - 1)
    {
        char* tokens[11];
        int token_count = parse_csv_line(line, tokens, 11);

        if (token_count >= 6)
        {
            int stop_id = atoi(tokens[0]);
            char* stop_name = tokens[2];
            double lat = atof(tokens[4]);
            double lng = atof(tokens[5]);

            // Clean up stop name (remove quotes)
            if (stop_name && stop_name[0] == '"')
            {
                stop_name++;
                char* end_quote = strchr(stop_name, '"');
                if (end_quote)
                    *end_quote = '\0';
            }

            // Determine stop type
            StopType type = BUS_STOP;
            if (strstr(stop_name, "Interchange") || strstr(stop_name, "Transit Centre"))
            {
                type = INTERCHANGE;
            }
            else if (strstr(stop_name, "Hospital"))
            {
                type = HOSPITAL;
            }
            else if (strstr(stop_name, "University") || strstr(stop_name, "UTAS") || strstr(stop_name, "College"))
            {
                type = UNIVERSITY;
            }
            else if (strstr(stop_name, "Shopping") || strstr(stop_name, "Mall"))
            {
                type = SHOPPING;
            }

            // Extract suburb from stop name
            char suburb[50] = "Unknown";
            if (strstr(stop_name, "Hobart"))
                strcpy(suburb, "Hobart");
            else if (strstr(stop_name, "Sandy Bay"))
                strcpy(suburb, "Sandy Bay");
            else if (strstr(stop_name, "Glenorchy"))
                strcpy(suburb, "Glenorchy");
            else if (strstr(stop_name, "Kingston"))
                strcpy(suburb, "Kingston");
            else if (strstr(stop_name, "Rosny"))
                strcpy(suburb, "Rosny Park");
            else if (strstr(stop_name, "New Town"))
                strcpy(suburb, "New Town");
            else if (strstr(stop_name, "Moonah"))
                strcpy(suburb, "Moonah");
            else if (strstr(stop_name, "Lindisfarne"))
                strcpy(suburb, "Lindisfarne");
            else if (strstr(stop_name, "Claremont"))
                strcpy(suburb, "Claremont");
            else if (strstr(stop_name, "Brighton"))
                strcpy(suburb, "Brighton");
            else if (strstr(stop_name, "Bellerive"))
                strcpy(suburb, "Bellerive");

            if (add_bus_stop(graph, stop_id, type, stop_name, suburb, lat, lng))
            {
                stops_added++;
                if (stops_added % 500 == 0)
                {
                    printf("Loaded %d stops...\n", stops_added);
                }
            }
        }
    }

    fclose(file);
    printf("✓ Loaded %d bus stops from GTFS data\n", stops_added);
}

void parse_gtfs_connections(TransportGraph* graph, const char* stop_times_file)
{
    FILE* file = fopen(stop_times_file, "r");
    if (!file)
    {
        printf("Error: Cannot open stop_times file %s\n", stop_times_file);
        return;
    }

    char line[256];
    // Skip header
    if (!fgets(line, sizeof(line), file))
    {
        fclose(file);
        return;
    }

    char current_trip[64] = "";
    int prev_stop = -1;
    int connections_added = 0;
    int lines_processed = 0;

    while (fgets(line, sizeof(line), file))
    {
        lines_processed++;
        if (lines_processed % 10000 == 0)
        {
            printf("Processed %d stop times, added %d connections\n", lines_processed, connections_added);
        }

        char* tokens[6];
        int token_count = parse_csv_line(line, tokens, 6);

        if (token_count >= 5)
        {
            char* trip_id = tokens[0];
            char* stop_id_str = tokens[3];

            if (trip_id && stop_id_str)
            {
                int stop_id = atoi(stop_id_str);

                // Check if this is a new trip
                if (strcmp(current_trip, trip_id) != 0)
                {
                    strcpy(current_trip, trip_id);
                    prev_stop = stop_id;
                    continue;
                }

                // Add connection from previous stop to current stop
                if (prev_stop != -1 && prev_stop != stop_id)
                {
                    int from_idx = find_stop_index(graph, prev_stop);
                    int to_idx = find_stop_index(graph, stop_id);

                    if (from_idx != -1 && to_idx != -1)
                    {
                        // Extract route number from trip_id
                        int route_id = atoi(trip_id);
                        int travel_time = 5; // Default 5 minutes

                        if (add_bus_connection(graph, prev_stop, stop_id, travel_time, route_id))
                        {
                            connections_added++;
                        }
                        // Add reverse connection for bidirectional travel
                        add_bus_connection(graph, stop_id, prev_stop, travel_time, route_id);
                    }
                }

                prev_stop = stop_id;
            }
        }
    }

    fclose(file);
    printf("✓ Added %d bus connections from GTFS data\n", connections_added);
}

TransportGraph* load_gtfs_data(const char* gtfs_directory)
{
    printf("🚌 Loading Metro Tasmania GTFS data...\n");

    TransportGraph* graph = create_transport_graph(4000);
    if (!graph)
    {
        printf("Error: Failed to create transport graph\n");
        return NULL;
    }

    char stops_file[256], stop_times_file[256];
    snprintf(stops_file, sizeof(stops_file), "%s/stops.txt", gtfs_directory);
    snprintf(stop_times_file, sizeof(stop_times_file), "%s/stop_times.txt", gtfs_directory);

    parse_gtfs_stops(graph, stops_file);
    parse_gtfs_connections(graph, stop_times_file);

    printf("🎉 GTFS data loading complete!\n");
    printf("   Total stops: %d\n", graph->num_stops);
    printf("   Total connections: %d\n", graph->num_connections);

    return graph;
}
