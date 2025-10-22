#include <stdio.h>
#include "vector.h"

int main() {
	Vector *test = malloc(sizeof(Vector));
	initVector(test);

	createVector(test, 10);

	for (int i = 0; i < 10; ++i)
		printf("%d ", test->items[i].intNumber);
	puts("");

	test->items[3].intNumber = 24;

	for (int i = 0; i < 10; ++i)
		printf("%d ", test->items[i].intNumber);
	puts("");

	return 0;
}
