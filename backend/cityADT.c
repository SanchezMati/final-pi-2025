#include "cityADT.h"

#define MAX_AMOUNT_OF_CHARACTER 50
#define NULL_ID -1

enum status {ERROR = 0, SUCCES};

typedef struct infractionsByYear
{
    char name[MAX_AMOUNT_OF_CHARACTER+1]; //Nombre de la infraccion
    int year; //Anio de la 
    size_t total; //Cantidad de esa infraccion emitida en ese anio

    struct infractionsByYear* next;
}
tInfractionsByYear;

struct cityCDT
{
    //Query1 =================
    tInfractionsByYear* first;
    tInfractionsByYear* iter;

    char** infractionsName; //Vector con el nombre de las infracciones    
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
    freeInfractions(city->infractionsName, city->topID);
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

    city->first = NULL;
    city->iter = NULL;
    city->infractionsName = NULL;
    city->topID = NULL_ID;

    return city;
}

void addTicket(cityADT city, int id, int year) //Esta funcion le podriamos agruegar mas cosas, ahora solo la hago para la query1, pero se podria adaptar
{
    city->first = addInfractionRec(city->first, city->infractionsName[id-1], year);
}

static tInfractionsByYear* addInfractionRec(tInfractionsByYear* list, char* name, int year) //La funcion esta bien, esta check.
{
    int c;
    if(list == NULL || (c = strcmp(list->name, name)) > 0 || (c == 0 && list->year > year)) 
    {
        errno = 0;
        tInfractionsByYear* aux = malloc(sizeof(tInfractionsByYear));
        if(aux == NULL || errno == ENOMEM)
        {
            return list;
        }
        strcpy(aux->name, name);
        aux->year = year;
        aux->total = 1;
        aux->next = list;
        return aux;
    }
    if(c == 0 && (list->year == year)) //Son iguales, desempata el anio
    {
        list->total++;
        return list;
        
    }
    //Caso list->year < year -> sigo buscando ||
    //Caso c < 0 -> name va despues de list->name, sigo buscando su lugar
    list->next = addInfractionRec(list->next, name, year);
    return list;
}

int addInfraction(cityADT city, int id, char* description)
{
    errno = 0;  
    if(id > city->topID)
    {
        city->infractionsName = realloc(sizeof(char*), id);
        if(city->infractionsName == NULL || errno == ENOMEM)
        {
            return ERROR;
        }
        city->topID = id;
    }

    //Para hacerlo mas eficiente podriamos crear una funcion que realoque de a bloques
    int dim = strlen(description) + 1;
    city->infractionsName[id-1] = malloc(sizeof(char)*dim);
    //No se si esta validacion es correcta
    if(errno == ENOMEM)
    {
        return ERROR;
    }

    strcpy(city->infractionsName[id-1], description);
    return SUCCES;
}

void toBegin(cityADT city)
{
    city->iter = city->first;    
}

int hasNext(cityADT city)
{
    return city->iter != NULL;
}

char* next(cityADT city, int* year, int* tickets)
{
    errno = 0;
    if(!hasNext(city))
    {
        exit(EXIT_FAILURE);  
    }

    *year = city->iter->year;
    *tickets = city->iter->total;
    
    //Lo mismo, deberia haber una funcion que sea strCopy
    int dim = strlen(city->iter->name) + 1;
    char* name = malloc(sizeof(char)*dim);
    if(errno == ENOMEM)
    {
        return ERROR;
    }
    strcpy(name, city->iter->name);
    city->iter = city->iter->next;
    return name;
}