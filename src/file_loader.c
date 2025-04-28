#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "airline_types.h"
#include "file_loader.h"

#define MAX_LINE_LENGTH 256
#define DATE_FORMAT "%Y-%m-%d %H:%M:%S"

// Helper function to strip newline characters
static void strip_newline(char* str) {
    char* pos;
    if ((pos = strchr(str, '\n')) != NULL) {
        *pos = '\0';
    }
    if ((pos = strchr(str, '\r')) != NULL) {
        *pos = '\0';
    }
}

// Helper function to parse a date string into time_t
static time_t parse_date(const char* date_str) {
    struct tm tm = {0};
    if (strptime(date_str, DATE_FORMAT, &tm) == NULL) {
        return 0;
    }
    return mktime(&tm);
}

// Load flights from a CSV file
Flight* load_flights(const char* filename, int* count) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Failed to open file: %s\n", filename);
        *count = 0;
        return NULL;
    }
    
    // Count lines to determine array size
    int line_count = 0;
    char buffer[MAX_LINE_LENGTH];
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        line_count++;
    }
    
    // Adjust for header line
    line_count--;
    
    // Rewind file to beginning
    rewind(file);
    
    // Skip header line
    fgets(buffer, sizeof(buffer), file);
    
    // Allocate memory for flights
    Flight* flights = (Flight*)malloc(line_count * sizeof(Flight));
    if (flights == NULL) {
        fprintf(stderr, "Memory allocation failed for flights\n");
        fclose(file);
        *count = 0;
        return NULL;
    }
    
    // Read flight data
    int i = 0;
    while (fgets(buffer, sizeof(buffer), file) != NULL && i < line_count) {
        strip_newline(buffer);
        
        char date_str[MAX_LINE_LENGTH];
        sscanf(buffer, "%d,%[^,],%[^,],%[^,],%[^,],%d",
               &flights[i].id,
               flights[i].flightNumber,
               flights[i].origin,
               flights[i].destination,
               date_str,
               &flights[i].capacity);
               
        flights[i].departureTime = parse_date(date_str);
        i++;
    }
    
    fclose(file);
    *count = line_count;
    return flights;
}

// Load passengers from a CSV file
Flight* load_passengers(const char* filename, int* count) {
    // Similar implementation as load_flights
    *count = 0;
    return NULL;
}

// Load reservation records from a CSV file
ReservationRecord* load_reservations(const char* filename, int* count) {
    // Similar implementation as load_flights
    *count = 0;
    return NULL;
}
