.SILENT: all

all:
	echo Making...	
	gcc -std=c11 -g -Wall -Werror -Wextra -Wshadow -Wformat=2 -O3 -pedantic main.c -o coocp -lm
	gcc -std=c11 -Wall -Werror -Wextra -pedantic -Ofast random.c -o random
	echo Done
