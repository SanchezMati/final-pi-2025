#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define AMOUNT_OF_ARGUMENTS 2

#define TICKETS 1
#define INFRACTIONS 2

#define ERROR -1

int main(int argc, char* argv[])
{
    if(argc != AMOUNT_OF_ARGUMENTS+1)
    {
        fprintf(stderr, "Error: Expected %d .csv files, but %d arguments were provided.\n", AMOUNT_OF_ARGUMENTS, argc-1);
        exit(EXIT_FAILURE);
    }


    //Creo que no hace falta validarlo, (pag 4, enunciado)
    // for (int i = 1; i <= AMOUNT_OF_ARGUMENTS; i++) 
    // {
    //     if (!isCsvFile(argv[i])) 
    //     {
    //         fprintf(stderr, "Error: The argument %s is not a .csv file.\n", argv[i]);
    //         exit(EXIT_FAILURE);
    //     }
    // }

    FILE* tickets = fopen(argv[TICKETS], "r");
    if (tickets == NULL)
    {
        fprintf(stderr, "Error al abrir el archivo: %s\n", argv[TICKETS]);
        exit(EXIT_FAILURE);
    }

    FILE* infractions = fopen(argv[INFRACTIONS], "r");
    if (infractions == NULL) 
    {
        fprintf(stderr, "Error al abrir el archivo: %s\n", argv[INFRACTIONS]);
        exit(EXIT_FAILURE);
    }

    cityADT city = newCity();

    if(city == NULL)
    {
        fprintf(stderr, "Error: Creating CityADT\n");
        fclose(tickets);
        fclose(infractions);
        exit(EXIT_FAILURE);
    }

    infractionsReader(infractions, city);
    ticketsReader(tickets, city);

    fclose(tickets) ;
    fclose(infractions) ;

    if (makeQuery1(city) == ERROR) 
    {
        handleQueryError(city, 1);
    }

    if (makeQuery2(city) == ERROR) 
    {
        handleQueryError(city, 2);
    }

    if (makeQuery3(city) == ERROR) 
    {
        handleQueryError(city, 3);
    }

    if (makeQuery4(city) == ERROR) 
    {
        handleQueryError(city, 4);
    }

    freeCity(city);

    puts("OK!");
    return 0;
}