#include "queries.h"

static FILE* openFile(const char* query, const char* header)
{
    FILE* file = fopen(query, "w+");
    if(file == NULL) 
    {
        fclose(file);
        fprintf(stderr, "Error al abrir el archivo\n");
        return ERROR;
    }
    fprintf(file, "%s\n", header);

    return file;
}

static void handleQueryError(cityADT city, int queryNum) 
{
    fprintf(stderr, "Error generating query %d\n", queryNum);
    freeCity(city);
    exit(EXIT_FAILURE);
}

//TODO: 
int makeQuery1(cityADT city)
{
    puts("OK!");
    return 0;
}