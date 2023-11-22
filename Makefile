#Makefile for IFJ compiler

CC = gcc
CFLAGS = -std=c11 -Werror -Wall -Wextra -pedantic
CFLAGS += -g

PROGS = ./scanner/scanner.c ./scanner/scanner.h ./structures/*
PARSER_PROGS = ./parser/* stack.c stack.h ./semantics/*
SEMANTIC_PROGS = ./semantics/*
TESTS = ./tests/* ./scanner/scanner.c ./scanner/scanner.h ./structures/*
SEM_TESTS = ./semantic_tests/*

all:
	$(CC) $(CFLAGS) $^ -o $@ -lm
main:
	$(CC) $(CFLAGS) $^ -o $@ -lm

scanner-build: $(PROGS) /scanner/scanner_tests.c
	$(CC) $(CFLAGS) $^ -o $@ -lm

scanner_tests: $(TESTS)
	$(CC) $(CFLAGS) -o $@ $^ -lm

parser-build: $(PARSER_PROGS) $(PROGS) main.c
	$(CC) $(CFLAGS) $^ -o $@ -lm -DPARS_DEBUG

semantic-build: $(SEMANTIC_PROGS) $(PARSER_PROGS) $(PROGS) main.c
	$(CC) $(CFLAGS) $^ -o $@ -lm -DSEM_DEBUG -DPARS_DEBUG

semantic-tests: $(PROGS) $(PARSER_PROGS) $(SEM_TESTS)
	$(CC) $(CFLAGS) $^ -o $@ -lm -DSEM_DEBUG -DPARS_DEBUG

clean:
	rm -rf *.o scanner-build parser-build scanner_tests semantic-build