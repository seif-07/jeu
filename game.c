#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include "game.h"
#include "audio.h"
#include "graphics.h"
#include "score.h"
#include "perso.h"
#include "enemy.h"


SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Texture* backgroundTexture = NULL;
SDL_Texture* secondaryBackgroundTexture = NULL;
SDL_Rect backgroundRect = {0, 0, BACKGROUND_WIDTH, BACKGROUND_HEIGHT};
SDL_Rect cameraRect[2] = {
    {0, BACKGROUND_HEIGHT - WINDOW_HEIGHT, WINDOW_WIDTH / 2, WINDOW_HEIGHT},
    {BACKGROUND_WIDTH / 2 - WINDOW_WIDTH / 4, BACKGROUND_HEIGHT - WINDOW_HEIGHT, WINDOW_WIDTH / 2, WINDOW_HEIGHT}
};
SDL_Texture* heartTexture = NULL;
SDL_Rect heartRects[NUM_HEARTS];
SDL_Texture* buttonTexture = NULL;
SDL_Rect buttonRect[2] = {
    {10, 10, BUTTON_WIDTH, BUTTON_HEIGHT},
    {WINDOW_WIDTH / 2 + 10, 10, BUTTON_WIDTH, BUTTON_HEIGHT}
};
SDL_Texture* buttonTexture3 = NULL;
SDL_Rect buttonRect3 = {50, 100, BUTTON_WIDTH, BUTTON_HEIGHT};
SDL_Texture* buttonTexture4 = NULL;
SDL_Rect buttonRect4 = {50, 225, BUTTON_WIDTH, BUTTON_HEIGHT};
SDL_Texture* starTexture = NULL;

SDL_Rect starRect[2] = {
    {0, 0, 70, 70},
    {0, 0, 70, 70}
};

SDL_Texture* minimapTexture = NULL;
SDL_Rect minimapRect[2] = {
    {WINDOW_WIDTH / 4 - MINIMAP_WIDTH / 2, 0, MINIMAP_WIDTH, MINIMAP_HEIGHT}, 
    {WINDOW_WIDTH / 4 - MINIMAP_WIDTH / 2, 0, MINIMAP_WIDTH, MINIMAP_HEIGHT},  
};
SDL_Rect miniPlayerRect1 = {0, 0, 5, 5}; 
SDL_Rect miniPlayerRect2 = {0, 0, 5, 5}; 


Mix_Music* backgroundMusic = NULL;
Mix_Chunk* clickSound = NULL;

perso player1;
perso player2;

enum BackgroundState currentBackground[2] = {MAIN_BACKGROUND, MAIN_BACKGROUND};

SDL_Texture* playerTexture = NULL;

bool paused = false;


bool loadMinimapTexture() {
    minimapTexture = IMG_LoadTexture(renderer, "mini2.png");
    if (minimapTexture == NULL) {
        printf("Failed to load minimap texture! SDL_Error: %s\n", SDL_GetError());
        return false;
    }
    return true;
}

bool loadPlayerTexture() {
    playerTexture = IMG_LoadTexture(renderer, "walking right 1.png");
    if (playerTexture == NULL) {
        printf("Failed to load player texture! SDL_Error: %s\n", SDL_GetError());
        return false;
    }
    return true;
}

bool initSDL() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return false;
    }
    window = SDL_CreateWindow("Background Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (renderer == NULL) {
        printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
        return false;
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
        return false;
    }
    if (TTF_Init() == -1) {
    	printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
    	return false;
    }
    if (!loadPlayerTexture()) {  
        return false;
    }
    if (!loadEnemyTexture("right/background0.png")) { 
        return false;
    }

    return true;
}

bool loadHeartTexture() {
    SDL_Surface* heartSurface = SDL_LoadBMP("heart.bmp");
    if (heartSurface == NULL) {
        printf("Unable to load heart image! SDL_Error: %s\n", SDL_GetError());
        return false;
    }
    heartTexture = SDL_CreateTextureFromSurface(renderer, heartSurface);
    SDL_FreeSurface(heartSurface);
    if (heartTexture == NULL) {
        printf("Unable to create heart texture! SDL_Error: %s\n", SDL_GetError());
        return false;
    }
    return true;
}

bool loadAudio() {
    backgroundMusic = Mix_LoadMUS("ost.mp3");
    if (backgroundMusic == NULL) {
        printf("Failed to load background music! SDL_mixer Error: %s\n", Mix_GetError());
        return false;
    }

    clickSound = Mix_LoadWAV("click.wav");
    if (clickSound == NULL) {
        printf("Failed to load click sound effect! SDL_mixer Error: %s\n", Mix_GetError());
        return false;
    }

    return true;
}

void chargerMap(char* background, char* secondaryBackground, char* button, char* button3, char* button4) {
    if (!loadMedia(background, button, button3, button4)) {
        printf("Failed to load media!\n");
        exit(1);
    }

    SDL_Surface* secondaryBackgroundSurface = IMG_Load(secondaryBackground);
    if (secondaryBackgroundSurface == NULL) {
        printf("Unable to load secondary background image! SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }
    secondaryBackgroundTexture = SDL_CreateTextureFromSurface(renderer, secondaryBackgroundSurface);
    SDL_FreeSurface(secondaryBackgroundSurface);
    if (secondaryBackgroundTexture == NULL) {
        printf("Unable to create secondary background texture! SDL_Error: %s\n", SDL_GetError());
        exit(1);
    }
}

void initPlayers() {
    
    initPerso(&player1);
    player1.pos.x = 0; 
    player1.pos.y = WINDOW_HEIGHT - player1.pos.h; 

    initPerso(&player2);
    player2.pos.x = WINDOW_WIDTH - player2.pos.w; 
    player2.pos.y = WINDOW_HEIGHT - player2.pos.h; 
}


void playBackgroundMusic() {
    if (Mix_PlayingMusic() == 0) {
        Mix_PlayMusic(backgroundMusic, -1);
    }
}

void playClickSound() {
    Mix_PlayChannel(-1, clickSound, 0);
}



void scrolling(int player, const Uint8 *keyState) {
    if (player == 0) {
        if (keyState[SDL_SCANCODE_A] && cameraRect[player].x > 0)
            cameraRect[player].x -= SCROLL_SPEED;
        if (keyState[SDL_SCANCODE_D] && cameraRect[player].x < (BACKGROUND_WIDTH  - WINDOW_WIDTH / 2))
            cameraRect[player].x += SCROLL_SPEED;
    } else if (player == 1) {
        if (keyState[SDL_SCANCODE_LEFT] && cameraRect[player].x > (BACKGROUND_WIDTH / 2 - WINDOW_WIDTH / 2))
            cameraRect[player].x -= SCROLL_SPEED;
        if (keyState[SDL_SCANCODE_RIGHT] && cameraRect[player].x < BACKGROUND_WIDTH - WINDOW_WIDTH / 2)
            cameraRect[player].x += SCROLL_SPEED;
    }
}

void AfficherMap(int playerHealth[2], const Uint8 *keyState) {

    SDL_RenderClear(renderer);

    SDL_RenderSetViewport(renderer, &((SDL_Rect){0, 0, WINDOW_WIDTH / 2, WINDOW_HEIGHT}));
    SDL_RenderCopy(renderer, backgroundTexture, &cameraRect[0], NULL);
    SDL_RenderCopy(renderer, buttonTexture, NULL, &buttonRect[0]);
    SDL_RenderCopy(renderer, starTexture, NULL, &((SDL_Rect){starRect[0].x, starRect[0].y, starRect[0].w, starRect[0].h}));
    SDL_RenderCopy(renderer, minimapTexture, NULL, &minimapRect[0]);


 
    updateMiniPlayerPosition(&player1, &miniPlayerRect1);
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); 
    SDL_RenderFillRect(renderer, &miniPlayerRect1);

    
    renderHearts(playerHealth);

    SDL_RenderSetViewport(renderer, &((SDL_Rect){WINDOW_WIDTH / 2, 0, WINDOW_WIDTH / 2, WINDOW_HEIGHT}));
    SDL_RenderCopy(renderer, backgroundTexture, &cameraRect[1], NULL);
    SDL_RenderCopy(renderer, buttonTexture, NULL, &buttonRect[1]);
    SDL_RenderCopy(renderer, starTexture, NULL, &((SDL_Rect){starRect[1].x, starRect[1].y, starRect[1].w, starRect[1].h}));
    SDL_RenderCopy(renderer, minimapTexture, NULL, &minimapRect[1]);


  
    updateMiniPlayerPosition(&player2, &miniPlayerRect2);
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); 
    SDL_RenderFillRect(renderer, &miniPlayerRect2);

    renderHearts(playerHealth);

    if (currentBackground[0] == SECONDARY_BACKGROUND) {
        SDL_RenderCopy(renderer, secondaryBackgroundTexture, NULL, NULL);
        SDL_RenderCopy(renderer, buttonTexture3, NULL, &buttonRect3);
        SDL_RenderCopy(renderer, buttonTexture4, NULL, &buttonRect4);
    }
    if (currentBackground[1] == SECONDARY_BACKGROUND) {
        SDL_RenderCopy(renderer, secondaryBackgroundTexture, NULL, &cameraRect[1]);
    }


    
    if (TTF_Init() == -1) {
        printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
        return;
    }
    if (paused) {
    TTF_Font* font = TTF_OpenFont("pepsi.ttf", FONT_SIZE);
    if (!font) {
    	printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
    	return;
    }



    SDL_Color textColor = {0, 0, 0}; 
    char scoreText[20]; 

   
    snprintf(scoreText, sizeof(scoreText), "Score: %d", getScore());

    SDL_Surface* textSurface = TTF_RenderText_Solid(font, scoreText, textColor);
    if (!textSurface) {
        printf("Failed to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
        return;
    }

    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (!textTexture) {
        printf("Failed to create text texture! SDL Error: %s\n", SDL_GetError());
        return;
    }

    int textWidth, textHeight;
    SDL_QueryTexture(textTexture, NULL, NULL, &textWidth, &textHeight);

    int textX = 0;
    int textY = 20; 
   
    printf("Text position: x=%d, y=%d\n", textX, textY);

    SDL_Rect textRect = {textX, textY, textWidth, textHeight};

    SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

    SDL_FreeSurface(textSurface);
    SDL_DestroyTexture(textTexture);
    TTF_CloseFont(font);
    }
    SDL_RenderPresent(renderer);
}

void handleMouseClick(SDL_Event event) {
    if (event.button.x >= buttonRect[0].x && event.button.x <= buttonRect[0].x + buttonRect[0].w &&
        event.button.y >= buttonRect[0].y && event.button.y <= buttonRect[0].y + buttonRect[0].h) {
        currentBackground[0] = (currentBackground[0] == MAIN_BACKGROUND) ? SECONDARY_BACKGROUND : MAIN_BACKGROUND;
        playClickSound();
    } else if (currentBackground[0] == SECONDARY_BACKGROUND &&
               event.button.x >= cameraRect[0].x + 850 && event.button.x <= cameraRect[0].x + 850 + BUTTON_WIDTH &&
               event.button.y >= cameraRect[0].y + 225 && event.button.y <= cameraRect[0].y + 225 + BUTTON_HEIGHT) {
	paused = !paused;
        playClickSound();
    } else if (currentBackground[0] == SECONDARY_BACKGROUND &&
               event.button.x >= cameraRect[0].x + 850 && event.button.x <= cameraRect[0].x + 850 + BUTTON_WIDTH &&
               event.button.y >= cameraRect[0].y + 100 && event.button.y <= cameraRect[0].y + 100 + BUTTON_HEIGHT) {
        	playClickSound();
		exit(0); 
    }

}


void closeSDL() {
    SDL_DestroyTexture(minimapTexture);
    SDL_DestroyTexture(playerTexture);
    SDL_DestroyTexture(backgroundTexture);
    SDL_DestroyTexture(secondaryBackgroundTexture);
    SDL_DestroyTexture(buttonTexture);
    SDL_DestroyTexture(buttonTexture3);
    SDL_DestroyTexture(buttonTexture4);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    Mix_FreeMusic(backgroundMusic);
    Mix_FreeChunk(clickSound);
    Mix_Quit();
    SDL_Quit();
}

