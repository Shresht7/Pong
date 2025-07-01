#include "ai.h"
#include "game.h"

// Player 2 AI (right paddle)
void ai_update()
{
    if (ball_y + 1 > paddle2_y + 1 + PADDLE_HEIGHT / 2)
    { // Ball is below paddle center
        if (paddle2_y + AI_PADDLE_SPEED + PADDLE_HEIGHT < BOTTOM_WALL_Y)
        { // Ensure paddle doesn't go below bottom border
            paddle2_y += AI_PADDLE_SPEED;
        }
        else
        {
            paddle2_y = BOTTOM_WALL_Y - PADDLE_HEIGHT; // Snap to border
        }
    }
    else if (ball_y + 1 < paddle2_y + 1 + PADDLE_HEIGHT / 2)
    { // Ball is above paddle center
        if (paddle2_y - AI_PADDLE_SPEED > TOP_WALL_Y)
        { // Ensure paddle doesn't go above top border
            paddle2_y -= AI_PADDLE_SPEED;
        }
    }
}
