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

typedef struct query1CDT
{  
    tInfractionsByYear* first;
    tInfractionsByYear* iter;
    char* infractions[MAX_AMOUNT_OF_CHARACTER+1]; //La idea es ir llenando en este vector con el nombre de las infracciones segun su indice. //EJ: la infraccion 48, BIKE LANE se guarda en infractions[47] = BIKE LANE;
}
query1CDT;

// typedef struct query2CDT
// {
    
// }
// query2CDT;

// typedef struct query3CDT
// {
    
// }
// query3CDT;

// typedef struct query4CDT
// {
    
// }
// query4CDT;

struct cityCDT
{
    query1CDT query1;
    // query2CDT query2;
    // query3CDT query3;
    // query4CDT query4;
};


