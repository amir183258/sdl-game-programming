#ifndef SDL_VERSION
#include <SDL.h>
#endif

#ifndef LTEXTURE_INCLUDED
#include "LTexture.h"
#endif

#define DOT_DEFINED

#define DOT_WIDTH 20
#define DOT_HEIGHT 20
#define DOT_VEL 10

struct circle {
	int x, y;
	int r;
};
typedef struct circle Circle;

struct Dot {
	int mPosX, mPosY;

	int mVelX, mVelY;
};
typedef struct Dot Dot;

void initDot(Dot *dot);
void handleEventDot(Dot *dot, SDL_Event *e);
void moveDot(Dot *dot);
void renderDot(Dot *dot, LTexture *texture, SDL_Renderer *renderer);
int getPosXDot(Dot *dot);
int getPosYDot(Dot *dot);
