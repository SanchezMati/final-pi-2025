# Variables
CC = gcc
FLAGS = -Wall -pedantic -std=c99 -lm
SANITIZE = -fsanitize=address -g

INCLUDES = -I ./frontend/inc -I ./backend/inc

OBJS = main.o backend.o frontend.o

OUTPUT_FILE_NYC = parkingTicketsNYC
OUTPUT_FILE_CHI = parkingTicketsCHI

SOURCE_FILE = main.c

BACKEND = backend/src/cityADT.c
FRONTEND = frontend/src/dataValidation.c frontend/src/processData.c frontend/src/queries.c

QUERIES = query1.csv query2.csv query3.csv query4.csv


# Regla principal para compilar y ejecutar
testNYC: $(SOURCE_FILE) $(BACKEND) $(FRONTEND)
	$(CC) $(FLAGS) $(SANITIZE) $(INCLUDES) $(SOURCE_FILE) $(BACKEND) $(FRONTEND) -o $(OUTPUT_FILE_NYC)

testCHI: $(SOURCE_FILE) $(BACKEND) $(FRONTEND)
	$(CC) $(FLAGS) $(SANITIZE) $(INCLUDES) $(SOURCE_FILE) $(BACKEND) $(FRONTEND) -o $(OUTPUT_FILE_CHI)


# Limpieza de archivos generados
clean:
	rm -f $(OUTPUT_FILE_CHI) $(OUTPUT_FILE_NYC) $(QUERIES) parkingTicketsNYC parkingTicketsCHI


NYC: $(OBJS)
	$(CC) $(FLAGS) $(SANITIZE) $(INCLUDES) $(OBJS) -o ($OUTPUT_FILE_NYC)

CHI: $(OBJS)
	$(CC) $(FLAGS) $(SANITIZE) $(INCLUDES) $(OBJS) -o ($OUTPUT_FILE_CHI)

main.o: ($SOURCE_FILE)
	$(CC) $(FLAGS) $(SANITIZE) $(INCLUDES) -c ($SOURCE_FILE)

back.o: ($BACKEND)
	$(CC) $(FLAGS) $(SANITIZE) $(INCLUDES) -c ($BACKEND)

front.o: ($FRONTEND)
	$(CC) $(FLAGS) $(SANITIZE) $(INCLUDES) -c ($FRONTEND)



