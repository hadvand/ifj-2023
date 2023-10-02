#Makefile for IFJ compiler

CC = gcc
CFLAGS = -std=c11 -Werror -Wall -Wextra -pedantic
CFLAGS += -g

PROGS = ./scanner/* ./structures/*

main: main.c
	$(CC) $(CFLAGS) $^ -o $@ -lm

scanner_test: $(PROGS)
	$(CC) $(CFLAGS) $^ -o $@ -lm