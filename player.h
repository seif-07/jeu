#ifndef PLAYER_H
#define PLAYER_H

#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>


#define FONT_PATH "arial.ttf"
#define FONT_SIZE 24

#define WINDOW_WIDTH 1600
#define WINDOW_HEIGHT 1000
#define BACKGROUND_WIDTH 2048
#define BACKGROUND_HEIGHT 1382
#define SCROLL_SPEED 5
#define HEART_WIDTH 30
#define HEART_HEIGHT 30
#define HEART_SPACING 10
#define NUM_HEARTS 5
#define BUTTON_WIDTH 200
#define BUTTON_HEIGHT 100

#include <SDL2/SDL.h>
#include "perso.h"

extern SDL_Renderer* renderer;
extern SDL_Texture* backgroundTexture;
extern SDL_Texture* buttonTexture;
extern SDL_Texture* buttonTexture3;
extern SDL_Texture* buttonTexture4;
extern SDL_Texture* starTexture;
extern SDL_Texture* heartTexture;

enum BackgroundState {
    MAIN_BACKGROUND,
    SECONDARY_BACKGROUND
};

extern SDL_Rect starRect;

SDL_Texture* loadTexture(const char* filename); 


bool initSDL();
void chargerMap(char* background,char* secondaryBackground, char* button, char* button3, char* button4);
bool loadHeartTexture();
bool loadAudio();
bool loadPlayerTexture();

void afficherPerso(SDL_Renderer *renderer, perso p);

void initPlayer();
void closeSDL();
void scrolling(int player, const Uint8 *keyState); 
void AfficherMap(int playerHealth, const Uint8 *keyState, perso player); 
void playBackgroundMusic();
void handleMouseClick(SDL_Event event);


#endif

