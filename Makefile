#Makefile for IFJ compiler

CC = gcc
CFLAGS = -std=c11 -Werror -Wall -Wextra -pedantic
CFLAGS += -g

PROGS = src/scanner.c src/scanner.h src/error.h src/string.h src/string.c
PARSER_PROGS = src/hash.h src/hash.c src/parser.h src/parser.c src/table_stack.h src/table_stack.c src/stack.c src/stack.h src/semantics.c src/semantics.h src/generator.h src/generator.c
SEMANTIC_PROGS = src/semantics.c src/semantics.h
GENERATOR_PROGS = src/generator.c src/generator.h
TESTS = ./tests/* src/scanner.c src/scanner.h src/error.h src/string.h src/string.c
SEM_TESTS = ./semantic_tests/*

all: main

main: $(PARSER_PROGS) $(PROGS) src/main.c
	$(CC) $(CFLAGS) $^ -o $@ -lm

scanner-build: $(PROGS) tests/scanner_tests.c
	$(CC) $(CFLAGS) $^ -o $@ -lm

scanner_test: $(TESTS)
	$(CC) $(CFLAGS) -o $@ $^ -lm

parser-debug: $(PARSER_PROGS) $(PROGS) src/main.c
	$(CC) $(CFLAGS) $^ -o $@ -lm -DPARS_DEBUG

semantic-build: $(SEMANTIC_PROGS) $(PARSER_PROGS) $(PROGS) src/main.c
	$(CC) $(CFLAGS) $^ -o $@ -lm -DSEM_DEBUG -DPARS_DEBUG

semantic-tests: $(PROGS) $(PARSER_PROGS) $(SEM_TESTS)
	$(CC) $(CFLAGS) $^ -o $@ -lm -DSEM_DEBUG -DPARS_DEBUG

#generator-tests: $(GENERATOR_PROGS)
#	$(CC) $(CFLAGS) $^ -o gen_tests -lm
generator-debug: $(PARSER_PROGS) $(PROGS) $(GENERATOR_PROGS) src/main.c
	$(CC) $(CFLAGS) $^ -o $@ -lm

test: compiler
	./compiler < Taylor.swift > generator_out
	./ifj23-tests/ic23int generator_out

compiler: $(PARSER_PROGS) $(PROGS) $(GENERATOR_PROGS) src/main.c
	$(CC) $(CFLAGS) $^ -o $@ -lm

clean:
	rm -rf *.o scanner-build parser-build semantic-build semantic-tests scanner_test parser-debug compiler generator-tests main