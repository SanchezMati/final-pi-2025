#include "processData.h"

#define LENTH_OF_AGENCY_NAME (35 + 1) //35 chars + '\0'
#define LENTH_OF_PLATES (10 + 1)  //plates lenght
 
#define SIZE_LINE_INFRACTIONS 32
#define SIZE_LINE_TICKETS 65

enum fileType {INVALID = 0, NYC, CHI};

void infractionsReader(FILE * file, cityADT city) {   
    char line[SIZE_LINE_INFRACTIONS];
    fgets(line, sizeof(line), file);
    
    while (fgets(line, sizeof(line), file)) {
        char * infractionId = NULL;
        char * description = NULL;
        char * token = strtok(line, ";");
        
        if (token != NULL) {
            infractionId = token;
            token = strtok(NULL, "\n"); 
            if (token != NULL) {
                description = token;
                
                char *newline = strchr(description, '\n');
                if (newline) *newline = '\0';
                
                int id = atoi(infractionId);
                if (id > 0) {  
                    addInfraction(city, id, description);
                }
            }
        }
    }
}

void ticketsReader(FILE * file, cityADT city, int fileType) 
{

    char line[SIZE_LINE_TICKETS];
    printf("Starting ticketsReader with fileType=%d\n", fileType);
    
    fgets(line, sizeof(line), file);
    printf("Header read: %s", line);
    
    if (fileType == NYC) 
    {
        printf("Processing NYC file\n");
        while (fgets(line, sizeof(line), file)) 
        {
           // printf("Reading line: %s", line);
            if (strlen(line) > 0) {
                processNYCTicketLine(line, city);
            }
        }
    }
    else
    {
        while (fgets(line, sizeof(line), file)) 
        {
            if (strlen(line) > 0) {
                processCHITicketLine(line, city);
            }
        }
    }
}


void processNYCTicketLine(const char * line, cityADT city) 
{
    if (line == NULL || city == NULL) {
        return;
    }
    char plate[LENTH_OF_PLATES] = {0};
    char agencyName[LENTH_OF_AGENCY_NAME] = {0};
    int infractionId = 0, fineAmount = 0, year = 0, month = 0, day = 0;

    char lineCopy[SIZE_LINE_TICKETS];
    strncpy(lineCopy, line, SIZE_LINE_TICKETS - 1);
    lineCopy[SIZE_LINE_TICKETS - 1] = '\0';

    // Format for NYC: plate;issueDate;infractionId;fineAmount;issuingAgency
    char* token = strtok(lineCopy, ";"); // Get plate
    if (token == NULL) return;
    strncpy(plate, token, LENTH_OF_PLATES - 1);
    plate[LENTH_OF_PLATES - 1] = '\0';

    token = strtok(NULL, "-"); // Get year
    if (token == NULL) return;
    year = atoi(token);

    token = strtok(NULL, "-"); // Get month
    if (token == NULL) return;
    month = atoi(token);

    token = strtok(NULL, ";"); // Get day
    if (token == NULL) return;
    day = atoi(token);

    token = strtok(NULL, ";"); // Get infraction ID
    if (token == NULL) return;
    infractionId = atoi(token);

    token = strtok(NULL, ";"); // Get fine amount
    if (token == NULL) return;
    fineAmount = atoi(token);

    token = strtok(NULL, "\n"); // Get agency name
    if (token == NULL) return;
    strncpy(agencyName, token, LENTH_OF_AGENCY_NAME - 1);
    agencyName[LENTH_OF_AGENCY_NAME - 1] = '\0';

    char* p = agencyName + strlen(agencyName) - 1;
    while (p >= agencyName && (*p == ' ' || *p == '\n' || *p == '\r')) {
        *p = '\0';
        p--;
    }

    if (year > 0 && month > 0 && day > 0 && infractionId > 0) {
        processTicket(city, plate, year, day, month, infractionId, fineAmount, agencyName);
    }
}


void processCHITicketLine(const char * line, cityADT city)
{
    if (line == NULL || city == NULL) {
        return;
    }

    char plate[LENTH_OF_PLATES] = {0};
    char agencyName[LENTH_OF_AGENCY_NAME] = {0};
    int infractionId = 0, fineAmount = 0, year = 0, month = 0, day = 0;

    char lineCopy[SIZE_LINE_TICKETS];
    strncpy(lineCopy, line, SIZE_LINE_TICKETS - 1);
    lineCopy[SIZE_LINE_TICKETS - 1] = '\0';

    // Format for CHI: issueDate;plateRedacted;unitDescription;infractionCode;fineLevel1Amount
    char* token = strtok(lineCopy, "-");  // Get year
    if (token == NULL) return;
    year = atoi(token);

    token = strtok(NULL, "-");  // Get month
    if (token == NULL) return;
    month = atoi(token);

    token = strtok(NULL, ";");  // Get day
    if (token == NULL) return;
    day = atoi(token);

    token = strtok(NULL, ";");  // Get plate
    if (token == NULL) return;
    strncpy(plate, token, LENTH_OF_PLATES - 1);
    plate[LENTH_OF_PLATES - 1] = '\0';

    token = strtok(NULL, ";");  // Get agency
    if (token == NULL) return;
    strncpy(agencyName, token, LENTH_OF_AGENCY_NAME - 1);
    agencyName[LENTH_OF_AGENCY_NAME - 1] = '\0';

    token = strtok(NULL, ";");  // Get infraction code
    if (token == NULL) return;
    infractionId = atoi(token);

    token = strtok(NULL, "\n");  // Get amount
    if (token == NULL) return;
    fineAmount = atoi(token);

    if (year > 0 && month > 0 && day > 0 && infractionId > 0) {
        processTicket(city, plate, year, day, month, infractionId, fineAmount, agencyName);
    }
}
 

void processTicket(cityADT city, char plate[], int year, int day, int month, int id, int fineAmount, char agencyName[])
{
   // printf("\nProcessing ticket: plate=%s, agency=%s, amount=%d\n", plate, agencyName, fineAmount);
    if (city == NULL || plate == NULL || agencyName == NULL || 
        id <= 0 || year <= 0 || month <= 0 || month > 12 || 
        day <= 0 || day > 31 || fineAmount < 0) {
        printf("Invalid data in processTicket!\n");
        return;
    }
    
    
    addTicketToMakeQuery1(city, id, year);
    //TODO: Cambiar nombre a addTicketToMakeQuery2
    addTicket(city, agencyName, plate, fineAmount);
   // printf("Ticket processed successfully\n");
}