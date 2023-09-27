#ifndef SDL_VERSION
#include <SDL.h>
#endif

#define VECTOR_IS_DEFINED

union vectorNode {
	int intNumber;
	SDL_Rect rectangle;
};
typedef union vectorNode VectorNode;

struct vector {
	VectorNode *items;
	
	int size;
};
typedef struct vector Vector;

void initVector(Vector *vector);
void createVector(Vector *vector, int vectorSize);

