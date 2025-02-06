# Variables
CC = gcc
FLAGS = -Wall -pedantic -std=c99 -lm
SANITIZE = -fsanitize=address -g

INCLUDES = -I ./frontend/inc -I ./backend/inc

OUTPUT_FILE_NYC = parkingTicketsNYC
OUTPUT_FILE_CHI = parkingTicketsCHI

SOURCE_FILE = main.c

BACKEND_SRCS = backend/src/cityADT.c
BACKEND_OBJS = cityADT.o
FRONTEND_SRCS = frontend/src/dataValidation.c frontend/src/processData.c frontend/src/queries.c
FRONTEND_OBJS = dataValidation.o processData.o queries.o

OBJS = $(BACKEND_OBJS) $(FRONTEND_OBJS) main.o

QUERIES = query1.csv query2.csv query3.csv query4.csv


# Regla principal para compilar y ejecutar

NYC: $(OBJS)
	$(CC) $(FLAGS) $(SANITIZE) $(INCLUDES) $(OBJS) -o $(OUTPUT_FILE_NYC)

CHI: $(OBJS)
	$(CC) $(FLAGS) $(SANITIZE) $(INCLUDES) $(OBJS) -o $(OUTPUT_FILE_CHI)

main.o: $(SOURCE_FILE)
	$(CC) $(FLAGS) $(SANITIZE) $(INCLUDES) -c $(SOURCE_FILE)

cityADT.o: backend/src/cityADT.c
	$(CC) $(FLAGS) $(SANITIZE) $(INCLUDES) -c backend/src/cityADT.c

dataValidation.o: frontend/src/dataValidation.c
	$(CC) $(FLAGS) $(SANITIZE) $(INCLUDES) -c frontend/src/dataValidation.c

processData.o: frontend/src/processData.c	
	$(CC) $(FLAGS) $(SANITIZE) $(INCLUDES) -c frontend/src/processData.c

queries.o: frontend/src/queries.c
	$(CC) $(FLAGS) $(SANITIZE) $(INCLUDES) -c frontend/src/queries.c


# Limpieza de archivos generados
clean:
	rm -f $(OUTPUT_FILE_CHI) $(OUTPUT_FILE_NYC) $(QUERIES) parkingTicketsNYC parkingTicketsCHI *.o


