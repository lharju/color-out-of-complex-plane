.SILENT: all

all:
	echo Making...	
	gcc -std=c11 -g -Wall -Werror -Wextra -Wshadow -Wformat=2 -O3 -pedantic main.c -o coocp -lm
	echo Done
