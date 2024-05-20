#include <stdio.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "run.h"
#include "game.h"
#include "score.h"
#include "perso.h"
#include "enemy.h"
#include "utils.h"


void run() {
    if (!initSDL()) {
        printf("Failed to initialize SDL!\n");
        return;
    }

    initializeScore();



    chargerMap("back3.bmp", "imge1.jpg", "bac2.jpg", "imge3.jpg", "bac2.jpg");
    if (!loadHeartTexture()) {
        printf("Failed to load heart texture!\n");
        return;
    }
    if (!loadAudio()) {
        printf("Failed to load audio files!\n");
        return;
    }

    if (!loadMinimapTexture()) {
        printf("Failed to load minimap texture!\n");
        return;
    }

    initPlayers(); 
    perso player1;
    perso player2;
    initPerso(&player1);
    initPerso(&player2);
    
    if (!loadPlayerTexture()) {
        printf("Failed to load player texture!\n");
        return;
    }

    //enemyInit();
    initEnemies(); 


    int playerHealth[2] = {5, 5};
    bool running = true;
    SDL_Event event;
    bool movingRight = true;
    int starSpeed = 40;

    Uint32 lastTime = SDL_GetTicks();
    Uint32 currentTime;
    Uint32 deltaTime = 0;
    Uint32 updateInterval = 1000;

    

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                handleMouseClick(event);
            }
        }
	if (!paused) {
        	if (movingRight) {
            		starRect[0].x += starSpeed;
            		starRect[1].x += starSpeed;
            		if( (starRect[0].x >= 200) &&(starRect[1].x >= 800)) {
                		movingRight = false;
            		}
        	} else {
            		starRect[0].x -= starSpeed;
            		starRect[1].x -= starSpeed;
            		if ((starRect[0].x <=0) && (starRect[1].x <= 200)) {
                		movingRight = true;
            		}
        	}
	
        	printf("Score: %d\n", getScore());

        	const Uint8 *keyState = SDL_GetKeyboardState(NULL);
	
       
		deplacer_Perso(&player1, keyState, &player1, &player2);
        	saut(&player1, keyState, &player1, &player2);
        	animer_Perso(&player1); 

       
        	deplacer_Perso(&player2, keyState, &player1, &player2);
        	saut(&player2, keyState, &player1, &player2);
        	animer_Perso(&player2); 

       
        	scrolling(0, keyState);
        
      
        	scrolling(1, keyState);


        	playBackgroundMusic();
        	AfficherMap(playerHealth, keyState);

  
        	SDL_RenderSetViewport(renderer, &((SDL_Rect){0, 0, WINDOW_WIDTH / 2, WINDOW_HEIGHT}));
        	enemyMove(&player1, &player2);	
		enemyRender();
        	afficherPerso(renderer, player1);
        	renderHearts(playerHealth);

   
        	SDL_RenderSetViewport(renderer, &((SDL_Rect){WINDOW_WIDTH / 2, 0, WINDOW_WIDTH / 2, WINDOW_HEIGHT}));
        	enemyMove(&player1, &player2);
		enemyRender();
        	afficherPerso(renderer, player2);
        	renderHearts(playerHealth);

        	SDL_RenderPresent(renderer);

        	currentTime = SDL_GetTicks();
        	deltaTime += currentTime - lastTime;
        	lastTime = currentTime;

        	if (deltaTime >= updateInterval) {
            		updateScore();
            		deltaTime -= updateInterval;
        	}
	}
        SDL_Delay(50);
    }
    finalizeScore();

    closeSDL();
}




