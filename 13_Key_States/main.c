#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include "./LTexture.h"

#define SCREEN_WIDTH  640
#define SCREEN_HEIGHT 480

SDL_Window *gWindow = NULL;
SDL_Renderer *gRenderer = NULL;

LTexture *gPressTexture = NULL;
LTexture *gUpTexture = NULL;
LTexture *gDownTexture = NULL;
LTexture *gLeftTexture = NULL;
LTexture *gRightTexture = NULL;

int init();
int loadMedia();
void finish();

int main(int argc, char *argv[]) {
	if (!init())
		puts("Failed to initialize!\n");
	else {
		gPressTexture = malloc(sizeof(LTexture));
		initTexture(gPressTexture);

		gUpTexture = malloc(sizeof(LTexture));
		initTexture(gUpTexture);

		gDownTexture = malloc(sizeof(LTexture));
		initTexture(gDownTexture);

		gLeftTexture = malloc(sizeof(LTexture));
		initTexture(gLeftTexture);

		gRightTexture = malloc(sizeof(LTexture));
		initTexture(gRightTexture);

		if (!loadMedia())
			puts("Failed to load media!\n");
		else {
			int quit = 0;

			SDL_Event e;

			LTexture *currentTexture = NULL;

			while (!quit) {
				while (SDL_PollEvent(&e) != 0)
					if (e.type == SDL_QUIT)
						quit = 1;

				const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);

				if (currentKeyStates[SDL_SCANCODE_UP])
					currentTexture = gUpTexture;
				else if (currentKeyStates[SDL_SCANCODE_DOWN])
					currentTexture = gDownTexture;
				else if (currentKeyStates[SDL_SCANCODE_LEFT])
					currentTexture = gLeftTexture;
				else if (currentKeyStates[SDL_SCANCODE_RIGHT])
					currentTexture = gRightTexture;
				else
					currentTexture = gPressTexture;

				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( gRenderer );


				renderTexture(currentTexture, gRenderer, 0, 0, NULL, 0,
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

	if (!loadFromFile(gPressTexture, gRenderer, "./press.png")) {
		printf("Failed to load  press texture!\n");
		success = 0;
	}

	if (!loadFromFile(gUpTexture, gRenderer, "./up.png")) {
		printf("Failed to load up texture!\n");
		success = 0;
	}

	if (!loadFromFile(gDownTexture, gRenderer, "./down.png")) {
		printf("Failed to load down texture!\n");
		success = 0;
	}

	if (!loadFromFile(gLeftTexture, gRenderer, "./left.png")) {
		printf("Failed to load left texture!\n");
		success = 0;
	}

	if (!loadFromFile(gRightTexture, gRenderer, "./right.png")) {
		printf("Failed to load right texture!\n");
		success = 0;
	}

	return success;
}

void finish() {
	freeTexture(gPressTexture);
	gPressTexture = NULL;
	freeTexture(gUpTexture);
	gUpTexture = NULL;
	freeTexture(gDownTexture);
	gDownTexture = NULL;
	freeTexture(gLeftTexture);
	gLeftTexture = NULL;
	freeTexture(gRightTexture);
	gRightTexture = NULL;

	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gRenderer = NULL;
	gWindow = NULL;

	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}
