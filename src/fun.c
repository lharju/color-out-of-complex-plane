#include "main.h"

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
	temp.juliaReal = -0.8;
	temp.juliaImag = 0.156;
	temp.name = malloc(6);
	strcpy(temp.name, "image");
	temp.threadCount = 1;
	return temp;
}

void sleep(double time) {
	double stop = time * 0.001 * CLOCKS_PER_SEC + clock();
	while (clock()  < stop);
}

extern MainArgs args;

void *OldThread(void *param) {
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
				z = z * z + args.juliaReal + I * args.juliaImag;
			break;
		default:
			error("mode incorrect ?");
			break;
		}

		if (k == args.iterations) {
			color = 0;
		}
		else {
			color =  k + 1 - log(log(cabsl(z))) / LN2 ;
		}
		
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

void *NewThread(void *param) {
	uint8_t n = *((int *) param);
	*((int *)param) += 1;
	unsigned int i, k = 0;
	unsigned int threshold = args.size * args.size;
	
	long double x, y, u, v;
	long double xx, yy;
	double color;

	for (i = n; i < threshold; i += args.threadCount) {

		x = args.x + (i % args.size) * args.delta;
		y = args.y - (i / args.size) * args.delta;

		xx = x * x;
		yy = y * y;

		switch (args.mode) {
		case MANDELBROT:
			u = x;
			v = y;
			break;
		case JULIA:
			u = args.juliaReal;
			v = args.juliaImag;
			break;
		default:
			u = v = 0;
			error("mode incorrect ?");
			break;
		}

		for (k = 0; k < args.iterations && (xx + yy) < 4; ++k) {
				y = 2 * x * y + v;
				x = xx - yy + u;
				xx = x * x;
				yy = y * y;
		}
		
		if (k == args.iterations) {
			color = 0;
		}
		else {
			//color =  k + 1 - log(log(sqrt(xx + yy))) / LN2 ;
			color = 1 - ((double) k  / args.iterations);
		}
		
		if (!args.grayscale) {
			args.buffer[i * 3 + 0] = color * args.rateRed;
			args.buffer[i * 3 + 1] = color * args.rateGreen;
			args.buffer[i * 3 + 2] = color * args.rateBlue;
		}
		else {
			args.buffer[i] = color * 255;
		}
	}
	return NULL;
}
