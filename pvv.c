// pvv.c - programa principal
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "commands.h"
#include "structure.h"

// prototipo
int CharToInt(const char *str);

int main(int argc, char *argv[]) {

    if (argc <= 1) {
        printf("Escriba: ./pvv help para ver los comandos disponibles\n");
        return 0;
    }

    // Recorremos TODOS los argumentos 
    for (int i = 1; i < argc; i++) {

        // help
        if (strcmp(argv[i], "help") == 0) {
            printf("Comandos disponibles:\n");
            printf("./pvv start <numero> : Crea un grafo con el numero de nodos especificado\n");
            printf("./pvv read <archivo> : Lee enlaces desde archivo (ej: ruta1.txt)\n");
            printf("./pvv graph         : Imprime la representacion del grafo actual\n");
            printf("./pvv exit          : Libera memoria y termina el programa\n");
        }

        // start <numero>
        else if (strcmp(argv[i], "start") == 0) {

            if (i + 1 >= argc) {
                printf("Falta el numero de nodos. Use: ./pvv start <numero>\n");
                return 1;
            }

            int nodes = CharToInt(argv[i + 1]);
            i++; // consumimos el argumento del número

            if (nodes < 1) {
                printf("Numero de nodos invalido.\n");
                return 1;
            }

            if (pvv_start(nodes) != 0) {
                return 1;
            }

            printf("Grafo creado con %d nodos\n", nodes);
        }

        // read <archivo>
        else if (strcmp(argv[i], "read") == 0) {

            if (i + 1 >= argc) {
                printf("Indique el nombre de un archivo a leer, (./pvv read <nombre_archivo.txt>)\n");
                return 1;
            }

            const char *filename = argv[i + 1];
            i++; // consumimos el nombre del archivo

            if (pvv_read(filename) != 0) {
                return 1;
            }
        }

        // graph
        else if (strcmp(argv[i], "graph") == 0) {
            pvv_graph();
        }

        // exit
        else if (strcmp(argv[i], "exit") == 0) {
            pvv_exit();
            return 0;
        }

        // comando desconocido
        else {
            printf("Comando no reconocido: %s\n", argv[i]);
            printf("Escriba: ./pvv help para ver los comandos disponibles\n");
            return 1;
        }
    }

    // Al final de todos los comandos, liberamos memoria por si quedo algo
    pvv_exit();
    
    return 0;
}

// convierte string a entero (>0 válido, <=0 inválido)
int CharToInt(const char *str) {
    int valor = atoi(str);
    if (valor <= 0) {
        return -1;
    }
    return valor;
}
