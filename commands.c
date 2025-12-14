#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "commands.h"
#include "structure.h"

//prototipos 
static void tsp_solve(const Graph *g);
static void tsp_search(const Graph *g,
                       int start,
                       int current,
                       int depth,
                       int current_cost,
                       int *visited,
                       int *path,
                       int *best_cost,
                       int *best_path);



//comandos
int pvv_start(int nodes) {
    // liberar grafo anterior si existe
    if (g_graph != NULL) {
        graph_free(g_graph);
        g_graph = NULL;
    }

    g_graph = graph_create(nodes);
    if (g_graph == NULL) {
        printf("Error al crear la estructura del grafo.\n");
        return 1;
    }

    return 0;
}

// Lee enlaces desde archivo: lineas "A B 15"
int pvv_read(const char *filename) {
    if (g_graph == NULL) {
        printf("Primero debe crear un grafo con: ./pvv start <numero>\n");
        return 1;
    }

    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("No existe el archivo especificado: %s\n", filename);
        return 1;
    }

    printf("Agregando enlaces desde archivo.\n");

    char from_c, to_c;
    int cost;
    int count = 0;

    while (1) {
        int r = fscanf(file, " %c %c %d", &from_c, &to_c, &cost);
        if (r == EOF) break;

        if (r != 3) {
            printf("Linea invalida en archivo.\n");
            continue;
        }

        int from = from_c - 'A';
        int to   = to_c   - 'A';

        if (graph_add_edge(g_graph, from, to, cost) != 0) {
            printf("Enlace invalido en archivo: %c %c %d (ignorado)\n",
                   from_c, to_c, cost);
            continue;
        }

        count++;
    }

    fclose(file);

    printf("Se cargaron %d enlaces.\n", count);

    // Aquí resolvemos la ruta mínima
    tsp_solve(g_graph);

    return 0;
}

void pvv_graph(void) {
    if (g_graph == NULL) {
        printf("No hay grafo cargado. Use ./pvv start <numero> y ./pvv read <archivo>.\n");
        return;
    }

    graph_print(g_graph);
}

void pvv_exit(void) {
    if (g_graph != NULL) {
        graph_free(g_graph);
        g_graph = NULL;
    }
    printf("Limpiando cache y saliendo ...\n");
}

//implementacion de TSP 

//funcion principal:busca la mejro ruta
//empiece en A 
//visite todos los nodos una vez y vuelva al principio
static void tsp_solve(const Graph *g) {
    if (g == NULL) {
        printf("No hay grafo cargado.\n");
        return;
    }

    int n = g->n;
    if (n <= 1) {
        printf("Grafo demasiado pequeno para buscar ruta.\n");
        return;
    }

    int *visited   = calloc(n, sizeof(int));
    int *path      = malloc(n * sizeof(int));
    int *best_path = malloc(n * sizeof(int));

    if (!visited || !path || !best_path) {
        printf("Error de memoria en tsp_solve.\n");
        free(visited);
        free(path);
        free(best_path);
        return;
    }

    int start = 0;  // ciudad 'A'
    for (int i = 0; i < n; i++) {
        visited[i] = 0;
    }

    visited[start] = 1;
    path[0] = start;

    int best_cost = INT_MAX;

    tsp_search(g, start, start, 0, 0, visited, path, &best_cost, best_path);

    if (best_cost == INT_MAX) {
        printf("No existe una ruta que recorra todas las ciudades y vuelva al origen.\n");
    } else {
        printf("Ruta de menor costo (comenzando en %c): ", 'A' + start);
        for (int i = 0; i < n; i++) {
            printf("%c -> ", 'A' + best_path[i]);
        }
        printf("%c\n", 'A' + start);
        printf("Costo total: %d\n", best_cost);
    }

    free(visited);
    free(path);
    free(best_path);
}

// backtracking
static void tsp_search(const Graph *g,
                       int start,
                       int current,
                       int depth,
                       int current_cost,
                       int *visited,
                       int *path,
                       int *best_cost,
                       int *best_path) {
    int n = g->n;

    // Si ya hemos colocado n-1 saltos (depth), path[0..depth] tiene depth+1 nodos
    if (depth == n - 1) {
        // Intentar volver al origen
        if (g->cost[current][start] > 0) {
            int total = current_cost + g->cost[current][start];
            if (total < *best_cost) {
                *best_cost = total;
                // Guardar mejor ruta encontrada
                for (int i = 0; i < n; i++) {
                    best_path[i] = path[i];
                }
            }
        }
        return;
    }

    // Probar ir a cada nodo no visitado
    for (int next = 0; next < n; next++) {
        if (!visited[next] && g->cost[current][next] > 0) {
            visited[next] = 1;
            path[depth + 1] = next;

            tsp_search(g,
                       start,
                       next,
                       depth + 1,
                       current_cost + g->cost[current][next],
                       visited,
                       path,
                       best_cost,
                       best_path);

            visited[next] = 0;  // backtrack
        }
    }
}
