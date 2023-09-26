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

	dot->mCollider.w = DOT_WIDTH;
	dot->mCollider.h = DOT_HEIGHT;
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

void moveDot(Dot *dot, SDL_Rect *wall) {
	dot->mPosX += dot->mVelX;
	dot->mCollider.x = dot->mPosX;

	if ((dot->mPosX < 0) || (dot->mPosX + DOT_WIDTH > SCREEN_WIDTH) ||
			checkCollision(dot->mCollider, *wall)) {
		dot->mPosX -= dot->mVelX;
		dot->mCollider.x = dot->mPosX;
	}

	dot->mPosY += dot->mVelY;
	dot->mCollider.y = dot->mPosY;

	if ((dot->mPosY < 0) || (dot->mPosY + DOT_HEIGHT > SCREEN_HEIGHT) ||
			checkCollision(dot->mCollider, *wall)) {
		dot->mPosY -= dot->mVelY;
		dot->mCollider.y = dot->mPosY;
	}
}

void renderDot(Dot *dot, LTexture *texture, SDL_Renderer *renderer) {
	renderTexture(texture, renderer, 
			dot->mPosX, 
			dot->mPosY,
			NULL, 0,
			NULL, SDL_FLIP_NONE);
}

int checkCollision(SDL_Rect a, SDL_Rect b) {
	int leftA, leftB;
	int rightA, rightB;
	int topA, topB;
	int bottomA, bottomB;

	leftA = a.x;
	rightA = a.x + a.w;
	topA = a.y;
	bottomA = a.y + a.h;

	leftB = b.x;
	rightB = b.x + b.w;
	topB = b.y;
	bottomB = b.y + b.h;

	if (bottomA <= topB)
		return 0;
	if (topA >= bottomB)
		return 0;
	if (rightA <= leftB)
		return 0;
	if (leftA >= rightB)
		return 0;

	return 1;
}

