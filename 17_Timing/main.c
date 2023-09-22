#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <stdio.h>
#include <math.h>
#include "./LTexture.h"

#define SCREEN_WIDTH  640
#define SCREEN_HEIGHT 480

#define JOYSTICK_DEAD_ZONE 8000

SDL_Window *gWindow = NULL;
SDL_Renderer *gRenderer = NULL;

TTF_Font* gFont = NULL;

LTexture *gTimeTextTexture = NULL;
LTexture *gPromptTextTexture = NULL;

int init();
int loadMedia();
void finish();

int main(int argc, char *argv[]) {
	if (!init())
		puts("Failed to initialize!\n");
	else {
		gTimeTextTexture = malloc(sizeof(LTexture));
		gPromptTextTexture = malloc(sizeof(LTexture));
		initTexture(gTimeTextTexture);
		initTexture(gPromptTextTexture);	

		if (!loadMedia())
			puts("Failed to load media!\n");
		else {
			int quit = 0;

			SDL_Event e;

			SDL_Color textColor = {0, 0, 0, 255};

			Uint32 startTime = 0;

			char timeText[40];

			while (!quit) {
				while (SDL_PollEvent(&e) != 0) {
					if (e.type == SDL_QUIT)
						quit = 1;
					else if (e.type == SDL_KEYDOWN &&
							e.key.keysym.sym == SDLK_RETURN)
						startTime = SDL_GetTicks();
				}

				sprintf(timeText, "Milliseconds since start time %d",
						SDL_GetTicks() - startTime);

				if (!loadFromRenderedText(gTimeTextTexture, gRenderer, gFont,
							timeText, textColor))
					printf("Unable to render time texture!\n");

				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( gRenderer );

				renderTexture(gPromptTextTexture, gRenderer, 
					(SCREEN_WIDTH -
					 getTextureWidth(gPromptTextTexture)) / 2, 
					0,
					NULL, 0,
					NULL, SDL_FLIP_NONE);

				renderTexture(gTimeTextTexture, gRenderer, 
					(SCREEN_WIDTH -
					 getTextureWidth(gTimeTextTexture)) / 2, 
					(SCREEN_HEIGHT -
					 getTextureHeight(gTimeTextTexture)) / 2,
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

	gFont = TTF_OpenFont("./lazy.ttf", 28);
	if (gFont == NULL) {
		printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
		success = 0;
	}
	else {
		SDL_Color textColor = {0, 0, 0, 255};

		if (!loadFromRenderedText(gPromptTextTexture, gRenderer, gFont,
					"Press Enter to Reset Start Time.", textColor)) {
			printf("Unable to render prompt texture!\n");
			success = 0;
		}
	}

	return success;
}

void finish() {

	freeTexture(gTimeTextTexture);
	gTimeTextTexture = NULL;
	freeTexture(gPromptTextTexture);
	gPromptTextTexture= NULL;

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
