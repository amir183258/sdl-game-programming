#ifndef SDL_VERSION
#include <SDL.h>
#endif

#ifndef LTEXTURE_INCLUDED
#include "LTexture.h"
#endif

#define DOT_DEFINED

#define DOT_WIDTH 20
#define DOT_HEIGHT 20
#define DOT_VEL 1

struct circle {
	int x, y;
	int r;
};
typedef struct circle Circle;

struct Dot {
	int mPosX, mPosY;

	int mVelX, mVelY;

	Circle mCollider;
};
typedef struct Dot Dot;


void initDot(Dot *dot, int x, int y);
void handleEventDot(Dot *dot, SDL_Event *e);
void moveDot(Dot *dot, SDL_Rect *wall, Circle *circle);
void renderDot(Dot *dot, LTexture *texture, SDL_Renderer *renderer);
int checkCollisionRect(Circle *a, SDL_Rect *b);
int checkCollisionCirc(Circle *a, Circle *b);
void shiftColliders(Dot *dot);
double distanceSquared(int x1, int y1, int x2, int y2);
Circle *getCollider(Dot *dot);
