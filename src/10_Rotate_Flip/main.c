#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include "./LTexture.h"

#define SCREEN_WIDTH  640
#define SCREEN_HEIGHT 480

SDL_Window *gWindow = NULL;
SDL_Renderer *gRenderer = NULL;

LTexture *gArrowTexture;

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

			double degrees = 0;

			SDL_RendererFlip flipType = SDL_FLIP_NONE;

			while (!quit) {
				while (SDL_PollEvent(&e) != 0) {
					if (e.type == SDL_QUIT)
						quit = 1;
					else {
						switch (e.key.keysym.sym) {
							case SDLK_a:
								degrees -= 60;
								break;
							case SDLK_d:
								degrees += 60;
								break;
							case SDLK_q:
								flipType = SDL_FLIP_HORIZONTAL;
								break;
							case SDLK_w:
								flipType = SDL_FLIP_NONE;
								break;
							case SDLK_e:
								flipType = SDL_FLIP_VERTICAL;
								break;
						}


					}
				}

				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderClear(gRenderer);

				renderTexture(gArrowTexture, gRenderer,
						(SCREEN_WIDTH - getTextureWidth(gArrowTexture)) / 2,
						(SCREEN_HEIGHT - getTextureHeight(gArrowTexture)) / 2,
						NULL, degrees, NULL, flipType);

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
			}
		}
	}
}

int loadMedia() {
	int success = 1;

	if (!loadFromFile(gArrowTexture, gRenderer, "./arrow.png")) {
		printf("Failed to load walking animation texture!\n");
		success = 0;
	}

	return success;
}

void finish() {
	freeTexture(gArrowTexture);
	gArrowTexture = NULL;

	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gRenderer = NULL;
	gWindow = NULL;

	IMG_Quit();
	SDL_Quit();
}
