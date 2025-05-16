#pragma once
typedef struct edge {
	int to_vertex;
	int weight;
} Edge;

typedef struct edgeNode {
	Edge edge;
	struct edgeNode* next;
} *EdgeNodePtr;

typedef struct edgeList {
	EdgeNodePtr head;
} EdgeList;

typedef struct graph {
	int V;
	EdgeList* edges;
} Graph;

void add_edge(Graph* self, int from, int to, int w);
void calculate_pagerank(Graph* G, int num_iterations, float damping_factor, float** pageranks);