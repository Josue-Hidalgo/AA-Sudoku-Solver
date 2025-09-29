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

int main() {
    /*Sudoku sudoku = {0};

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