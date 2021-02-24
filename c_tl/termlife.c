#include <stdio.h>
#include <stdlib.h>
#include "util.h"


// returns `1` if cell will be alive in the next generation and `0` if not
int next_state(int rows, int cols, int board[rows][cols], int y, int x) {
    int cell = board[y][x];
    int neighbors = 0;

    // getting a cell's neighbors by the 3x3 area surrounding the cell
    for (int i = max(0, y - 1); i < min(rows, y + 2); ++i) {
        for (int j = max(0, x - 1); j < min(cols, x + 2); ++j) {
            if (!(i == y && j == x)) {
                neighbors += board[i][j];
            }
        }
    }

    return cell && 1 < neighbors && neighbors < 4
        || !cell && neighbors == 3;
}


// adjusts the board to the next generation
void next_generation(int rows, int cols, int board[rows][cols]) {
    int next_gen[rows][cols];

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            next_gen[i][j] = next_state(rows, cols, board, i, j);
        }
    }

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            board[i][j] = next_gen[i][j];
        }
    }
}

void print_board(int rows, int cols, int board[rows][cols]) {
    printf("\x1b[2J"); // clears screen
    printf("\x1b[H"); // moves cursor to top-left

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            putchar(" #"[board[i][j]]);
        }
    }
}

int main() {
    struct winsize w = get_term_size();
    const int rows = w.ws_row;
    const int cols = w.ws_col;

    int board[rows][cols];
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            board[i][j] = rand() % 100 < 10; // 10% chance of alive cell
        }
    }

    setbuf(stdout, NULL); // prevents cursor jumping around screen

    for (int iters = 0;; ++iters) {
        print_board(rows, cols, board);
        sleep_ms(50);
        next_generation(rows, cols, board);
    }
}