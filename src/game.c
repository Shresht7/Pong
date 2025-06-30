#include "game.h"
#include "terminal.h" // Include the new terminal module
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Platform-specific headers for timing
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

// Game state variables
static int ball_x, ball_y;
static int ball_dx, ball_dy;
static int paddle1_y, paddle2_y;
static int score1, score2;

// Initializes game variables to their starting values
void setup() {
    ball_x = WIDTH / 2;
    ball_y = HEIGHT / 2;
    ball_dx = 1;
    ball_dy = 1;
    paddle1_y = HEIGHT / 2 - PADDLE_HEIGHT / 2;
    paddle2_y = HEIGHT / 2 - PADDLE_HEIGHT / 2;
    score1 = 0;
    score2 = 0;

    srand(time(NULL)); // Seed random number generator
}

// Draws the game board and elements
void draw() {
    terminal_clear_screen(); // Clear screen using terminal module

    // Draw top wall
    for (int i = 0; i < WIDTH; i++) {
        printf("#");
    }
    printf("\n");

    // Draw game area borders
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (x == 0 || x == WIDTH - 1) { // Left and right borders
                printf("|");
            } else {
                printf(" "); // Empty space
            }
        }
        printf("\n");
    }

    // Draw bottom wall
    for (int i = 0; i < WIDTH; i++) {
        printf("#");
    }
    printf("\n");
}

// Handles user input (to be implemented)
void input() {
    // Placeholder for input handling
}

// Updates game state and logic (to be implemented)
void logic() {
    // Placeholder for game logic
}

// Initializes the game: terminal setup and game state
void game_init() {
    terminal_setup(); // Setup terminal using terminal module
    terminal_hide_cursor(); // Hide cursor
    setup();
}

// Main game loop: continuously draws the game
void game_run() {
    while (1) { // Infinite loop for the game
        draw(); // Redraw the game screen

        // Game loop delay to control speed
#ifdef _WIN32
        Sleep(50); // Windows: 50 milliseconds
#else
        usleep(50000); // Unix-like: 50,000 microseconds (50 ms)
#endif
    }
}

// Shuts down the game: restores terminal settings
void game_shutdown() {
    terminal_restore(); // Restore terminal using terminal module
    terminal_show_cursor(); // Show cursor
}
