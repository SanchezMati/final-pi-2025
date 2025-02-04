# Variables
CC = gcc
FLAGS = -Wall -pedantic -std=c99 
SANITIZE = -fsanitize=address -g
INCLUDES = -I ./frontend/inc -I ./backend/inc
OUTPUT_FILE_NYC = testNYC
OUTPUT_FILE_CHI = testCHI
SOURCE_FILE = main.c
BACKEND = backend/src/cityADT.c
FRONTEND = frontend/src/dataValidation.c frontend/src/processData.c frontend/src/queries.c
QUERY1 = "query1.csv"
QUERY2 = "query2.csv"


# Regla principal para compilar y ejecutar
testNYC: $(SOURCE_FILE) $(BACKEND) $(FRONTEND)
	$(CC) $(FLAGS) $(SANITIZE) $(INCLUDES) $(SOURCE_FILE) $(BACKEND) $(FRONTEND) -o $(OUTPUT_FILE_NYC)

testCHI: $(SOURCE_FILE) $(BACKEND) $(FRONTEND)
	$(CC) $(FLAGS) $(SANITIZE) $(INCLUDES) $(SOURCE_FILE) $(BACKEND) $(FRONTEND) -o $(OUTPUT_FILE_CHI)


# Limpieza de archivos generados
clean:
	rm -f $(OUTPUT_FILE_CHI) $(OUTPUT_FILE_NYC) $(QUERY1) $(QUERY2) parkingTicketsNYC parkingTicketsCHI


all: parkingTicketsNYC parkingTicketsCHI

parkingTicketsNYC: $(SOURCE_FILE) $(BACKEND) $(FRONTEND)
	$(CC) $(FLAGS) $(SANITIZE) $(INCLUDES) $(SOURCE_FILE) $(BACKEND) $(FRONTEND) -o parkingTicketsNYC

parkingTicketsCHI: $(SOURCE_FILE) $(BACKEND) $(FRONTEND)
	$(CC) $(FLAGS) $(SANITIZE) $(INCLUDES) $(SOURCE_FILE) $(BACKEND) $(FRONTEND) -o parkingTicketsCHI


