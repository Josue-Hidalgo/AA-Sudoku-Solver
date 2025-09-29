#include <stdio.h>
#include <stdlib.h>

#define SIZE 3

/* FUNCIONES PREVIAS */
typedef struct {int sudoku[SIZE][SIZE][SIZE][SIZE];} Sudoku;
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
/* FUNCIONES PREVIAS */

/* FUNCIONES A PASAR */

void recorrerSudokuFilaXFila (const Sudoku *s) 
{
    int iG, jG, i, j;

    for (iG = 0; iG < SIZE; iG++) { 
        for (jG = 0; jG < SIZE; jG++) {
            for (i = 0; i < SIZE; i++) {
                for (j = 0; j < SIZE; j++) {
                    int valor = s->sudoku[iG][jG][i][j];
                    printf("%d ", valor);
                }
                printf("\n");
            }
            printf("\n");
        }
    }
}

void sudokuBacktracking (Sudoku *s) 
{
    // Busca Casilla Actual (Primera Casilla Vacia)
    
}

/* FUNCIONES A PASAR */

int main() {
    Sudoku sudoku;
    inicializarSudoku(&sudoku);
    printSudoku(&sudoku);

    sudokuBacktracking(&sudoku);

    return 0;
}