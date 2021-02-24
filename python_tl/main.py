from random import random
from shutil import get_terminal_size
from sys import argv
from time import sleep


def get_dims(board) -> tuple:
    """Returns the (y, x) dimensions of a matrix"""
    return len(board), len(board[0])


def next_state(board, y, x) -> int:
    """Returns `1` if the next generation of board[y][x] is alive else `0`"""
    cell = board[y][x]
    neighbors = 0    
    rows, cols = get_dims(board)

    for i in range(max(0, y - 1), min(rows, y + 2)):
        for j in range(max(0, x - 1), min(cols, x + 2)):
            if not (i == y and j == x):
                neighbors += board[i][j]

    return int(cell and 1 < neighbors < 4 or not cell and neighbors == 3)


def next_generation(board) -> list:
    """Modifies a board to its next generation"""
    rows, cols = get_dims(board)
    return [[next_state(board, i, j) for j in range(cols)] for i in range(rows)]


def print_board(board) -> None:
    """Prints out the board, clearing the terminal as it does so"""
    print("\033[2J") # clears terminal
    print("\033[H") # moves to top-left of terminal
    for row in board:
        for i in row:
            print(" #"[i], end="") # buffer isn't flushed
        # flushes buffer, so the whole screen is printed at once
        # prevents cursor from jumping around screen
        print()

if __name__ == "__main__":
    size = get_terminal_size()
    density = float(argv[1]) if len(argv) > 1 else 0.1
    # randomly populating board
    board = [[random() <= density for _ in range(size.columns)] for _ in range(size.lines)]
    # making a glider
    # board[1][1] = 1
    # board[2][2] = 1
    # board[2][3] = 1
    # board[3][1] = 1
    # board[3][2] = 1
    print_board(board)
    try:
        while True:
            print_board(board)
            sleep(0.05) # arbitrary delay (20 fps) that I think looks good
            board = next_generation(board)
    except KeyboardInterrupt: # just to get rid of the ugly exit error  
        exit()
