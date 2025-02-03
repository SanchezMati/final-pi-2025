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
#define BLOCK 25
#define ALPHABET 'z' - 'a' + 1

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

void addTicketToMakeQuery2(cityADT city, const char* agencyName, const char* plate, int amount);

//Funciones de iteracion para la query1

//Setea el iterador al inicio de la lista
void toBegin(cityADT city);

//Devuelve 0 si no hay otro nodo en la lista 1 si hay
int hasNext(cityADT city);

//@parametros:

//@year: devuelve por punteros el anio de la infraccion
//@tickets: devuelve la cantiadad de infracciones 
//devuelve una copia del nombre de la infraccion 
char* next(cityADT city, int* year, int* tickets);

void toBeginQuery2(cityADT city);
int hasNextQuery2(cityADT city);
void nextQuery2(cityADT city, char* agencyName, char* topPlate, size_t* total);

#endif