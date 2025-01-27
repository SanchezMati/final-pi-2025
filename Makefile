# Variables
CC = gcc
FLAGS = -Wall -pedantic -std=c99 -fsanitize=address -g
OUTPUT_FILE = test
SOURCE_FILE = main.c

# Regla principal para compilar y ejecutar
test: $(SOURCE_FILE)
	$(CC) $(FLAGS) -o $(OUTPUT_FILE) $(SOURCE_FILE)

# Limpieza de archivos generados
clean:
	rm -f $(OUTPUT_FILE)
