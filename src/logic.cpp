#include "tictactoe.h"
#include <iostream>
#include <vector>
#include <cstdlib>
#include <unistd.h> // For Linux sleep, or use std::this_thread::sleep_for for cross-platform compatibility.



// Function to check if the game is a tie
bool checkTie() {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == ' ') return false;  // If there's still an empty space, no tie
        }
    }
    return checkWin() == ' ';  // Only a tie if there is no winner
}

// Function to check if someone has won
char checkWin() {
    // Check rows
    for (int i = 0; i < 3; i++) {
        if (board[i][0] != ' ' &&
            board[i][0] == board[i][1] &&
            board[i][1] == board[i][2]) {
            return board[i][0];
        }
    }

    // Check columns
    for (int i = 0; i < 3; ++i) {
        if (board[0][i] != ' ' &&
            board[0][i] == board[1][i] &&
            board[1][i] == board[2][i]) {
            return board[0][i];
        }
    }

    // Check diagonal top-left to bottom-right
    if (board[0][0] != ' ' &&
        board[0][0] == board[1][1] &&
        board[1][1] == board[2][2]) {
        return board[0][0];
    }

    // Check diagonal top-right to bottom-left
    if (board[0][2] != ' ' &&
        board[0][2] == board[1][1] &&
        board[1][1] == board[2][0]) {
        return board[0][2];
    }

    // No winner
    return ' ';
}





// Function to return a list of open positions on the board
std::vector<std::vector<int>> open_pos() {
    std::vector<std::vector<int>> to_return;

    // Loop through the board
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (board[i][j] == ' ') {  // If the spot is empty
                to_return.push_back({i, j});  // Add coordinates to list
                
            }
        }
    }

    // If no open positions are found
    if (to_return.empty()) {
        std::cout << "No open positions available!\n";
    }

    return to_return;
} 

// Function to make a random move for the AI
void randomMove(bool aivp) {
    std::vector<std::vector<int>> open_positions = open_pos();
    if (open_positions.empty()) {
        return; // No moves available
    }

    // Randomly select an open position
    while (!open_positions.empty()) {
        int index = rand() % open_positions.size();
       
    

    
        // If we evaluate the move and it's valid
        if (handleMoveAndCheckWin(open_positions[index][1], open_positions[index][0]) == 0) {  // Insert returns 0 on success
            break;
        }        
        open_positions.erase(open_positions.begin() + index);  // Remove the chosen position
    }
    
}
char get_win_value(int x, int y) {
    char local_board[3][3];
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            local_board[i][j] = board[i][j];
        }
    }
    local_board[y][x] = (o) ? 'O' : 'X';  // Simulate placing an 'X' or 'O' in the position

    for (int i = 0; i < 3; i++) {
        if (local_board[i][0] != ' ' &&
            local_board[i][0] == local_board[i][1] &&
            local_board[i][1] == local_board[i][2]) {
            return local_board[i][0];
        }
    }
    
    // Check columns
    for (int i = 0; i < 3; ++i) {
        if (local_board[0][i] != ' ' &&
            local_board[0][i] == local_board[1][i] &&
            local_board[1][i] == local_board[2][i]) {
            return local_board[0][i];
        }
    }
    
    // Check diagonal top-left to bottom-right
    if (local_board[0][0] != ' ' &&
        local_board[0][0] == local_board[1][1] &&
        local_board[1][1] == local_board[2][2]) {
        return local_board[0][0];
    }
    
    // Check diagonal top-right to bottom-left
    if (local_board[0][2] != ' ' &&
        local_board[0][2] == local_board[1][1] &&
        local_board[1][1] == local_board[2][0]) {
        return local_board[0][2];
    }
    
    // No winner
    return ' ';
    
    
}


int evalMove(std::vector<std::string> last_move, bool use_open_pos, bool ai_is_x) {
    int x = std::stoi(last_move[0]);
    int y = std::stoi(last_move[1]);

    // Get the current value at the position (x, y)
    char win_value = get_win_value(x, y);

    if (use_open_pos) {
        if (win_value == 'X') {
            return (ai_is_x) ? 1 : -1; // AI as 'X' wins, otherwise loses
        }
        else if (win_value == 'O') {
            return (ai_is_x) ? -1 : 1; // AI as 'O' wins, otherwise loses
        }
        else {
            return 0; // No winner yet, no impact on the evaluation
        }
    } else {
        if (win_value == 'X') {
            return (ai_is_x) ? 1 : -1; // AI as 'X' wins, otherwise loses
        }
        else if (win_value == 'O') {
            return (ai_is_x) ? -1 : 1; // AI as 'O' wins, otherwise loses
        }
        else {
            return 0; // No winner yet
        }
    }
}



