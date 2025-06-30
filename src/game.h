#ifndef GAME_H
#define GAME_H

// Game constants
#define WIDTH 80
#define HEIGHT 24
#define PADDLE_HEIGHT 4

void game_init();     // Initializes the game state and terminal settings
void game_run();      // Contains the main game loop
void game_shutdown(); // Cleans up and restores terminal settings

// Terminal control functions: Handle raw mode for direct input and output
void setup_terminal();   // Configures the terminal for raw mode
void restore_terminal(); // Restores the terminal to its original mode

#endif // GAME_H
