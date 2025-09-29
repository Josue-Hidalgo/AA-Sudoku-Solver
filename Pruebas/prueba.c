#include <stdio.h>
#include <stdlib.h>

#define SIZE 9

typedef struct {
    int sudoku[SIZE][SIZE];
} Sudoku;

void printSudoku(const Sudoku *s) {
    for (int y = 0; y < SIZE; y++) {
        for (int x = 0; x < SIZE; x++)
            printf("%3d ", s->sudoku[y][x]);
        printf("\n\n");
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

int promising(Sudoku *s, int row, int col, int num) {
    // Verificar fila
    for (int x = 0; x < SIZE; x++) {
        if (s->sudoku[row][x] == num)
            return 0;
    }

    // Verificar columna
    for (int y = 0; y < SIZE; y++) {
        if (s->sudoku[y][col] == num)
            return 0; //
    }

    // Verificar subcuadro 3x3
    int startRow = row - row % 3;
    int startCol = col - col % 3;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (s->sudoku[i + startRow][j + startCol] == num)
                return 0; 
        }
    }
    return 1;
}

void incrementarCasilla (int row, int col){
    if (col < SIZE - 1) {
        col++;
    } else {
        col = 0;
        row++;
    }
}

void emptyCellsExist(Sudoku *s) {
    for (int y = 0; y < SIZE; y++) {
        for (int x = 0; x < SIZE; x++) {
            if (s->sudoku[y][x] == 0)
                s->sudoku[y][x] = 1;
        }
    }
}

void solver(Sudoku *s) {
    int end = 0;
    for (int y = 0; y < SIZE; y++) {
        for (int x = 0; x < SIZE; x++) {
            if (s->sudoku[y][x] == 0)
                s->sudoku[y][x] = 1;
            int num = 2;
            while (!end) {
                while (!promising(s, y, x, num) && s->sudoku[y][x] <= 9)
                    num++;
                if (s->sudoku[y][x] <= 9) {
                    if (s->sudoku[y][x] != s->sudoku[8][8]) {

                    } else {
                        end = 1;
                    }
                } else {
                    
                }
            }
        }
    }
}


int main() {
    /*Sudoku sudoku = {0}; // inicializa toda la matriz en 0

    sudoku.sudoku[2][1] = 2;
    sudoku.sudoku[0][1] = 7;
    sudoku.sudoku[2][0] = 6;

    printf("Sudoku antes de guardar:\n");
    printSudoku(&sudoku);

    // Guardar en archivo
    if (guardarSudoku("sudoku.bin", &sudoku)) {
        printf("Sudoku guardado exitosamente!\n\n");
    } else {
        printf("Error al guardar el sudoku.\n\n");
    }*/

    // descomentar de aquí hasta arriba para guardar un sudoku (cambiar nombre)

    // Crear otro sudoku y cargar desde archivo
    Sudoku sudokuCargado;
    
    if (cargarSudoku("sudoku.bin", &sudokuCargado)) {
        printf("Sudoku cargado exitosamente:\n");
        printSudoku(&sudokuCargado);
    } else {
        printf("Error al cargar el sudoku.\n");
    }

    Sudoku sudokuCargado2;
    
    if (cargarSudoku("sudoku2.bin", &sudokuCargado2)) {
        printf("Sudoku cargado exitosamente:\n");
        printSudoku(&sudokuCargado2);
    } else {
        printf("Error al cargar el sudoku.\n");
    }

    return 0;
}