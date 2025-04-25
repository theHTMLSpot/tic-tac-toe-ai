// tictactoe.h
#ifndef TICTACTOE_H
#define TICTACTOE_H

#include <iostream>
#include <string>
#include <termios.h>
#include <unistd.h>
#include <thread>
#include <mutex>
#include <vector>
#include <random>

char getChar();
void randomMove(bool aivp);
bool checkTie();
char checkWin();
int insert(int x, int y, bool o);
void printBoard(int cursorX, int cursorY);
int player_vs_ai(bool aivp, int level);
int play_game_with_two_players();
void easy(bool aivp);
void get_start_input_loop(int* cursor, int* numOptions, bool* done);
std::vector<std::vector<int>> open_pos();
int start();
void handleMovementInput(char input, int& x, int& y);
bool handleMoveAndCheckWin(int x, int y);
void resetBoard();
void gameplay();

extern char board[3][3];
extern bool done;
extern int cursor;
extern int numOptions;
extern std::string options[];
extern int choice;
extern int x_score;
extern int o_score;
extern std::mutex mtx;
extern bool o;

#endif
