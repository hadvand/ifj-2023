cmake_minimum_required(VERSION 3.16)
project(IFJ_Project C)

set(CMAKE_C_STANDARD 11)

include_directories(scanner)


add_executable(
        scanner/scanner.c
        scanner/scanner.h
        scanner/scanner_tests.c
)


