#include "game.h"
#include "terminal.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <wchar.h>

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

// Renders the game borders to the buffer
void render_borders() {
    // Draw top border
    terminal_write_char_to_buffer(0, 1, 0x250C); // Top-left corner
    for (int i = 1; i < WIDTH - 1; i++) {
        terminal_write_char_to_buffer(i, 1, 0x2500); // Horizontal line
    }
    terminal_write_char_to_buffer(WIDTH - 1, 1, 0x2510); // Top-right corner

    // Draw side borders
    for (int y = 2; y < HEIGHT - 1; y++) {
        terminal_write_char_to_buffer(0, y, 0x2502); // Left vertical line
        terminal_write_char_to_buffer(WIDTH - 1, y, 0x2502); // Right vertical line
    }

    // Draw bottom border
    terminal_write_char_to_buffer(0, HEIGHT - 1, 0x2514); // Bottom-left corner
    for (int i = 1; i < WIDTH - 1; i++) {
        terminal_write_char_to_buffer(i, HEIGHT - 1, 0x2500); // Horizontal line
    }
    terminal_write_char_to_buffer(WIDTH - 1, HEIGHT - 1, 0x2518); // Bottom-right corner
}

// Renders the ball to the buffer
void render_ball() {
    terminal_write_char_to_buffer(ball_x, ball_y + 1, 0x2022); // U+2022 for bullet
}

// Renders the player scores to the buffer
void render_scores() {
    // Player 1 score (left half)
    wchar_t score1_str[20];
    swprintf(score1_str, sizeof(score1_str)/sizeof(wchar_t), L"P1: %d", score1);
    int score1_len = wcslen(score1_str);
    int score1_x = (WIDTH / 2 - score1_len) / 2;
    for (int i = 0; i < score1_len; i++) {
        terminal_write_char_to_buffer(score1_x + i, 0, score1_str[i]);
    }

    // Player 2 score (right half)
    wchar_t score2_str[20];
    swprintf(score2_str, sizeof(score2_str)/sizeof(wchar_t), L"P2: %d", score2);
    int score2_len = wcslen(score2_str);
    int score2_x = WIDTH / 2 + (WIDTH / 2 - score2_len) / 2;
    for (int i = 0; i < score2_len; i++) {
        terminal_write_char_to_buffer(score2_x + i, 0, score2_str[i]);
    }
}

// Draws the game board and elements to the buffer
void draw() {
    // Clear the buffer with spaces
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            terminal_write_char_to_buffer(x, y, L' '); // Use L' ' for wide space
        }
    }

    render_borders();
    render_ball();
    render_scores();

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

    // Ball collision with top and bottom walls (adjusted for shifted game area)
    if (ball_y <= 0 || ball_y >= HEIGHT - 2) { // HEIGHT - 2 because top border is at y=1, bottom at HEIGHT-1
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
