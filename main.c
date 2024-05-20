#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <stdbool.h>
//#include "run2.h"
#include "run.h"

#define SCREEN_WIDTH 1680
#define SCREEN_HEIGHT 1050
#define MAX_VOLUME_LEVEL 5

SDL_Window *window;
SDL_Renderer *renderer;
SDL_Texture *image1_texture;
SDL_Texture *image2_texture;
SDL_Texture *image_texture;
SDL_Texture *startButton_texture;
SDL_Texture *optionButton_texture;
SDL_Texture *player1button_texture;
SDL_Texture *player2button_texture;
SDL_Texture *exitButton_texture;
SDL_Texture *backButton_texture;
SDL_Texture *text_texture;
SDL_Texture *yesButton_texture;
SDL_Texture *noButton_texture;
SDL_Texture *inmuteButton_texture;
SDL_Texture *muteButton_texture;
SDL_Texture *settingsBackground_texture;
SDL_Texture *volume1_texture;
SDL_Texture *volume2_texture;
SDL_Texture *volume3_texture;
SDL_Texture *volume4_texture;
SDL_Texture *volumeMax_texture;

SDL_Rect positionImage;
SDL_Rect positionImageButton;
SDL_Rect positionStartButton;
SDL_Rect positionOptionButton;
SDL_Rect positionPlayer1Button;
SDL_Rect positionPlayer2Button;
SDL_Rect positionExitButton;
SDL_Rect positionBackButton;
SDL_Rect positionTexteButton;
SDL_Rect positionYesButton;
SDL_Rect positionNoButton;
SDL_Rect positionInMuteButton;
SDL_Rect positionMuteButton;
SDL_Rect positionVolume1Button;
SDL_Rect positionVolume2Button;
SDL_Rect positionVolume3Button;
SDL_Rect positionVolume4Button;
SDL_Rect positionVolumeMaxButton;

SDL_Event event;

Mix_Music *musique;
Mix_Chunk *clickSound;

bool isMuted = false;
bool showInmuteButton = true;
bool showmuteButton = false;
bool showvolume1 = true;
bool showvolume2 = false;
bool showvolume3 = false;
bool showvolume4 = false;
bool showvolumeMax = false;

Mix_Chunk *loadSound(const char *click) {
    Mix_Chunk *sound = Mix_LoadWAV(click);
    if (!sound) {
        fprintf(stderr, "Failed to load sound %s: %s\n", click, Mix_GetError());
        exit(1);
    }
    return sound;
}

SDL_Texture *loadTexture(const char *filename) {
    SDL_Surface *surface = IMG_Load(filename);
    if (!surface) {
        fprintf(stderr, "Failed to load image %s: %s\n", filename, IMG_GetError());
        exit(1);
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture) {
        fprintf(stderr, "Failed to create texture from %s: %s\n", filename, SDL_GetError());
        exit(1);
    }

    SDL_FreeSurface(surface);

    return texture;
}

int main(int argc, char **argv) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        printf("Failed to initialize SDL: %s\n", SDL_GetError());
        return 1;
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096) == -1) {
        printf("Failed to initialize audio: %s\n", Mix_GetError());
        return 1;
    }

    window = SDL_CreateWindow("Game Menu", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        printf("Failed to create window: %s\n", SDL_GetError());
        return 1;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer) {
        printf("Failed to create renderer: %s\n", SDL_GetError());
        return 1;
    }

    image1_texture = loadTexture("img1.png");
    image2_texture = loadTexture("img5.png");
    image_texture = loadTexture("img9.jpg");
    text_texture = loadTexture("img10.jpg");
    startButton_texture = loadTexture("img2.jpg");
    optionButton_texture = loadTexture("img3.jpg");
    exitButton_texture = loadTexture("img4.jpg");
    backButton_texture = loadTexture("img6.jpg");
    yesButton_texture = loadTexture("img7.jpg");
    noButton_texture = loadTexture("img8.jpg");
    inmuteButton_texture = loadTexture("img11.jpg");
    muteButton_texture = loadTexture("img12.jpg");
    player1button_texture = loadTexture("img13.jpg");
    player2button_texture = loadTexture("img14.jpg");
    settingsBackground_texture = loadTexture("settings.jpg");
    volume1_texture = loadTexture("volume1.png");
    volume2_texture = loadTexture("volume2.png");
    volume3_texture = loadTexture("volume3.png");
    volume4_texture = loadTexture("volume4.png");
    volumeMax_texture = loadTexture("volumemax.png");


    positionImage.x = 0;
    positionImage.y = 0;

    SDL_QueryTexture(image_texture, NULL, NULL, &positionImage.w, &positionImage.h);

    SDL_QueryTexture(image_texture, NULL, NULL, &positionImageButton.w, &positionImageButton.h);
    positionImageButton.x = (SCREEN_WIDTH - positionImageButton.w) / 2;
    positionImageButton.y = 230;

    SDL_QueryTexture(startButton_texture, NULL, NULL, &positionStartButton.w, &positionStartButton.h);
    positionStartButton.x = (SCREEN_WIDTH - positionStartButton.w) / 2;
    positionStartButton.y = 475;

    SDL_QueryTexture(optionButton_texture, NULL, NULL, &positionOptionButton.w, &positionOptionButton.h);
    positionOptionButton.x = (SCREEN_WIDTH - positionOptionButton.w) / 2;
    positionOptionButton.y = positionStartButton.y + positionStartButton.h + 20;

    SDL_QueryTexture(player1button_texture, NULL, NULL, &positionPlayer1Button.w, &positionPlayer1Button.h);
    positionPlayer1Button.x = (SCREEN_WIDTH - positionPlayer1Button.w) / 2;
    positionPlayer1Button.y = 475;
  
    SDL_QueryTexture(player2button_texture, NULL, NULL, &positionPlayer2Button.w, &positionPlayer2Button.h);
    positionPlayer2Button.x = (SCREEN_WIDTH - positionPlayer2Button.w) / 2;
    positionPlayer2Button.y = positionPlayer1Button.y + positionPlayer1Button.h + 20;

    SDL_QueryTexture(exitButton_texture, NULL, NULL, &positionExitButton.w, &positionExitButton.h);
    positionExitButton.x = (SCREEN_WIDTH - positionExitButton.w) / 2;
    positionExitButton.y = positionOptionButton.y + positionOptionButton.h + 40;

    SDL_QueryTexture(backButton_texture, NULL, NULL, &positionBackButton.w, &positionBackButton.h);
    positionBackButton.x = 30;
    positionBackButton.y = positionExitButton.y + 20;

    SDL_QueryTexture(text_texture, NULL, NULL, &positionTexteButton.w, &positionTexteButton.h);
    positionTexteButton.x = 465;
    positionTexteButton.y = 350;

    SDL_QueryTexture(yesButton_texture, NULL, NULL, &positionYesButton.w, &positionYesButton.h);
    positionYesButton.x = (SCREEN_WIDTH - positionYesButton.w) / 2 + 100;
    positionYesButton.y = 550;

    SDL_QueryTexture(noButton_texture, NULL, NULL, &positionNoButton.w, &positionNoButton.h);
    positionNoButton.x = (SCREEN_WIDTH - positionNoButton.w) / 2 - 100;
    positionNoButton.y = 550;

    SDL_QueryTexture(inmuteButton_texture, NULL, NULL, &positionInMuteButton.w, &positionInMuteButton.h);
    positionInMuteButton.x = SCREEN_WIDTH - positionInMuteButton.w - 30;
    positionInMuteButton.y = 0;

    SDL_QueryTexture(muteButton_texture, NULL, NULL, &positionMuteButton.w, &positionMuteButton.h);
    positionMuteButton.x = SCREEN_WIDTH - positionMuteButton.w - 30;
    positionMuteButton.y = 0;

    SDL_QueryTexture(volume1_texture, NULL, NULL, &positionVolume1Button.w, &positionVolume1Button.h);
    positionVolume1Button.x = 840;
    positionVolume1Button.y = 525;

    SDL_QueryTexture(volume2_texture, NULL, NULL, &positionVolume2Button.w, &positionVolume2Button.h);
    positionVolume2Button.x = 840;
    positionVolume2Button.y = 525;

    SDL_QueryTexture(volume3_texture, NULL, NULL, &positionVolume3Button.w, &positionVolume3Button.h);
    positionVolume3Button.x = 840;
    positionVolume3Button.y = 525;

    SDL_QueryTexture(volume4_texture, NULL, NULL, &positionVolume4Button.w, &positionVolume4Button.h);
    positionVolume4Button.x = 840;
    positionVolume4Button.y = 525;

    SDL_QueryTexture(volumeMax_texture, NULL, NULL, &positionVolumeMaxButton.w, &positionVolumeMaxButton.h);
    positionVolumeMaxButton.x = 840;
    positionVolumeMaxButton.y = 525;

    musique = Mix_LoadMUS("ost.mp3");
    if (!musique) {
        fprintf(stderr, "Failed to load music: %s\n", Mix_GetError());
        return 1;
    }

    clickSound = loadSound("click.wav");
    if (!clickSound) {
        return 1;
    }

    Mix_PlayMusic(musique, -1);

    int quit = 0;
    int secondBackgroundShown = 0;
    int settingsBackgroundShown = 0;
    int thirdBackgroundShown = 0;
    int confirmationMode = 0;
    
    while (!quit) {
        SDL_RenderClear(renderer);

        if (!secondBackgroundShown && !thirdBackgroundShown && !settingsBackgroundShown) {
            SDL_RenderCopy(renderer, image1_texture, NULL, NULL);
            if (showInmuteButton) {
                SDL_RenderCopy(renderer, inmuteButton_texture, NULL, &positionInMuteButton);
            }
            if (showmuteButton) {
                SDL_RenderCopy(renderer, muteButton_texture, NULL, &positionMuteButton);
            }
            SDL_RenderCopy(renderer, startButton_texture, NULL, &positionStartButton);
            SDL_RenderCopy(renderer, optionButton_texture, NULL, &positionOptionButton);
            SDL_RenderCopy(renderer, exitButton_texture, NULL, &positionExitButton);
        } else {
            if (secondBackgroundShown == 1) {
                SDL_RenderCopy(renderer, image2_texture, NULL, NULL);
                SDL_RenderCopy(renderer,  player1button_texture, NULL, &positionPlayer1Button);
		SDL_RenderCopy(renderer,  player2button_texture, NULL, &positionPlayer2Button);
                SDL_RenderCopy(renderer, backButton_texture, NULL, &positionBackButton);
                if (showInmuteButton) {
                    SDL_RenderCopy(renderer, inmuteButton_texture, NULL, &positionInMuteButton);
                }
                if (showmuteButton) {
                    SDL_RenderCopy(renderer, muteButton_texture, NULL, &positionMuteButton);
                }
            } else if (thirdBackgroundShown == 1) {
                SDL_RenderCopy(renderer, image_texture, NULL, &positionImageButton);
                SDL_RenderCopy(renderer, text_texture, NULL, &positionTexteButton);
                SDL_RenderCopy(renderer, yesButton_texture, NULL, &positionYesButton);
                SDL_RenderCopy(renderer, noButton_texture, NULL, &positionNoButton);
            } else if (settingsBackgroundShown == 1) {
                SDL_RenderCopy(renderer, settingsBackground_texture, NULL, NULL);
                if (showvolume1) {
                    SDL_RenderCopy(renderer, volume1_texture, NULL, &positionVolume1Button);
                }
                if (showvolume2) {
                    SDL_RenderCopy(renderer, volume2_texture, NULL, &positionVolume2Button);
                }
                if (showvolume3) {
                    SDL_RenderCopy(renderer, volume3_texture, NULL, &positionVolume3Button);
                }
                if (showvolume4) {
                    SDL_RenderCopy(renderer, volume4_texture, NULL, &positionVolume4Button);
                }
                if (showvolumeMax) {
                    SDL_RenderCopy(renderer, volumeMax_texture, NULL, &positionVolumeMaxButton);
                }
                SDL_RenderCopy(renderer, backButton_texture, NULL, &positionBackButton);
                if (showInmuteButton) {
                    SDL_RenderCopy(renderer, inmuteButton_texture, NULL, &positionInMuteButton);
                }
                if (showmuteButton) {
                    SDL_RenderCopy(renderer, muteButton_texture, NULL, &positionMuteButton);
                }
            }
        }

        if (confirmationMode) {
            SDL_RenderCopy(renderer, yesButton_texture, NULL, &positionYesButton);
            SDL_RenderCopy(renderer, noButton_texture, NULL, &positionNoButton);
        }

        SDL_RenderPresent(renderer);

        SDL_WaitEvent(&event);
        switch (event.type) {
            case SDL_QUIT:
                if (!confirmationMode) {
                    quit = 1;
                } else {
                    confirmationMode = 0;
                }
                break;
            case SDL_MOUSEBUTTONDOWN:
                if (!confirmationMode && event.button.button == SDL_BUTTON_LEFT) {
                    int x = event.button.x;
                    int y = event.button.y;
                    if (x >= positionInMuteButton.x && x <= positionInMuteButton.x + positionInMuteButton.w &&
                        y >= positionInMuteButton.y && y <= positionInMuteButton.y + positionInMuteButton.h) {
                        if (isMuted) {
                            Mix_VolumeMusic(MIX_MAX_VOLUME);
                            isMuted = false;
                            showmuteButton = false;
                            showInmuteButton = true;
                        } else {
                            Mix_VolumeMusic(0);
                            isMuted = true;
                            showInmuteButton = false;
                            showmuteButton = true;
                        }
                    } else if (settingsBackgroundShown) {
                        if (x >= positionVolume1Button.x && x <= positionVolume1Button.x + positionVolume1Button.w &&
                            y >= positionVolume1Button.y && y <= positionVolume1Button.y + positionVolume1Button.h) {
                            Mix_VolumeMusic(MIX_MAX_VOLUME / (MAX_VOLUME_LEVEL + 1));
                            showvolume1 = false;
                            showvolume2 = true;
                            showvolume3 = false;
                            showvolume4 = false;
                            showvolumeMax = false;
                        } else if (x >= positionVolume2Button.x && x <= positionVolume2Button.x + positionVolume2Button.w &&
                                   y >= positionVolume2Button.y && y <= positionVolume2Button.y + positionVolume2Button.h) {
                            Mix_VolumeMusic((MIX_MAX_VOLUME / (MAX_VOLUME_LEVEL + 1)) * 2);
                            showvolume1 = false;
                            showvolume2 = false;
                            showvolume3 = true;
                            showvolume4 = false;
                            showvolumeMax = false;
                        } else if (x >= positionVolume3Button.x && x <= positionVolume3Button.x + positionVolume3Button.w &&
                                   y >= positionVolume3Button.y && y <= positionVolume3Button.y + positionVolume3Button.h) {
                            Mix_VolumeMusic((MIX_MAX_VOLUME / (MAX_VOLUME_LEVEL + 1)) * 3);
                            showvolume1 = false;
                            showvolume2 = false;
                            showvolume3 = false;
                            showvolume4 = true;
                            showvolumeMax = false;
                        } else if (x >= positionVolume4Button.x && x <= positionVolume4Button.x + positionVolume4Button.w &&
                                   y >= positionVolume4Button.y && y <= positionVolume4Button.y + positionVolume4Button.h) {
                            Mix_VolumeMusic((MIX_MAX_VOLUME / (MAX_VOLUME_LEVEL + 1)) * 4);
                            showvolume1 = false;
                            showvolume2 = false;
                            showvolume3 = false;
                            showvolume4 = false;
                            showvolumeMax = true;
                        }
                    } else {
                        if (x >= positionStartButton.x && x <= positionStartButton.x + positionStartButton.w &&
                            y >= positionStartButton.y && y <= positionStartButton.y + positionStartButton.h) {
			    Mix_PlayChannel(-1, clickSound, 0);
			    secondBackgroundShown = 1;
                            break;
                        } else if (x >= positionOptionButton.x && x <= positionOptionButton.x + positionOptionButton.w &&
                                   y >= positionOptionButton.y && y <= positionOptionButton.y + positionOptionButton.h) {
                            Mix_PlayChannel(-1, clickSound, 0);
                            settingsBackgroundShown = 1;
                            break;
                        } else if (x >= positionExitButton.x && x <= positionExitButton.x + positionExitButton.w &&
                                   y >= positionExitButton.y && y <= positionExitButton.y + positionExitButton.h) {
                            Mix_PlayChannel(-1, clickSound, 0);
                            thirdBackgroundShown = 1;
                            confirmationMode = 1;
                            break;
                        }
                    }

                    if (secondBackgroundShown && event.button.button == SDL_BUTTON_LEFT) {
                        int x = event.button.x;
                        int y = event.button.y;
                        if (x >= positionBackButton.x && x <= positionBackButton.x + positionBackButton.w &&
                            y >= positionBackButton.y && y <= positionBackButton.y + positionBackButton.h) {
                            Mix_PlayChannel(-1, clickSound, 0);
                            secondBackgroundShown = 0;
                        }
			else if(x >= positionPlayer1Button.x && x <= positionPlayer1Button.x + positionPlayer1Button.w &&
                            y >= positionPlayer1Button.y && y <= positionPlayer1Button.y + positionPlayer1Button.h){
			    Mix_PlayChannel(-1, clickSound, 0);
			    //rune();
                            //return 2;
			}
			else if(x >= positionPlayer2Button.x && x <= positionPlayer2Button.x + positionPlayer2Button.w &&
                            y >= positionPlayer2Button.y && y <= positionPlayer2Button.y + positionPlayer2Button.h){
			    Mix_PlayChannel(-1, clickSound, 0);
			    run();
                            return 2;
			}
                    } else if (confirmationMode && thirdBackgroundShown && event.button.button == SDL_BUTTON_LEFT) {
                        int x = event.button.x;
                        int y = event.button.y;
                        if (x >= positionYesButton.x && x <= positionYesButton.x + positionYesButton.w &&
                            y >= positionYesButton.y && y <= positionYesButton.y + positionYesButton.h) {
                            Mix_PlayChannel(-1, clickSound, 0);
			    confirmationMode = 1;
                            quit =1;
                        } else if (x >= positionNoButton.x && x <= positionNoButton.x + positionNoButton.w &&
                                   y >= positionNoButton.y && y <= positionNoButton.y + positionNoButton.h) {
                            Mix_PlayChannel(-1, clickSound, 0);
                            confirmationMode = 0;
                            thirdBackgroundShown = 0;
                        }
                    } else if (settingsBackgroundShown && event.button.button == SDL_BUTTON_LEFT) {
                        int x = event.button.x;
                        int y = event.button.y;
                        if (x >= positionBackButton.x && x <= positionBackButton.x + positionBackButton.w &&
                            y >= positionBackButton.y && y <= positionBackButton.y + positionBackButton.h) {
                            Mix_PlayChannel(-1, clickSound, 0);
                            settingsBackgroundShown = 0;
                        }
                    }
                }
                break;
            default:
                break;
        }
    }

    SDL_DestroyTexture(image1_texture);
    SDL_DestroyTexture(image2_texture);
    SDL_DestroyTexture(image_texture);
    SDL_DestroyTexture(startButton_texture);
    SDL_DestroyTexture(optionButton_texture);
    SDL_DestroyTexture(exitButton_texture);
    SDL_DestroyTexture(player1button_texture);
    SDL_DestroyTexture(player2button_texture);
    SDL_DestroyTexture(backButton_texture);
    SDL_DestroyTexture(text_texture);
    SDL_DestroyTexture(yesButton_texture);
    SDL_DestroyTexture(noButton_texture);
    SDL_DestroyTexture(inmuteButton_texture);
    SDL_DestroyTexture(muteButton_texture);
    SDL_DestroyTexture(settingsBackground_texture);
    SDL_DestroyTexture(volume1_texture);
    SDL_DestroyTexture(volume2_texture);
    SDL_DestroyTexture(volume3_texture);
    SDL_DestroyTexture(volume4_texture);
    SDL_DestroyTexture(volumeMax_texture);

    Mix_FreeMusic(musique);
    Mix_FreeChunk(clickSound);

    Mix_CloseAudio();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

