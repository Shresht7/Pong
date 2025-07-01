#ifndef RENDER_H
#define RENDER_H

// Forward declarations for game state variables (defined in game.c)
// These are needed by render functions to know what to draw.

extern int ball_x, ball_y;
extern int paddle1_y, paddle2_y;
extern int score1, score2;

// Game constants (defined in game.h, included here for convenience)
#include "game.h"

// Rendering functions

void render_borders();
void render_ball();
void render_paddles();
void render_scores();
void render_game_over_screen();

#endif // RENDER_H
