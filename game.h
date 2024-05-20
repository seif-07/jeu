#ifndef GAME_H
#define GAME_H
#include <stdbool.h> 
#include <SDL2/SDL.h>

#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL.h>
#include "perso.h"

#define FONT_PATH "arial.ttf"
#define FONT_SIZE 24



#define WINDOW_WIDTH 1600
#define WINDOW_HEIGHT 1000
#define BACKGROUND_WIDTH 1500
#define BACKGROUND_HEIGHT 1000
#define SCROLL_SPEED 5
#define HEART_WIDTH 30
#define HEART_HEIGHT 30
#define HEART_SPACING 10
#define NUM_HEARTS 5
#define BUTTON_WIDTH 200
#define BUTTON_HEIGHT 100

#define MINIMAP_WIDTH 200
#define MINIMAP_HEIGHT 150

#define MINI_PLAYER_WIDTH 20
#define MINI_PLAYER_HEIGHT 20





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

extern SDL_Rect starRect[2];
extern SDL_Rect minimapRect[2];
extern bool paused;


bool initSDL();
bool loadMinimapTexture();
void chargerMap(char* background, char* secondaryBackground, char* button, char* button3, char* button4);
bool loadHeartTexture();
bool loadAudio();
bool loadMedia(char* background, char* button, char* button3, char* button4);


bool loadPlayerTexture();
void initPlayers();

void closeSDL();
void scrolling(int player, const Uint8 *keyState);
void AfficherMap(int playerHealth[2], const Uint8 *keyState);
void playBackgroundMusic();
void handleMouseClick(SDL_Event event);
void renderHearts(int playerHealth[2]); 


#endif 

