#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include "./LTexture.h"

int loadFromFile(LTexture *texture, SDL_Renderer *renderer, char *path) {
	freeTexture(texture);

	SDL_Texture *newTexture = NULL;

	SDL_Surface *loadedSurface = IMG_Load(path);
	if (loadedSurface == NULL)
		printf("Unable to load image %s! SDL_image Error: %s\n", path, IMG_GetError());
	else {
		SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGB(loadedSurface->format, 0, 0xFF, 0xFF));

		newTexture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
		if (newTexture == NULL)
			printf("Unable to create texture %s! SDL Error: %s\n", path, SDL_GetError());
		else {
			texture->mWidth = loadedSurface->w;
			texture->mHeight = loadedSurface->h;
		}

		SDL_FreeSurface(loadedSurface);
	}

	texture->mTexture = newTexture;
	return texture->mTexture != NULL;
}

void freeTexture(LTexture *texture) {
	if (texture->mTexture != NULL) {
		SDL_DestroyTexture(texture->mTexture);
		texture->mTexture = NULL;
		texture->mWidth = 0;
		texture->mHeight= 0;
	}
	else
		puts("The texture is NULL!");
}

void renderTexture(LTexture *texture, SDL_Renderer *renderer, int x, int y) {
	SDL_Rect renderQuad = {x, y, texture->mWidth, texture->mHeight};

	SDL_RenderCopy(renderer, texture->mTexture, NULL, &renderQuad);
}

int getTextureWidth(LTexture *texture) {
	return texture->mWidth;
}

int getTextureHeight(LTexture *texture) {
	return texture->mHeight;
}

void initTexture(LTexture *texture) {
	texture->mTexture = NULL;
	texture->mWidth = 0;
	texture->mHeight = 0;
}
