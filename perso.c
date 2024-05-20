#include <stdio.h>
#include <SDL2/SDL_image.h>
#include "perso.h"
#include "game.h" 




void initPerso(perso *p) {
    p->etat_droite.img = IMG_Load("walking right 1.png");
    p->etat_gauche.img = IMG_Load("walking left 1.png");

    if (p->etat_droite.img == NULL || p->etat_gauche.img == NULL) {
        printf("Error: Failed to load player images\n");
        return;
    }

    int h = p->etat_droite.img->h / 5;
    int w = p->etat_droite.img->w / 6; 

    p->dir = 0;         
    p->etat = 0;        
    p->mouv = 0;        
    p->pos.x = 0;      
    p->pos.y = 0;      
    p->pos.w = w;       
    p->pos.h = h;      
    p->saut = 0;        
    p->nbr = 0;         
    p->jumpSpeed = JUMP_SPEED;

  
    int x = 0, y = 0;
    for (int i = 0; i < 6; i++) {
        x = 0;
        for (int j = 0; j < 7; j++) {
            p->etat_droite.pos[i][j].x = x;
            p->etat_droite.pos[i][j].y = y;
            p->etat_droite.pos[i][j].h = h;
            p->etat_droite.pos[i][j].w = w;
            x += w; 
        }
        y += h; 
    }

    x = 0;
    y = 0;
    for (int i = 0; i < 7; i++) {
        x = 0;
        for (int j = 5; j >= 0; j--) {
            p->etat_gauche.pos[i][j].x = x;
            p->etat_gauche.pos[i][j].y = y;
            p->etat_gauche.pos[i][j].h = h;
            p->etat_gauche.pos[i][j].w = w;
            x += w; 
        }
        y += h; 
    }
    p->pos.x = 0; 
    p->pos.y = WINDOW_HEIGHT - p->pos.h;
}



void afficherPerso(SDL_Renderer *renderer, perso p) {
    SDL_Rect destRect = {p.pos.x, p.pos.y, p.pos.w, p.pos.h};
    SDL_Texture *texture = NULL;

    if (p.saut == 1) {
        if (p.dir == 0) {
            texture = SDL_CreateTextureFromSurface(renderer, p.etat_droite.img);
            SDL_RenderCopy(renderer, texture, &p.etat_droite.pos[4][p.mouv], &destRect);
        } else {
            texture = SDL_CreateTextureFromSurface(renderer, p.etat_gauche.img);
            SDL_RenderCopy(renderer, texture, &p.etat_gauche.pos[4][p.mouv], &destRect);
        }
    } else {
        if (p.dir == 0) {
            texture = SDL_CreateTextureFromSurface(renderer, p.etat_droite.img);
            SDL_RenderCopy(renderer, texture, &p.etat_droite.pos[p.etat][p.mouv], &destRect);
        } else {
            texture = SDL_CreateTextureFromSurface(renderer, p.etat_gauche.img);
            SDL_RenderCopy(renderer, texture, &p.etat_gauche.pos[p.etat][p.mouv], &destRect);
        }
    }

    SDL_DestroyTexture(texture);
}

void animer_Perso(perso *p) {

    if (p->saut == 1) {
        if (p->nbr == 0) {
            p->mouv = 0;
        } else if (p->nbr < JUMP_SPEED) {
            p->mouv = 1;
        } else if (p->nbr == JUMP_SPEED) {
            p->mouv = 2;
        } else if (p->nbr == JUMP_SPEED * 2 - 1 || p->nbr == JUMP_SPEED * 2 - 2) {
            p->mouv = 5;
        } else if (p->nbr > JUMP_SPEED) {
            p->mouv = 3;
        }
    }else if ((p->etat == 2 && p->mouv == 6) || (p->etat == 0 && p->mouv == 4)) {
        p->mouv = 0;
    } else if (p->mouv == 6) {
        p->mouv = 0;
    }
}

void updateMinimapPosition(perso *p, perso *player1, perso *player2) {
    if (p == (perso *)player1) {
        minimapRect[0].x = p->pos.x * MINIMAP_WIDTH / BACKGROUND_WIDTH + WINDOW_WIDTH / 4 - MINIMAP_WIDTH / 2;
        minimapRect[0].y = p->pos.y * MINIMAP_HEIGHT / BACKGROUND_HEIGHT;
    } else if (p == (perso *)player2) {
        minimapRect[1].x = p->pos.x * MINIMAP_WIDTH / BACKGROUND_WIDTH + WINDOW_WIDTH / 4 - MINIMAP_WIDTH / 2;
        minimapRect[1].y = p->pos.y * MINIMAP_HEIGHT / BACKGROUND_HEIGHT;
    }
}

void updateMiniPlayerPosition(perso *realPlayer, SDL_Rect *miniPlayerRect) {

    miniPlayerRect->x = realPlayer->pos.x * MINIMAP_WIDTH / BACKGROUND_WIDTH + WINDOW_WIDTH / 4 - MINI_PLAYER_WIDTH / 2;
    miniPlayerRect->y = realPlayer->pos.y * MINIMAP_HEIGHT / BACKGROUND_HEIGHT;
    miniPlayerRect->w = MINI_PLAYER_WIDTH;
    miniPlayerRect->h = MINI_PLAYER_HEIGHT;
}

void deplacer_Perso(perso *p, const Uint8 *keyState, perso *player1, perso *player2) {

    if (p == player1) {
        if (keyState[SDL_SCANCODE_A] && p->pos.x > 0) {
            p->pos.x -= 10; 
            p->dir = 1; 
            p->mouv = (p->mouv + 1) % 5 + 6;
            printf("Player1 position after moving left: x=%d, y=%d\n", p->pos.x, p->pos.y); 
        }
        if (keyState[SDL_SCANCODE_D] && p->pos.x + p->pos.w < WINDOW_WIDTH / 2) {
            p->pos.x += 10; 
            p->dir = 0; 
            p->mouv = (p->mouv + 1) % 5 + 6;
            printf("Player1 position after moving right: x=%d, y=%d\n", p->pos.x, p->pos.y); 
        }
    }

    if (p == player2) {
        if (keyState[SDL_SCANCODE_LEFT] && p->pos.x > 0) {
            p->pos.x -= 10; 
            p->dir = 1; 
            p->mouv = (p->mouv + 1) % 5 + 6; 
            printf("Player2 position after moving left: x=%d, y=%d\n", p->pos.x, p->pos.y); 
        }
        if (keyState[SDL_SCANCODE_RIGHT] && p->pos.x + p->pos.w < WINDOW_WIDTH / 2) {
            p->pos.x += 10; 
            p->dir = 0; 
            p->mouv = (p->mouv + 1) % 5 + 6; 
            printf("Player2 position after moving right: x=%d, y=%d\n", p->pos.x, p->pos.y);
        }
    }
  
    if (!(keyState[SDL_SCANCODE_A] || keyState[SDL_SCANCODE_D] || keyState[SDL_SCANCODE_LEFT] || keyState[SDL_SCANCODE_RIGHT])) {
        if (p == player1) {
            p->mouv = 6; 
        }
        if (p == player2) {
            p->mouv = 6; 
        }
    }

}



void saut(perso *p, const Uint8 *keyState, perso *player1, perso *player2) {
    const int GRAVITY = 1;
    if (p == player1) {
        if (keyState[SDL_SCANCODE_SPACE] && p->saut == 0) {
            p->saut = 1; 
            p->nbr = 1; 
            p->jumpSpeed = JUMP_SPEED; 
            printf("Player1 position before jumping: x=%d, y=%d\n", p->pos.x, p->pos.y);
        }
    }


    if (p == player2) {
        if (keyState[SDL_SCANCODE_UP] && p->saut == 0) {
            p->saut = 1; 
            p->nbr = 1; 
            p->jumpSpeed = JUMP_SPEED; 
            printf("Player2 position before jumping: x=%d, y=%d\n", p->pos.x, p->pos.y); 
        }
    }

   
    if (p->saut) {
        
        p->pos.y -= p->jumpSpeed; 
        p->jumpSpeed -= GRAVITY; 

       
        if (p->jumpSpeed <= 0 || p->pos.y >= WINDOW_HEIGHT - p->pos.h) {
            p->saut = 0; 
            p->pos.y = WINDOW_HEIGHT - p->pos.h; 
            p->jumpSpeed = JUMP_SPEED; 
        }
    }
}



