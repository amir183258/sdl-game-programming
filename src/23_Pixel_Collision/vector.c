#include <stdlib.h>
#include "./vector.h"

void initVector(Vector *vector) {
	vector->items = NULL;

	vector->size = 0;
}

void createVector(Vector *vector, int vectorSize) {
	vector->items = calloc(vectorSize, sizeof(VectorNode));
	vector->size = vectorSize;
}
