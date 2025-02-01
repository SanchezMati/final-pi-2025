# Variables
CC = gcc
FLAGS = -Wall -pedantic -std=c99 
SANITIZE = -fsanitize=address -g
INCLUDES = -I headers
OUTPUT_FILE_NYC = test.NYC
OUTPUT_FILE_CHI = test.CHI
SOURCE_FILE = main.c
BACKEND = backend/cityADT.c
FRONTEND = frontend/dataValidation.c frontend/processData.c frontend/queries.c

# Regla principal para compilar y ejecutar
testNYC: $(SOURCE_FILE) $(BACKEND) $(FRONTEND)
	$(CC) $(FLAGS) $(SANITIZE) $(INCLUDES) $(SOURCE_FILE) $(BACKEND) $(FRONTEND) -o $(OUTPUT_FILE_NYC)

testCHI: $(SOURCE_FILE) $(BACKEND) $(FRONTEND)
	$(CC) $(FLAGS) $(SANITIZE) $(INCLUDES) $(SOURCE_FILE) $(BACKEND) $(FRONTEND) -o $(OUTPUT_FILE_CHI)


# Limpieza de archivos generados
clean:
	rm -f $(OUTPUT_FILE_CHI) $(OUTPUT_FILE_NYC)

