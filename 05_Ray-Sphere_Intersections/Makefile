# $@ - Target name
# $^ - Target dependencies

CC = gcc
CFLAGS = -std=c99 -Wall -Wpedantic -Wextra

main: main.c
	$(CC) $^ $(CFLAGS) -fsanitize=address,undefined -lm -O3 -o $@.o

val: main.c
	$(CC) $^ $(CFLAGS) -O3 -lm -o $@.o

debug: main.c
	$(CC) $^ $(CFLAGS) -fsanitize=address,undefined -Og -g -lm -o $@.o
