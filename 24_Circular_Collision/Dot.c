#ifndef DOT_DEFINED
#include "Dot.h"
#endif

#ifndef SCREEN_WIDTH
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#endif

void initDot(Dot *dot, int x, int y) {
	dot->mPosX = x;
	dot->mPosY = y;

	dot->mVelX = 0;
	dot->mVelY = 0;

	dot->mCollider.r = DOT_WIDTH / 2;

	shiftColliders(dot);
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

void moveDot(Dot *dot, SDL_Rect *wall, Circle *circle) {
	dot->mPosX += dot->mVelX;
	shiftColliders(dot);

	if ((dot->mPosX - dot->mCollider.r < 0) ||
			(dot->mPosX + dot->mCollider.r > SCREEN_WIDTH) ||
			checkCollisionRect(&(dot->mCollider), wall) ||
			checkCollisionCirc(&(dot->mCollider), circle)) {
		dot->mPosX -= dot->mVelX;
		shiftColliders(dot);
	}

	dot->mPosY += dot->mVelY;
	shiftColliders(dot);

	if ((dot->mPosY - dot->mCollider.r < 0) ||
			(dot->mPosY + dot->mCollider.r > SCREEN_HEIGHT) ||
			checkCollisionRect(&(dot->mCollider), wall) ||
			checkCollisionCirc(&(dot->mCollider), circle)) {
		dot->mPosY -= dot->mVelY;
		shiftColliders(dot);
	}
}

void renderDot(Dot *dot, LTexture *texture, SDL_Renderer *renderer) {
	renderTexture(texture, renderer, 
			dot->mPosX - dot->mCollider.r, 
			dot->mPosY - dot->mCollider.r,
			NULL, 0,
			NULL, SDL_FLIP_NONE);
}

int checkCollisionCirc(Circle *a, Circle *b) {
	int totalRadiusSquared = a->r + b->r;
	totalRadiusSquared = totalRadiusSquared * totalRadiusSquared;

	if (distanceSquared(a->x, a->y, b->x, b->y) < (totalRadiusSquared))
		return 1;
	return 0;
}

int checkCollisionRect(Circle *a, SDL_Rect *b) {
	int cX, cY;

	if (a->x < b->x)
		cX = b->x;
	else if (a->x > b->x + b->w)
		cX = b->x + b->w;
	else
		cX = a->x;

	if (a->y < b->y)
		cY = b->y;
	else if (a->y > b->y + b->h)
		cY = b->y + b->h;
	else
		cY = a->y;

	if (distanceSquared(a->x, a->y, cX, cY) < a->r * a->r)
		return 1;
	return 0;
}

void shiftColliders(Dot *dot) {
	dot->mCollider.x = dot->mPosX;
	dot->mCollider.y = dot->mPosY;
}

double distanceSquared(int x1, int y1, int x2, int y2) {
	int deltaX = x2 - x1;
	int deltaY = y2 - y1;

	return deltaX * deltaX + deltaY * deltaY;
}

Circle *getCollider(Dot *dot) {
	return &(dot->mCollider);
}

