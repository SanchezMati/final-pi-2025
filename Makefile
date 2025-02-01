# Variables
CC = gcc
FLAGS = -Wall -pedantic -std=c99 
SANITIZE = -fsanitize=address -g
INCLUDES = -I headers
SOURCE_FILE = main.c
BACKEND = backend/cityADT.c
FRONTEND = frontend/dataValidation.c frontend/processData.c frontend/queries.c

all: parkingTicketsNYC parkingTicketsCHI

parkingTicketsNYC: $(SOURCE_FILE) $(BACKEND) $(FRONTEND)
	$(CC) $(FLAGS) $(SANITIZE) $(INCLUDES) $(SOURCE_FILE) $(BACKEND) $(FRONTEND) -o test.NYC

parkingTicketsCHI: $(SOURCE_FILE) $(BACKEND) $(FRONTEND)
	$(CC) $(FLAGS) $(SANITIZE) $(INCLUDES) $(SOURCE_FILE) $(BACKEND) $(FRONTEND) -o test.CHI

# Limpieza de archivos generados
clean:
	rm -f parkingTicketsNYC parkingTicketsCHI


