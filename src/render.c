#include "render.h"
#include "terminal.h"
#include <stdio.h>
#include <wchar.h>

// Game state variables (declared as extern in render.h)
extern int ball_x, ball_y;
extern int paddle1_y, paddle2_y;
extern int score1, score2;

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
