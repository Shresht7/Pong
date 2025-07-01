#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>

#include "game.h"
#include "terminal.h"
#include "render.h"
#include "ai.h"

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

// Game state variables
GameState current_state;
bool is_running;

// Ball reset timer
int ball_reset_timer = 0;

// Resets the ball's position and direction
void reset_ball()
{
    ball_x = WIDTH / 2;
    ball_y = HEIGHT / 2;
    ball_dx = (rand() % 2 == 0) ? INITIAL_BALL_SPEED : -INITIAL_BALL_SPEED; // Increased initial horizontal speed
    ball_dy = (rand() % 2 == 0) ? INITIAL_BALL_SPEED : -INITIAL_BALL_SPEED; // Increased initial vertical speed
    ball_reset_timer = BALL_RESET_DELAY;                                    // 40 * 50ms = 2 seconds delay
}

// Initializes game variables to their starting values
void setup()
{
    is_running = true;
    current_state = STATE_PLAYING;

    ai_init();
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
            terminal_write_char_to_buffer(x, y, L' ');
        }
    }

    if (current_state == STATE_PLAYING)
    {
        render_borders();
        render_ball();
        render_paddles();
        render_scores();
    }
    else if (current_state == STATE_GAME_OVER)
    {
        render_game_over_screen();
    }

    terminal_print_buffer();
}

// Handles user input
void input()
{
    int key = terminal_read_key();

    if (current_state == STATE_PLAYING)
    {
        switch (key)
        {
        case 'w':
            if (paddle1_y > TOP_WALL_Y)
            {
                paddle1_y--;
            }
            break;
        case 's':
            if (paddle1_y + PADDLE_HEIGHT < BOTTOM_WALL_Y)
            {
                paddle1_y++;
            }
            break;
        case 27: // ESC key
        case 'q':  // 'q' key
            game_over();
            break;
        }
    }
    else if (current_state == STATE_GAME_OVER)
    {
        if (key != -1) // Any key press
        {
            is_running = false;
        }
    }
}

// Displays game over message and exits
void game_over()
{
    current_state = STATE_GAME_OVER;
}

void update_ball_position()
{
    ball_x += ball_dx; // Move ball horizontally
    ball_y += ball_dy; // Move ball vertically
}

void handle_ball_collisions()
{
    // Ball collision with top and bottom walls (adjusted for shifted game area)
    if (ball_y <= TOP_WALL_Y || ball_y >= BOTTOM_WALL_Y - 1)
    {
        ball_dy *= -1; // Reverse vertical direction
    }

    // Ball collision with left paddle
    if (ball_dx < 0 && ball_x + ball_dx <= LEFT_PADDLE_X + 1 && ball_y >= paddle1_y + 1 && ball_y < paddle1_y + 1 + PADDLE_HEIGHT)
    {
        ball_dx *= -1;               // Reverse horizontal direction
        ball_dy += (rand() % 3) - 1; // Add random vertical change (-1, 0, or 1)
    }

    // Ball collision with right paddle
    if (ball_dx > 0 && ball_x + ball_dx >= RIGHT_PADDLE_X && ball_y >= paddle2_y + 1 && ball_y < paddle2_y + 1 + PADDLE_HEIGHT)
    {
        ball_dx *= -1;               // Reverse horizontal direction
        ball_dy += (rand() % 3) - 1; // Add random vertical change (-1, 0, or 1)
    }
}

void check_scoring()
{
    // Scoring
    if (ball_x <= LEFT_SCORE_LIMIT)
    {
        score2++;
        reset_ball(); // Reset ball only
        if (score2 >= MAX_SCORE)
        {
            game_over();
        }
    }
    if (ball_x >= RIGHT_SCORE_LIMIT)
    {
        score1++;
        reset_ball(); // Reset ball only
        if (score1 >= MAX_SCORE)
        {
            game_over();
        }
    }
}

// Updates game state and logic
void logic()
{
    if (current_state != STATE_PLAYING)
    {
        return; // Don't run game logic if not playing
    }

    ai_update();

    if (ball_reset_timer > 0)
    {
        ball_reset_timer--;
        return; // Don't move ball if timer is active
    }

    update_ball_position();
    handle_ball_collisions();
    check_scoring();
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
    while (is_running)
    {
        draw();
        input(); // Call input handler
        logic(); // Call game logic updater

#ifdef _WIN32
        Sleep(50);
#else
        usleep(GAME_SPEED_MS * 1000);
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
