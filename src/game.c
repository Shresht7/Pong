#include "game.h"
#include "terminal.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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

// Draws the game board and elements to the buffer
void draw() {
    // Clear the buffer with spaces
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            terminal_write_char_to_buffer(x, y, ' ');
        }
    }

    // Draw top wall to buffer
    for (int i = 0; i < WIDTH; i++) {
        terminal_write_char_to_buffer(i, 0, '#');
    }

    // Draw game area borders to buffer
    for (int y = 0; y < HEIGHT; y++) {
        terminal_write_char_to_buffer(0, y, '|'); // Left border
        terminal_write_char_to_buffer(WIDTH - 1, y, '|'); // Right border
    }

    // Draw bottom wall to buffer
    for (int i = 0; i < WIDTH; i++) {
        terminal_write_char_to_buffer(i, HEIGHT - 1, '#');
    }

    // Draw ball to buffer
    terminal_write_char_to_buffer(ball_x, ball_y, 'O');

    terminal_print_buffer(); // Print the entire buffer to the console
}

// Handles user input (to be implemented)
void input() {
    // Placeholder for input handling
}

// Updates game state and logic
void logic() {
    ball_x += ball_dx; // Move ball horizontally
    ball_y += ball_dy; // Move ball vertically

    // Ball collision with top and bottom walls
    if (ball_y <= 0 || ball_y >= HEIGHT - 1) {
        ball_dy *= -1; // Reverse vertical direction
    }
}

// Initializes the game: terminal setup, buffer setup, and game state
void game_init() {
    terminal_setup();
    terminal_hide_cursor();
    terminal_init_buffer(WIDTH, HEIGHT); // Initialize the double buffer
    setup();
}

// Main game loop: continuously draws the game
void game_run() {
    while (1) {
        draw();
        input(); // Call input handler
        logic(); // Call game logic updater

#ifdef _WIN32
        Sleep(50);
#else
        usleep(50000);
#endif
    }
}

// Shuts down the game: restores terminal settings and destroys buffer
void game_shutdown() {
    terminal_restore();
    terminal_show_cursor();
    terminal_destroy_buffer(); // Destroy the double buffer
}