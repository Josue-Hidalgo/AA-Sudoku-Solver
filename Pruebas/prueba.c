#include <stdio.h>
#include <stdlib.h>

#define SIZE 3

typedef struct {
    int sudoku[SIZE][SIZE][SIZE][SIZE];
} Sudoku;

void printSudoku(const Sudoku *s) {
    printf("---------------------\n");
    for (int iG = 0; iG < SIZE; iG++) {
        for (int jG = 0; jG < SIZE; jG++) {
            for (int i = 0; i < SIZE; i++) {
                for (int j = 0; j < SIZE; j++) {
                    int valor = s->sudoku[iG][jG][i][j];
                    printf("%2d ", valor);
                }
                printf("| ");
            }
            printf("\n");
        }
        if (iG < SIZE - 1)
            printf("------+-------+------\n"); // Separador entre grupos de submatrices
    }
}

void inicializarSudoku(Sudoku *s) {
    // llenar las matrices de 0s
    // la G representa que es la y (filas) y x (columnas) de la matriz grande
    for (int iG = 0; iG < SIZE; iG++) { 
        for (int jG = 0; jG < SIZE; jG++) {
            // estas ya son para las submatrices
            for (int i = 0; i < SIZE; i++) {
                for (int j = 0; j < SIZE; j++) {
                    s->sudoku[iG][jG][i][j] = 0;
                }
            }
        }
    }
}

int guardarSudoku(const char *filename, const Sudoku *s) {
    FILE *file = fopen(filename, "wb");
    if (file == NULL)
        return 0;
    size_t escritos = fwrite(s, sizeof(Sudoku), 1, file);
    fclose(file);

    return (escritos == 1); // 1 si se abrió el archivo, 0 si fue error
}

int cargarSudoku(const char *filename, Sudoku *s) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL)
        return 0;
    size_t leidos = fread(s, sizeof(Sudoku), 1, file);
    fclose(file);

    return (leidos == 1); // 1 si se abrió el archivo, 0 si fue error
}

int main() {
    Sudoku sudoku;


    return 0;
}