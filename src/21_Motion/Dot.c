#include "Dot.h"

#ifndef SCREEN_WIDTH
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#endif

void initDot(Dot *dot) {
	dot->mPosX = 0;
	dot->mPosY = 0;

	dot->mVelX = 0;
	dot->mVelY = 0;
}

void handleEventDot(Dot *dot, SDL_Event *e) {
	if (e->type == SDL_KEYDOWN && e->key.repeat == 0) {
		switch (e->key.keysym.sym) {
			case SDLK_UP:
				dot->mVelY -= DOT_VEL;
				break;
			case SDLK_DOWN:
				dot->mVelY += DOT_VEL;
				break;
			case SDLK_LEFT:
				dot->mVelX -= DOT_VEL;
				break;
			case SDLK_RIGHT:
				dot->mVelX += DOT_VEL;
				break;
		}
	}
	else if (e->type == SDL_KEYUP && e->key.repeat == 0) {
		switch(e->key.keysym.sym) {
			case SDLK_UP:
				dot->mVelY += DOT_VEL;
				break;
			case SDLK_DOWN:
				dot->mVelY -= DOT_VEL;
				break;
			case SDLK_LEFT:
				dot->mVelX += DOT_VEL;
				break;
			case SDLK_RIGHT:
				dot->mVelX -= DOT_VEL;
				break;
		}
	}
}

void move(Dot *dot) {
	dot->mPosX += dot->mVelX;

	if ((dot->mPosX < 0) || (dot->mPosX + DOT_WIDTH > SCREEN_WIDTH))
		dot->mPosX -= dot->mVelX;

	dot->mPosY += dot->mVelY;

	if ((dot->mPosY < 0) || (dot->mPosY + DOT_HEIGHT > SCREEN_HEIGHT))
		dot->mPosY -= dot->mVelY;
}

void renderDot(Dot *dot, LTexture *texture, SDL_Renderer *renderer) {
	renderTexture(texture, renderer, 
			dot->mPosX, 
			dot->mPosY,
			NULL, 0,
			NULL, SDL_FLIP_NONE);
}

