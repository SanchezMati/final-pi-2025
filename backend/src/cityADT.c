#include "cityADT.h"

typedef struct plateNode {
    char plate[PLATE+1];        // Plate
    size_t total;               // Total sum of fine amounts for this plate
    struct plateNode* next;     
} tPlateNode;

typedef struct agencyNode {
    char name[AGENCY_NAME+1];   // Agency name
    char topPlate[PLATE+1];     // Plate with highest grossing for this agency
    size_t maxTotal;            // Total amount of topPlate fines
    
    tPlateNode* plates;         // List of plates and their total amounts
    
    struct agencyNode* next;   
} tAgencyNode;

typedef struct infractionsByYear
{
    char name[MAX_AMOUNT_OF_CHARACTER+1]; // Infraction name
    int year; // Year of the infraction
    size_t total; // Amount of that infraction issued in that year

    struct infractionsByYear* next;
}
tInfractionsByYear;

struct cityCDT
{
    //Query1 =================
    tInfractionsByYear* first;
    tInfractionsByYear* iter;
    char** infractionsName; // Vector with the name of the infractions 
    int topID;
    /*========================*/

    //Query2 =================
    tAgencyNode* firstAgency;
    tAgencyNode* agencyIter;
    /*========================*/

    //Query3 =================
    size_t* infractionsByMonth[MONTHS]; //Vector donde v[0][0] == cantidad de infracciones de id 1 en enero 
    /*========================*/
};

// Hidden function headers
static void dynamic_strcpy(char** dest, const char* src);
static void freeQuery1(cityADT city);
static void freeQuery1Rec(tInfractionsByYear* list);
static void freeInfractions(char** v, int dim);
static tInfractionsByYear* addInfractionRec(tInfractionsByYear* list, char* name, int year);
static void freeAgenciesRec(tAgencyNode* current); 
static void freePlatesRec(tPlateNode* current); 
static void freeQuery3(cityADT city);

void freeCity(cityADT city)
{
    freeQuery1(city);
    freeQuery3(city);
    freeAgenciesRec(city->firstAgency);
    free(city);
}

static void freeQuery3(cityADT city)
{
    for(int i = 0; i < MONTHS; i++)
    {
        free(city->infractionsByMonth[i]);
    }
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

static void freePlatesRec(tPlateNode* current) {
    if (current == NULL) {
        return;
    }
    freePlatesRec(current->next);
    free(current);
}

static void freeAgenciesRec(tAgencyNode* current) {
    if (current == NULL) {
        return;
    }
    freeAgenciesRec(current->next);
    freePlatesRec(current->plates);
    free(current);
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

    city->firstAgency = NULL;
    city->agencyIter = NULL;

    return city;
}

bool initInfractionsByMonth(cityADT city){
    for (int i = 0; i < MONTHS; i++){
        if ((city->infractionsByMonth[i] = calloc(city->topID, sizeof(size_t))) == NULL){
            return false;
        }
    }
    return true;
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
        for(int i = city->topID; i < id; i++) {
            aux[i] = NULL;
        }
        
        city->infractionsName = aux;    
        city->topID = id;
    }

    if (city->infractionsName[id - 1] == NULL){
        dynamic_strcpy(&(city->infractionsName[id-1]), description);
        if(city->infractionsName[id-1] == NULL || errno == ENOMEM) {
            return ERROR;
        }
    }
    
    return SUCCESS;
}


static tAgencyNode* findOrCreateAgencyRec(tAgencyNode* current, tAgencyNode** prev, const char* agencyName) {
    if (current == NULL || strcmp(current->name, agencyName) > 0) {
        return NULL;
    }
    
    if (strcmp(current->name, agencyName) == 0) {
        return current;
    }
    
    *prev = current;
    return findOrCreateAgencyRec(current->next, prev, agencyName);
}

static tAgencyNode* createAgency(const char* agencyName) {
    tAgencyNode* new = calloc(1, sizeof(tAgencyNode));
    if (new == NULL) {
        return NULL;
    }
    strncpy(new->name, agencyName, AGENCY_NAME);
    new->name[AGENCY_NAME] = '\0';
    new->maxTotal = 0;
    new->topPlate[0] = '\0';
    new->plates = NULL;
    new->next = NULL;
    return new;
}

static void updateTopPlate(tAgencyNode* agency, const char* plate, int amount) {
    if (amount > agency->maxTotal || 
        (amount == agency->maxTotal && strcmp(plate, agency->topPlate) < 0)) {
        agency->maxTotal = amount;
        strncpy(agency->topPlate, plate, PLATE);
        agency->topPlate[PLATE] = '\0';
    }
}

static tPlateNode* updateAgencyPlateRec(tPlateNode* current, tPlateNode** prev, 
                                      const char* plate, size_t amount, tAgencyNode* agency) {
    while (current != NULL && strcmp(current->plate, plate) < 0) {
        *prev = current;
        current = current->next;
    }

    if (current != NULL && strcmp(current->plate, plate) == 0) {
        current->total += amount;
        updateTopPlate(agency, plate, current->total);
        return NULL;
    }

    tPlateNode* new = malloc(sizeof(tPlateNode));
    if (new == NULL) {
        return NULL;
    }
    
    strncpy(new->plate, plate, PLATE);
    new->plate[PLATE] = '\0';
    new->total = amount;
    
    updateTopPlate(agency, plate, amount);
    
    if (*prev == NULL) {
        new->next = current;
        return new;  
    } else {
        new->next = current;
        (*prev)->next = new;
        return NULL;  
    }
}

void addTicketToMakeQuery2(cityADT city, const char* agencyName, const char* plate, int amount) 
{
    if (city == NULL || agencyName == NULL || plate == NULL) {
        return ;
    }
    
    tAgencyNode* prev = NULL;
    tAgencyNode* agency = findOrCreateAgencyRec(city->firstAgency, &prev, agencyName);
    
    if (agency == NULL && prev == NULL) {
        // Si es la primera agencia o va al principio
        city->firstAgency = createAgency(agencyName);
        agency = city->firstAgency;
    } else if (agency == NULL) {
        // Si va en el medio o al final
        agency = createAgency(agencyName);
        agency->next = prev->next;
        prev->next = agency;
    }
    
    if (agency != NULL) {
        tPlateNode** firstPlate = &(agency->plates);
        tPlateNode* prevPlate = NULL;
        tPlateNode* result = updateAgencyPlateRec(*firstPlate, &prevPlate, plate, amount, agency);
        
        if (result != NULL) {
            agency->plates = result;
        }
    }
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
    dynamic_strcpy(&name, city->iter->name);
    if(name == NULL || errno == ENOMEM) 
    {
        return NULL;  
    }
    city->iter = city->iter->next;
    return name;
}

static void dynamic_strcpy(char** dest, const char* src)
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
                if (*dest != NULL){
                    free(*dest);
                }
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

void toBeginQuery2(cityADT city) {
    city->agencyIter = city->firstAgency;    
}

int hasNextQuery2(cityADT city) {
    return city->agencyIter != NULL;
}

void nextQuery2(cityADT city, char* agencyName, char* topPlate, size_t* total) {
    if (!hasNextQuery2(city)) {
        return ;
    }
    
    strncpy(agencyName, city->agencyIter->name, AGENCY_NAME);
    agencyName[AGENCY_NAME] = '\0';
    
    strncpy(topPlate, city->agencyIter->topPlate, PLATE);
    topPlate[PLATE] = '\0';
    
    *total = city->agencyIter->maxTotal;
    
    city->agencyIter = city->agencyIter->next;
}

void addTicketToMakeQuery3(cityADT city, int month, int id)
{
    (city->infractionsByMonth[month-1][id-1])++;
}

char* getTopInfraction(cityADT city, int month)
{
    int dim = city->topID;
    int* topID = NULL; //Vector con el id de las infracciones mas populares en ese mes
    int topID_dim = 0;
    
    size_t current, max = 0;
    
    for(int i = 0; i < dim; i++)
    { 
        current = city->infractionsByMonth[month][i];
        if(current >= max)
        {
            int aux_dim = (current > max) ? 1:topID_dim+1;
            int * aux = realloc(topID, aux_dim*sizeof(int));
            if(aux == NULL)
            {
                continue;
            }
            topID = aux;
            topID_dim = aux_dim;
            topID[topID_dim-1] = i; 
            max = current;
        }
    }

    int id = topID[0];

    char * currentTopInfractionName = city->infractionsName[id];

    for(int i = 1; i < topID_dim; i++)
    {
        id = topID[i];
        if(strcmp(city->infractionsName[id], currentTopInfractionName) < 0)
        {
            currentTopInfractionName = city->infractionsName[id];
        }
    }

    free(topID);
    // Create a clone of the current top infraction name for data protection
    char* ans;
    dynamic_strcpy(&ans, currentTopInfractionName);
    return ans;
}