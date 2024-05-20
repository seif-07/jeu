#include <stdbool.h> 
#include <SDL2/SDL.h>
#include "graphics.h"
#include "game.h"




void renderHearts(int playerHealth[2]) {
    for (int i = 0; i < NUM_HEARTS; i++) {
        SDL_Rect heartRect1 = {WINDOW_WIDTH - (i + 1) * (HEART_WIDTH + HEART_SPACING), HEART_SPACING, HEART_WIDTH, HEART_HEIGHT};
        SDL_Rect heartRect2 = {WINDOW_WIDTH / 2 - (i + 1) * (HEART_WIDTH + HEART_SPACING), HEART_SPACING, HEART_WIDTH, HEART_HEIGHT};
        if (i < playerHealth[0]) {
            SDL_RenderCopy(renderer, heartTexture, NULL, &heartRect1);
        }
        if (i < playerHealth[1]) {
            SDL_RenderCopy(renderer, heartTexture, NULL, &heartRect2);
        }
    }
}

