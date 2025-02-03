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
    FILE* file = openFile(QUERY1, HEADER1);
    if(file == FILE_ERROR) 
    {
        return ERROR;
    }

    toBegin(city);
    while(hasNext(city))
    {
        int year;
        int tickets;
        char* infraction = next(city, &year, &tickets);
        fprintf(file, "%s%s%d%s%d\n", infraction, DELIMITER, year, DELIMITER, tickets);
        free(infraction);
    }
    fclose(file);
    return SUCCESS;
}       

int makeQuery2(cityADT city) 
{
    FILE* file = openFile(QUERY2, HEADER2);
    if(file == FILE_ERROR) {
        return ERROR;
    }

    char agencyName[AGENCY_LENTH+1];
    char topPlate[PLATE_LENTH+1];
    int total;
    
    toBeginQuery2(city);    
    while(hasNextQuery2(city)) 
    {
        nextQuery2(city, agencyName, topPlate, &total);
        fprintf(file, "%s;%s;%d\n", agencyName, topPlate, total);
    }
    
    fclose(file);
    return SUCCESS;
}