#include <stdio.h>
#include <stdlib.h>
#include "cityADT.h"

#define MAX_AMOUNT_OF_CHARACTER 50

typedef struct infractionsByYear
{
    char name[MAX_AMOUNT_OF_CHARACTER+1]; //Nombre de la infraccion
    int year; //Anio de la infraccion
    struct infractionsByYear* next;
}
tInfractionsByYear;

struct cityCDT
{
    //Query1 =================
    tInfractionsByYear* first;
    tInfractionsByYear* iter;
    char** infractions; //Vector con el nombre de las infracciones    
    int topID;
    //La idea es ir llenando en este vector con el nombre de las infracciones segun su indice. //EJ: la infraccion 48, BIKE LANE se guarda en infractions[47] = BIKE LANE;
    /*========================*/
};

//HEADERS de funciones ocultas
static void freeQuery1(cityADT city);
static void freeQuery1Rec(tInfractionsByYear* list);
static void freeInfractions(char** v, int dim);

void freeCity(cityADT city)
{
    freeQuery1(city);
    free(city);
}

static void freeQuery1(cityADT city)
{
    freeQuery1Rec(city->first);
    freeInfractions(city->infractions, city->topID);
}

static void freeQuery1Rec(tInfractionsByYear* list)
{
    if(list == NULL)
    {
        return ;
    }

    freeQuery1Rec(list->next);
    free(list);
}

static void freeInfractions(char** v, int dim)
{
    for(int i = 0; i < dim; i++)
    {
        /*No estoy seguro si habria que checkear que v[i] != NULL*/
        free(v[i]); 
    }
    free(v);
}

cityADT newCity(void)
{
    errno = 0;
    cityADT city = malloc(sizeof(struct cityCDT));
    if(city == NULL || errno == ENOMEM)
    {
        return NULL;
    }

    city->first = malloc(sizeof(tInfractionsByYear));
    if(city->first == NULL || errno == ENOMEM)
    {
        return NULL;
    }
    
    city->infractions = NULL;
    city->first->next = NULL;
    city->topID = 0;

    return city;
}

//PROBAR NEWCITY EN PYTUTOR