#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <stdio.h>
#include <math.h>
#include "./LTexture.h"

#define SCREEN_WIDTH  640
#define SCREEN_HEIGHT 480

#define JOYSTICK_DEAD_ZONE 8000

SDL_Window *gWindow = NULL;
SDL_Renderer *gRenderer = NULL;

LTexture *gPromptTexture = NULL;

Mix_Music *gMusic = NULL;

Mix_Chunk *gScratch = NULL;
Mix_Chunk *gHigh = NULL;
Mix_Chunk *gMedium = NULL;
Mix_Chunk *gLow = NULL;

int init();
int loadMedia();
void finish();

int main(int argc, char *argv[]) {
	if (!init())
		puts("Failed to initialize!\n");
	else {
		gPromptTexture = malloc(sizeof(LTexture));
		initTexture(gPromptTexture);

		if (!loadMedia())
			puts("Failed to load media!\n");
		else {
			int quit = 0;

			SDL_Event e;

			while (!quit) {
				while (SDL_PollEvent(&e) != 0) {
					if (e.type == SDL_QUIT)
						quit = 1;
					else if (e.type == SDL_KEYDOWN) {
						switch (e.key.keysym.sym) {
							case SDLK_1:
								Mix_PlayChannel(-1, gHigh, 0);
								break;

							case SDLK_2:
								Mix_PlayChannel(-1, gMedium, 0);
								break;
							case SDLK_3:
								Mix_PlayChannel(-1, gLow, 0);
								break;
							case SDLK_4:
								Mix_PlayChannel(-1, gScratch, 0);
								break;
							case SDLK_9:
								if (Mix_PlayingMusic() == 0)
									Mix_PlayMusic(gMusic, -1);
								else {
									if (Mix_PausedMusic() == 1)
										Mix_ResumeMusic();
									else
										Mix_PauseMusic();
								} break;
							case SDLK_0:
								Mix_HaltMusic();
								break;
						}
					}
				}
					

				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( gRenderer );

				renderTexture(gPromptTexture, gRenderer, 
					0, 
					0,
					NULL, 0,
					NULL, SDL_FLIP_NONE);

				SDL_RenderPresent(gRenderer);
				
			}
		}
	}

	finish();

	return 0;
}

int init() {
	int success = 1;

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = 0;
	}
	else {
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
			puts("Warning: Linear texture filtering not enabled!");

		gWindow = SDL_CreateWindow("SDL Tutorial",
				           SDL_WINDOWPOS_UNDEFINED,
				           SDL_WINDOWPOS_UNDEFINED,
					   SCREEN_WIDTH,
					   SCREEN_HEIGHT,
					   SDL_WINDOW_SHOWN);
		if (gWindow == NULL) {
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = 0;
		}
		else {
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED |
					SDL_RENDERER_PRESENTVSYNC);
			if (gRenderer == NULL) {
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = 0;
			}
			else {
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

				int imgFlags = IMG_INIT_PNG;
				if (!(IMG_Init(imgFlags) & imgFlags)) {
					printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
					success = 0;
				}

				if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048 / 2) < 0) {
					printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
					success = 0;
				}

			}
		}
	}
	return success;
}

int loadMedia() {
	int success = 1;

	if (!loadFromFile(gPromptTexture, gRenderer, "./prompt.png")) {
		printf("Failed to load  arrow texture!\n");
		success = 0;
	}

	gMusic = Mix_LoadMUS("./beat.wav");
	if (gMusic == NULL) {
		printf("Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError());
		success = 0;
	}

	gScratch = Mix_LoadWAV("./scratch.wav");
	if (gScratch == NULL) {
		printf("Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError());
		success = 0;

	}

	gHigh = Mix_LoadWAV("./high.wav");
	if (gHigh == NULL) {
		printf("Failed to load high sound effect! SDL_mixer Error: %s\n", Mix_GetError());
		success = 0;

	}

	gMedium = Mix_LoadWAV("./medium.wav");
	if (gMedium == NULL) {
		printf("Failed to load medium sound effect! SDL_mixer Error: %s\n", Mix_GetError());
		success = 0;
	}

	gLow= Mix_LoadWAV("./low.wav");
	if (gLow == NULL) {
		printf("Failed to load low sound effect! SDL_mixer Error: %s\n", Mix_GetError());
		success = 0;
	}

	return success;
}

void finish() {
	freeTexture(gPromptTexture);
	gPromptTexture= NULL;

	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gRenderer = NULL;
	gWindow = NULL;

	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}
