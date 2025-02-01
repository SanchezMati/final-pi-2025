#ifndef __CITY_ADT_H__
#define __CITY_ADT_H__

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_AMOUNT_OF_CHARACTER 50
#define AGENCY_NAME 35
#define PLATE 10
#define NULL_ID -1

enum status {ERROR = 0, SUCCESS};

typedef struct cityCDT* cityADT;

//Crea un nuevo TAD
cityADT newCity(void);

//Libeara la memoria reservada por el TAD
void freeCity(cityADT city);

//Lee una infraccion para poder identificarla
int addInfraction(cityADT city, int id, char* description);

//Agruega un ticket 
void addTicketToMakeQuery1(cityADT city, int id, int year);

//Funciones de iteracion para la query1
void toBegin(cityADT city);

int hasNext(cityADT city);

char* next(cityADT city, int* year, int* tickets);

#endif