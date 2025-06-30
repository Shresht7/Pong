#ifndef GAME_H
#define GAME_H

// Game constants: Define the dimensions of the game area and paddle height
#define WIDTH 80
#define HEIGHT 24
#define PADDLE_HEIGHT 4

// Function declarations: Public functions exposed by the game module
void game_init();      // Initializes the game state and terminal settings
void game_run();       // Contains the main game loop
void game_shutdown();  // Cleans up and restores terminal settings

// Internal game rendering functions (declared here for game.c to use)
void setup();
void draw();
void input();
void logic();

// New rendering functions
void render_borders();
void render_ball();
void render_scores();

#endif // GAME_H