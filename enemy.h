#ifndef ENEMY_H
#define ENEMY_H


#define ENEMY_WIDTH 70
#define ENEMY_HEIGHT 70

#define ENEMY_SPEED 5

#define NUM_ENEMIES 1



typedef struct {
    int x, y;
    bool alive;
} Enemy;

extern Enemy enemies[NUM_ENEMIES];


void enemyInit();

bool loadEnemyTexture(const char *filePath);

void enemyMove(perso *player1, perso *player2);


bool checkCollision(Enemy enemy, perso player);

void gameOver();

void initEnemies();

void enemyRender();

void enemyCleanup();

#endif

