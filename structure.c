#include <stdio.h>
#include <stdlib.h>
#include "structure.h"

Graph *g_graph = NULL;

Graph *graph_create(int n) {
    Graph *g = malloc(sizeof(Graph));
    if (g == NULL) return NULL;

    g->n = n;
    g->cost = malloc(n * sizeof(int *));
    if (g->cost == NULL) {
        free(g);
        return NULL;
    }

    for (int i = 0; i < n; i++) {
        g->cost[i] = malloc(n * sizeof(int));
        if (g->cost[i] == NULL) {
            // liberar lo ya reservado
            for (int j = 0; j < i; j++) {
                free(g->cost[j]);
            }
            free(g->cost);
            free(g);
            return NULL;
        }
        // inicializar en 0 (sin enlace)
        for (int j = 0; j < n; j++) {
            g->cost[i][j] = 0;
        }
    }

    return g;
}

void graph_free(Graph *g) {
    if (g == NULL) return;

    for (int i = 0; i < g->n; i++) {
        free(g->cost[i]);
    }
    free(g->cost);
    free(g);
}

int graph_add_edge(Graph *g, int from, int to, int cost) {
    if (g == NULL) return 1;
    if (from < 0 || from >= g->n) return 1;
    if (to   < 0 || to   >= g->n) return 1;

    g->cost[from][to] = cost;
    g->cost[to][from] = cost;   // grafo no dirigido
    return 0;
}

void graph_print(const Graph *g) {
    if (g == NULL) {
        printf("No hay grafo cargado.\n");
        return;
    }

    int n = g->n;

    // encabezado
    printf("    ");
    for (int j = 0; j < n; j++) {
        printf(" %c  ", 'A' + j);
    }
    printf("\n");

    for (int i = 0; i < n; i++) {
        printf(" %c ", 'A' + i);
        for (int j = 0; j < n; j++) {
            if (g->cost[i][j] == 0) {
                printf("  . ");
            } else {
                printf("%3d ", g->cost[i][j]);
            }
        }
        printf("\n");
    }
}
