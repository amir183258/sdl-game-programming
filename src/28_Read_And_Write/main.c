#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "./LTexture.h"

#define SCREEN_WIDTH  640
#define SCREEN_HEIGHT 480

#define TOTAL_DATA 10

SDL_Window *gWindow = NULL;
SDL_Renderer *gRenderer = NULL;

TTF_Font *gFont = NULL;

LTexture *gPromptTextTexture;
LTexture *gDataTextures[TOTAL_DATA];

Sint32 gData[TOTAL_DATA];

int init();
int loadMedia();
void finish();

int main(int argc, char *argv[]) {
	if (!init())
		puts("Failed to initialize!\n");
	else {
		gPromptTextTexture = malloc(sizeof(LTexture));
		initTexture(gPromptTextTexture);

		for (int i = 0; i < TOTAL_DATA; ++i) {
			gDataTextures[i] = malloc(sizeof(LTexture));

			initTexture(gDataTextures[i]);
		}

		if (!loadMedia())
			puts("Failed to load media!\n");
		else {
			int quit = 0;

			SDL_Event e;

			SDL_Color textColor = {0, 0, 0, 0xFF};
			SDL_Color highlightColor = {0xFF, 0, 0, 0xFF};

			int currentData = 0;
			char buffer[20];

			while (!quit) {
				int renderText = 0;

				while (SDL_PollEvent(&e) != 0) {
					if (e.type == SDL_QUIT)
						quit = 1;
					else if (e.type == SDL_KEYDOWN) {
						switch (e.key.keysym.sym) {
							case SDLK_UP:
								sprintf(buffer, "%d", gData[currentData]);
								loadFromRenderedText(gDataTextures[currentData], gRenderer, gFont, buffer, textColor);
								--currentData;
								if (currentData < 0)
									currentData = TOTAL_DATA - 1;
								
								sprintf(buffer, "%d", gData[currentData]);
								loadFromRenderedText(gDataTextures[currentData], gRenderer, gFont, buffer, highlightColor);
								break;
							case SDLK_DOWN:
								sprintf(buffer, "%d", gData[currentData]);
								loadFromRenderedText(gDataTextures[currentData], gRenderer, gFont, buffer, textColor);

								++currentData;
								if (currentData == TOTAL_DATA)
									currentData = 0;
								sprintf(buffer, "%d", gData[currentData]);
								loadFromRenderedText(gDataTextures[currentData], gRenderer, gFont, buffer, highlightColor);
								break;
							case SDLK_LEFT:
								--gData[currentData];
								sprintf(buffer, "%d", gData[currentData]);
								loadFromRenderedText(gDataTextures[currentData], gRenderer, gFont, buffer, highlightColor);
								break;
							case SDLK_RIGHT:
								++gData[currentData];
								sprintf(buffer, "%d", gData[currentData]);
								loadFromRenderedText(gDataTextures[currentData], gRenderer, gFont, buffer, highlightColor);
								break;
						}
					}
				}

				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderClear(gRenderer);

				renderTexture(gPromptTextTexture, gRenderer,
						(SCREEN_WIDTH -
						 getTextureWidth(gPromptTextTexture)) / 2, 0,
						NULL, 0, NULL, SDL_FLIP_NONE);

				for (int i = 0; i < TOTAL_DATA; ++i)
					renderTexture(gDataTextures[i], gRenderer,
						(SCREEN_WIDTH -
						getTextureWidth(gDataTextures[i])) / 2,
						getTextureHeight(gPromptTextTexture) +
						getTextureHeight(gDataTextures[i]) * i,
						NULL, 0, NULL, SDL_FLIP_NONE);

				SDL_RenderPresent(gRenderer);
			}
		}
	}

	finish();

	return 0;
}

int init() {
	int success = 1;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
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
			gRenderer = SDL_CreateRenderer(gWindow, -1,
					SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
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

				if (TTF_Init() == -1) {
					printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
					success = 0;
				}
			}
		}
	}
	return success;
}

int loadMedia() {
	SDL_Color textColor = {0, 0, 0, 0xFF};
	SDL_Color highlightColor = {0xFF, 0, 0, 0xFF};

	int success = 1;

	gFont = TTF_OpenFont("./lazy.ttf", 28);
	if (gFont == NULL) {
		printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
		success = 0;
	}
	else {
		if (!loadFromRenderedText(gPromptTextTexture,
					gRenderer, gFont, "Enter Text:", textColor)) {
			printf("Failed to render prompt text!\n");
			success = 0;
		}
	}

	SDL_RWops *file = SDL_RWFromFile("./nums.bin", "r+b");
	if (file == NULL) {
		printf("Warning: Unable to open file! SDL Error: %s\n", SDL_GetError());

		file = SDL_RWFromFile("./nums.bin", "w+b");
		if (file != NULL) {
			printf("New file created!\n");
			for (int i = 0; i < TOTAL_DATA; ++i) {
				gData[i] = 0;
				SDL_RWwrite(file, &gData[i], sizeof(Sint32), 1);
			}
			SDL_RWclose(file);
		}
		else {
			printf("Error: Unable to create file! SDL Error: %s\n", SDL_GetError());
			success = 0;
		}
	}
	else {
		printf("Reading file...!\n");
		for (int i = 0; i < TOTAL_DATA; ++i)
			SDL_RWread(file, &gData[i], sizeof(Sint32), 1);
		SDL_RWclose(file);
	}

	char temp[20];
	sprintf(temp, "%d", gData[0]);
	loadFromRenderedText(gDataTextures[0], gRenderer, gFont, temp, highlightColor);

	for (int i = 1; i < TOTAL_DATA; ++i) {
		sprintf(temp, "%d", gData[i]);
		loadFromRenderedText(gDataTextures[i], gRenderer, gFont, temp, textColor);
	}

	return success;
}

void finish() {
	SDL_RWops *file = SDL_RWFromFile("./nums.bin", "w+b");
	if (file != NULL) {
		for (int i = 0; i < TOTAL_DATA; ++i)
			SDL_RWwrite(file, &gData[i], sizeof(Sint32), 1);
		SDL_RWclose(file);
	}
	else
		printf("Error: Unable to save file! %s\n", SDL_GetError());


	freeTexture(gPromptTextTexture);
	gPromptTextTexture = NULL;

	for (int i = 0; i < TOTAL_DATA; ++i) {
		freeTexture(gDataTextures[i]);
		gDataTextures[i] = NULL;
	}

	TTF_CloseFont(gFont);
	gFont = NULL;

	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gRenderer = NULL;
	gWindow = NULL;

	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}
