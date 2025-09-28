#include <stdio.h>
#include <stdlib.h>

void printMatrix(int square[3][3]) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            printf("%3d ", square[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

int main() {
    int matrizG[3][3][3][3]; // G de grande, muy descriptivo

    // llenar las matrices de 0s
    // la G representa que es la y (filas) y x (columnas) de la matriz grande
    for (int iG = 0; iG < 3; iG++) { 
        for (int jG = 0; jG < 3; jG++) {
            // estas ya son para las submatrices
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    matrizG[iG][jG][i][j] = 0;
                }
            }
        }
    }

    matrizG[1][2][0][0] = 1; // en la matriz [1][2], pongo un 1 en la posición [0][0]
    matrizG[0][0][2][1] = 3; // en la matriz [0][0], pongo un 3 en la posición [2][1]
    
    // imprimir todas las matrices
    for (int iG = 0; iG < 3; iG++) {
        for (int jG = 0; jG < 3; jG++) {
            printf("matrizG[%d][%d]:\n", iG, jG);
            printMatrix(matrizG[iG][jG]);
        }
    }

    return 0;
}