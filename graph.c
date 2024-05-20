#include <stdbool.h> 
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "graph.h"
#include "player.h"

void renderHearts(int playerHealth) {
    for (int i = 0; i < NUM_HEARTS; i++) {
        SDL_Rect heartRect1 = {WINDOW_WIDTH - (i + 1) * (HEART_WIDTH + HEART_SPACING), HEART_SPACING, HEART_WIDTH, HEART_HEIGHT};
        if (i < playerHealth) {
            SDL_RenderCopy(renderer, heartTexture, NULL, &heartRect1);
        }
    }
}

