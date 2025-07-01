#include <stdio.h>
#include <wchar.h>

#include "terminal.h"
#include "render.h"

// Game state variables (declared as extern in render.h)

extern int ball_x, ball_y;
extern int paddle1_y, paddle2_y;
extern int score1, score2;

// Unicode characters as constants with visual representation

const wchar_t BORDER_HORIZONTAL = 0x2500;   // ─
const wchar_t BORDER_VERTICAL = 0x2502;     // │
const wchar_t BORDER_TOP_LEFT = 0x250C;     // ┌
const wchar_t BORDER_TOP_RIGHT = 0x2510;    // ┐
const wchar_t BORDER_BOTTOM_LEFT = 0x2514;  // └
const wchar_t BORDER_BOTTOM_RIGHT = 0x2518; // ┘
const wchar_t PADDLE_CHAR = 0x2588;         // █
const wchar_t BALL_CHAR = 0x2022;           // •

// Renders the game borders to the buffer
void render_borders()
{
    // Draw top border
    terminal_write_char_to_buffer(0, TOP_WALL_Y, BORDER_TOP_LEFT); // Top-left corner
    for (int i = 1; i < WIDTH - 1; i++)
    {
        terminal_write_char_to_buffer(i, TOP_WALL_Y, BORDER_HORIZONTAL); // Horizontal line
    }
    terminal_write_char_to_buffer(WIDTH - 1, TOP_WALL_Y, BORDER_TOP_RIGHT); // Top-right corner

    // Draw side borders
    for (int y = TOP_WALL_Y + 1; y < BOTTOM_WALL_Y; y++)
    {
        terminal_write_char_to_buffer(0, y, BORDER_VERTICAL);         // Left vertical line
        terminal_write_char_to_buffer(WIDTH - 1, y, BORDER_VERTICAL); // Right vertical line
    }

    // Draw bottom border
    terminal_write_char_to_buffer(0, BOTTOM_WALL_Y, BORDER_BOTTOM_LEFT); // Bottom-left corner
    for (int i = 1; i < WIDTH - 1; i++)
    {
        terminal_write_char_to_buffer(i, BOTTOM_WALL_Y, BORDER_HORIZONTAL); // Horizontal line
    }
    terminal_write_char_to_buffer(WIDTH - 1, BOTTOM_WALL_Y, BORDER_BOTTOM_RIGHT); // Bottom-right corner
}

// Renders the ball to the buffer
void render_ball()
{
    terminal_write_char_to_buffer(ball_x, ball_y + 1, BALL_CHAR);
}

// Renders the paddles to the buffer
void render_single_paddle(int x, int y)
{
    for (int i = 0; i < PADDLE_HEIGHT; i++)
    {
        terminal_write_char_to_buffer(x, y + 1 + i, PADDLE_CHAR);
    }
}

// Renders the paddles to the buffer
void render_paddles()
{
    render_single_paddle(LEFT_PADDLE_X, paddle1_y);
    render_single_paddle(RIGHT_PADDLE_X, paddle2_y);
}

void render_single_score(int x, int y, const wchar_t *score_str)
{
    int score_len = wcslen(score_str);
    for (int i = 0; i < score_len; i++)
    {
        terminal_write_char_to_buffer(x + i, y, score_str[i]);
    }
}

// Renders the player scores to the buffer
void render_scores()
{
    // Player 1 score (left half)
    wchar_t score1_str[20];
    swprintf(score1_str, sizeof(score1_str) / sizeof(wchar_t), L"P1: %d", score1);
    int score1_x = (WIDTH / 2 - wcslen(score1_str)) / 2;
    render_single_score(score1_x, 0, score1_str);

    // Player 2 score (right half)
    wchar_t score2_str[20];
    swprintf(score2_str, sizeof(score2_str) / sizeof(wchar_t), L"P2: %d", score2);
    int score2_x = WIDTH / 2 + (WIDTH / 2 - wcslen(score2_str)) / 2;
    render_single_score(score2_x, 0, score2_str);
}

void render_game_over_screen()
{
    wchar_t message[50];
    if (score1 >= MAX_SCORE)
    {
        swprintf(message, sizeof(message) / sizeof(wchar_t), L"Player 1 Wins! Final Score: %d - %d", score1, score2);
    }
    else if (score2 >= MAX_SCORE)
    {
        swprintf(message, sizeof(message) / sizeof(wchar_t), L"Player 2 Wins! Final Score: %d - %d", score2, score1);
    }
    else
    {
        swprintf(message, sizeof(message) / sizeof(wchar_t), L"Game Over! Final Score: %d - %d", score1, score2);
    }

    int msg_len = wcslen(message);
    int msg_x = (WIDTH - msg_len) / 2;
    int msg_y = HEIGHT / 2;

    for (int i = 0; i < msg_len; i++)
    {
        terminal_write_char_to_buffer(msg_x + i, msg_y, message[i]);
    }

    const wchar_t *prompt = L"Press any key to exit...";
    int prompt_len = wcslen(prompt);
    int prompt_x = (WIDTH - prompt_len) / 2;
    int prompt_y = msg_y + 2;

    for (int i = 0; i < prompt_len; i++)
    {
        terminal_write_char_to_buffer(prompt_x + i, prompt_y, prompt[i]);
    }
}
