CC = gcc
FLAGS = -Wall -pedantic -std=c99 -fsanitize=address -g
OUTPUT_FILE = test
OBJ = main.o backend.o