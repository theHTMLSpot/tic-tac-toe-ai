#include "tictactoe.h"

void get_start_input_loop(int* cursor, int* numOptions, bool* done) {
    while (!*done) {
        char input = getChar();
        std::lock_guard<std::mutex> lock(mtx);
        switch (input) {
            case 'u':
                if (*cursor > 0) (*cursor)--;
                break;
            case 'd':
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

int start() {
    int localCursor = 0;
    int localNumOptions = 3;
    bool localDone = false;

    std::string options[] = {"2 Player", "Vs AI", "AI vs AI"};

    std::thread t(get_start_input_loop, &localCursor, &localNumOptions, &localDone);

    while (!localDone) {
        {
            std::lock_guard<std::mutex> lock(mtx);
            std::cout << "\033c";
            std::cout << "Choose Your Game Mode\n(W/S to move, Space/Enter to confirm)\n\n";

            for (int i = 0; i < localNumOptions; i++) {
                if (i == localCursor) {
                    std::cout << "[-] " << options[i] << "\n";
                } else {
                    std::cout << "[ ] " << options[i] << "\n";
                }
            }
        }
        usleep(100000); // 10Hz refresh
    }

    t.join();

    if (localCursor == 1) { // Vs AI
        // Choose Player Shape
        localCursor = 0;
        localDone = false;
        localNumOptions = 2;
        std::string player_options[] = {"X", "O"};

        std::thread shapeThread(get_start_input_loop, &localCursor, &localNumOptions, &localDone);

        while (!localDone) {
            {
                std::lock_guard<std::mutex> lock(mtx);
                std::cout << "\033c";
                std::cout << "Choose Shape (You):\n\n";
                for (int i = 0; i < localNumOptions; i++) {
                    if (i == localCursor) {
                        std::cout << "[-] " << player_options[i] << "\n";
                    } else {
                        std::cout << "[ ] " << player_options[i] << "\n";
                    }
                }
            }
            usleep(100000);
        }

        shapeThread.join();
        int shapeChoice = localCursor; // 0 = X, 1 = O

        // Choose AI Difficulty
        localCursor = 0;
        localDone = false;
        localNumOptions = 3;
        std::string ai_options[] = {"Easy", "Medium", "Hard"};

        std::thread aiThread(get_start_input_loop, &localCursor, &localNumOptions, &localDone);

        while (!localDone) {
            {
                std::lock_guard<std::mutex> lock(mtx);
                std::cout << "\033c";
                std::cout << "Choose AI Difficulty:\n\n";
                for (int i = 0; i < localNumOptions; i++) {
                    if (i == localCursor) {
                        std::cout << "[-] " << ai_options[i] << "\n";
                    } else {
                        std::cout << "[ ] " << ai_options[i] << "\n";
                    }
                }
            }
            usleep(100000);
        }

        aiThread.join();

        // Return value: X = 11/12/13, O = 21/22/23
        return (shapeChoice == 0 ? 10 : 20) + (localCursor + 1);
    }

    return localCursor; // 0 = 2 Player, 2 = AI vs AI
}
