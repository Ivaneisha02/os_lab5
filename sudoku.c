#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define SIZE 9

// Sudoku board. Modify this to test your checker!
// 0 == empty cell; 1-9 is the filled in digit.
int board[SIZE][SIZE] = {
    {1,0,0,0,0,0,0,0,0},
    {0,2,0,0,0,0,0,0,0},
    {0,0,3,0,0,0,0,0,0},
    {0,0,0,4,0,0,0,0,0},
    {0,0,0,0,5,0,0,0,0},
    {0,0,0,0,0,6,0,0,0},
    {0,0,0,0,0,0,7,0,0},
    {0,0,0,0,0,0,0,8,0},
    {0,0,0,0,0,0,0,0,9},
};

bool row_check[SIZE];
bool col_check[SIZE];
bool box_check[SIZE];

void printBoard(int board[SIZE][SIZE]) {
    int i,j;
    for(i = 0; i<SIZE; i++) {
        for(j = 0; j<SIZE; j++) {
            printf("%5d", board[i][j]);
        }
        printf("\n");
    }
    printf("\n");
}

// Checks the given row for duplicate numbers, and updates the row_check
// value for that row appropriately. If no number is repeated in that row,
// row_check[row] will be set to true; otherwise, it will be false.
void* checkRow(void* args) {
    int row = *((int*)args);
    int j;
    // Array to keep track of numbers encountered in the row
    bool num_check[SIZE + 1] = {false};

    for (j = 0; j < SIZE; j++) {
        int num = board[row][j];

        if (num_check[num]) {
            // Duplicate found in the row
            row_check[row] = false;
            pthread_exit(NULL);
        }

        num_check[num] = true;
    }

    // No duplicates found in the row
    row_check[row] = true;
    pthread_exit(NULL);
    return NULL;
}

// Checks the given col for duplicate numbers, and updates the col_check
// value for that col appropriately. If no number is repeated in that col,
// col_check[col] will be set to true; otherwise, it will be false.
void* checkCol(void* args) {
    int col = *((int*)args);

    // Array to keep track of numbers encountered in the row
    bool num_check[SIZE + 1] = {false};
    int j;
    for (j = 0; j < SIZE; j++) {
        int num = board[j][col];

        if (num_check[num]) {
            // Duplicate found in the row
            col_check[col] = false;
            pthread_exit(NULL);
        }

        num_check[num] = true;
    }

    // No duplicates found in the row
    col_check[col] = true;
    pthread_exit(NULL);
    return NULL;
    return NULL;
}

// Checks the given 3x3 box for duplicate numbers, and updates the box_check
// value for that box appropriately. If no number is repeated in that box,
// box_check[box] will be set to true; otherwise, it will be false.
void* checkBox(void* args) {
    int box = *((int*) args);

    // Array to keep track of numbers encountered in the box
    bool num_check[SIZE + 1] = {false};

    // Calculate the starting row and column for the box
    int startRow = (box / 3) * 3;
    int startCol = (box % 3) * 3;
    int i, j;
    for (i = startRow; i < startRow + 3; i++) {
        for (j = startCol; j < startCol + 3; j++) {
            int num = board[i][j];

            if (num_check[num]) {
                // Duplicate found in the box
                box_check[box] = false;
                pthread_exit(NULL);
            }

            num_check[num] = true;
        }
    }

    // No duplicates found in the box
    box_check[box] = true;
    pthread_exit(NULL);
    return NULL;
}

// Spawn a thread to fill each cell in each result matrix.
// How many threads will you spawn?
int main() { 
    // 1. Print the board.
    printf("Board:\n");
    printBoard(board);
    
    // 2. Create pthread_t objects for our threads.
    pthread_t row_thread[SIZE];
    pthread_t col_thread[SIZE];
    pthread_t box_thread[SIZE];
    // 3. Create a thread for each cell of each matrix operation.
    int i,j;
    for (i = 0; i < SIZE; i++) {
        int* row = malloc(sizeof(int));
        *row = i;
        pthread_create(&row_thread[i], NULL, checkRow, &row);

        int* col = malloc(sizeof(int));
        *col = i;
        pthread_create(&col_thread[i], NULL, checkCol, &col);

        int* box = malloc(sizeof(int));
        *box = i;
        pthread_create(&box_thread[i], NULL, checkBox, &box);
    }
    // 4. Wait for all threads to finish.
    
    for (i = 0; i < SIZE; i++) {
        pthread_join(row_thread[i], NULL);
        pthread_join(col_thread[i], NULL);
        pthread_join(box_thread[i], NULL);
    }
    // 5. Print the results.
    printf("Results:\n");
    bool all_rows_passed = true;
    printf("Rows:\n");
    for (i = 0; i < SIZE; i++) {
        if (!row_check[i]) {
            printf("Row %i did not pass\n", i);
            all_rows_passed = false;
        }
    }
    if (all_rows_passed) {
        printf("All rows passed!\n");
    }

    bool all_cols_passed = true;
    printf("Cols:\n");
    for (i = 0; i < SIZE; i++) {
        if (!col_check[i]) {
            printf("Col %i did not pass\n", i);
            all_cols_passed = false;
        }
    }
    if (all_cols_passed) {
        printf("All cols passed!\n");
    }

    bool all_boxes_passed = true;
    printf("Boxes:\n");
    for ( i = 0; i < SIZE; i++) {
        if (!box_check[i]) {
            printf("Box %i did not pass\n", i);
            all_boxes_passed = false;
        }
    }
    if (all_boxes_passed) {
        printf("All boxes passed!\n");
    }

    return 0;


}

