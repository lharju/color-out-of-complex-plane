.SILENT: cli gui
.DEFAULT: cli

flags = -std=c11 -g -Wall -Werror -Wextra -Wshadow -O3
files = src/*.c src/*.h Makefile

cli:
	gcc $(flags) src/*.c -o cocp -lm -lpthread
edit:
	vim -p $(files)

