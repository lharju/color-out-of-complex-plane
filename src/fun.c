#include "main.h"
#define LN2 		0.6931471806

struct arguments InitArgs(void) {
	struct arguments temp;
	temp.mode = MANDELBROT;
	temp.origin = false;
	temp.grayscale = false;
	temp.size = 1000; 
	temp.iterations = 200;
	temp.x = -2; 
	temp.y = 2;
	temp.diameter = 4;
	temp.rateRed = 1;
	temp.rateGreen = 2;
	temp.rateBlue = 10;
	temp.julia = -0.8+0.156*I;
	temp.name = malloc(6);
	strcpy(temp.name, "image");
	temp.threadCount = 1;
	return temp;
}

void sleep(double time) {
	clock_t start = clock();
	while ((clock() - start) / (0.001 * CLOCKS_PER_SEC) < time);
}

extern MainArgs args;

void *Thread(void *param) {
	uint8_t n = *((int *) param);
	*((int *)param) += 1;
	unsigned int i, k = 0;
	unsigned int threshold = args.size * args.size;
	long double complex z, c;
	double color;
	for (i = n; i < threshold; i += args.threadCount) {
		z = args.x+(i%args.size)*args.delta+I*(args.y-(i/args.size)*args.delta);
		switch (args.mode) {
		case MANDELBROT:
			c = z;
			z = 0;
			for (k = 0; k < args.iterations && cabsl(z) < 2.0; ++k)
				z = z * z + c;
			break;
		case JULIA:
			for (k = 0; k < args.iterations && cabsl(z) < 2.0; ++k)
				z = z * z + args.julia;
			break;
		default:
			error("mode incorrect ?");
			break;
		}
		color = k!=args.iterations ? k+1-log(log(cabsl(z)))/LN2 : 0;
		if (!args.grayscale) {
			args.buffer[i * 3 + 0] = color * args.rateRed;
			args.buffer[i * 3 + 1] = color * args.rateGreen;
			args.buffer[i * 3 + 2] = color * args.rateBlue;
		}
		else {
			args.buffer[i] = color * args.rateRed;
		}
	}
	return NULL;
}
