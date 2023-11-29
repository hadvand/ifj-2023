#Makefile for IFJ compiler

CC = gcc
CFLAGS = -std=c11 -Werror -Wall -Wextra -pedantic
CFLAGS += -g

PROGS = scanner.c scanner.h error.h string.h string.c
PARSER_PROGS = hash.h hash.c parser.h parser.c table_stack.h table_stack.c stack.c stack.h semantics.c semantics.h
SEMANTIC_PROGS = semantics.c semantics.h
TESTS = ./tests/* scanner.c scanner.h error.h string.h string.c
SEM_TESTS = ./semantic_tests/*

all: main

main: $(PARSER_PROGS) $(PROGS) main.c
	$(CC) $(CFLAGS) $^ -o $@ -lm

scanner-build: $(PROGS) scanner_tests.c
	$(CC) $(CFLAGS) $^ -o $@ -lm

scanner_test: $(TESTS)
	$(CC) $(CFLAGS) -o $@ $^ -lm

parser-debug: $(PARSER_PROGS) $(PROGS) main.c
	$(CC) $(CFLAGS) $^ -o $@ -lm -DPARS_DEBUG

semantic-build: $(SEMANTIC_PROGS) $(PARSER_PROGS) $(PROGS) main.c
	$(CC) $(CFLAGS) $^ -o $@ -lm -DSEM_DEBUG -DPARS_DEBUG

semantic-tests: $(PROGS) $(PARSER_PROGS) $(SEM_TESTS)
	$(CC) $(CFLAGS) $^ -o $@ -lm -DSEM_DEBUG -DPARS_DEBUG

compiler: $(PARSER_PROGS) $(PROGS) main.c
	$(CC) $(CFLAGS) $^ -o $@ -lm

clean:
	rm -rf *.o scanner-build parser-build scanner_tests semantic-build semantic-tests scanner_test parser-debug compiler