#ifndef SDL_VERSION
#include <SDL.h>
#endif

#ifndef VECTOR_IS_DEFINED
#include "vector.h"
#endif

#ifndef LTEXTURE_INCLUDED
#include "LTexture.h"
#endif

#define DOT_WIDTH 20
#define DOT_HEIGHT 20
#define DOT_VEL 1

struct Dot {
	int mPosX, mPosY;

	int mVelX, mVelY;

	Vector mColliders;
};
typedef struct Dot Dot;


void initDot(Dot *dot, int x, int y);
void handleEventDot(Dot *dot, SDL_Event *e);
void moveDot(Dot *dot, Vector *mColliders);
void renderDot(Dot *dot, LTexture *texture, SDL_Renderer *renderer);
int checkCollision(Vector *a, Vector *b);
void shiftColliders(Dot *dot);
Vector *getColliders(Dot *dot);
