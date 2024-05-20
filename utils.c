// utils.c

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdlib.h>
#include "score.h"
#include "utils.h"

extern SDL_Renderer *renderer; // Make sure renderer is accessible

void displayDeadImageAndExit() {
    // Load the dead image
    SDL_Surface* surface = IMG_Load("dead.png");
    if (surface == NULL) {
        printf("Failed to load dead image! SDL_Error: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    // Create texture from surface
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (texture == NULL) {
        printf("Failed to create texture from surface! SDL_Error: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    // Clear renderer and render the dead image
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);

    // Delay for a few seconds before exiting
    SDL_Delay(3000);

    // Clean up and exit
    SDL_DestroyTexture(texture);

    finalizeScore();

    SDL_Quit();
    exit(EXIT_SUCCESS);
}

