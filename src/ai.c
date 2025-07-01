#include "ai.h"
#include "game.h"
#include <stdlib.h> // For rand()

static int ai_target_y;

void ai_init()
{
    ai_target_y = HEIGHT / 2; // Start in the middle
}

// Player 2 AI (right paddle)
void ai_update()
{
    // 1. Reaction Delay: Decide if the AI should react in this frame
    if ((rand() % 100) < AI_REACTION_PROBABILITY)
    {
        // 2. Imprecision: Add a random offset to the ball's position
        int inaccuracy = (rand() % (AI_INACCURACY_AMOUNT * 2 + 1)) - AI_INACCURACY_AMOUNT;
        ai_target_y = ball_y + inaccuracy;
    }

    // 3. Inertia: Move the paddle smoothly towards the target Y
    if (paddle2_y + PADDLE_HEIGHT / 2 < ai_target_y)
    {
        if (paddle2_y + AI_PADDLE_SPEED + PADDLE_HEIGHT < BOTTOM_WALL_Y)
        {
            paddle2_y += AI_PADDLE_SPEED;
        }
        else
        {
            paddle2_y = BOTTOM_WALL_Y - PADDLE_HEIGHT; // Snap to border
        }
    }
    else if (paddle2_y + PADDLE_HEIGHT / 2 > ai_target_y)
    {
        if (paddle2_y - AI_PADDLE_SPEED > TOP_WALL_Y)
        {
            paddle2_y -= AI_PADDLE_SPEED;
        }
        else
        {
            paddle2_y = TOP_WALL_Y; // Snap to border
        }
    }
}
