#include "main.h"


MainArgs args;

extern struct argp_option options[];
extern char args_doc[];
extern char doc[];
extern struct argp argp;

int main(int argc, char **argv) {
	args = InitArgs();
	argp_parse(&argp, argc, argv, 0, 0, &args);

	if (args.origin) {
		args.x -= (args.diameter/2);
		args.y += (args.diameter/2);
	}

	args.delta = args.diameter / args.size;

	args.buffer = calloc(1, args.size * args.size * (args.grayscale? 1: 3));
	if (!args.buffer) {
		error("memory allocation failed");
		return EXIT_FAILURE;
	}
	int n, nc;
	pthread_t threads[args.threadCount];
	for (n = 0; n < args.threadCount; n++) {
		nc = n;
		if (pthread_create(&threads[n], NULL, Thread, &nc)) {
			error("thread not created");
			return EXIT_FAILURE;
		}
		while (nc == n) sleep(1);
	}
	for (n = 0; n < args.threadCount; n++) {
		if (pthread_join(threads[n], NULL)) {
			error("thread not joined");
			return EXIT_FAILURE;
		}
	}

	FILE *fp = fopen(strcat(args.name, (args.grayscale ? ".pgm" :".ppm")), "wb");
	if(!fp) {
		error("unable to create file");
		return EXIT_FAILURE;
	}
	fprintf(fp, "P%d\n%d %d\n255\n", (args.grayscale ? 5 : 6),args.size, args.size);
	fwrite(args.buffer, args.grayscale ? 1 : 3, args.size * args.size, fp);
	fclose(fp);

	free(args.buffer);
	free(args.name);

	return EXIT_SUCCESS;
}
