#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <complex.h>
#include <inttypes.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

typedef struct point {
	long double x;
	long double y;
} Point;

typedef struct cube {
	unsigned int den;
	long double dia;
	Point pt;
} Cube;

typedef struct lang {
	long double real;
	long double imag;
} com;


void cmult(com *i, com *j) {
	double long tmp;
       	tmp = i->real * j->real - i->imag * j->imag;
	i->imag = i->real * j->imag + i->imag * j->real;
	i->real = tmp;
}
