#include <stdio.h>  
#include <stdlib.h>  
#include "graph.h"

int main() {  
   Graph G;  
   FILE* file = fopen("data.txt", "r");  

   if (file == NULL) {  
       printf("Error opening file!\n");  
       return 1;  
   }  

   char first_line[256] = {0};
   if (fgets(first_line, sizeof(first_line), file) == NULL) {  
       printf("Error reading the first line of the file!\n");  
       fclose(file);  
       return 1;  
   }  

   printf("First line of file: %s\n", first_line);  

   if (sscanf(first_line, "%d", &G.V) != 1) {  
       if (sscanf(first_line, "%*d,%d", &G.V) != 1) {  
           printf("Error reading vertex count from file! Ensure the first line contains a valid integer.\n");  
           fclose(file);  
           return 1;  
       }  
   }  

   printf("Initializing graph with %d vertices...\n", G.V);  

   if (G.V <= 0 || G.V > 1000000) {  
       printf("Invalid vertex count: %d. Ensure the file contains a valid positive integer.\n", G.V);  
       fclose(file);  
       return 1;  
   }  

   printf("Allocating memory for %d vertices...\n", G.V);  
   G.edges = (EdgeList*)malloc(G.V * sizeof(EdgeList));  
   if (G.edges == NULL) {  
       printf("Memory allocation failed for graph edges! Required: %zu bytes\n",  
           G.V * sizeof(EdgeList));  
       fclose(file);  
       return 1;  
   }  

   for (int v = 0; v < G.V; v++) {  
       G.edges[v].head = NULL;  
   }  
   printf("Memory allocation and initialization for edges completed successfully.\n");  

   printf("Reading edges from file...\n");  

   int from, to;  
   int edge_count = 0;  
   int edge_errors = 0;  

   while (fscanf(file, "%d,%d", &from, &to) == 2) {  
       if (from < 0 || from >= G.V || to < 0 || to >= G.V) {  
           edge_errors++;  
           continue;  
       }  

       add_edge(&G, from, to, 0);  
       edge_count++;  

       if (edge_count % 10000 == 0) {  
           printf("Processed %d edges...\n", edge_count);  
       }  
   }  

   fclose(file);  
   printf("Loaded %d edges successfully (skipped %d invalid edges)\n", edge_count, edge_errors);  

   printf("Calculating in-degrees...\n");  
   int* in_degrees = (int*)calloc(G.V, sizeof(int));  
   if (in_degrees == NULL) {  
       printf("Memory allocation failed for in-degrees array! Required: %zu bytes\n",  
           G.V * sizeof(int));  

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

   for (int v = 0; v < G.V; v++) {  
       EdgeNodePtr current = G.edges[v].head;  
       while (current != NULL) {  
           int to_vertex = current->edge.to_vertex;  
           in_degrees[to_vertex]++;  
           current = current->next;  
       }  
   }  

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

   printf("\nGraph Analysis Results:\n");  
   printf("Number of vertices: %d\n", G.V);  
   printf("Total number of edges: %d\n", total_in_degrees);  
   printf("Average in-degree: %.2f\n", (float)total_in_degrees / G.V);  
   printf("Vertices with no incoming edges: %d (%.1f%%)\n",  
       empty_vertices, 100.0f * empty_vertices / G.V);  
   printf("Most connected vertex: %d with in-degree %d\n",  
       most_connected_vertex, max_in_degree);  

   printf("\nSample in-degrees (first 10 vertices):\n");  
   for (int v = 0; v < 10 && v < G.V; v++) {  
       printf("Vertex %d: %d\n", v, in_degrees[v]);  
   }  

   printf("\nCalculating PageRank (10 iterations, damping factor 0.85)...\n");  
   float* pageranks = NULL;  
   calculate_pagerank(&G, 10, 0.85, &pageranks);  

   if (pageranks != NULL) {  
       float max_pr = 0.0;  
       int max_pr_vertex = -1;  
       for (int v = 0; v < G.V; v++) {  
           if (pageranks[v] > max_pr) {  
               max_pr = pageranks[v];  
               max_pr_vertex = v;  
           }  
       }  

       printf("\nPageRank Results:\n");  
       printf("Vertex with highest PageRank: %d (PageRank: %.6f)\n", max_pr_vertex, max_pr);  
       printf("\nTop 10 vertices by PageRank:\n");  

       int* indices = (int*)malloc(G.V * sizeof(int));  
       if (indices != NULL) {  
           for (int i = 0; i < G.V; i++) {  
               indices[i] = i;  
           }  

           for (int i = 0; i < G.V - 1; i++) {  
               for (int j = 0; j < G.V - i - 1; j++) {  
                   if (pageranks[indices[j]] < pageranks[indices[j + 1]]) {  
                       int temp = indices[j];  
                       indices[j] = indices[j + 1];  
                       indices[j + 1] = temp;  
                   }  
               }  
           }  

           int limit = (G.V < 10) ? G.V : 10;  
           for (int i = 0; i < limit; i++) {  
               printf("Vertex %d: PageRank %.6f\n", indices[i], pageranks[indices[i]]);  
           }  

           free(indices);  
       }  
       else {  
           printf("\nSample PageRanks (first 10 vertices):\n");  
           for (int v = 0; v < 10 && v < G.V; v++) {  
               printf("Vertex %d: %.6f\n", v, pageranks[v]);  
           }  
       }  

       free(pageranks);  
   }  
   else {  
       printf("failed");  
   }  

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
    if (from < 0 || from >= self->V || to < 0 || to >= self->V) {
        return;
    }

    EdgeNodePtr new_node = (EdgeNodePtr)malloc(sizeof(struct edgeNode));
    if (new_node == NULL) {
        printf("Memory allocation failed for new edge (%d->%d)!\n", from, to);
        return;
    }

    new_node->edge.to_vertex = to;
    new_node->edge.weight = w;
    new_node->next = self->edges[from].head;
    self->edges[from].head = new_node;
}


void calculate_pagerank(Graph* G, int num_iterations, float damping_factor, float** pageranks) {
    int n = G->V;
    float* pr = (float*)malloc(n * sizeof(float));
    float* next_pr = (float*)malloc(n * sizeof(float));
    float* sums = (float*)malloc(n * sizeof(float));
    int* out_degrees = (int*)calloc(n, sizeof(int));

    if (!pr || !next_pr || !sums || !out_degrees) {
        printf("Memory allocation failed in PageRank calculation\n");
        if (pr) free(pr);
        if (next_pr) free(next_pr);
        if (sums) free(sums);
        if (out_degrees) free(out_degrees);
        *pageranks = NULL;
        return;
    }

    for (int i = 0; i < n; i++) {
        EdgeNodePtr current = G->edges[i].head;
        while (current != NULL) {
            out_degrees[i]++;
            current = current->next;
        }
    }

    for (int i = 0; i < n; i++) {
        pr[i] = 1.0;
    }

    for (int iter = 0; iter < num_iterations; iter++) {
        for (int i = 0; i < n; i++) {
            sums[i] = 0.0;
        }

        for (int j = 0; j < n; j++) {
            if (out_degrees[j] > 0) { 
                float contribution = pr[j] / out_degrees[j];

                EdgeNodePtr current = G->edges[j].head;
                while (current != NULL) {
                    int i = current->edge.to_vertex; 
                    sums[i] += contribution;
                    current = current->next;
                }
            }
        }

        for (int i = 0; i < n; i++) {
            next_pr[i] = (1.0 - damping_factor) + damping_factor * sums[i];
        }

        for (int i = 0; i < n; i++) {
            pr[i] = next_pr[i];
        }
    }

    free(next_pr);
    free(sums);
    free(out_degrees);

    *pageranks = pr;
}