#include <stdio.h>
#include <stdbool.h> 

#include <SDL2/SDL_image.h>
#include "audio.h"
#include "player.h"


bool loadMedia(char* background, char* button, char* button3, char* button4) {
    SDL_Surface* backgroundSurface = SDL_LoadBMP(background);
    if (backgroundSurface == NULL) {
        printf("Unable to load background image! SDL_Error: %s\n", SDL_GetError());
        return false;
    }
    backgroundTexture = SDL_CreateTextureFromSurface(renderer, backgroundSurface);
    SDL_FreeSurface(backgroundSurface);
    if (backgroundTexture == NULL) {
        printf("Unable to create background texture! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    SDL_Surface* buttonSurface = IMG_Load(button);
    if (buttonSurface == NULL) {
        printf("Unable to load button image! SDL_Error: %s\n", SDL_GetError());
        return false;
    }
    buttonTexture = SDL_CreateTextureFromSurface(renderer, buttonSurface);
    SDL_FreeSurface(buttonSurface);
    if (buttonTexture == NULL) {
        printf("Unable to create button texture! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    SDL_Surface* buttonSurface3 = IMG_Load(button3);
    if (buttonSurface3 == NULL) {
        printf("Unable to load button 3 image! SDL_Error: %s\n", SDL_GetError());
        return false;
    }
    buttonTexture3 = SDL_CreateTextureFromSurface(renderer, buttonSurface3);
    SDL_FreeSurface(buttonSurface3);
    if (buttonTexture3 == NULL) {
        printf("Unable to create button 3 texture! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    SDL_Surface* buttonSurface4 = IMG_Load(button4);
    if (buttonSurface4 == NULL) {
        printf("Unable to load button 4 image! SDL_Error: %s\n", SDL_GetError());
        return false;
    }
    buttonTexture4 = SDL_CreateTextureFromSurface(renderer, buttonSurface4);
    SDL_FreeSurface(buttonSurface4);
    if (buttonTexture4 == NULL) {
        printf("Unable to create button 4 texture! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    SDL_Surface* starSurface = IMG_Load("imge5.jpg");
    if (starSurface == NULL) {
        printf("Unable to load star image! SDL_Error: %s\n", SDL_GetError());
        return false;
    }
    starTexture = SDL_CreateTextureFromSurface(renderer, starSurface);
    SDL_FreeSurface(starSurface);
    if (starTexture == NULL) {
        printf("Unable to create star texture! SDL_Error: %s\n", SDL_GetError());
        return false;
    }

    return true;
}

