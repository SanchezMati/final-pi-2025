# Variables
CC = clang
FLAGS = -Wall -pedantic -std=c99 
SANITIZE = -fsanitize=address -g
INCLUDES = -I headers
OUTPUT_FILE = test.NYC
SOURCE_FILE = main.c
BACKEND = backend/cityADT.c
FRONTEND = frontend/dataValidation.c frontend/processData.c frontend/queries.c

# Regla principal para compilar y ejecutar
test: $(SOURCE_FILE) $(BACKEND) $(FRONTEND)
	$(CC) $(FLAGS) $(SANITIZE) $(INCLUDES) $(SOURCE_FILE) $(BACKEND) $(FRONTEND) -o $(OUTPUT_FILE)

# Limpieza de archivos generados
clean:
	rm -f $(OUTPUT_FILE)

