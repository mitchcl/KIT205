#include <stdio.h>  
#include <stdlib.h>  
#include "graph.h"  

int main() {
    Graph G;
    FILE* file = fopen("musae_git_edges.csv", "r");

    if (file == NULL) {
        printf("Error opening file! Ensure the file exists and the path is correct.\n");
        return 1; // Return error code
    }

    printf("File opened successfully.\n");

    // Debug: Read and print the first line of the file
    char first_line[256];
    if (fgets(first_line, sizeof(first_line), file) == NULL) {
        printf("Error: File is empty or unreadable.\n");
        fclose(file);
        return 1;
    }
    printf("First line of file: %s\n", first_line);

    // Parse the vertex count (adjusted for multiple values in the first line)
    if (sscanf(first_line, "%*d,%d", &G.V) != 1) {
        printf("Error reading vertex count from file! Ensure the first line contains a valid integer.\n");
        fclose(file);
        return 1;
    }

    printf("Initializing graph with %d vertices...\n", G.V);

    // Check for unreasonable vertex count
    if (G.V <= 0 || G.V > 1000000) {
        printf("Invalid vertex count: %d. Ensure the file contains a valid positive integer.\n", G.V);
        fclose(file);
        return 1;
    }

    // Allocate memory for edge lists with error checking
    printf("Allocating memory for %d vertices...\n", G.V);
    G.edges = (EdgeList*)malloc(G.V * sizeof(EdgeList));
    if (G.edges == NULL) {
        printf("Memory allocation failed for graph edges! Required: %zu bytes\n",
            G.V * sizeof(EdgeList));
        fclose(file);
        return 1;
    }

    // Initialize all edge lists to empty
    for (int v = 0; v < G.V; v++) {
        G.edges[v].head = NULL; // Ensure each edge list is initialized
    }
    printf("Memory allocation and initialization for edges completed successfully.\n");

    printf("Reading edges from file...\n");

    int from, to;
    int edge_count = 0;
    int edge_errors = 0;

    // Read edges from file, skipping invalid edges
    while (fscanf(file, "%d,%d", &from, &to) == 2) {
        // Skip invalid vertices
        if (from < 0 || from >= G.V || to < 0 || to >= G.V) {
            edge_errors++;
            continue;
        }

        add_edge(&G, from, to, 0);
        edge_count++;

        // Show progress for large datasets
        if (edge_count % 10000 == 0) {
            printf("Processed %d edges...\n", edge_count);
        }
    }

    fclose(file);
    printf("Loaded %d edges successfully (skipped %d invalid edges)\n", edge_count, edge_errors);

    // Calculate in-degrees
    printf("Calculating in-degrees...\n");
    int* in_degrees = (int*)calloc(G.V, sizeof(int));
    if (in_degrees == NULL) {
        printf("Memory allocation failed for in-degrees array! Required: %zu bytes\n",
            G.V * sizeof(int));

        // Free graph memory before exiting
        for (int v = 0; v < G.V; v++) {
            EdgeNodePtr current = G.edges[v].head;
            while (current != NULL) {
                EdgeNodePtr next = current->next;
                free(current);
                current = next;
            }
        }
        free(G.edges);
        return 1;
    }

    // Count in-degrees for each vertex
    for (int v = 0; v < G.V; v++) {
        EdgeNodePtr current = G.edges[v].head;
        while (current != NULL) {
            int to_vertex = current->edge.to_vertex;
            in_degrees[to_vertex]++;
            current = current->next;
        }
    }

    // Calculate statistics
    int total_in_degrees = 0;
    int max_in_degree = 0;
    int most_connected_vertex = -1;
    int empty_vertices = 0;

    for (int v = 0; v < G.V; v++) {
        total_in_degrees += in_degrees[v];

        if (in_degrees[v] == 0) {
            empty_vertices++;
        }

        if (in_degrees[v] > max_in_degree) {
            max_in_degree = in_degrees[v];
            most_connected_vertex = v;
        }
    }

    // Print results
    printf("\nGraph Analysis Results:\n");
    printf("Number of vertices: %d\n", G.V);
    printf("Total number of edges: %d\n", total_in_degrees);
    printf("Average in-degree: %.2f\n", (float)total_in_degrees / G.V);
    printf("Vertices with no incoming edges: %d (%.1f%%)\n",
        empty_vertices, 100.0f * empty_vertices / G.V);
    printf("Most connected vertex: %d with in-degree %d\n",
        most_connected_vertex, max_in_degree);

    // Print first 10 vertices' in-degrees as a sample
    printf("\nSample in-degrees (first 10 vertices):\n");
    for (int v = 0; v < 10 && v < G.V; v++) {
        printf("Vertex %d: %d\n", v, in_degrees[v]);
    }

    // Clean up memory
    printf("\nCleaning up memory...\n");
    free(in_degrees);

    for (int v = 0; v < G.V; v++) {
        EdgeNodePtr current = G.edges[v].head;
        while (current != NULL) {
            EdgeNodePtr next = current->next;
            free(current);
            current = next;
        }
    }
    free(G.edges);
    printf("Memory cleanup complete\n");

    return 0;
}

void add_edge(Graph* self, int from, int to, int w) {
    // Validate vertex indices
    if (from < 0 || from >= self->V || to < 0 || to >= self->V) {
        return; // Skip invalid vertices
    }

    // Allocate memory for new edge node
    EdgeNodePtr new_node = (EdgeNodePtr)malloc(sizeof(struct edgeNode));
    if (new_node == NULL) {
        // Just report the error but don't exit, allow program to continue
        printf("Memory allocation failed for new edge (%d->%d)!\n", from, to);
        return;
    }

    // Initialize the new edge
    new_node->edge.to_vertex = to;
    new_node->edge.weight = w;

    // Insert at front of list
    new_node->next = self->edges[from].head;
    self->edges[from].head = new_node;
}
