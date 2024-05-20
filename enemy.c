#include <stdbool.h>
#include <SDL2/SDL.h>
#include <stdlib.h> 
#include "run.h"
#include "game.h"
#include "enemy.h"
#include "perso.h"
#include <time.h> 
#include "utils.h"

SDL_Rect cameraRect[2];

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Texture *enemyTexture;
int enemyPosX = 0;
int enemySpeed = 3;
int enemyFrame = 0;
int enemyDirection = 1; 

Enemy enemies[NUM_ENEMIES];

bool loadEnemyTexture(const char *filePath) {
    SDL_Surface* surface = IMG_Load(filePath);
    if (surface == NULL) {
        printf("Failed to load enemy texture! SDL_Error: %s\n", SDL_GetError());
        return false;
    }
    enemyTexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (enemyTexture == NULL) {
        printf("Failed to create enemy texture! SDL_Error: %s\n", SDL_GetError());
        return false;
    }
    return true;
}


void enemyInit() {
    initEnemies();
}

void enemyMove(perso *player1, perso *player2) {
    for (int i = 0; i < NUM_ENEMIES; i++) {
        if (enemies[i].alive) {
            if (enemyDirection) {
                // Move enemy to the right
                enemies[i].x += ENEMY_SPEED;
                if (enemies[i].x >= BACKGROUND_WIDTH - ENEMY_WIDTH) {
                    // If enemy reaches right end of background, change direction to left
                    enemyDirection = false;
                }
            } else {
                // Move enemy to the left
                enemies[i].x -= ENEMY_SPEED;
                if (enemies[i].x <= 0) {
                    // If enemy reaches left end of background, change direction to right
                    enemyDirection = true;
                }
            }

            // Check for collision with player1
            if (checkCollision(enemies[i], *player1)) {
                printf("Enemy collided with player1!\n");
                // Display dead image and exit game
                displayDeadImageAndExit();
            }

            // Check for collision with player2
            if (checkCollision(enemies[i], *player2)) {
                printf("Enemy collided with player2!\n");
                // Display dead image and exit game
                displayDeadImageAndExit();
            }
        }
    }
}


// Function to check collision between enemy and player
bool checkCollision(Enemy enemy, perso player) {
    if (enemy.x + ENEMY_WIDTH >= player.pos.x &&
        enemy.x <= player.pos.x + player.pos.w &&
        enemy.y + ENEMY_HEIGHT >= player.pos.y &&
        enemy.y <= player.pos.y + player.pos.h) {
        return true;
    }
    return false;
}

void initEnemies() {
    //srand(time(NULL)); 

    for (int i = 0; i < NUM_ENEMIES; i++) {
        enemies[i].alive = true;

        // Position the first enemy on the left side of the background
        /*if (i == 0) {
            enemies[i].x = rand() % (BACKGROUND_WIDTH / 2 - ENEMY_WIDTH); 
        } 
        // Position the second enemy on the right side of the background
        else if (i == 1) {
            enemies[i].x = rand() % (BACKGROUND_WIDTH / 2 - ENEMY_WIDTH) + BACKGROUND_WIDTH / 2;
        }*/
	enemies[i].x = WINDOW_WIDTH - ENEMY_WIDTH;
        enemies[i].y = WINDOW_HEIGHT - ENEMY_HEIGHT;
    }
}



void enemyRender() {
    for (int i = 0; i < NUM_ENEMIES; i++) {
        if (enemies[i].alive) {
            SDL_Rect renderRect;
            if (enemies[i].x >= cameraRect[0].x && enemies[i].x < cameraRect[0].x + WINDOW_WIDTH / 2) {
                // Render enemy on the first player's screen
                renderRect.x = enemies[i].x - cameraRect[0].x;
                renderRect.y = enemies[i].y;
                renderRect.w = ENEMY_WIDTH;
                renderRect.h = ENEMY_HEIGHT;
            } else if (enemies[i].x >= cameraRect[1].x && enemies[i].x < cameraRect[1].x + WINDOW_WIDTH / 2) {
                // Render enemy on the second player's screen
                renderRect.x = enemies[i].x - cameraRect[1].x;
                renderRect.y = enemies[i].y;
                renderRect.w = ENEMY_WIDTH;
                renderRect.h = ENEMY_HEIGHT;
            } else {
                // Enemy is not within either player's screen, skip rendering
                continue;
            }
            SDL_RenderCopy(renderer, enemyTexture, NULL, &renderRect);
        }
    }
}


void enemyCleanup() {
    SDL_DestroyTexture(enemyTexture);
}


