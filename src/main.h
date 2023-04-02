#ifndef HEADERS
#define HEADERS

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include <ctype.h>
#include <complex.h>
#include <inttypes.h>
#include <math.h>
#include <string.h>
#include <time.h>

//#include <ncurses.h>
#include <pthread.h>
#include <argp.h>

#define LN2 		0.6931471806

#define error(a)	fprintf(stderr, "ERROR:%s: %s\n", __func__, a)

typedef struct arguments {
	enum {MANDELBROT, JULIA, GASTON} mode;
	bool origin;
	bool grayscale;
	uint32_t size; 
	uint32_t iterations;
	long double x;
	long double y;
	long double diameter;
	long double delta;
	double rateRed;
	double rateGreen;
	double rateBlue;
	long double juliaReal;
	long double juliaImag;
	char *name;
	uint8_t *buffer;
	uint8_t threadCount;	
} MainArgs;

void sleep(double);
void *OldThread(void *);
void *NewThread(void *);
struct arguments InitArgs(void);
unsigned int Mandelbrot(long double complex, int); 
unsigned int Julia(long double complex, long double complex, int);

error_t parse_opt(int, char *, struct argp_state *);
#endif
