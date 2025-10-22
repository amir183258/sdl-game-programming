#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <stdio.h>
#include <math.h>
#include "./LTexture.h"
#include "./Dot.h"

#define SCREEN_WIDTH  640
#define SCREEN_HEIGHT 480

SDL_Window *gWindow = NULL;
SDL_Renderer *gRenderer = NULL;

LTexture *gDotTexture;
LTexture *gBGTexture;

int init();
int loadMedia();
void finish();

int main(int argc, char *argv[]) {
	if (!init())
		puts("Failed to initialize!\n");
	else {
		gDotTexture = malloc(sizeof(LTexture));
		initTexture(gDotTexture);

		gBGTexture = malloc(sizeof(LTexture));
		initTexture(gBGTexture);

		if (!loadMedia())
			puts("Failed to load media!\n");
		else {
			int quit = 0;

			SDL_Event e;

			Dot *dot = malloc(sizeof(Dot));
			initDot(dot);

			int scrollingOffset = 0;

			while (!quit) {
				while (SDL_PollEvent(&e) != 0) {
					if (e.type == SDL_QUIT)
						quit = 1;

					handleEventDot(dot, &e);
				}

				moveDot(dot);

				--scrollingOffset;
				if (scrollingOffset < -getTextureWidth(gBGTexture))
					scrollingOffset = 0;
				
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderClear(gRenderer);

				renderTexture(gBGTexture, gRenderer, scrollingOffset, 0,
						NULL, 0, NULL, SDL_FLIP_NONE);

				renderTexture(gBGTexture, gRenderer,
						scrollingOffset + getTextureWidth(gBGTexture), 0,
						NULL, 0, NULL, SDL_FLIP_NONE);

				renderDot(dot, gDotTexture, gRenderer);

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
	int success = 1;

	if (!loadFromFile(gDotTexture, gRenderer, "./dot.bmp")) {
		printf("Failed to load dot texture!\n");
		success = 0;
	}

	if (!loadFromFile(gBGTexture, gRenderer, "./bg.png")) {
		printf("Failed to load background texture!\n");
		success = 0;
	}

	return success;
}

void finish() {
	freeTexture(gDotTexture);
	gDotTexture = NULL;

	freeTexture(gBGTexture);
	gBGTexture = NULL;

	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gRenderer = NULL;
	gWindow = NULL;

	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}
