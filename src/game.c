#include "game.h"
#include "terminal.h"
#include "render.h" // Include the new render module
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

// Game state variables (defined here, declared extern in game.h)
int ball_x, ball_y;
int ball_dx, ball_dy;
int paddle1_y, paddle2_y;
int score1, score2;

// Resets the ball's position and direction
void reset_ball()
{
    ball_x = WIDTH / 2;
    ball_y = HEIGHT / 2;
    ball_dx = (rand() % 2 == 0) ? 2 : -2; // Increased initial horizontal speed
    ball_dy = (rand() % 2 == 0) ? 2 : -2; // Increased initial vertical speed
}

// Initializes game variables to their starting values
void setup()
{
    reset_ball(); // Reset ball position and direction
    paddle1_y = HEIGHT / 2 - PADDLE_HEIGHT / 2;
    paddle2_y = HEIGHT / 2 - PADDLE_HEIGHT / 2;
    score1 = 0;
    score2 = 0;

    srand(time(NULL)); // Seed random number generator
}

// Draws the game board and elements to the buffer
void draw()
{
    // Clear the buffer with spaces
    for (int y = 0; y < HEIGHT; y++)
    {
        for (int x = 0; x < WIDTH; x++)
        {
            terminal_write_char_to_buffer(x, y, L' '); // Use L' ' for wide space
        }
    }

    render_borders();
    render_ball();
    render_paddles();
    render_scores();

    terminal_print_buffer(); // Print the entire buffer to the console
}

// Handles user input
void input()
{
    int key = terminal_read_key();

    switch (key)
    {
    case 'w':
        if (paddle1_y > 1)
        { // Ensure paddle doesn't go above top border
            paddle1_y--;
        }
        break;
    case 's':
        if (paddle1_y + PADDLE_HEIGHT < HEIGHT - 1)
        { // Ensure paddle doesn't go below bottom border
            paddle1_y++;
        }
        break;
    case 27:     // ESC key to exit
    case 'q':    // 'q' key to exit
        exit(0); // Exit the game
        break;
    }
}

// Updates game state and logic
void logic()
{
    ball_x += ball_dx; // Move ball horizontally
    ball_y += ball_dy; // Move ball vertically

    // Ball collision with top and bottom walls (adjusted for shifted game area)
    if (ball_y <= 1 || ball_y >= HEIGHT - 2)
    {
        ball_dy *= -1; // Reverse vertical direction
    }

    // Ball collision with left paddle
    if (ball_x == 2 && ball_y >= paddle1_y + 1 && ball_y < paddle1_y + 1 + PADDLE_HEIGHT)
    {
        ball_dx *= -1;               // Reverse horizontal direction
        ball_dy += (rand() % 3) - 1; // Add random vertical change (-1, 0, or 1)
    }

    // Player 2 AI (right paddle)
    if (ball_y + 1 > paddle2_y + 1 + PADDLE_HEIGHT / 2)
    { // Ball is below paddle center
        if (paddle2_y + AI_PADDLE_SPEED + PADDLE_HEIGHT < HEIGHT - 1)
        { // Ensure paddle doesn't go below bottom border
            paddle2_y += AI_PADDLE_SPEED;
        }
        else
        {
            paddle2_y = HEIGHT - 1 - PADDLE_HEIGHT; // Snap to border
        }
    }
    else if (ball_y + 1 < paddle2_y + 1 + PADDLE_HEIGHT / 2)
    { // Ball is above paddle center
        if (paddle2_y - AI_PADDLE_SPEED > 1)
        { // Ensure paddle doesn't go above top border
            paddle2_y -= AI_PADDLE_SPEED;
        }
    }

    // Ball collision with right paddle
    if (ball_x == WIDTH - 3 && ball_y >= paddle2_y + 1 && ball_y < paddle2_y + 1 + PADDLE_HEIGHT)
    {
        ball_dx *= -1;               // Reverse horizontal direction
        ball_dy += (rand() % 3) - 1; // Add random vertical change (-1, 0, or 1)
    }

    // Scoring
    if (ball_x <= 1)
    {
        score2++;
        reset_ball(); // Reset ball only
    }
    if (ball_x >= WIDTH - 2)
    {
        score1++;
        reset_ball(); // Reset ball only
    }
}

// Initializes the game: terminal setup, buffer setup, and game state
void game_init()
{
    terminal_setup();
    terminal_hide_cursor();
    terminal_clear_screen();             // Clear screen after setup
    terminal_init_buffer(WIDTH, HEIGHT); // Initialize the double buffer
    setup();
}

// Main game loop: continuously draws the game
void game_run()
{
    while (1)
    {
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
void game_shutdown()
{
    terminal_clear_screen(); // Clear screen before restoring terminal
    terminal_restore();
    terminal_show_cursor();
    terminal_destroy_buffer(); // Destroy the double buffer
}
