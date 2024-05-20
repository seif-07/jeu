#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include "player.h"
#include "audio.h"
#include "graph.h"
#include "score.h"
#include "perso.h" 



SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Texture* backgroundTexture = NULL;
SDL_Rect backgroundRect = {0, 0, BACKGROUND_WIDTH, BACKGROUND_HEIGHT};
SDL_Texture* secondaryBackgroundTexture = NULL;
SDL_Rect cameraRect = {0, BACKGROUND_HEIGHT - WINDOW_HEIGHT, WINDOW_WIDTH, WINDOW_HEIGHT};
SDL_Texture* heartTexture = NULL;
SDL_Rect heartRects[NUM_HEARTS];
SDL_Texture* buttonTexture = NULL;
SDL_Rect buttonRect = {10, 10, BUTTON_WIDTH, BUTTON_HEIGHT};
SDL_Texture* buttonTexture3 = NULL;
SDL_Rect buttonRect3 = {700, 350, BUTTON_WIDTH, BUTTON_HEIGHT};
SDL_Texture* buttonTexture4 = NULL;
SDL_Rect buttonRect4 = {700, 500, BUTTON_WIDTH, BUTTON_HEIGHT};
SDL_Texture* starTexture = NULL;
SDL_Rect starRect = {0, 0, 70, 70};

Mix_Music* backgroundMusic = NULL;
Mix_Chunk* clickSound = NULL;

enum BackgroundState currentBackground = MAIN_BACKGROUND;

perso player;


SDL_Texture* playerTexture = NULL;
TTF_Font* font = NULL;

void afficherPerso(SDL_Renderer *renderer, perso p);

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

void chargerMap(char* background,char* secondaryBackground, char* button, char* button3, char* button4) {
	
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

void initPlayer() {
  
    initPerso(&player);
    
    player.pos.x = 0; 
    player.pos.y = WINDOW_HEIGHT - player.pos.h; 
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
        if (keyState[SDL_SCANCODE_LEFT] && cameraRect.x > 0)
            cameraRect.x -= SCROLL_SPEED;
        if (keyState[SDL_SCANCODE_RIGHT] && cameraRect.x < (BACKGROUND_WIDTH - WINDOW_WIDTH))
            cameraRect.x += SCROLL_SPEED;
    }
}

void AfficherMap(int playerHealth, const Uint8 *keyState, perso player) {
    
    SDL_RenderClear(renderer);

    SDL_RenderCopy(renderer, backgroundTexture, &cameraRect, NULL);
    SDL_RenderCopy(renderer, buttonTexture, NULL, &buttonRect);
    SDL_RenderCopy(renderer, starTexture, NULL, &starRect);

    renderHearts(playerHealth);
    afficherPerso(renderer, player);




    if (currentBackground == SECONDARY_BACKGROUND) {
        SDL_RenderCopy(renderer, secondaryBackgroundTexture, NULL, NULL);
        SDL_RenderCopy(renderer, buttonTexture3, NULL, &buttonRect3);
        SDL_RenderCopy(renderer, buttonTexture4, NULL, &buttonRect4);
    }

    if (TTF_Init() == -1) {
        printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
        return;
    }

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


    SDL_RenderPresent(renderer);
}

void handleMouseClick(SDL_Event event) {
    if (event.button.x >= buttonRect.x && event.button.x <= buttonRect.x + buttonRect.w &&
        event.button.y >= buttonRect.y && event.button.y <= buttonRect.y + buttonRect.h) {
        currentBackground = SECONDARY_BACKGROUND;
        playClickSound();
    } else if (currentBackground == SECONDARY_BACKGROUND &&
               event.button.x >= buttonRect3.x && event.button.x <= buttonRect3.x + buttonRect3.w &&
               event.button.y >= buttonRect3.y && event.button.y <= buttonRect3.y + buttonRect3.h) {
	playClickSound();
	exit(0);
    } else if (currentBackground == SECONDARY_BACKGROUND &&
               event.button.x >= buttonRect4.x && event.button.x <= buttonRect4.x + buttonRect4.w &&
               event.button.y >= buttonRect4.y && event.button.y <= buttonRect4.y + buttonRect4.h) {
        playClickSound();
        currentBackground = MAIN_BACKGROUND;
    }
}



void closeSDL() {
    SDL_DestroyTexture(playerTexture);
    SDL_DestroyTexture(backgroundTexture);
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

