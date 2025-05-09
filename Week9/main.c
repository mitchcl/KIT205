#include "graph.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    Graph G;
    
    FILE* file = fopen("graph.txt", "r");
    if (file == NULL) {
        printf("Error opening file!\n");
        return 1;
    }

    fscanf(file, "%d", &G.V);
    printf("Number of vertices: %d\n", G.V);

    G.edges = malloc(G.V * sizeof(EdgeList));
    for (int i = 0; i < G.V; i++) {
        G.edges[i].head = NULL;
    }
    

    fclose(file);
    return 0;
}