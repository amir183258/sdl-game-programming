#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include "./LTexture.h"

#define SCREEN_WIDTH  640
#define SCREEN_HEIGHT 480

SDL_Window *gWindow = NULL;
SDL_Renderer *gRenderer = NULL;

LTexture *gModulatedTexture;

int init();
int loadMedia();
void finish();

int main(int argc, char *argv[]) {
	if (!init())
		puts("Failed to initialize!\n");
	else {
		gModulatedTexture = malloc(sizeof(LTexture));
		initTexture(gModulatedTexture);

		if (!loadMedia())
			puts("Failed to load media!\n");
		else {
			int quit = 0;

			SDL_Event e;

			Uint8 r = 255;
			Uint8 g = 255;
			Uint8 b = 255;

			while (!quit) {
				while (SDL_PollEvent(&e) != 0) {
					if (e.type == SDL_QUIT) {
						quit = 1;
					}
					else if (e.type == SDL_KEYDOWN) {
						switch (e.key.keysym.sym) {
							case SDLK_q:
								r += 32;
								break;
							case SDLK_w:
								g += 32;
								break;
							case SDLK_e:
								b += 32;
								break;
							case SDLK_a:
								r -= 32;
								break;
							case SDLK_s:
								g -= 32;
								break;
							case SDLK_d:
								b -= 32;
								break;
						}
					}
				}

				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderClear(gRenderer);

				setColor(gModulatedTexture, r, g, b);
				renderTexture(gModulatedTexture, gRenderer, 0, 0, NULL);

				SDL_RenderPresent(gRenderer);
			}
		}
	}

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
			gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
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

	if (!loadFromFile(gModulatedTexture, gRenderer, "./colors.png")) {
		printf("Failed to load 'dot' texture image!\n");
		success = 0;
	}

	return success;
}

void finish() {
	freeTexture(gModulatedTexture);
	gModulatedTexture = NULL;

	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gRenderer = NULL;
	gWindow = NULL;

	IMG_Quit();
	SDL_Quit();
}
