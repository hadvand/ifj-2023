#Makefile for IFJ compiler

CC = gcc
CFLAGS = -std=c11 -Werror -Wall -Wextra -pedantic
CFLAGS += -g

PROGS = ./scanner/scanner.c ./scanner/scanner.h ./structures/*
PARSER_PROGS = ./parser/* stack.c stack.h main.c
SEMANTIC_PROGS = ./semantics/*
TESTS = ./tests/* ./scanner/scanner.c ./structures/*

all:
	$(CC) $(CFLAGS) $^ -o $@ -lm
main:
	$(CC) $(CFLAGS) $^ -o $@ -lm

scanner-build: $(PROGS)
	$(CC) $(CFLAGS) $^ -o $@ -lm

scanner_tests: $(TESTS)
	$(CC) $(CFLAGS) -o $@ $^ -lm

parser-build: $(PARSER_PROGS) $(PROGS)
	$(CC) $(CFLAGS) $^ -o $@ -lm -DPARS_DEBUG

semantic-build: $(SEMANTIC_PROGS) $(PARSER_PROGS) $(PROGS)
	$(CC) $(CFLAGS) $^ -o $@ -lm -DSEM_DEBUG -DPARS_DEBUG

clean:
	rm -rf *.o scanner-build parser-build scanner_tests semantic-build