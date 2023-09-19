#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <math.h>
#include "./LTexture.h"

#define SCREEN_WIDTH  640
#define SCREEN_HEIGHT 480

#define JOYSTICK_DEAD_ZONE 8000

SDL_Window *gWindow = NULL;
SDL_Renderer *gRenderer = NULL;

LTexture *gArrowTexture;

SDL_GameController *gGameController = NULL;

int init();
int loadMedia();
void finish();

int main(int argc, char *argv[]) {
	if (!init())
		puts("Failed to initialize!\n");
	else {
		gArrowTexture = malloc(sizeof(LTexture));
		initTexture(gArrowTexture);

		if (!loadMedia())
			puts("Failed to load media!\n");
		else {
			int quit = 0;

			SDL_Event e;

			int xDir = 0;
			int yDir = 0;

			while (!quit) {
				while (SDL_PollEvent(&e) != 0) {
					if (e.type == SDL_QUIT)
						quit = 1;
					else if (e.type == SDL_CONTROLLERAXISMOTION) {
						if ( e.caxis.which == 0) {
							if (e.caxis.axis == 1) {
								if (e.caxis.value < -JOYSTICK_DEAD_ZONE)
									xDir = -1;

								if (e.caxis.value > JOYSTICK_DEAD_ZONE)
									xDir = 1;
								else
									xDir = 0;
							}
							else if (e.caxis.axis == 0){
								if (e.caxis.value < -JOYSTICK_DEAD_ZONE)
									yDir = -1;

								if (e.caxis.value > JOYSTICK_DEAD_ZONE)
									yDir = 1;
								else
									yDir = 0;
							}
						}
					}
				}

				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( gRenderer );

				printf("x: %d\n", xDir);
				printf("y: %d\n", yDir);

				double joystickAngle = atan2((double) yDir, (double) xDir) * (180.0 / M_PI);
				if (xDir == 0 && yDir == 0)
					joystickAngle = 0;

				renderTexture(gArrowTexture, gRenderer, 
					(SCREEN_WIDTH - getTextureWidth(gArrowTexture)) / 2, 
					(SCREEN_HEIGHT - getTextureHeight(gArrowTexture)) /2,
					NULL, joystickAngle,
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

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER) < 0) {
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = 0;
	}
	else {
		if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
			puts("Warning: Linear texture filtering not enabled!");

		if (SDL_NumJoysticks() < 1)
			printf("Warning: No joysticks connected!\n");
		else {
			gGameController = SDL_GameControllerOpen(0);
			if (gGameController == NULL)
				printf("Warning: Unable to open game "
						"controller! SDL Error: %s\n", SDL_GetError());
		}

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

	if (!loadFromFile(gArrowTexture, gRenderer, "./arrow.png")) {
		printf("Failed to load  arrow texture!\n");
		success = 0;
	}

	return success;
}

void finish() {
	freeTexture(gArrowTexture);
	gArrowTexture = NULL;

	SDL_GameControllerClose(gGameController);
	gGameController = NULL;

	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gRenderer = NULL;
	gWindow = NULL;

	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}
