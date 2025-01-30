#include "processData.h"

#define LENTH_OF_AGENCY_NAME (35 + 1) //35 chars + '\0'
#define LENTH_OF_PLATES (10 + 1)  //plates lenght
 
#define SIZE_LINE_INFRACTIONS 32
#define SIZE_LINE_TICKETS 65

enum fileType {INVALID = 0, NYC, CHI};

void infractionsReader(FILE * file, cityADT city) 
{   
    char line[SIZE_LINE_INFRACTIONS];
    while (fgets(line, sizeof(line), file)) {
        char * infractionId = NULL;
        char * description = NULL;
        char * token = strtok(line, ";");
        
        if ( token != NULL ) {
            infractionId = token;
        }
        token = strtok(NULL, ";");
        if ( token != NULL ) {
            description = token;
        }
        addInfraction(city, infractionId, description);
    }
}

void ticketsReader(FILE * file, cityADT city, int fileType) 
{
    char line[SIZE_LINE_TICKETS];
    if (fileType == NYC) 
    {
        while (fgets(line, sizeof(line), file)) 
        {
            processNYCTicketLine(line, city);
        }
    }
    else
    {
        while (fgets(line, sizeof(line), file)) 
        {
            processCHITicketLine(line, city);
        }
    }
}

void processNYCTicketLine(const char * line, cityADT city) 
{
    char plate[LENTH_OF_PLATES], agencyName[LENTH_OF_AGENCY_NAME];
    size_t infractionId, fineAmount, year, month, day;

    char * token = strtok((char *)line, ";");
    strncpy(plate, token, LENTH_OF_PLATES - 1);
    plate[LENTH_OF_PLATES - 1] = '\0';

    token = strtok(NULL, "-");
    year = atoi(token);
    token = strtok(NULL, "-");
    month = atoi(token) ;
    token = strtok(NULL, "-");
    day = atoi(token);

    token = strtok(NULL, ";");
    infractionId = (size_t)atoi(token);

    token = strtok(NULL, ";");
    fineAmount = (size_t)atoi(token);

    token = strtok(NULL, ";");
    strcpy(agencyName, token, LENTH_OF_AGENCY_NAME);

    processTicket(city, plate, year, month, infractionId, fineAmount, agencyName);     
}

void processCHITicketLine(const char * line, cityADT city)
{
    char plate[LENTH_OF_PLATES], agencyName[LENTH_OF_AGENCY_NAME];
    size_t infractionId, fineAmount, year, month, day;

    char * token = strtok((char *)line, "-");
    year = atoi(token);
    token = strtok(NULL, "-");
    month = atoi(token);
    token = strtok(NULL, "-");
    day = atoi(token) ;

    token = strtok(NULL, ";");
    strcpy(plate, token, LENTH_OF_PLATES);

    token = strtok(NULL, ";");
    strcpy(agencyName, token, LENTH_OF_AGENCY_NAME);

    token = strtok(NULL, ";");
    infractionId = (size_t)atoi(token);

    token = strtok(NULL, ";");
    fineAmount = (size_t)atoi(token);

    processTicket(city, plate, year, month, infractionId, fineAmount, agencyName);     
}