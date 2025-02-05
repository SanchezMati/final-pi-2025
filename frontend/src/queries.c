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

int makeQuery1(cityADT city)
{
    printf("\n=== Starting Query 1 ===\n");
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
    printf("=== Query 1 Completed ===\n");
    return SUCCESS;
}

int makeQuery2(cityADT city) 
{
    printf("\n=== Starting Query 2 ===\n");
    FILE* file = openFile(QUERY2, "agency;topPlate;topTotal");
    if(file == FILE_ERROR) 
    {
        return ERROR;
    }

    char agencyName[AGENCY_NAME+1];
    char topPlate[PLATE+1];
    size_t total;

    toBeginQuery2(city);
    while(hasNextQuery2(city)) 
    {
        nextQuery2(city, agencyName, topPlate, &total);
        fprintf(file, "%s%s%s%s%zu\n", agencyName, DELIMITER, topPlate, DELIMITER, total);
    }
    printf("=== Query 2 Completed ===\n");
    fclose(file);
    return SUCCESS;
}

int makeQuery3(cityADT city)
{
    printf("\n=== Starting Query 3 ===\n");

    FILE* file = openFile(QUERY3, HEADER3);
    if(file == FILE_ERROR) 
    {
        return ERROR;
    }

    char* infraction;
    char* month;
    for(int i = 0; i < MONTHS; i++)
    {
        infraction = getTopInfraction(city, i);
        month = intToMonth(i);
        fprintf(file, "%s%s%s\n", month, DELIMITER, infraction);
        free(infraction);
    }
    printf("=== Query 3 Completed ===\n");
    fclose(file);
    return SUCCESS;
}

char* intToMonth(int number)
{
    char* months[] = {
        "January", "February", "March", "April", "May", "June",
        "July", "August", "September", "October", "November", "December"
    };

    return months[number];
}
