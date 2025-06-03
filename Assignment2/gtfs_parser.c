/**
 * GTFS Data Parser
 * Reads and processes GTFS CSV files
 */

#include "graph.h"

/**
 * Parse a line from stops.txt
 */
int parse_stop_line(const char* line, int* stop_id, char* name, double* lat, double* lon, char* suburb) {
    if (!line || !stop_id || !name || !lat || !lon || !suburb) {
        return -1;
    }
    
    // Simple CSV parsing - assumes comma-separated values
    int result = sscanf(line, "%d,%[^,],%lf,%lf,%s", stop_id, name, lat, lon, suburb);
    
    return (result == 5) ? 0 : -1;
}

/**
 * Load stops from GTFS stops.txt file
 */
int load_stops_from_file(TransportGraph* graph, const char* filename) {
    if (!graph || !filename) return -1;
    
    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: Cannot open file %s\n", filename);
        return -1;
    }
    
    char line[512];
    char name[MAX_NAME_LENGTH];
    char suburb[MAX_SUBURB_LENGTH];
    int stop_id;
    double lat, lon;
    int stops_loaded = 0;
    
    // Skip header line
    if (fgets(line, sizeof(line), file)) {
        // Continue to data lines
    }
    
    while (fgets(line, sizeof(line), file)) {
        // Remove newline
        line[strcspn(line, "\n")] = 0;
        
        if (parse_stop_line(line, &stop_id, name, &lat, &lon, suburb) == 0) {
            if (add_bus_stop(graph, stop_id, name, lat, lon, suburb) >= 0) {
                stops_loaded++;
            }
        }
    }
    
    fclose(file);
    printf("Loaded %d stops from %s\n", stops_loaded, filename);
    return stops_loaded;
}
