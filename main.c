//TODO: Agruegar los path en la compilacion con el flag -I
#include "main.h"
#include <time.h> 

/*Este mismo enum se repite en los archivos de proccesData.c y dataValidation.c.
no se si es de buen estilo o si seria mejor ponerlo todo en un solo archivo .h 
e incluirlo en los tres archivos*/
/*tal vez con agruegarlos en sus respectivos .h alcanze*/

int main(int argc, char* argv[])
{

    clock_t start, end;
    double cpu_time_used;

    start = clock();  // Inicia la medición

    if(argc != AMOUNT_OF_ARGUMENTS+1)
    {
        fprintf(stderr, "Error: Expected %d .csv files, but %d arguments were provided.\n", AMOUNT_OF_ARGUMENTS, argc-1);
        exit(EXIT_FAILURE);
    }

    int fileType = isValidExecutable(argv[0]);
    if(fileType == INVALID)
    {
        fprintf(stderr, "Error: The executable must end in NYC or CHI.\n");
        exit(EXIT_FAILURE);
    }

    //Creo que no hace falta validarlo, (pag 4, enunciado)
    for (int i = 1; i <= AMOUNT_OF_ARGUMENTS; i++) 
    {
        if (!isCsvFile(argv[i])) 
        {
            fprintf(stderr, "Error: The argument %s is not a .csv file.\n", argv[i]);
            exit(EXIT_FAILURE);
        }
    }

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
    if (! initInfractionsByMonth(city))
    {
        fprintf(stderr, "Error: No Memory");
        exit(EXIT_FAILURE);
    }
    ticketsReader(tickets, city, fileType);

    fclose(tickets) ;
    fclose(infractions) ;

    if (makeQuery1(city) == ERROR) 
    {
        handleQueryError(city, 1);
    }

    // if (makeQuery2(city) == ERROR) 
    // {
    //     handleQueryError(city, 2);
    // }

    if (makeQuery3(city) == ERROR) 
    {
        handleQueryError(city, 3);
    }

    // if (makeQuery4(city) == ERROR) 
    // {
    //     handleQueryError(city, 4);
    // }

    freeCity(city);

    end = clock();  // Finaliza la medición
    // Calcula el tiempo en segundos
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    printf("Tiempo de ejecución: %f segundos\n", cpu_time_used);
    puts("OK!");
    
    return 0;
}