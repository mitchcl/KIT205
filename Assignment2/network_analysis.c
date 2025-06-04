/**
 * Network Analysis Implementation
 * BFS shortest path algorithm for transport networks
 */

#include "graph.h"

typedef struct QueueNode
{
    int stop_index;
    struct QueueNode* next;
} QueueNode;

typedef struct
{
    QueueNode* front;
    QueueNode* rear;
} Queue;

// Queue operations for BFS
Queue* create_queue()
{
    Queue* q = malloc(sizeof(Queue));
    if (q)
    {
        q->front = q->rear = NULL;
    }
    return q;
}

void enqueue(Queue* q, int stop_index)
{
    QueueNode* new_node = malloc(sizeof(QueueNode));
    if (!new_node || !q)
        return;

    new_node->stop_index = stop_index;
    new_node->next = NULL;

    if (q->rear == NULL)
    {
        q->front = q->rear = new_node;
    }
    else
    {
        q->rear->next = new_node;
        q->rear = new_node;
    }
}

int dequeue(Queue* q)
{
    if (!q || q->front == NULL)
        return -1;

    QueueNode* temp = q->front;
    int stop_index = temp->stop_index;

    q->front = q->front->next;
    if (q->front == NULL)
    {
        q->rear = NULL;
    }

    free(temp);
    return stop_index;
}

bool is_queue_empty(Queue* q)
{
    return (q == NULL || q->front == NULL);
}

void free_queue(Queue* q)
{
    if (!q)
        return;

    while (!is_queue_empty(q))
    {
        dequeue(q);
    }
    free(q);
}

/**
 * BFS shortest path algorithm
 * Returns PathResult with shortest path between start and end stops
 */
PathResult* find_shortest_path(TransportGraph* graph, int start_stop, int end_stop)
{
    if (!graph)
        return NULL;

    PathResult* result = malloc(sizeof(PathResult));
    if (!result)
        return NULL;

    // Initialise result structure
    result->path = NULL;
    result->path_length = 0;
    result->total_transfers = 0;
    result->total_time = 0;
    result->path_found = false;

    int start_index = find_stop_index(graph, start_stop);
    int end_index = find_stop_index(graph, end_stop);

    if (start_index == -1 || end_index == -1)
    {
        return result; // Stops not found
    }

    if (start_index == end_index)
    {
        // Same stop - create trivial path
        result->path = malloc(sizeof(int));
        result->path[0] = start_stop;
        result->path_length = 1;
        result->path_found = true;
        return result;
    }

    // BFS data structures
    bool* visited = calloc(graph->num_stops, sizeof(bool));
    int* parent = malloc(sizeof(int) * graph->num_stops);
    int* distance = malloc(sizeof(int) * graph->num_stops);

    if (!visited || !parent || !distance)
    {
        free(visited);
        free(parent);
        free(distance);
        return result;
    }

    // Initialize BFS arrays
    for (int i = 0; i < graph->num_stops; i++)
    {
        parent[i] = -1;
        distance[i] = -1;
    }

    Queue* q = create_queue();
    if (!q)
    {
        free(visited);
        free(parent);
        free(distance);
        return result;
    }

    // Start BFS
    visited[start_index] = true;
    distance[start_index] = 0;
    enqueue(q, start_index);

    while (!is_queue_empty(q))
    {
        int current_index = dequeue(q);

        if (current_index == end_index)
        {
            result->path_found = true;
            break;
        }

        // Explore all connections from current stop
        Connection* conn = graph->adjacency_list[current_index];
        while (conn)
        {
            int next_stop_index = find_stop_index(graph, conn->destination_stop_id);

            if (next_stop_index != -1 && !visited[next_stop_index])
            {
                visited[next_stop_index] = true;
                parent[next_stop_index] = current_index;
                distance[next_stop_index] = distance[current_index] + 1;
                enqueue(q, next_stop_index);
            }

            conn = conn->next;
        }
    }

    // Reconstruct path if found
    if (result->path_found)
    {
        // Count path length
        int length = 0;
        int current = end_index;
        while (current != -1)
        {
            length++;
            current = parent[current];
        }

        // Allocate and fill path array
        result->path = malloc(sizeof(int) * length);
        result->path_length = length;

        if (result->path)
        {
            current = end_index;
            for (int i = length - 1; i >= 0; i--)
            {
                result->path[i] = graph->stops[current].stop_id;
                current = parent[current];
            }

            result->total_transfers = length - 1; // Number of connections
        }
    }

    // Cleanup
    free_queue(q);
    free(visited);
    free(parent);
    free(distance);

    return result;
}

/**
 * Free PathResult structure
 */
void free_path_result(PathResult* result)
{
    if (!result)
        return;

    free(result->path);
    free(result);
}

/**
 * Print path information
 */
void print_path(TransportGraph* graph, PathResult* result)
{
    if (!result || !graph)
        return;

    if (!result->path_found)
    {
        printf("No path found between specified stops.\n");
        return;
    }

    printf("Shortest path found:\n");
    printf("Path length: %d stops\n", result->path_length);
    printf("Total transfers: %d\n", result->total_transfers);

    printf("Route: ");
    for (int i = 0; i < result->path_length; i++)
    {
        int stop_index = find_stop_index(graph, result->path[i]);
        if (stop_index != -1)
        {
            printf("%s", graph->stops[stop_index].name);
            if (i < result->path_length - 1)
            {
                printf(" -> ");
            }
        }
    }
    printf("\n");
}
