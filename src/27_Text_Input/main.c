#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "./LTexture.h"

#define SCREEN_WIDTH  640
#define SCREEN_HEIGHT 480

SDL_Window *gWindow = NULL;
SDL_Renderer *gRenderer = NULL;

TTF_Font *gFont = NULL;

LTexture *gPromptTextTexture;
LTexture *gInputTextTexture;

int init();
int loadMedia();
void finish();

int main(int argc, char *argv[]) {
	if (!init())
		puts("Failed to initialize!\n");
	else {
		gPromptTextTexture = malloc(sizeof(LTexture));
		initTexture(gPromptTextTexture);

		gInputTextTexture = malloc(sizeof(LTexture));
		initTexture(gInputTextTexture);

		if (!loadMedia())
			puts("Failed to load media!\n");
		else {
			int quit = 0;

			SDL_Event e;

			SDL_Color textColor = {0, 0, 0, 0xFF};

			char inputText[20] = "Some Text";
			loadFromRenderedText(gInputTextTexture,
					gRenderer, gFont, inputText, textColor);

			SDL_StartTextInput();

			while (!quit) {
				int renderText = 0;

				while (SDL_PollEvent(&e) != 0) {
					if (e.type == SDL_QUIT)
						quit = 1;
					else if (e.type == SDL_KEYDOWN) {
						if (e.key.keysym.sym == SDLK_BACKSPACE &&
								strlen(inputText) > 0) {
							int i;
							for (i = 0; inputText[i] != '\0'; ++i)
								;
							if (i != 0)
								inputText[i - 1] = '\0';
								
							renderText = 1;
						}
						else if (e.key.keysym.sym == SDLK_c &&
								SDL_GetModState() & KMOD_CTRL)
							SDL_SetClipboardText(inputText);
						else if (e.key.keysym.sym == SDLK_v &&
								SDL_GetModState() & KMOD_CTRL) {
							char *temp = SDL_GetClipboardText();
							int i;
							for (i = 0; i < strlen(
								temp); ++i)
								inputText[i] = temp[i];
							inputText[i + 1] = '\0';

							renderText = 1;
						}
					}
					else if (e.type == SDL_TEXTINPUT) {

						if (!(SDL_GetModState() & KMOD_CTRL &&
							(e.text.text[0] == 'c' ||
							 e.text.text[0] == 'C' ||
							 e.text.text[0] == 'v' ||
							 e.text.text[0] == 'V'))) {
							int i;
							for (i = 0; inputText[i] != 
									'\0'; ++i)
								;
							char *temp = e.text.text;
							for (int j = 0; j <
								strlen(temp); ++j)
								inputText[i + j] =
									temp[j];
							puts("hi");
							renderText = 1;
						}
					}
				}

				if (renderText) {
					if (inputText[0] != '\0')
						loadFromRenderedText(gInputTextTexture,
						gRenderer, gFont, inputText, textColor);
					else
						loadFromRenderedText(gInputTextTexture,
						gRenderer, gFont, " ", textColor);
				}

				SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
				SDL_RenderClear(gRenderer);

				renderTexture(gPromptTextTexture, gRenderer,
						(SCREEN_WIDTH -
						 getTextureWidth(gPromptTextTexture)) / 2, 0,
						NULL, 0, NULL, SDL_FLIP_NONE);

				renderTexture(gInputTextTexture, gRenderer,
						(SCREEN_WIDTH -
						getTextureWidth(gInputTextTexture)) / 2,
						getTextureHeight(gInputTextTexture),
						NULL, 0, NULL, SDL_FLIP_NONE);

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

	gFont = TTF_OpenFont("./lazy.ttf", 28);
	if (gFont == NULL) {
		printf("Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError());
		success = 0;
	}
	else {
		SDL_Color textColor = {0, 0, 0, 0xFF};
		if (!loadFromRenderedText(gPromptTextTexture,
					gRenderer, gFont, "Enter Text:", textColor)) {
			printf("Failed to render prompt text!\n");
			success = 0;
		}
	}

	return success;
}

void finish() {
	freeTexture(gPromptTextTexture);
	gPromptTextTexture = NULL;

	freeTexture(gInputTextTexture);
	gInputTextTexture = NULL;

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
