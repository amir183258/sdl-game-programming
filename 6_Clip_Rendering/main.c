#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include "./LTexture.h"

#define SCREEN_WIDTH  640
#define SCREEN_HEIGHT 480

SDL_Window *gWindow = NULL;
SDL_Renderer *gRenderer = NULL;

SDL_Rect gSpriteClips[4];
LTexture *gSpriteSheetTexture;

int init();
int loadMedia();
void finish();

int main(int argc, char *argv[]) {
	if (!init())
		puts("Failed to initialize!\n");
	else {
		gSpriteSheetTexture = malloc(sizeof(LTexture));
		initTexture(gSpriteSheetTexture);

		if (!loadMedia())
			puts("Failed to load media!\n");
		else {
			int quit = 0;

			SDL_Event e;

			while (!quit) {
				while (SDL_PollEvent(&e) != 0)
					if (e.type == SDL_QUIT)
						quit = 1;
				
				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderClear(gRenderer);

				renderTexture(gSpriteSheetTexture, gRenderer, 
						0,
						0, &gSpriteClips[0]);

				renderTexture(gSpriteSheetTexture, gRenderer, 
						SCREEN_WIDTH - gSpriteClips[1].w,
						0, &gSpriteClips[1]);

				renderTexture(gSpriteSheetTexture, gRenderer, 
						0,
						SCREEN_HEIGHT - gSpriteClips[2].h,
						&gSpriteClips[2]);

				renderTexture(gSpriteSheetTexture, gRenderer, 
						SCREEN_WIDTH - gSpriteClips[3].w,
						SCREEN_HEIGHT - gSpriteClips[3].h,
						&gSpriteClips[3]);

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

	if (!loadFromFile(gSpriteSheetTexture, gRenderer, "./dots.png")) {
		printf("Failed to load 'dot' texture image!\n");
		success = 0;
	}
	else {
		gSpriteClips[0].x =   0;
		gSpriteClips[0].y =   0;
		gSpriteClips[0].w = 100;
		gSpriteClips[0].h = 100;

		gSpriteClips[1].x = 100;
		gSpriteClips[1].y =   0;
		gSpriteClips[1].w = 100;
		gSpriteClips[1].h = 100;

		gSpriteClips[2].x =   0;
		gSpriteClips[2].y = 100;
		gSpriteClips[2].w = 100;
		gSpriteClips[2].h = 100;

		gSpriteClips[3].x = 100;
		gSpriteClips[3].y = 100;
		gSpriteClips[3].w = 100;
		gSpriteClips[3].h = 100;
	}

	return success;
}

void finish() {
	freeTexture(gSpriteSheetTexture);
	gSpriteSheetTexture = NULL;

	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gRenderer = NULL;
	gWindow = NULL;

	IMG_Quit();
	SDL_Quit();
}
