#include "queries.h"
#include "cityADT.h"

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
    printf("\n=== Starting Query 2 ===\n");
    FILE* file = openFile(QUERY2, "agency;topPlate;topTotal");
    if(file == FILE_ERROR) {
        return ERROR;
    }

    char agencyName[AGENCY_NAME+1];
    char topPlate[PLATE+1];
    size_t total;

    toBeginQuery2(city);
    printf("Starting iteration...\n");
    
    while(hasNextQuery2(city)) {
        nextQuery2(city, agencyName, topPlate, &total);
        printf("Processing: agency=%s, plate=%s, total=%zu\n", 
               agencyName, topPlate, total);
        fprintf(file, "%s;%s;%zu\n", agencyName, topPlate, total);
    }
    
    fclose(file);
    printf("=== Query 2 Completed ===\n");
    return SUCCESS;
}
