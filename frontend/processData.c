#include "processData.h"
#include "cityADT.h"
#include <stdio.h>

#define SIZE_LINE_INFRACTIONS 32
#define SIZE_LINE_TICKETS 65

void infractionsReader(FILE * file, cityADT city) {
    char line[SIZE_LINE_INFRACTIONS] ;
    while (fgets(line, sizeof(line), file)) {
        char * infractionId = NULL ;
        char * description = NULL ;
        char * token = strtok(line, ";") ;
        
        if ( token != NULL ) {
            infractionId = token ;
        }
        token = strtok(NULL, ";") ;
        if ( token != NULL ) {
            description = token ;
        }
        addInfraction(city, infractionId, description) ;
    }
}

void ticketsReader(FILE * file, cityADT city) {
    char line[SIZE_LINE_TICKETS] ;
    if ( isNYC(file) ) {
        while (fgets(line, sizeof(line), file)) {
            processNYCTicketLine(line, city) ;
        }
    }
    else {
        while (fgets(line, sizeof(line), file)) {
            processCHITicketLine(line, city) ;
        }
    }
}

void processNYCTicketLine(const char * line, cityADT city) {
    char plate[LPLATE], agencyName[LAGENCY] ;
    size_t infractionId, fineAmount, year, month, day ;

    char * token = strtok((char *)line, ";") ;
    strncpy(plate, token, LPLATE) ;

    token = strtok(NULL, "-") ;
    year = atoi(token);
    token = strtok(NULL, "-");
    month = atoi(token) ;
    token = strtok(NULL, "-");
    day = atoi(token);

    token = strtok(NULL, ";");
    infractionId = (size_t)atoi(token) ;

    token = strtok(NULL, ";");
    fineAmount = (size_t)atoi(token);

    token = strtok(NULL, ";");
    strcpy(agencyName, token, LAGENCY);

    processTicket(city, plate, year, month, infractionId, fineAmount, agencyName);     
}

void processCHITicketLine(const char * line, cityADT city) {
    char plate[LPLATE], agencyName[LAGENCY] ;
    size_t infractionId, fineAmount, year, month, day ;

    char * token = strtok((char *)line, "-") ;
    year = atoi(token);
    token = strtok(NULL, "-") ;
    month = atoi(token);
    token = strtok(NULL, "-");
    day = atoi(token) ;

    token = strtok(NULL, ";");
    strcpy(plate, token, LPLATE);

    token = strtok(NULL, ";");
    strcpy(agencyName, token, LAGENCY);

    token = strtok(NULL, ";");
    infractionId = (size_t)atoi(token) ;

    token = strtok(NULL, ";");
    fineAmount = (size_t)atoi(token);

    processTicket(city, plate, year, month, infractionId, fineAmount, agencyName);     
}