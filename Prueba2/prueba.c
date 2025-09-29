#include <stdio.h>
#include <stdlib.h>

#define SIZE 3

typedef struct {
    int sudoku[SIZE][SIZE][SIZE][SIZE];
} Sudoku;
typedef struct {
    int Y, X, y, x;
} coo;

void printSudoku(const Sudoku *s) {
    printf("-------------------------------+\n");
    for (int iG = 0; iG < SIZE; iG++) {
        for (int jG = 0; jG < SIZE; jG++) {
            for (int i = 0; i < SIZE; i++) {
                for (int j = 0; j < SIZE; j++) {
                    int value = s->sudoku[iG][jG][i][j];
                    printf("%2d ", value);
                }
                printf("| "); // separates horizontal submatrices
            }
            printf("\n");
        }
        if (iG < SIZE - 1)
            printf("---------+----------+----------+\n"); // Separator between submatrix groups
    }
}
void initializeSudoku(Sudoku *s) {
    // G represents the y (rows) and x (columns) of the large matrix
    for (int iG = 0; iG < SIZE; iG++) { 
        for (int jG = 0; jG < SIZE; jG++) {
            // these are now for the submatrices
            for (int i = 0; i < SIZE; i++) {
                for (int j = 0; j < SIZE; j++) {
                    s->sudoku[iG][jG][i][j] = 0;
                }
            }
        }
    }
}
int saveSudoku(const char *filename, const Sudoku *s) {
    FILE *file = fopen(filename, "wb");
    if (file == NULL)
        return 0;
    size_t written = fwrite(s, sizeof(Sudoku), 1, file);
    fclose(file);

    return (written == 1); // 1 if file opened successfully, 0 if error
}
int loadSudoku(const char *filename, Sudoku *s) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL)
        return 0;
    size_t read = fread(s, sizeof(Sudoku), 1, file);
    fclose(file);

    return (read == 1); // 1 if file opened successfully, 0 if error
}



int main() {
    /*Sudoku sudoku;

    initializeSudoku(&sudoku);

    sudoku.sudoku[0][1][2][1] = 3;
    sudoku.sudoku[1][0][0][1] = 4;
    sudoku.sudoku[0][2][2][0] = 1;

    printf("Sudoku antes de guardar:\n");
    printSudoku(&sudoku);

    // Save to file
    if (saveSudoku("sudoku.bin", &sudoku)) {
        printf("Sudoku saved successfully!\n\n");
    } else {
        printf("Error saving sudoku.\n\n");
    }*/

    // uncomment from here to above to save a sudoku (change filename)

    // Create another sudoku and load from file
    Sudoku loadedSudoku;
    
    if (loadSudoku("sudoku2.bin", &loadedSudoku)) {
        printf("Sudoku loaded successfully:\n");
        printSudoku(&loadedSudoku);
    } else {
        printf("Error loading sudoku.\n");
    }

    Sudoku loadedSudoku2;
    
    if (loadSudoku("sudoku.bin", &loadedSudoku2)) {
        printf("Sudoku loaded successfully:\n");
        printSudoku(&loadedSudoku2);
    } else {
        printf("Error loading sudoku.\n");
    }

    return 0;
}