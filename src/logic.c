#include <stdio.h>
#include <stdlib.h>

#define SIZE 9

typedef struct {
    int sudoku[SIZE][SIZE];
} Sudoku;

typedef struct {
    int row;
    int col;
} Cell;

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
    for (int x = 0; x < SIZE; x++)
        if (s->sudoku[row][x] == num)
            return 0;

    // Verificar columna
    for (int y = 0; y < SIZE; y++)
        if (s->sudoku[y][col] == num)
            return 0; //

    // Verificar subcuadro 3x3
    int startRow = row - row % 3;
    int startCol = col - col % 3;

    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            if (s->sudoku[i + startRow][j + startCol] == num)
                return 0; 
    return 1;
}

// Función para encontrar la siguiente celda vacía
int findEmptyCell(const Sudoku *s, Cell *cell) {
    for (cell->row = 0; cell->row < SIZE; cell->row++) {
        for (cell->col = 0; cell->col < SIZE; cell->col++) {
            if (s->sudoku[cell->row][cell->col] == 0) {
                return 1; // Encontró celda vacía
            }
        }
    }
    return 0; // No hay celdas vacías
}

int solver(Sudoku *s) {
    Cell current;
    
    // Si no hay celdas vacías, el sudoku está resuelto
    if (!findEmptyCell(s, &current))
        return 1; // Éxito
    
    int row = current.row;
    int col = current.col;
    
    // Probar números del 1 al 9
    for (int num = 1; num <= 9; num++) {
        if (promising(s, row, col, num)) {
            // Intentar con este número
            s->sudoku[row][col] = num;
            
            // Recursivamente resolver el resto
            if (solver(s))
                return 1; // Éxito
            
            // Si falla, deshacer (backtrack)
            s->sudoku[row][col] = 0;
        }
    }
    
    return 0; // No hay solución
}

