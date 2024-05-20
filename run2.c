#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "run2.h"
#include "player.h"
#include "score.h"
#include "perso.h"

void rune() {
    if (!initSDL()) {
        printf("Failed to initialize SDL!\n");
        return;
    }
    if (!loadPlayerTexture()) {
    	printf("Failed to load player texture!\n");
    	return;
    }

    initializeScore();

    chargerMap("bac1.bmp","imge1.jpg" , "bac2.jpg", "imge3.jpg", "imge4.jpg");
    if (!loadHeartTexture()) {
        printf("Failed to load heart texture!\n");
        return;
    }
    if (!loadAudio()) {
        printf("Failed to load audio files!\n");
        return;
    }

    int playerHealth = 5; 

    bool running = true;
    SDL_Event event;
    bool movingRight = true;
    int starSpeed = 40;

    Uint32 lastTime = SDL_GetTicks();
    Uint32 currentTime;
    Uint32 deltaTime = 0;
    Uint32 updateInterval = 1000;
    
    initPlayer();
    perso player; 
    initPerso(&player);
    
    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                handleMouseClick(event);
            }
        }

        if (movingRight) {
            starRect.x += starSpeed; 
            if (starRect.x >= 1600) {
                movingRight = false;
            }
        } else {
            starRect.x -= starSpeed; 
            if (starRect.x <= 0) {
                movingRight = true;
            }
        }
        SDL_RenderClear(renderer);
 

        printf("Score: %d\n", getScore());

        const Uint8 *keyState = SDL_GetKeyboardState(NULL);

        deplacer_Perso(&player, keyState);
        saut(&player, keyState);
        scrolling(0, keyState);
        animer_Perso(&player);




        playBackgroundMusic();
        AfficherMap(playerHealth, keyState, player);


        currentTime = SDL_GetTicks();
        deltaTime += currentTime - lastTime;
        lastTime = currentTime;

        if (deltaTime >= updateInterval) {
            updateScore();
            deltaTime -= updateInterval;
        }

        SDL_Delay(50);
    }

    closeSDL();
}

