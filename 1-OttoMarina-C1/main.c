#include <stdio.h>
#include <stdlib.h>
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#define MAXN 100000
#pragma warning (disable:4996)
typedef struct vertex
{
    int vert;
    struct vertex* next;
} vertex_t;

typedef struct graph_of_vertexes
{
    int number_of_vert;
    int* visited;
    struct vertex_t** adjLists;
} graph_t;

int get_minimum_vert(graph_t* graph, vertex_t* temp)
{
    vertex_t* cur = temp;
    int min_size = MAXN;
    while (cur)
    {
        if (cur->vert < min_size && graph->visited[cur->vert] != 1)
        {
            min_size = cur->vert;
        }
        cur = cur->next;
    }
    return min_size == MAXN ? -1 : min_size;
}

int counter_of_not_visited(graph_t* graph, vertex_t* start_of_adjList)
{
    int cnt = 0;
    vertex_t* cur = start_of_adjList;
    while (cur)
    {
        if (graph->visited[cur->vert] != 1)
        {
            cnt++;
        }
        cur = cur->next;
    }
    return cnt;
}

void dfs(graph_t* graph, int vert) {
    struct node* start_of_adjList = graph->adjLists[vert];
    struct node* this_vert_iterator = start_of_adjList;

    if (vert == -1)
        return;

    int number_of_not_visited_verts_at_this_moment = counter_of_not_visited(graph, this_vert_iterator);
    graph->visited[vert] = 1;
    printf("%d ", vert);

    while (number_of_not_visited_verts_at_this_moment--) {
        int connected_vert = get_minimum_vert(graph, this_vert_iterator);

        if (graph->visited[connected_vert] == 0) {
            dfs(graph, connected_vert);
        }
    }
}

struct node* create_node(int number_of_vert)
{
    vertex_t* vertex_created = (vertex_t*)malloc(sizeof(vertex_t));
    vertex_created->vert = number_of_vert;
    vertex_created->next = NULL;
    return vertex_created;
}


graph_t* create_graph(int quantity_of_verts)
{
    graph_t* graph = (graph_t*)malloc(sizeof(graph_t));
    graph->number_of_vert = quantity_of_verts;
    graph->adjLists = malloc(quantity_of_verts * sizeof(vertex_t*));
    graph->visited = malloc(quantity_of_verts * sizeof(int));

    for (int i = 0; i < quantity_of_verts; i++) {
        graph->adjLists[i] = NULL;
        graph->visited[i] = 0;
    }
    return graph;
}


void add_connection(graph_t* graph, int first_connected, int second_connected)
{
    vertex_t* newNode = create_node(second_connected);
    newNode->next = graph->adjLists[first_connected];
    graph->adjLists[first_connected] = newNode;
    newNode = create_node(first_connected);
    newNode->next = graph->adjLists[second_connected];
    graph->adjLists[second_connected] = newNode;
}


void free_graph(graph_t* graph) {
    int i = 0;
    for (; i < graph->number_of_vert; i++) {
        free(graph->adjLists[i]);
    }
    free(graph->adjLists);
    free(graph->visited);
    free(graph);
}


void MemoryLeaks(void) {
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_FILE);
    _CrtSetReportFile(_CRT_WARN, _CRTDBG_FILE_STDOUT);
}


int main(void)
{
    int size_of_graph;
    scanf("%d", &size_of_graph);
    graph_t* new_graph = create_graph(size_of_graph);

    for (int i = 0; i < size_of_graph; i++)
    {
        int vert; char trash;
        scanf("%d%c", &vert, &trash);
        while (trash == ' ')
        {
            scanf("%d%c", &vert, &trash);
            add_connection(new_graph, i, vert);
        }
    }

    dfs(new_graph, 0);
	free_graph(new_graph);
    MemoryLeaks();

    return 0;
}