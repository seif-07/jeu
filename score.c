#include <stdio.h>
#include <stdlib.h>
#include "score.h"

#define MAX_SCORES 3
#define SCORE_FILE "bestscore.txt"

static int seconds = 0;

void initializeScore() {
    seconds = 0;
}

void updateScore() {
    seconds++;
}

int getScore() {
    return seconds;
}

void saveScoreToFile(int score) {
    int scores[MAX_SCORES];
    int i, j;

    // Initialize scores array to 0
    for (i = 0; i < MAX_SCORES; i++) {
        scores[i] = 0;
    }

    // Read existing scores from file
    FILE *file = fopen(SCORE_FILE, "r");
    if (file) {
        for (i = 0; i < MAX_SCORES; i++) {
            if (fscanf(file, "%d", &scores[i]) != 1) {
                break;
            }
        }
        fclose(file);
    }

    // Check if the current score should be included in the top scores
    for (i = 0; i < MAX_SCORES; i++) {
        if (score > scores[i]) {
            // Shift lower scores down
            for (j = MAX_SCORES - 1; j > i; j--) {
                scores[j] = scores[j - 1];
            }
            // Insert the current score
            scores[i] = score;
            break;
        }
    }

    // Write the updated scores back to the file
    file = fopen(SCORE_FILE, "w");
    if (file) {
        for (i = 0; i < MAX_SCORES; i++) {
            fprintf(file, "%d\n", scores[i]);
        }
        fclose(file);
    } else {
        printf("Failed to open score file for writing\n");
    }
}

void displayBestScores() {
    int scores[MAX_SCORES];
    int i;

    // Initialize scores array to 0
    for (i = 0; i < MAX_SCORES; i++) {
        scores[i] = 0;
    }

    // Read scores from file
    FILE *file = fopen(SCORE_FILE, "r");
    if (file) {
        for (i = 0; i < MAX_SCORES; i++) {
            if (fscanf(file, "%d", &scores[i]) != 1) {
                break;
            }
        }
        fclose(file);
    } else {
        printf("Failed to open score file for reading\n");
    }

    // Display the scores
    printf("Best Scores:\n");
    for (i = 0; i < MAX_SCORES; i++) {
        printf("%d. %d\n", i + 1, scores[i]);
    }
}

void finalizeScore() {
    saveScoreToFile(seconds);
    displayBestScores();
}

