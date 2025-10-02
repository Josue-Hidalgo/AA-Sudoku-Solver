#ifndef SUDOKU_LOGIC_H
#define SUDOKU_LOGIC_H

#define SIZE 9

typedef struct {
    int sudoku[SIZE][SIZE];
} Sudoku;

typedef struct {
    int row;
    int col;
} Cell;

// Funciones de lógica
void printSudoku(const Sudoku *s);
int guardarSudoku(const char *filename, const Sudoku *s);
int cargarSudoku(const char *filename, Sudoku *s);
int promising(Sudoku *s, int row, int col, int num);
int findEmptyCell(const Sudoku *s, Cell *cell);
int solver(Sudoku *s);

#endif