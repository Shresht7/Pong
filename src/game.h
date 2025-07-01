#ifndef GAME_H
#define GAME_H

// Game constants: Define the dimensions of the game area and paddle height
#define WIDTH 80
#define HEIGHT 24
#define PADDLE_HEIGHT 4
#define AI_PADDLE_SPEED 1 // AI paddle movement speed (per move)
#define MAX_SCORE 5       // Score needed to win the game

// AI behavior
#define AI_REACTION_PROBABILITY 85 // Percentage (0-100) chance to update target position each frame
#define AI_INACCURACY_AMOUNT 2     // Max random offset from the ball's actual position

// Gameplay constants
#define GAME_SPEED_MS 50
#define INITIAL_BALL_SPEED 2
#define BALL_RESET_DELAY 40 // In game ticks (40 * 50ms = 2s)

// Wall and paddle positions
#define TOP_WALL_Y 1
#define BOTTOM_WALL_Y (HEIGHT - 1)
#define LEFT_PADDLE_X 1
#define RIGHT_PADDLE_X (WIDTH - 2)
#define LEFT_SCORE_LIMIT 1
#define RIGHT_SCORE_LIMIT (WIDTH - 2)

// Game state variables (declared as extern so render.c can access them)
extern int ball_x, ball_y;
extern int ball_dx, ball_dy;
extern int paddle1_y, paddle2_y;
extern int score1, score2;
extern int ball_reset_timer;

// Function declarations: Public functions exposed by the game module
void game_init();     // Initializes the game state and terminal settings
void game_run();      // Contains the main game loop
void game_shutdown(); // Cleans up and restores terminal settings

// Internal game functions
void setup();
void draw();
void input();
void logic();
void game_over(); // Declare new game_over function

#endif // GAME_H
