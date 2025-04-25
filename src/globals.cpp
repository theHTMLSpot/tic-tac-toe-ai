// globals.cpp
#include "tictactoe.h"

char board[3][3] = {
    {' ', ' ', ' '},
    {' ', ' ', ' '},
    {' ', ' ', ' '}
};

bool done = false;
bool o = false;
int cursor = 0;
int numOptions = 3;
std::string options[] = {"PvP", "PvAI", "AIvAI"};
int choice = 404;
int x_score = 0;
int o_score = 0;
std::mutex mtx;

