#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TAMANO 16

typedef struct {
    char palabra[TAMANO];
    int longitud;
} Palabra;

void cargarPalabrasDesdeArchivo(const char *nombreArchivo, Palabra *palabras, int *numPalabras);
void inicializarCrucigrama(char crucigrama[TAMANO][TAMANO]);
void mostrarCrucigrama(char crucigrama[TAMANO][TAMANO]);
int intentarColocarPalabra(char palabra[TAMANO], int fila, int columna, char direccion, char crucigrama[TAMANO][TAMANO]);

int main() {
    Palabra palabras[TAMANO];
    int numPalabras = 0;
    char crucigrama[TAMANO][TAMANO];

    srand((unsigned int)time(NULL));

    // Cargar palabras desde el archivo
    cargarPalabrasDesdeArchivo("palabras.txt", palabras, &numPalabras);

    // Inicializar el crucigrama
    inicializarCrucigrama(crucigrama);

    // Colocar las palabras en el crucigrama
    for (int i = 0; i < numPalabras; i++) {
        char direccion = (rand() % 2 == 0) ? 'H' : 'V';
        int fila, columna;

        // Intentar colocar la palabra en posiciones aleatorias
        do {
            fila = rand() % TAMANO;
            columna = rand() % TAMANO;
        } while (!intentarColocarPalabra(palabras[i].palabra, fila, columna, direccion, crucigrama));

        // Marcar la palabra en el crucigrama
        for (int j = 0; j < palabras[i].longitud; j++) {
            crucigrama[(direccion == 'H') ? fila : fila + j][(direccion == 'H') ? columna + j : columna] = palabras[i].palabra[j];
        }
    }

    // Mostrar el crucigrama
    mostrarCrucigrama(crucigrama);

    return 0;
}

void cargarPalabrasDesdeArchivo(const char *nombreArchivo, Palabra *palabras, int *numPalabras) {
    FILE *archivo = fopen(nombreArchivo, "r");
    if (archivo == NULL) {
        printf("No se pudo abrir el archivo %s.\n", nombreArchivo);
        exit(EXIT_FAILURE);
    }

    *numPalabras = 0;
    char linea[256];

    while (fscanf(archivo, "%[^,],", palabras[*numPalabras].palabra) == 1) {
        palabras[*numPalabras].longitud = strlen(palabras[*numPalabras].palabra);
        (*numPalabras)++;
    }

    fclose(archivo);
}

void inicializarCrucigrama(char crucigrama[TAMANO][TAMANO]) {
    for (int i = 0; i < TAMANO; i++) {
        for (int j = 0; j < TAMANO; j++) {
            crucigrama[i][j] = '_';
        }
    }
}

void mostrarCrucigrama(char crucigrama[TAMANO][TAMANO]) {
    printf("Crucigrama:\n");

    for (int i = 0; i < TAMANO; i++) {
        for (int j = 0; j < TAMANO; j++) {
            printf("%c ", crucigrama[i][j]);
        }
        printf("\n");
    }
}

int intentarColocarPalabra(char palabra[TAMANO], int fila, int columna, char direccion, char crucigrama[TAMANO][TAMANO]) {
    int longitud = strlen(palabra);

    if ((direccion == 'H' && columna + longitud <= TAMANO) || (direccion == 'V' && fila + longitud <= TAMANO)) {
        for (int i = 0; i < longitud; i++) {
            char c = crucigrama[(direccion == 'H') ? fila : fila + i][(direccion == 'H') ? columna + i : columna];
            if (c != '_' && c != palabra[i]) {
                return 0; // No se puede colocar la palabra en esta posición
            }
        }
        return 1; // La palabra puede colocarse en esta posición
    }

    return 0; // No se puede colocar la palabra fuera de los límites de la matriz
}
