#include "queries.h"

static FILE* openFile(const char* query, const char* header)
{
    FILE* file = fopen(query, "w+");
    if(file == NULL) 
    {
        fprintf(stderr, "Error al abrir el archivo %s\n", query);
        return FILE_ERROR;
    }
    fprintf(file, "%s\n", header);
    return file;
}

void handleQueryError(cityADT city, int queryNum) 
{
    fprintf(stderr, "Error generating query %d\n", queryNum);
    freeCity(city);
    exit(EXIT_FAILURE);
}

//TODO: 
int makeQuery1(cityADT city)
{
    puts("Query1!");
    return 1;
}       