# Variables
CC = gcc
FLAGS = -Wall -pedantic -std=c99 -fsanitize=address -g
INCLUDES = -I headers
OUTPUT_FILE = test.NYC
SOURCE_FILE = main.c
BACKEND = backend/cityADT.c
FRONTEND = frontend/dataValidation.c

# Regla principal para compilar y ejecutar
test: $(SOURCE_FILE) $(BACKEND) $(FRONTEND)
	$(CC) $(FLAGS) $(INCLUDES) $(SOURCE_FILE) $(BACKEND) $(FRONTEND) -o $(OUTPUT_FILE)

# Limpieza de archivos generados
clean:
	rm -f $(OUTPUT_FILE)

