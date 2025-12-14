#ifndef STRUCTURE_H
#define STRUCTURE_H

typedef struct{
    int n;
    int **cost; // Matriz de costos
} Graph;

extern Graph *g_graph; //puntero global al grafo actual

//operaciones con grafos
Graph *graph_create(int n);
void   graph_free(Graph *g);
int    graph_add_edge(Graph *g, int from, int to, int cost);
void   graph_print(const Graph *g);

#endif 

