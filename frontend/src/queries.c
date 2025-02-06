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

char* intToMonth(int number)
{
    char* months[] = {
        "January", "February", "March", "April", "May", "June",
        "July", "August", "September", "October", "November", "December"
    };

    return months[number];
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

    int year;
    int tickets;
    char* infraction;

    toBegin(city);
    while(hasNext(city))
    {
        infraction = next(city, &year, &tickets);
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


int makeQuery4(cityADT city)
{
    printf("\n=== Starting Query 4 ===\n");
    FILE* file = openFile(QUERY4, HEADER4);
    if(file == FILE_ERROR) 
    {
        return ERROR;
    }

    char* agency;
    float maxDailyAvg, minDailyAvg, spread;
    char maxDailyDate[MAX_DATE_LENGTH];
    char minDailyDate[MAX_DATE_LENGTH];
    
    toBeginQuery4(city);
    while(hasNextQuery4(city))
    {
        // agency;maxDailyAvg;maxDailyDate;minDailyAvg;minDailyDate;spread
        agency = nextQuery4(city, &maxDailyAvg, &minDailyAvg, maxDailyDate, minDailyDate);
        
        maxDailyAvg = trunc(maxDailyAvg*100)/100;
        minDailyAvg = trunc(minDailyAvg*100)/100;
        
        spread = maxDailyAvg - minDailyAvg;

        spread = trunc(spread*100)/100;

        fprintf(file, "%s%s%f%s%s%s%f%s%s%s%f\n", agency, DELIMITER, maxDailyAvg, DELIMITER, maxDailyDate, DELIMITER, minDailyAvg, DELIMITER, minDailyDate, DELIMITER, spread);
        free(agency);
    }
    
    fclose(file);
    printf("=== Query 4 Completed ===\n");
    return SUCCESS;
}