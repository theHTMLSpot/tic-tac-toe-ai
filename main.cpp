#include <iostream>
#include <string>
#include <termios.h>
#include <unistd.h>
#include <thread>
#include <mutex>
#include <vector>
#include <random>

std::mutex mtx;

char board[3][3] = {
    {' ', ' ' , ' '},
    {' ', ' ', ' '},
    {' ', ' ', ' '}
};

bool done = false;
int cursor = 0;
int numOptions = 4;
std::string options[] = {"PvP", "PvAI", "AIvAI"};
int choice = 404;
int x_score,o_score = 0;

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
    if (tcsetattr(STDIN_FILENO, TCSADRAIN, &old) < 0) perror("tcsetattr()");

    return buf;
}

bool checkTie(){
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (board[i][j] == ' ') return false;
        }
    }
    return checkWin() == ' '; // Only tie if there's no winner
}

char checkWin() {
    // Check rows
    bool tie = checkTie();
    if (tie) {
        return 't';
    }
    for (int i = 0; i < 3; ++i) {
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

    // Diagonal top-left to bottom-right
    if (board[0][0] != ' ' &&
        board[0][0] == board[1][1] &&
        board[1][1] == board[2][2]) {
        return board[0][0];
    }

    // Diagonal top-right to bottom-left
    if (board[0][2] != ' ' &&
        board[0][2] == board[1][1] &&
        board[1][1] == board[2][0]) {
        return board[0][2];
    }

    // No winner
    return ' ';
}

int insert(int x, int y, bool o) {
    if (board[y][x] == ' ') {
        board[y][x] = (o) ? 'O' : 'X';
        return 0;
    }
    return 1;
}

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

int play_game_with_two_players() {
    bool o = false;
    int x = 0, y = 0;

    while (true) {
        printBoard(x, y);
        char input = getChar();
        char winner = checkWin();
        
        switch (input) {
            case 'w':
                if (y > 0) y--;
                break;
            case 's':
                if (y < 2) y++;
                break;
            case 'a':
                if (x > 0) x--;
                break;
            case 'd':
                if (x < 2) x++;
                break;
            case ' ':
            case '\n':
                if (insert(x, y, o) == 0) {
                    char winner = checkWin();  // move here
                    if (winner != ' ') {
                        printBoard(x, y);
                        if (winner == 't') std::cout << "\nIts a tie!"; else std::cout << "\nThe winner is " << winner << "!\n";
                        if (winner == 'X') x_score++; else if (winner == 'O') o_score++;
                        usleep(2000000); // Show result for 2 seconds
            
                        // Reset board
                        for (int i = 0; i < 3; ++i)
                            for (int j = 0; j < 3; ++j)
                                board[i][j] = ' ';
            
                        o = false;
                        x = 0;
                        y = 0;
                    } else {
                        o = !o;
                    }
                } else {
                    std::cout << "\nThere's already a piece there!";
                    usleep(1000000); // 1 second pause
                }
                break;
            
            case 'q':
                std::cout << "Quitting...\n";
                return 0;
        }
        
    }

    return 0;
}


void get_start_input_loop(int* cursor, int* numOptions, bool* done) {
    while (!*done) {
        char input = getChar();
        std::lock_guard<std::mutex> lock(mtx);
        switch (input) {
            case 'w':
                if (*cursor > 0) (*cursor)--;
                break;
            case 's':
                if (*cursor < *numOptions - 1) (*cursor)++;
                break;
            case ' ':
            case '\n':
                *done = true; // confirm choice
                break;
            case 'q':
                exit(0);
        }
    }
}

std::vector<std::vector<int>> open_pos() {
    std::vector<std::vector<int>> to_return;

    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            if (board[i][j] == ' ') {
                to_return.push_back({i, j}); // Correct C++ syntax
            }
        }
    }

    return to_return;
}



void randomMove(bool aivp) {
    std::vector<std::vector<int>> open_positions = open_pos();

    if (open_positions.empty()) {
        return; // TODO: handle error
    }
    int index = rand() % open_positions.size();
    insert(open_positions[index][0],open_positions[index][1], aivp);
}
void easy(bool aivp) {
    bool o = false;
    int x = 0, y = 0;

    while (true) {
        printBoard(x, y);
        char input = getChar();
        char winner = checkWin();
        
        if (o != aivp) {
            switch (input) {
                case 'w':
                    if (y > 0 && aivp != o) y--; else std::cout << "The ai is playing"; usleep(2000000);
                    break;
                case 's':
                    if (y < 2  && aivp != o) y++; else std::cout << "The ai is playing"; usleep(2000000);
                    break;
                case 'a':
                    if (x > 0  && aivp != o) x--; else std::cout << "The ai is playing"; usleep(2000000);
                    break;
                case 'd':
                    if (x < 2  && aivp != o) x++; else std::cout << "The ai is playing"; usleep(2000000);
                    break;
                case ' ':
                case '\n':
                    if (insert(x, y, o) == 0 && aivp != o) {
                        char winner = checkWin();  // move here
                        if (winner != ' ') {
                            printBoard(x, y);
                            if (winner == 't') std::cout << "\nIts a tie!"; else std::cout << "\nThe winner is " << winner << "!\n";
                            if (winner == 'X') x_score++; else if (winner == 'O') o_score++;
                            usleep(2000000); // Show result for 2 seconds
                
                            // Reset board
                            for (int i = 0; i < 3; ++i)
                                for (int j = 0; j < 3; ++j)
                                    board[i][j] = ' ';
                
                            o = false;
                            x = 0;
                            y = 0;
                        } else {
                            o = !o;
                        }
                    } else {
                        std::cout << "\nThere's already a piece there!";
                        usleep(1000000); // 1 second pause
                    }
                    break;
                
                case 'q':
                    std::cout << "Quitting...\n";
                    return;
            }
        } else {
            randomMove(aivp);
        }
        
        
    }
    
}

int player_vs_ai(bool aivp, int level) {
    switch (level) {
        case 0:
            easy(aivp);
            break;
    }
}



int start() {
    std::cout << "\033c"; 
    std::cout << "Choose Your Game Mode\n";

    std::thread t(get_start_input_loop, &cursor, &numOptions, &done);

    while (!done) {
        {
            std::lock_guard<std::mutex> lock(mtx);
            std::cout << "\033c";
            std::cout << "Choose Your Game Mode\n W S to move cursor Space or Enter to confirm\n\n\n";
            for (int i = 0; i < numOptions; i++) {
                if (i == cursor) {
                    std::cout << "[-] " << options[i] << "\n";
                } else {
                    std::cout << "[ ] " << options[i] << "\n";
                }
            }
        }
        usleep(100000); // refresh at 10Hz
    }

    
        
    

    t.join();
    return cursor;
}
int main(int argc, char* argv[]) {
    std::cout << "\033c"; 
    int choice = start();
    switch (choice)
    {
    case 0:
        play_game_with_two_players();
        break;
        
    
    default:
        break;
    }
    return 0;
}