#include "game.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#else
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#endif

// Game objects
static int ball_x, ball_y;
static int ball_dx, ball_dy;
static int paddle1_y, paddle2_y;
static int score1, score2;

void game_init() {
    printf("Initializing game...\n");
}

void game_run() {
    printf("Running game...\n");
}

void game_shutdown() {
    printf("Shutting down game...\n");
}

