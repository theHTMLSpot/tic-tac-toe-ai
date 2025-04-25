#include "tictactoe.h"


char getChar() {
    char buf = 0;
    struct termios old = {};
    if (tcgetattr(STDIN_FILENO, &old) < 0) perror("tcgetattr()");
    
    struct termios new_ = old;
    new_.c_lflag &= ~(ICANON | ECHO);
    new_.c_cc[VMIN] = 1;
    new_.c_cc[VTIME] = 0;
    
    if (tcsetattr(STDIN_FILENO, TCSANOW, &new_) < 0) perror("tcsetattr()");

    if (read(STDIN_FILENO, &buf, 1) < 0) perror("read()");

    if (buf == '\033') {
        // Arrow key sequence: ESC [ <letter>
        char seq[2];
        if (read(STDIN_FILENO, &seq[0], 1) < 0) perror("read()");
        if (read(STDIN_FILENO, &seq[1], 1) < 0) perror("read()");

        if (seq[0] == '[') {
            switch (seq[1]) {
                case 'A': buf = 'u'; break;  // Up
                case 'B': buf = 'd'; break;  // Down
                case 'C': buf = 'r'; break;  // Right
                case 'D': buf = 'l'; break;  // Left
                default: break;
            }
        }
    } else {
        // Normalize wasd
        switch (buf) {
            case 'w': case 'W': buf = 'u'; break;
            case 's': case 'S': buf = 'd'; break;
            case 'a': case 'A': buf = 'l'; break;
            case 'd': case 'D': buf = 'r'; break;
        }
    }

    if (tcsetattr(STDIN_FILENO, TCSADRAIN, &old) < 0) perror("tcsetattr()");
    return buf;
}


void handleMovementInput(char input, int& x, int& y) {
    switch (input) {
        case 'u': if (y > 0) y--; break;
        case 'd': if (y < 2) y++; break;
        case 'l': if (x > 0) x--; break;
        case 'r': if (x < 2) x++; break;
    }
}

bool handleMoveAndCheckWin(int x, int y) {
    if (insert(x, y, o) == 0) {
        char winner = checkWin() == ' ' ? (checkTie() ? 't' : ' ') : checkWin();
        
        if (winner != ' ') {
            printBoard(x, y);
            if (winner == 't') std::cout << "\nIt's a tie!";
            else std::cout << "\nThe winner is " << winner << "!\n";
            if (winner == 'X') x_score++;
            else if (winner == 'O') o_score++;
            usleep(2000000); // Show result
            resetBoard();
            o = false;
            return true;
        } else {
            o = !o;
        }
    } else {
        std::cout << "\nThere's already a piece there!";
        usleep(1000000);
    }
    return false;
}

void resetBoard() {
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            board[i][j] = ' ';
}

// Function to insert a move into the board
int insert(int x, int y, bool o) {
    if (board[y][x] != ' ') {
        
        return 1;
    }

    board[y][x] = o ? 'O' : 'X';
    return 0;
}