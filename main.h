#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <complex.h>
#include <inttypes.h>
#include <math.h>
#include <string.h>

typedef struct point {
	long double x;
	long double y;
} Point;

typedef struct cube {
	unsigned int den;
	long double dia;
	Point pt;
} Cube;

union colors {
	uint32_t full;
	struct {uint8_t red, green, blue, padding;} c;
};


