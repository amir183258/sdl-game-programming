#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>

/* screen dimension constants */
#define SCREEN_WIDTH	640
#define SCREEN_HEIGHT	480

/* global variables */
SDL_Window *gWindow = NULL;
SDL_Surface *gScreenSurface = NULL;
SDL_Surface *gPNGSurface = NULL;
SDL_Renderer *gRenderer = NULL;
SDL_Texture *gTexture = NULL;

/* prototypes */
int init(void);
int loadMedia(void);
void finish();
SDL_Surface *loadSurface(char *path);
SDL_Texture *loadTexture(char *path);

int main() {
	if (!init())
		puts("Failed to initialize!\n");
	else {
		if (!loadMedia())
			puts("Failed to load media!\n");
		else {
			int quit = 0;

			SDL_Event e;
			while (!quit) {
				while (SDL_PollEvent(&e) != 0)
					if (e.type == SDL_QUIT)
						quit = 1;
				
				SDL_RenderClear(gRenderer);

				SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);

				SDL_RenderPresent(gRenderer);
			}
		}
	}
	
	finish();

	return 0;
}

int init(void) {
	int success = 1;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = 0;
	}
	else {
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

	return success;
}

int loadMedia(void) {
	int success = 1;

	gTexture = loadTexture("./texture.png");
	if (gTexture == NULL) {
		printf("Failed to load texture image!\n");
		success = 0;
	}

	return success;
}

void finish(void) {
	SDL_DestroyTexture(gTexture);
	gTexture = NULL;

	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	gRenderer = NULL;

	IMG_Quit();
	SDL_Quit();
}


SDL_Surface* loadSurface(char* path) {
	SDL_Surface* optimizedSurface = NULL;

	SDL_Surface* loadedSurface = IMG_Load(path);
	if (loadedSurface == NULL)
		printf("Unable to load image %s! SDL_image Error: %s\n", path, IMG_GetError());
	else {
		optimizedSurface = SDL_ConvertSurface(loadedSurface, gScreenSurface->format, 0);

		if (optimizedSurface == NULL)
			printf("Unable to optimize image %s! SDL Error: %s\n", path, SDL_GetError());

		SDL_FreeSurface(loadedSurface);
	}

	return optimizedSurface;
}

SDL_Texture *loadTexture(char *path) {
	SDL_Texture *newTexture = NULL;

	SDL_Surface *loadedSurface = IMG_Load(path);
	if (loadedSurface == NULL)
		printf("Unable to load image %s! SDL_image Error: %s\n", path, IMG_GetError());
	else {
		newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
		if (newTexture == NULL)
			printf("Unable to crate texture from %s! SDL Error: %s\n", path, SDL_GetError());

		SDL_FreeSurface(loadedSurface);
	}

	return newTexture;
}
