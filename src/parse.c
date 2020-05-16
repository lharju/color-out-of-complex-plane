#include "main.h"

const char *argp_program_version = "color-out-of-complex-plane 1.0";
const char *argp_program_bug_address = "<santtu.harju@gmail.com>";
char doc[] = "Simple tool for creating images of fractals";
char args_doc[] = {0};
struct argp_option options[] = {
	{"mandelbrot", 'm' ,0 ,0, "Use Mandelbrot set", 1},
	{"julia", 'j', "R,C", OPTION_ARG_OPTIONAL, "Use Julia set. Use argument to set complex number to be added on each iteration, else -0.8+0.156i will be used", 1}, 
	{"iterations", 'i', "I", 0, "Set amount of iterations to be done", 2},
	{"size", 's', "S", 0, "Set image size to SxS pixels", 2},
	{"grayscale", 'g', "G", OPTION_ARG_OPTIONAL, "Image will use grayscale instead of rgb", 2},
	{"rate", 'r', "R,G,B", 0, "Set the rate of change for the colors", 2},
	{"name", 'n', "NAME", 0, "Set image name to NAME, extension will be determined by mode", 2},
	{"coordinates", 'c', "X,Y,D", 0, "Set coordinates of topleft corner to (X, Y) and diameter of the image to D ", 3},
	{"origin", 'o', 0, 0, "Coordinates used will be at origin of the image", 3},
	{"thread", 't', "T", 0, "Use T threads", 4},
	{ 0 }
};

error_t parse_opt(int key, char *arg, struct argp_state *state) {
	struct arguments *arguments = state->input;
	switch (key) {
		case 'm':
			arguments->mode = MANDELBROT;
			break;
		case 'j':
			arguments->mode = JULIA;
			if (arg) {
				arguments->julia = atof(strtok(arg, ","));
				arguments->julia += I * atof(strtok(NULL, ","));
			}
			break;
		case 'o':
			arguments->origin = true;
			break;
		case 's':
			arguments->size = atoi(arg);
			break;
		case 'i':
			arguments->iterations = atoi(arg);
			break;
		case 'g':
			arguments->grayscale = true;
			if (arg) {
				arguments->rateRed = atof(arg);
			}
			break;
		case 'r':
			arguments->rateRed = atof(strtok(arg, ","));
			arguments->rateGreen = atof(strtok(NULL, ","));
			arguments->rateBlue = atof(strtok(NULL, ","));
			break;
		case 'n':
			free(arguments->name);
			arguments->name = malloc(strlen(arg)+4);
			strcpy(arguments->name, arg);
			break;
		case 't':
			arguments->threadCount = atoi(arg);
			break;
		case 'c':
			arguments->x = atof(strtok(arg, ","));
			arguments->y = atof(strtok(NULL, ","));
			arguments->diameter = atof(strtok(NULL, ","));
			break;
		case ARGP_KEY_ARG:
			return 0;
		default:
			return ARGP_ERR_UNKNOWN;
	}
	return 0;
}
	
struct argp argp = { options, parse_opt, args_doc, doc, 0, 0, 0 };
