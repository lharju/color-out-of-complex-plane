#include "main.h"
#include <stdbool.h>

int mandelc(long double complex c, int i) {
	// checks if number c is part of mandelbrot set
	// does ITE iterations, return -1 if in set otherwise amount of iterations done
	long double complex z = 0;
	for (int k = 0; k < i; k++) {
		z = z * z + c;
		if (cabs(z) >= 2.0) {
			return k;
		}
	}
	return -1;
}

long double complex getC(Cube *data, int i, int j) {
	return  data->pt.x+(data->dia*i/data->den)+I*(data->pt.y-(data->dia*j)/data->den);
}

void help(void) {
	printf("Synopsis: Configurable tool for creating images of Mandelbrot set\n");
	printf("Usage: mandel [OPTIONS] [X,Y,D]\n");
	printf("Options:\n");
	printf("	--help, --h	prints this help and exit\n");
	printf("	--d=D		set image size to DxD pixels\n");
	printf("	--o  		X and Y will origo coordinats of image\n");
	printf("	--i=I		set amount of iterations to be done to check if number is in the set\n");
	printf("	--s=R,G,B	set speed of change for colors\n");	
}

int main(int argc, char **argv) {

	// Default values for image
	Cube data;
	data.den = 1000;
	data.pt.x = -2;
	data.pt.y = 2;
	data.dia = 4;
	
	// Default amount of iterations
	int ite = 200;

	// Defalt speed of change for colors
	double rs = 0.1;
	double gs = 0;
	double bs = 0.02;

	// Bool flag if coordinates used are at the origo of image
	bool ori = false;

	// Handling of arguments
	int count = 1;
	while (count < argc) {
		if (argv[count][0] == '-' && argv[count][1] == '-') {
			if (argv[count][2] == 'h') {
				// Print help and exit
				help();
				return EXIT_SUCCESS;
			}
			else if (argv[count][2] == 'd') {
				// Set size of image in pixels
				data.den = atoi(&argv[count][4]);
			}
			else if (argv[count][2] == 'o') {
				// Set origo mode to be used
				ori = true;
			}
			else if (argv[count][2] == 'i') {
				// Set amount of iterations to be done
				ite = atoi(&argv[count][4]);
			}
			else if (argv[count][2] == 's') {
				// Set speed of change for colors
				rs = atof(strtok(&argv[count][4], ","));
				gs = atof(strtok(NULL, ","));
				bs = atof(strtok(NULL, ","));
			}
		}
		else {
			// Set X and Y coordinates, on default they coordinates of topleft corner or if origo mode theyll be center coordinates of image
			data.pt.x = strtold(strtok(argv[count], ","), NULL);
			data.pt.y = strtold(strtok(NULL, ","), NULL);
			data.dia = strtold(strtok(NULL, ","), NULL);
		}
		count++;
	}
	
	// If origo mode in use convert coordinates to be topleft coordinates
	if (ori) {
		data.pt.x = data.pt.x - (data.dia/2);
		data.pt.y = data.pt.y + (data.dia/2);
	}	

	// Print delta per pixel
	printf("delta %.3Le\n",  data.dia / data.den);


	FILE *fp = fopen("mandel.ppm", "wb");
	if(!fp) {
		fprintf(stderr, "Couldnt open file\n");
		return EXIT_FAILURE;
	}
	fprintf(fp, "P6\n%d %d\n255\n", data.den, data.den);

	uint8_t black[3] = {0, 0, 0};
	union colors color;
	color.full = 0;
	int i, j, k;

	for (j = 0; j < data.den; ++j) {
		for (i = 0; i < data.den; ++i) {
			k = mandelc(getC(&data, i, j), ite);

			if (k == -1) {
				fwrite(black, 1, 3, fp);
			}
			else {
				color.c.red = (rs ? 255 * ((1+sin(k*rs))/2) : 0);			
				color.c.green = (gs ? 255 * ((1+sin(k*gs))/2) : 0);
				color.c.blue = (bs ? 255 * ((1+sin(k*bs))/2) : 0);
				fwrite(&(color.c.red), 1, 3, fp);
			}	
		}
	}
	fclose(fp);
	
	return EXIT_SUCCESS;
}

