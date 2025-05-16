#include <stdio.h>  
#include <stdlib.h>  

#include "graph.h"  

int main() {  
    Graph G;
    FILE* file = fopen("graph.txt", "r");

    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }

    fscanf(file, "%d", &G.V);

    G.edges = (EdgeList*)malloc(G.V * sizeof(EdgeList));
    if (G.edges == NULL) {
        printf("Memory allocation failed!\n");
        fclose(file);
        return 1;
    }

    for (int v = 0; v < G.V; v++) {
        G.edges[v].head = NULL;
    }

    int from, to, weight;
    while (fscanf(file, "%d,%d,%d", &from, &to, &weight) == 3) {
        add_edge(&G, from, to, weight);
    }

    fclose(file);

    printf("Graph loaded successfully with %d vertices.\n", G.V);

    for (int v = 0; v < G.V; v++) {
        EdgeNodePtr current = G.edges[v].head;
        while (current != NULL) {
            EdgeNodePtr next = current->next;
            free(current);
            current = next;
        }
    }
    free(G.edges);

    return 0;
}

void add_edge(Graph* self, int from, int to, int w) {
    EdgeNodePtr new_node = (EdgeNodePtr)malloc(sizeof(struct edgeNode));
    if (new_node == NULL) {
        printf("Memory allocation failed for new edge!\n");
        return;
    }

    new_node->edge.to_vertex = to;
    new_node->edge.weight = w;

    new_node->next = self->edges[from].head;
    self->edges[from].head = new_node;
}