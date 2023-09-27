#Makefile for IFJ compiler

CC = gcc
CFLAGS = -std=c11 -Werror -Wall -Wextra -pedantic
CFLAGS += -g

PROGS = main.c

main: $(PROGS)
	$(CC) $(CFLAGS) $^ -o $@ -lm