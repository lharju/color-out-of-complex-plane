.PHONY: all

all:
	gcc -std=c11 -Wall -Ofast -lm main.c -o coocp
	gcc -Wall -Ofast random.c -o random
