#include "tictactoe.h"

void printBoard(int cursorX, int cursorY) {
    std::string line(70, '-'); // makes "-------------------------"
    std::cout << "\033c"; // clear screen
    std::cout << "Use W A S D to move. Press SPACE or ENTER to place. Press Q to quit.\n"
          << line << "\n"
          << "X: " << x_score << " O: " << o_score << "\n\n\n";

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (cursorY == i && cursorX == j) {
                std::cout << "[" << board[i][j] << "]";
            } else {
                std::cout << " " << board[i][j] << " ";
            }
            if (j < 2) std::cout << "|";
        }
        std::cout << "\n";
        if (i < 2) std::cout << "---+---+---\n";
    }
}