#include <stdio.h>
#include <time.h>
#include <stdlib.h>

int main(void) {
	srand(time(NULL));
	fprintf(stdout, "%f,%f,%f", (rand() % 2 ? -1 : 1) * (rand() % 10000 / 10000.0),(rand() % 2 ? -1 : 1) * (rand() % 10000 / 10000.0),(rand() % 2 ? -1 : 1) * (rand() % 10000 / 10000.0));	
}
