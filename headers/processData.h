#ifndef __PROCESS_DATA_H__
#define __PROCESS_DATA_H__
#include "cityADT.h"

// Parses a file containing infraction data and updates the city's infractions information
void infractionsReader(FILE * file, cityADT city);

// Parses a file containing ticket data and updates the city's ticket and agency-related information
void ticketsReader(FILE * file, cityADT city);

// Takes a line of input from a CSV file formatted for NYC parking tickets,
// parses the data and updates the corresponding cityADT structure with the information.
void processNYCTicketLine(const char * line, cityADT city);

// Takes a line of input from a CSV file formatted for Chicago parking tickets,
// parses the data and updates the corresponding cityADT structure with the information.
void processCHITicketLine(const char * line, cityADT city);

#endif