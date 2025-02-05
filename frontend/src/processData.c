#include "processData.h"

#define LENGTH_OF_AGENCY_NAME (35 + 1) //35 chars + '\0'
#define LENGTH_OF_PLATES (10 + 1)  //plates lenght
 
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
    printf("Header read: %s\n", line);
    
    if (fileType == NYC) 
    {
        printf("Processing NYC file\n");
        while (fgets(line, sizeof(line), file)) 
        {
           // printf("Reading line: %s", line);
            if (line[0] != '\0') {
                processNYCTicketLine(line, city);
            }
        }
    }
    else
    {
        printf("Processing CHI file\n");
        while (fgets(line, sizeof(line), file)) 
        {
            if (line[0] != '\0') {
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
    char plate[LENGTH_OF_PLATES] = {0};
    char agencyName[LENGTH_OF_AGENCY_NAME] = {0};
    int infractionId = 0, fineAmount = 0, year = 0, month = 0, day = 0;

    char lineCopy[SIZE_LINE_TICKETS];
    strncpy(lineCopy, line, SIZE_LINE_TICKETS - 1);
    lineCopy[SIZE_LINE_TICKETS - 1] = '\0';

    // Format for NYC: plate;issueDate;infractionId;fineAmount;issuingAgency
    char* token = strtok(lineCopy, ";"); // Get plate
    if (token == NULL) return;
    strncpy(plate, token, LENGTH_OF_PLATES - 1);
    plate[LENGTH_OF_PLATES - 1] = '\0';

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
    strncpy(agencyName, token, LENGTH_OF_AGENCY_NAME - 1);
    agencyName[LENGTH_OF_AGENCY_NAME - 1] = '\0';

    // Remove all spaces, CR or LF characters at the end of the agency name
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

    char plate[LENGTH_OF_PLATES] = {0};
    char agencyName[LENGTH_OF_AGENCY_NAME] = {0};
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
    strncpy(plate, token, LENGTH_OF_PLATES - 1);
    plate[LENGTH_OF_PLATES - 1] = '\0';

    token = strtok(NULL, ";");  // Get agency
    if (token == NULL) return;
    strncpy(agencyName, token, LENGTH_OF_AGENCY_NAME - 1);
    agencyName[LENGTH_OF_AGENCY_NAME - 1] = '\0';

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

bool isLeapYear(const int year){
    return (year % 4 == 0 && year % 100 != 0) || year % 400 == 0;
}

bool isValidDate(const int day, const int month, const int year) {
    if (day <= 0 || month <= 0 || month > 12 || year <= 0) {
        return false;
    }

    int daysInMonth[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    if (month == 2 && isLeapYear(year)) {
        return day <= 29;
    }
    
    return day <= daysInMonth[month - 1];
}


void processTicket(cityADT city, char plate[], int year, int day, int month, int id, int fineAmount, char agencyName[])
{
   // printf("\nProcessing ticket: plate=%s, agency=%s, amount=%d\n", plate, agencyName, fineAmount);
    if (
        city == NULL || plate == NULL || agencyName == NULL || 
        id <= 0 || fineAmount < 0 || !isValidDate(day, month, year)
    ) {
        printf("Invalid data in processTicket!\n");
        return;
    }
    
    addTicketToMakeQuery1(city, id, year);
    
    addTicketToMakeQuery2(city, agencyName, plate, fineAmount);
    
    addTicketToMakeQuery3(city, month, id);
}