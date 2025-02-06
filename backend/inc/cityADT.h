#ifndef __CITY_ADT_H__
#define __CITY_ADT_H__

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define DAYS 31
#define MONTHS 12
#define MAX_AMOUNT_OF_CHARACTER 50
#define AGENCY_NAME 35
#define PLATE 10
#define NULL_ID 0
#define BLOCK 25
#define ALPHABET 'z' - 'a' + 1
#define MIN_MAX 2
#define DMY 3
#define DD 0
#define MM 1
#define YY 2
#define MIN 0
#define MAX 1

enum status {ERROR = 0, SUCCESS};

typedef struct cityCDT* cityADT;

// Creates a new ADT
cityADT newCity(void);

// With the topID already loaded, creates the violation matrix by month for Query 3
bool initInfractionsByMonth(cityADT city);

// Frees memory reserved by the ADT
void freeCity(cityADT city);

// Reads an infraction to identify it
int addInfraction(cityADT city, int id, char* description);

// Adds a ticket
void addTicketToMakeQuery1(cityADT city, int id, int year);

void addTicketToMakeQuery2(cityADT city, const char* agencyName, const char* plate, int amount);

void addTicketToMakeQuery3(cityADT city, int month, int id);

void addTicketToMakeQuery4(cityADT city, const char* agencyName, int fineAmount, int year, int month, int day);

// Iteration functions for query1

// Sets the iterator at the beginning of the list
void toBegin(cityADT city);

// Returns 0 if there is no other node in the list, 1 otherwise
int hasNext(cityADT city);

//@parametros:
//@year: devuelve por punteros el anio de la infraccion
//@tickets: devuelve la cantiadad de infracciones 
//devuelve una copia del nombre de la infraccion 
char* next(cityADT city, int* year, int* tickets);

void toBeginQuery2(cityADT city);
int hasNextQuery2(cityADT city);
void nextQuery2(cityADT city, char* agencyName, char* topPlate, size_t* total);

char* getTopInfraction(cityADT city, int month);

#endif