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
	int ite = ITE;

	Cube data;
	data.den = 1000;
	data.pt.x = -2;
	data.pt.y = 2;
	data.dia = 4;

	double rs = 0.1;
	double gs = 0;
	double bs = 0.02;

	bool ori = false;
	int count = 1;

	while (count < argc) {
		if (argv[count][0] == '-' && argv[count][1] == '-') {
			if (argv[count][2] == 'h') {
				help();
				return EXIT_SUCCESS;
			}
			else if (argv[count][2] == 'd') {
				data.den = atoi(&argv[count][4]);
			}
			else if (argv[count][2] == 'o') {
				ori = true;
			}
			else if (argv[count][2] == 'i') {
				ite = atoi(&argv[count][4]);
			}
			else if (argv[count][2] == 's') {
				//set speed for 
				rs = atof(strtok(&argv[count][4], ","));
				gs = atof(strtok(NULL, ","));
				bs = atof(strtok(NULL, ","));
			}
		}
		else {
			data.pt.x = strtold(strtok(argv[count], ","), NULL);
			data.pt.y = strtold(strtok(NULL, ","), NULL);
			data.dia = strtold(strtok(NULL, ","), NULL);
		}
		count++;
	}
	
	if (ori) {
		data.pt.x = data.pt.x - (data.dia/2);
		data.pt.y = data.pt.y + (data.dia/2);
	}	

	// print delta per pixel
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
				color.c.red = 255 * ((1+sin(k*rs))/2);			
				color.c.green = 255 * ((1+sin(k*gs))/2);
				color.c.blue = 255 * ((1+sin(k*bs))/2);
				fwrite(&(color.c.red), 1, 3, fp);
			}	
		}
	}
	fclose(fp);
	
	return EXIT_SUCCESS;
}

