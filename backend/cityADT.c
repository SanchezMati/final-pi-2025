#include "cityADT.h"

enum status {ERROR = 0, SUCCESS};

typedef struct plates
{
    char plate[PLATE_LENTH+1];
    int amount;
    struct plates* next;
}
tPlate;

typedef struct agency
{
    char name[AGENCY_LENTH+1];
    tPlate* first;
    struct agency* next;
}
tAgency;

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
    /*========================*/

    //Query2
    tAgency* firstAgency;
    tAgency* agencyIter;

};

//listERS de funciones ocultas
static void dinamic_strcpy(char** dest, const char* src);
static void freeQuery1(cityADT city);
static void freeQuery1Rec(tInfractionsByYear* list);
static void freeInfractions(char** v, int dim);
static tInfractionsByYear* addInfractionRec(tInfractionsByYear* list, char* name, int year);
static tPlate* addPlate(tPlate* list, char* plate, int fine);
static tAgency* addAgency(tAgency* list ,char* agency, char* plate, int fine);

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

void addTicketToMakeQuery1(cityADT city, int id, int year) //Esta funcion le podriamos agruegar mas cosas, ahora solo la hago para la query1, pero se podria adaptar
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

int addInfraction(cityADT city, int id, char* description) {
    if (city == NULL || description == NULL || id <= 0) {
        return ERROR;
    }

    errno = 0;  
    if(id > city->topID) {
        // Allocate array of pointers
        char** aux = realloc(city->infractionsName, sizeof(char*) * id);
        if(aux == NULL || errno == ENOMEM) {
            return ERROR;
        }
        
        // Initialize new pointers to NULL
        for(int i = city->topID + 1; i < id; i++) {
            aux[i] = NULL;
        }
        
        city->infractionsName = aux;    
        city->topID = id;
    }

    dinamic_strcpy(&(city->infractionsName[id-1]), description);
    if(city->infractionsName[id-1] == NULL || errno == ENOMEM) {
        return ERROR;
    }
    
    return SUCCESS;
}


void toBegin(cityADT city)
{
    city->iter = city->first;    
}

int hasNext(cityADT city)
{
    return city->iter != NULL;
}

char* next(cityADT city, int* year, int* tickets) {
    if(!hasNext(city)) {
        return NULL; 
    }

    *year = city->iter->year;
    *tickets = city->iter->total;
    
    char* name;
    dinamic_strcpy(&name, city->iter->name);
    if(name == NULL || errno == ENOMEM) 
    {
        return NULL;  
    }
    city->iter = city->iter->next;
    return name;
}

static void dinamic_strcpy(char** dest, const char* src)
{   
    errno = 0;
    int i;
    char* aux = NULL;
    for(i = 0; src[i] != '\0'; i++)
    {
        if(i%BLOCK == 0)
        {
            aux = realloc(aux, i+BLOCK);
            if(aux == NULL || errno == ENOMEM)
            {
                *dest = NULL;
            }
        }
        aux[i] = src[i];
    }

    aux = realloc(aux, i+1);
    aux[i] = '\0';

    *dest = aux;
    return ;
}

void addTicketToMakeQuery2(cityADT city, char* agency, char* plate, int fine)
{
    city->firstAgency = addAgency(city->firstAgency, agency, plate, fine);
}

static tAgency* addAgency(tAgency* list ,char* agency, char* plate, int fine)
{
    int c;
    if(list == NULL || (c = strcmp(list->name, agency)) > 0) //La agencia no existe
    {
        errno = 0;
        tAgency* aux = malloc(sizeof(tAgency));
        if(aux == NULL || errno == ENOMEM)
        {
            return list;
        }
        strcpy(aux->name, agency);

        aux->first = malloc(sizeof(tPlate));
        if(aux->first == NULL || errno == ENOMEM)
        {
            free(aux);
            return list;
        }
        strcpy(aux->first->plate, plate);
        aux->first->amount = fine;
        aux->first->next = NULL;

        aux->next = list;
        return aux;        
    }
    else if(c == 0) //La agencia ya esta creada. Agruego la patente
    {   
        list->first = addPlate(list->first, plate, fine);
        return list;
    }

    list->next = addAgency(list->next, agency, plate, fine);
    return list;
}

static tPlate* addPlate(tPlate* list, char* plate, int fine)
{
    tPlate* aux;
    tPlate* current = list;
    tPlate* prev = NULL;
    
    while (current != NULL) //Buscar si la patente ya existe
    {
        if (strcmp(current->plate, plate) == 0) //Caso 1: La patente existe.
        {
            current->amount += fine;
            if (prev && current->amount > prev->amount) 
            {
                prev->next = current->next;
                return addPlate(list, plate, current->amount);
            }
            return list;
        }
        prev = current;
        current = current->next;
    }
    
    aux = (tPlate*)malloc(sizeof(tPlate));
    if(aux == NULL || errno == ENOMEM)
    {
        return list;
    }

    strcpy(aux->plate, plate);
    aux->amount = fine;
    aux->next = NULL;
    
    if (!list || list->amount < fine || (list->amount == fine && strcmp(list->plate, plate) > 0)) 
    {
        aux->next = list;
        return aux;
    }
    
    current = list;
    while (current->next && (current->next->amount > fine || 
          (current->next->amount == fine && strcmp(current->next->plate, plate) < 0))) {
        current = current->next;
    }
    
    aux->next = current->next;
    current->next = aux;
    
    return list;
}

void toBeginQuery2(cityADT city)
{
    city->agencyIter = city->firstAgency;    
}

int hasNextQuery2(cityADT city)
{    
    return city->agencyIter != NULL;
}

void nextQuery2(cityADT city, char* agencyName, char* topPlate, int* total)
 {
    
    if (!hasNextQuery2(city)) 
    {
        return ;
    }
    
    strncpy(agencyName, city->agencyIter->name, AGENCY_LENTH);
    agencyName[AGENCY_LENTH] = '\0';
    
    strncpy(topPlate, city->agencyIter->first->plate, PLATE_LENTH);
    topPlate[PLATE_LENTH] = '\0';
    
    *total = city->agencyIter->first->amount;
    
    city->agencyIter = city->agencyIter->next;
}