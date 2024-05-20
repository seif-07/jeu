#ifndef PERSO_H
#define PERSO_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>


#define JUMP_SPEED 20 
#define MAX_JUMP_HEIGHT 100 


typedef struct {
    SDL_Surface *img;
    SDL_Rect pos[8][8];
} image_perso;


typedef struct {
    image_perso etat_droite;
    image_perso etat_gauche;
    int dir; 
    int etat;
    int mouv;
    SDL_Rect pos; 
    int saut; 
    int nbr; 
    int jumpSpeed; 
} perso;


extern SDL_Texture* playerTexture;

void updateMiniPlayerPosition(perso *realPlayer, SDL_Rect *miniPlayerRect);

void initPerso(perso *p);
void afficherPerso(SDL_Renderer *renderer, perso p);
void animer_Perso(perso *p);
void updateMinimapPosition(perso *p, perso *player1, perso *player2);
void deplacer_Perso(perso *p, const Uint8 *keyState, perso *player1, perso *player2);
void saut(perso *p, const Uint8 *keyState, perso *player1, perso *player2);

#endif

