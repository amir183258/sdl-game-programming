#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include "./LButton.h"
#ifndef test
#include "./LTexture.h"
#endif

#define SCREEN_WIDTH  640
#define SCREEN_HEIGHT 480

SDL_Window *gWindow = NULL;
SDL_Renderer *gRenderer = NULL;

SDL_Rect gSpriteClips[BUTTON_SPRITE_TOTAL];
LTexture *gButtonSpriteSheetTexture;

LButton *gButtons[TOTAL_BUTTONS];

int init();
int loadMedia();
void finish();

int main(int argc, char *argv[]) {
	if (!init())
		puts("Failed to initialize!\n");
	else {
		gButtonSpriteSheetTexture = malloc(sizeof(LTexture));
		initTexture(gButtonSpriteSheetTexture);

		for (int i = 0; i < TOTAL_BUTTONS; ++i) {
			gButtons[i] = malloc(sizeof(LButton));
			initButton(gButtons[i]);
		}

		if (!loadMedia())
			puts("Failed to load media!\n");
		else {
			int quit = 0;

			SDL_Event e;

			while (!quit) {
				while (SDL_PollEvent(&e) != 0) {
					if (e.type == SDL_QUIT)
						quit = 1;

					for (int i = 0; i < TOTAL_BUTTONS; ++i)
						handleEvent(gButtons[i], &e);
						
				}

				SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
				SDL_RenderClear( gRenderer );

				for (int i = 0; i < TOTAL_BUTTONS; ++i)
					renderButton(gButtons[i], gButtonSpriteSheetTexture, gRenderer, &gSpriteClips[(gButtons[i])->mCurrentSprite]);


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

	if (!loadFromFile(gButtonSpriteSheetTexture, gRenderer, "./button.png")) {
		printf("Failed to load button sprite texture!\n");
		success = 0;
	}
	else {
		for (int i = 0; i < BUTTON_SPRITE_TOTAL; ++i) {
			(gSpriteClips[i]).x = 0;
			gSpriteClips[i].y = i * 200;
			gSpriteClips[i].w = BUTTON_WIDTH;
			gSpriteClips[i].h = BUTTON_HEIGHT;
		}

		setPosition(gButtons[0], 0, 0);
		setPosition(gButtons[1], SCREEN_WIDTH - BUTTON_WIDTH, 0);
		setPosition(gButtons[2], 0, SCREEN_HEIGHT - BUTTON_HEIGHT);
		setPosition(gButtons[3], SCREEN_WIDTH - BUTTON_WIDTH, SCREEN_HEIGHT - BUTTON_HEIGHT);

	}


	return success;
}

void finish() {
	freeTexture(gButtonSpriteSheetTexture);
	gButtonSpriteSheetTexture= NULL;

	SDL_DestroyRenderer(gRenderer);
	SDL_DestroyWindow(gWindow);
	gRenderer = NULL;
	gWindow = NULL;

	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}
