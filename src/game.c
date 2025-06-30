#include <stdio.h>  // For standard input/output functions (printf, etc.)
#include <stdlib.h> // For general utilities (system, srand, etc.)
#include <time.h>   // For time-related functions (used to seed random number generator)
#include "game.h"

// Platform-specific includes for Windows and Unix-like systems
#ifdef _WIN32
#include <windows.h> // Windows API for console manipulation
#include <conio.h>   // For console input/output (e.g., _kbhit, _getch)
#else
#include <unistd.h>  // For POSIX operating system API (e.g., usleep)
#include <termios.h> // For terminal I/O (raw mode)
#include <fcntl.h>   // For file control options (e.g., non-blocking I/O)
#endif

// Game objects: Static to limit their scope to this file (encapsulation)
static int ball_x, ball_y;       // Ball's current X and Y coordinates
static int ball_dx, ball_dy;     // Ball's direction (delta X and Y)
static int paddle1_y, paddle2_y; // Y-coordinate of paddle 1 and paddle 2 (top edge)
static int score1, score2;       // Scores for player 1 and player 2

// Platform-specific variables for terminal control
#ifdef _WIN32
HANDLE hConsole;             // Handle to the console input buffer
DWORD original_console_mode; // Stores the original console mode to restore later
#else
struct termios original_termios; // Stores the original terminal attributes to restore later
#endif

// Sets up the terminal for raw mode (disables echoing, line buffering)
void setup_terminal()
{
#ifdef _WIN32
    hConsole = GetStdHandle(STD_INPUT_HANDLE);        // Get handle to standard input
    GetConsoleMode(hConsole, &original_console_mode); // Save original console mode
    // Disable line input (input is read character by character) and echo input
    SetConsoleMode(hConsole, original_console_mode & ~(ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT));
#else
    tcgetattr(STDIN_FILENO, &original_termios); // Get current terminal attributes
    struct termios raw = original_termios;      // Create a copy
    raw.c_lflag &= ~(ICANON | ECHO);            // Disable canonical mode (line buffering) and echoing
    tcsetattr(STDIN_FILENO, TCSANOW, &raw);     // Apply the new attributes immediately
#endif
}

// Restores the terminal to its original mode
void restore_terminal()
{
#ifdef _WIN32
    SetConsoleMode(hConsole, original_console_mode); // Restore original console mode
#else
    tcsetattr(STDIN_FILENO, TCSANOW, &original_termios); // Restore original terminal attributes
#endif
}

// Initializes the game state variables
void setup()
{
    ball_x = WIDTH / 2;                         // Ball starts in the middle horizontally
    ball_y = HEIGHT / 2;                        // Ball starts in the middle vertically
    ball_dx = 1;                                // Initial ball movement direction (right)
    ball_dy = 1;                                // Initial ball movement direction (down)
    paddle1_y = HEIGHT / 2 - PADDLE_HEIGHT / 2; // Paddle 1 starts centered vertically
    paddle2_y = HEIGHT / 2 - PADDLE_HEIGHT / 2; // Paddle 2 starts centered vertically
    score1 = 0;                                 // Player 1 score starts at 0
    score2 = 0;                                 // Player 2 score starts at 0

    srand(time(NULL)); // Seed the random number generator with current time
}

// Draws the game elements on the screen
void draw()
{
    // Clear the screen and move cursor to top-left (0,0)
#ifdef _WIN32
    system("cls"); // Windows command to clear console
#else
    printf("\033[2J\033[H"); // ANSI escape codes: clear screen, move cursor home
#endif

    printf("Hello, World!\n");
}

// Initializes the game: sets up terminal and game state
void game_init()
{
    setup_terminal();
    setup();
}

// Main game loop (currently just draws once)
void game_run()
{
    draw();
}

// Shuts down the game: restores terminal to its original state
void game_shutdown()
{
    restore_terminal();
}
