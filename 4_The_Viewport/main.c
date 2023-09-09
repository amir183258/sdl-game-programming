#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>

/* screen dimension constants */
#define SCREEN_WIDTH	640
#define SCREEN_HEIGHT	480

/* global variables */
SDL_Window *gWindow = NULL;
SDL_Renderer *gRenderer = NULL;
SDL_Texture *gTexture = NULL;

/* prototypes */
int init(void);
int loadMedia(void);
void finish();
SDL_Texture *loadTexture(char *path);

int main(int argc, char *argv[]) {
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

				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderClear(gRenderer);

				// Top left corner viewport
				SDL_Rect topLeftViewport;
				topLeftViewport.x = 0;
				topLeftViewport.y = 0;
				topLeftViewport.w = SCREEN_WIDTH / 2;
				topLeftViewport.h = SCREEN_HEIGHT / 2;
				SDL_RenderSetViewport(gRenderer, &topLeftViewport);

				SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);

				// Top right viewport
				SDL_Rect topRightViewport;
				topRightViewport.x = SCREEN_WIDTH / 2;
				topRightViewport.y = 0;
				topRightViewport.w = SCREEN_WIDTH / 2;
				topRightViewport.h = SCREEN_HEIGHT / 2;
				SDL_RenderSetViewport( gRenderer, &topRightViewport );
				
				SDL_RenderCopy( gRenderer, gTexture, NULL, NULL );

				//Bottom viewport
				SDL_Rect bottomViewport;
				bottomViewport.x = 0;
				bottomViewport.y = SCREEN_HEIGHT / 2;
				bottomViewport.w = SCREEN_WIDTH;
				bottomViewport.h = SCREEN_HEIGHT / 2;
				SDL_RenderSetViewport( gRenderer, &bottomViewport );

				
				//Render texture to screen
				SDL_RenderCopy( gRenderer, gTexture, NULL, NULL );


				//Update screen
				SDL_RenderPresent( gRenderer );
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

	return success;
}

int loadMedia(void) {
	int success = 1;

	gTexture = loadTexture("./viewport.png");
	if (gTexture == NULL) {
		puts("Failed to load texture image!\n");

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
