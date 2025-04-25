#include "tictactoe.h"

void easy(bool aivp) {

    int x = 0, y = 0;

    while (true) {
        printBoard(x, y);  // Print the board

        // Check if it's the AI's turn
        if (o == aivp) {
            std::cout << "AI's turn...\n";
            usleep(1000000);
            std::cout << "AI is making a move...\n";
              // Simulate AI thinking time
            randomMove(aivp); 
            continue;
        }
        

        // Player's turn
        char input = getChar();  // Get user input
        switch (input) {
            case 'u': case 'l': case 'r': case 'd':
                handleMovementInput(input, x, y);  // Move the cursor
                break;

            case ' ': case '\n':  // Place 'X' or 'O'
                if (handleMoveAndCheckWin(x, y)) {
                    std::cout << "Player wins! Resetting the board.\n";
                    usleep(10000);
                }
                break;

            case 'q':  // Quit the game
                std::cout << "Quitting...\n";
                return;  // Exit the game

            default:
                std::cout << "Invalid input. Use 'u', 'l', 'r', 'd' to move and ' ' to place a mark.\n";
                usleep(10000);
                break;
        }

        // Check for a tie after each player's move
        if (checkTie()) {
            std::cout << "It's a tie! Resetting the board.\n";
            x = 0; y = 0;  // Reset the board after a tie
        }
    }
}



int play_game_with_two_players() {
    
    int x = 0, y = 0;

    while (true) {
        printBoard(x, y);
        char input = getChar();

        switch (input) {
            case 'u': case 'l': case 'r': case 'd':
                handleMovementInput(input, x, y);
                break;
            case ' ': case '\n':
                if (handleMoveAndCheckWin(x, y)) {
                    x = 0; y = 0;
                }
                break;
            case 'q':
                std::cout << "Quitting...\n";
                return 0;
        }
    }

    return 0;
}

int player_vs_ai(bool aivp, int level) {
    switch (level) {
        case 1:
            easy(aivp);
            break;
        case 2:
            // Implement medium difficulty AI logic here
            break;
        case 3:
            // Implement hard difficulty AI logic here
            break;
        default:
            std::cout << "Invalid difficulty level.\n";
            return -1;
    }
    return 0;
}


void gameplay(){
    std::cout << "\033c"; 
    int choice = start();
    switch (choice)
    {
    case 0:
        play_game_with_two_players();
        break;
    case 11:
        player_vs_ai(true, 1);
        break;
    case 12:
        player_vs_ai(true, 2);
        break;
    case 13:
        player_vs_ai(true, 3);
        break;
    case 21:
        player_vs_ai(false, 1);
        break;
    case 22:
        player_vs_ai(false, 2);
        break;
    case 23:
        player_vs_ai(false, 3);
        break;
           
    
    default:
        break;
    }
}