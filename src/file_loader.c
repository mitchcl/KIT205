/*
 * File Loader Implementation
 * 
 * This file implements functions for loading data from CSV files into memory structures.
 * 
 * Sources used:
 * 1. The C Programming Language (K&R) - File I/O and string manipulation
 * 2. "C Interfaces and Implementations" by David R. Hanson - CSV parsing techniques
 * 3. "C: The Complete Reference" by Herbert Schildt - File handling best practices
 */

#define _XOPEN_SOURCE // Required for strptime
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "airline_types.h"
#include "file_loader.h"

#define DATE_FORMAT "%Y-%m-%d %H:%M:%S"
#define BUFFER_SIZE 4096

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
        fprintf(stderr, "Error parsing date: %s\n", date_str);
        return time(NULL); // Return current time if parsing fails
    }
    return mktime(&tm);
}

// Estimate the number of records in a CSV file without loading it entirely
int estimate_csv_record_count(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening file for estimation: %s\n", filename);
        return -1;
    }

    // Get the total file size
    fseek(file, 0, SEEK_END);
    long long total_size = ftell(file);
    fseek(file, 0, SEEK_SET);

    // Skip header line
    char buffer[BUFFER_SIZE];
    if (fgets(buffer, sizeof(buffer), file) == NULL) {
        fclose(file);
        return 0;
    }

    // Count lines in the sample (up to 1MB)
    long long sample_size = 0;
    int lines_in_sample = 0;
    int max_sample_bytes = 1024 * 1024; // 1MB sample
    size_t bytes_read;
    while ((bytes_read = fread(buffer, 1, sizeof(buffer), file)) > 0) {
        for (size_t i = 0; i < bytes_read; i++) {
            if (buffer[i] == '\n') {
                lines_in_sample++;
            }
        }
        sample_size += bytes_read;
        if (sample_size >= max_sample_bytes) break;
    }

    fclose(file);

    // If the sample is the whole file, just return the count
    if (sample_size == 0 || total_size <= sample_size) {
        return lines_in_sample;
    }

    // Estimate total lines based on the ratio of sample lines to sample size
    if (sample_size > 0 && lines_in_sample > 0) {
        double lines_per_byte = (double)lines_in_sample / sample_size;
        int estimated_lines = (int)(lines_per_byte * total_size);
        return estimated_lines;
    }

    return 0;
}

// Memory-optimized data loading for huge datasets
int load_data_optimized(const char* flights_file, const char* passengers_file,
                       const char* reservations_file,
                       Flight** flights, int* flight_count,
                       Passenger** passengers, int* passenger_count,
                       ReservationRecord** reservations, int* reservation_count) {
    printf("Starting memory-optimized loading for large datasets...\n");
    
    // Estimate record counts first to allocate properly
    int est_flights = estimate_csv_record_count(flights_file);
    int est_passengers = estimate_csv_record_count(passengers_file);
    int est_reservations = estimate_csv_record_count(reservations_file);
    
    printf("Estimated record counts: %d flights, %d passengers, %d reservations\n",
           est_flights, est_passengers, est_reservations);
           
    // If estimates are negative, fall back to standard loading
    if (est_flights < 0 || est_passengers < 0 || est_reservations < 0) {
        printf("Estimation failed, falling back to standard loading\n");
        *flights = load_flights(flights_file, flight_count);
        *passengers = load_passengers(passengers_file, passenger_count);
        *reservations = load_reservations(reservations_file, reservation_count);
        
        return (*flights != NULL && *passengers != NULL && *reservations != NULL) ? 1 : 0;
    }
    
    // Add a safety margin to estimates (10%)
    est_flights = (int)(est_flights * 1.1) + 10;
    est_passengers = (int)(est_passengers * 1.1) + 10;
    est_reservations = (int)(est_reservations * 1.1) + 10;
    
    // Load data with optimized pre-allocation
    printf("Pre-allocating memory for %d flights...\n", est_flights);
    *flights = (Flight*)malloc(est_flights * sizeof(Flight));
    if (*flights == NULL) {
        fprintf(stderr, "Memory allocation failed for flights array\n");
        return 0;
    }
    
    printf("Pre-allocating memory for %d passengers...\n", est_passengers);
    *passengers = (Passenger*)malloc(est_passengers * sizeof(Passenger));
    if (*passengers == NULL) {
        fprintf(stderr, "Memory allocation failed for passengers array\n");
        free(*flights);
        *flights = NULL;
        return 0;
    }
    
    printf("Pre-allocating memory for %d reservations...\n", est_reservations);
    *reservations = (ReservationRecord*)malloc(est_reservations * sizeof(ReservationRecord));
    if (*reservations == NULL) {
        fprintf(stderr, "Memory allocation failed for reservations array\n");
        free(*flights);
        free(*passengers);
        *flights = NULL;
        *passengers = NULL;
        return 0;
    }
    
    // Now load the actual data into pre-allocated arrays
    // Load flights
    FILE* file = fopen(flights_file, "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening flights file: %s\n", flights_file);
        goto cleanup_and_fail;
    }
    
    char line[1024];
    // Skip header line
    fgets(line, sizeof(line), file);
    
    // Read flight data
    int i = 0;
    while (i < est_flights && fgets(line, sizeof(line), file) != NULL) {
        strip_newline(line);
        
        char departure_str[32];
        
        // Parse line (CSV format: id,flightNumber,origin,destination,departureTime,capacity)
        if (sscanf(line, "%d,%[^,],%[^,],%[^,],%[^,],%d", 
                &(*flights)[i].id, (*flights)[i].flightNumber, 
                (*flights)[i].origin, (*flights)[i].destination, 
                departure_str, &(*flights)[i].capacity) == 6) {
            
            (*flights)[i].departureTime = parse_date(departure_str);
            i++;
        } else {
            fprintf(stderr, "Error parsing flight data: %s\n", line);
        }
    }
    *flight_count = i;
    fclose(file);
    
    // Load passengers
    file = fopen(passengers_file, "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening passengers file: %s\n", passengers_file);
        goto cleanup_and_fail;
    }
    
    // Skip header line
    fgets(line, sizeof(line), file);
    
    // Read passenger data
    i = 0;
    while (i < est_passengers && fgets(line, sizeof(line), file) != NULL) {
        strip_newline(line);
        
        // Parse line (CSV format: id,name,passportNumber)
        if (sscanf(line, "%d,%[^,],%s", 
                &(*passengers)[i].id, (*passengers)[i].name, (*passengers)[i].passportNumber) == 3) {
            i++;
        } else {
            fprintf(stderr, "Error parsing passenger data: %s\n", line);
        }
    }
    *passenger_count = i;
    fclose(file);
    
    // Load reservations
    file = fopen(reservations_file, "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening reservations file: %s\n", reservations_file);
        goto cleanup_and_fail;
    }
    
    // Skip header line
    fgets(line, sizeof(line), file);
    
    // Read reservation data
    i = 0;
    while (i < est_reservations && fgets(line, sizeof(line), file) != NULL) {
        strip_newline(line);
        
        char booking_date_str[32];
        
        // Parse line (CSV format: flightId,passengerId,bookingDate,seatNumber)
        if (sscanf(line, "%d,%d,%[^,],%s", 
                &(*reservations)[i].flightId, &(*reservations)[i].passengerId, 
                booking_date_str, (*reservations)[i].seatNumber) == 4) {
            
            (*reservations)[i].bookingDate = parse_date(booking_date_str);
            i++;
        } else {
            fprintf(stderr, "Error parsing reservation data: %s\n", line);
        }
    }
    *reservation_count = i;
    fclose(file);
    
    printf("Successfully loaded %d flights, %d passengers, %d reservations\n", 
           *flight_count, *passenger_count, *reservation_count);
    
    return 1;

cleanup_and_fail:
    free(*flights);
    free(*passengers);
    free(*reservations);
    *flights = NULL;
    *passengers = NULL;
    *reservations = NULL;
    *flight_count = 0;
    *passenger_count = 0;
    *reservation_count = 0;
    return 0;
}

// Load flights data from a CSV file
Flight* load_flights(const char* filename, int* count) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening file %s\n", filename);
        *count = 0;
        return NULL;
    }
    
    // Count the number of lines (flights) in the file
    char line[1024];
    int line_count = 0;
    while (fgets(line, sizeof(line), file) != NULL) {
        line_count++;
    }
    
    // Subtract 1 for the header row
    if (line_count > 0) {
        line_count--;
    }
    *count = line_count;
    
    if (line_count <= 0) {
        fprintf(stderr, "No flight data found in %s\n", filename);
        fclose(file);
        return NULL;
    }
    
    // Allocate memory for the flights
    Flight* flights = (Flight*)malloc(line_count * sizeof(Flight));
    if (flights == NULL) {
        fprintf(stderr, "Memory allocation failed for flights\n");
        fclose(file);
        *count = 0;
        return NULL;
    }
    
    // Reset file pointer to beginning
    rewind(file);
    
    // Skip header line
    fgets(line, sizeof(line), file);
    
    // Read flight data
    int i = 0;
    while (i < line_count && fgets(line, sizeof(line), file) != NULL) {
        strip_newline(line);
        
        char departure_str[32];
        
        // Parse line (CSV format: id,flightNumber,origin,destination,departureTime,capacity)
        if (sscanf(line, "%d,%[^,],%[^,],%[^,],%[^,],%d", 
                &flights[i].id, flights[i].flightNumber, 
                flights[i].origin, flights[i].destination, 
                departure_str, &flights[i].capacity) == 6) {
            
            flights[i].departureTime = parse_date(departure_str);
            i++;
        } else {
            fprintf(stderr, "Error parsing flight data: %s\n", line);
        }
    }
    
    fclose(file);
    *count = i; // Update count with actual number of successfully parsed flights
    return flights;
}

// Load passengers data from a CSV file
Passenger* load_passengers(const char* filename, int* count) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening file %s\n", filename);
        *count = 0;
        return NULL;
    }
    
    // Count the number of lines (passengers) in the file
    char line[1024];
    int line_count = 0;
    while (fgets(line, sizeof(line), file) != NULL) {
        line_count++;
    }
    
    // Subtract 1 for the header row
    if (line_count > 0) {
        line_count--;
    }
    *count = line_count;
    
    if (line_count <= 0) {
        fprintf(stderr, "No passenger data found in %s\n", filename);
        fclose(file);
        return NULL;
    }
    
    // Allocate memory for the passengers
    Passenger* passengers = (Passenger*)malloc(line_count * sizeof(Passenger));
    if (passengers == NULL) {
        fprintf(stderr, "Memory allocation failed for passengers\n");
        fclose(file);
        *count = 0;
        return NULL;
    }
    
    // Reset file pointer to beginning
    rewind(file);
    
    // Skip header line
    fgets(line, sizeof(line), file);
    
    // Read passenger data
    int i = 0;
    while (i < line_count && fgets(line, sizeof(line), file) != NULL) {
        strip_newline(line);
        
        // Parse line (CSV format: id,name,passportNumber)
        if (sscanf(line, "%d,%[^,],%s", 
                &passengers[i].id, passengers[i].name, passengers[i].passportNumber) == 3) {
            i++;
        } else {
            fprintf(stderr, "Error parsing passenger data: %s\n", line);
        }
    }
    
    fclose(file);
    *count = i; // Update count with actual number of successfully parsed passengers
    return passengers;
}

// Load reservation records from a CSV file
ReservationRecord* load_reservations(const char* filename, int* count) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening file %s\n", filename);
        *count = 0;
        return NULL;
    }
    
    // Count the number of lines (reservations) in the file
    char line[1024];
    int line_count = 0;
    while (fgets(line, sizeof(line), file) != NULL) {
        line_count++;
    }
    
    // Subtract 1 for the header row
    if (line_count > 0) {
        line_count--;
    }
    *count = line_count;
    
    if (line_count <= 0) {
        fprintf(stderr, "No reservation data found in %s\n", filename);
        fclose(file);
        return NULL;
    }
    
    // Allocate memory for the reservations
    ReservationRecord* reservations = (ReservationRecord*)malloc(line_count * sizeof(ReservationRecord));
    if (reservations == NULL) {
        fprintf(stderr, "Memory allocation failed for reservations\n");
        fclose(file);
        *count = 0;
        return NULL;
    }
    
    // Reset file pointer to beginning
    rewind(file);
    
    // Skip header line
    fgets(line, sizeof(line), file);
    
    // Read reservation data
    int i = 0;
    while (i < line_count && fgets(line, sizeof(line), file) != NULL) {
        strip_newline(line);
        
        char booking_date_str[32];
        
        // Parse line (CSV format: flightId,passengerId,bookingDate,seatNumber)
        if (sscanf(line, "%d,%d,%[^,],%s", 
                &reservations[i].flightId, &reservations[i].passengerId, 
                booking_date_str, reservations[i].seatNumber) == 4) {
            
            reservations[i].bookingDate = parse_date(booking_date_str);
            i++;
        } else {
            fprintf(stderr, "Error parsing reservation data: %s\n", line);
        }
    }
    
    fclose(file);
    *count = i; // Update count with actual number of successfully parsed reservations
    return reservations;
}