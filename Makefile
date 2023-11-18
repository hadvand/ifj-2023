#Makefile for IFJ compiler

CC = gcc
CFLAGS = -std=c11 -Werror -Wall -Wextra -pedantic
CFLAGS += -g

PROGS = ./scanner/* ./structures/*
PARSER_PROGS = ./parser/* stack.c stack.h
TESTS = ./tests/* ./scanner/scanner.c ./structures/*

all:
	$(CC) $(CFLAGS) $^ -o $@ -lm
main:
	$(CC) $(CFLAGS) $^ -o $@ -lm

scanner_test: $(PROGS)
	$(CC) $(CFLAGS) $^ -o $@ -lm

test: $(TESTS)
	$(CC) $(CFLAGS) -o $@ $^ -lm

parser-build: $(PARSER_PROGS) $(PROGS)
	$(CC) $(CFLAGS) $^ -o $@ -lm

clean:
	rm -rf *.o scanner_test parser_build