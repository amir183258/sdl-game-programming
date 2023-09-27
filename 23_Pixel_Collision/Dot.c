#include "Dot.h"

#ifndef SCREEN_WIDTH
#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#endif

void initDot(Dot *dot, int x, int y) {
	dot->mPosX = x;
	dot->mPosY = y;

	dot->mVelX = 0;
	dot->mVelY = 0;

	initVector(&(dot->mColliders));
	createVector(&(dot->mColliders), 11);

	dot->mColliders.items[0].rectangle.w = 6;
	dot->mColliders.items[0].rectangle.h = 1;
	
	dot->mColliders.items[1].rectangle.w = 10;
	dot->mColliders.items[1].rectangle.h = 1;

	dot->mColliders.items[2].rectangle.w = 14;
	dot->mColliders.items[2].rectangle.h = 1;

	dot->mColliders.items[3].rectangle.w = 16;
	dot->mColliders.items[3].rectangle.h = 2;

	dot->mColliders.items[4].rectangle.w = 18;
	dot->mColliders.items[4].rectangle.h = 2;

	dot->mColliders.items[5].rectangle.w = 20;
	dot->mColliders.items[5].rectangle.h = 6;

	dot->mColliders.items[6].rectangle.w = 18;
	dot->mColliders.items[6].rectangle.h = 2;

	dot->mColliders.items[7].rectangle.w = 16;
	dot->mColliders.items[7].rectangle.h = 2;
	
	dot->mColliders.items[8].rectangle.w = 14;
	dot->mColliders.items[8].rectangle.h = 1;

	dot->mColliders.items[9].rectangle.w = 10;
	dot->mColliders.items[9].rectangle.h = 1;

	dot->mColliders.items[10].rectangle.w = 6;
	dot->mColliders.items[10].rectangle.h = 1;

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

void moveDot(Dot *dot, Vector *otherColliders) {
	dot->mPosX += dot->mVelX;
	shiftColliders(dot);

	if ((dot->mPosX < 0) || (dot->mPosX + DOT_WIDTH > SCREEN_WIDTH) ||
			checkCollision(&(dot->mColliders), otherColliders)) {
		dot->mPosX -= dot->mVelX;
		shiftColliders(dot);
	}

	dot->mPosY += dot->mVelY;
	shiftColliders(dot);

	if ((dot->mPosY < 0) || (dot->mPosY + DOT_HEIGHT > SCREEN_HEIGHT) ||
			checkCollision(&(dot->mColliders), otherColliders)) {
		dot->mPosY -= dot->mVelY;
		shiftColliders(dot);
	}
}

void renderDot(Dot *dot, LTexture *texture, SDL_Renderer *renderer) {
	renderTexture(texture, renderer, 
			dot->mPosX, 
			dot->mPosY,
			NULL, 0,
			NULL, SDL_FLIP_NONE);
}

int checkCollision(Vector *a, Vector *b) {
	int leftA, leftB;
	int rightA, rightB;
	int topA, topB;
	int bottomA, bottomB;

	for (int Abox = 0; Abox < a->size; ++Abox) {
		leftA = a->items[Abox].rectangle.x;
		rightA = a->items[Abox].rectangle.x + a->items[Abox].rectangle.w;
		topA = a->items[Abox].rectangle.y;
		bottomA = a->items[Abox].rectangle.y + a->items[Abox].rectangle.h;

		for (int Bbox = 0; Bbox < b->size; ++Bbox) {
			leftB = b->items[Bbox].rectangle.x;
			rightB = b->items[Bbox].rectangle.x + b->items[Bbox].rectangle.w;
			topB = b->items[Bbox].rectangle.y;
			bottomB = b->items[Bbox].rectangle.y + b->items[Bbox].rectangle.h;

			if ( ( ( bottomA <= topB ) || ( topA >= bottomB ) ||
						( rightA <= leftB ) ||
						( leftA >= rightB ) ) == 0)
				return 1;

		}
	}

	return 0;
}

void shiftColliders(Dot *dot) {
	int r = 0;

	for (int set = 0; set < dot->mColliders.size; ++set) {
		dot->mColliders.items[set].rectangle.x = dot->mPosX +
			(DOT_WIDTH - dot->mColliders.items[set].rectangle.w) / 2;

		dot->mColliders.items[set].rectangle.y = dot->mPosY + r;

		r += dot->mColliders.items[set].rectangle.h;
	}
}

Vector *getColliders(Dot *dot) {
	return &(dot->mColliders);

}
