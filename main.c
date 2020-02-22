#include "main.h"

int mandelc(long double complex c, int i) {
	/* checks if number c is part of mandelbrot set
	 * does ITE iterations, return 0 if in set otherwise amount of iterations done
	 */
	long double complex z = 0;
	int k;
	for (k = 1; k <= i; k++) {
		z = z * z + c;
		if (cabsl(z) >= 2.0) {
			return k;
		}
	}
	return 0;
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
	double time;
	clock_t start = clock();

	// Default values for image
	Cube data;
	data.den = 1000;
	data.pt.x = -2;
	data.pt.y = 2;
	data.dia = 4;

	// Default amount of iterations
	int ite = 200;

	// Defalt speed of change for colors
	double rs = 0.5;
	double gs = 0.02;
	double bs = 0.07;

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
			// Set X and Y coordinates, on default they coordinates of topleft corner or
			// if origo mode theyll be center coordinates of image
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

	uint8_t color[3] = {0, 0, 0};
	unsigned int i, j;
	int k;

	for (j = 0; j < data.den; ++j) {
		for (i = 0; i < data.den; ++i) {
			k = mandelc(getC(&data, i, j), ite);
			color[0] = 255 * sin(k*rs);
			color[1] = 255 * sin(k*gs);
			color[2] = 255 * sin(k*bs);
			fwrite(color, 1, 3, fp);
		}
	}
	fclose(fp);

	clock_t end = clock();
	time = ((double) (end - start)) / CLOCKS_PER_SEC;
	printf("time %f\n", time);

	return EXIT_SUCCESS;
}
