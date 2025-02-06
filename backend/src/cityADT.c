#include "cityADT.h"

typedef struct amountDay{
    int totalAmount;
    int numFines;
}tAmountDay;

typedef struct year{
    int year;
    tAmountDay dateMtx[MONTHS][DAYS];
    struct year * next;
}tYear;

typedef struct agencyDaily{
    char name[AGENCY_NAME+1];
    float minAvg;
    float maxAvg;
    int date[MIN_MAX][DMY];
    
    tYear * firstYear;

    struct agencyDaily * next;
} tAgencyDaily;

typedef struct plateNode {
    char plate[PLATE+1];        
    size_t total;               // Total sum of fine amounts for this plate
    struct plateNode* next;     
} tPlateNode;

typedef struct agencyNode {
    char name[AGENCY_NAME+1];   
    char topPlate[PLATE+1];     // Plate with highest grossing for this agency
    size_t maxTotal;            // Total amount of topPlate fines
    
    tPlateNode * plates;         // List of plates and their total amounts
    
    struct agencyNode * next;   
} tAgencyNode;

typedef struct infractionsByYear
{
    char name[MAX_AMOUNT_OF_CHARACTER+1]; // Infraction name
    int year; // Year of the infraction
    size_t total; // Amount of that infraction issued in that year

    struct infractionsByYear * next;
} tInfractionsByYear;

struct cityCDT
{
    //Query1 =================
    tInfractionsByYear * first; 
    tInfractionsByYear * iter;
    char ** infractionsName; // Vector with the name of the infractions 
    int topID;
    /*========================*/

    //Query2 =================
    tAgencyNode* firstAgency;
    tAgencyNode* agencyIter;
    /*========================*/

    //Query3 =================
    size_t * infractionsByMonth[MONTHS]; //Vector donde v[0][0] == cantidad de infracciones de id 1 en enero 
    /*========================*/

    //Query4 =================
    tAgencyDaily * firstAgencyDaily;
    tAgencyDaily * agencyDailyIter;
    /*========================*/
};

// Hidden function headers
static void dynamic_strcpy(char** dest, const char* src);
static void freeQuery1(cityADT city);
static void freeQuery1Rec(tInfractionsByYear* list);
static void freeInfractions(char** v, int dim);
static tInfractionsByYear* addInfractionRec(tInfractionsByYear* list, char* name, int year);
static void freeQuery2(tAgencyNode* current); 
static void freePlatesRec(tPlateNode* current); 
static void freeQuery3(cityADT city);
static tAgencyDaily* addAgencyDaily(tAgencyDaily* list, char* name, int fineAmount, int year, int month, int day);
static tYear* addYear(tYear* list, int year, int month, int day, int fineAmount);
static void addAmount(tYear * year, int amount, int month, int day);
static void getMinMaxAvg(tAgencyDaily * agency, float * min, float * max);
static void getDateMinMax(tAgencyDaily * agency, char * maxDailyDate, char * minDailyDate);
static void avgData(tAgencyDaily * agency, tYear * year);
static void updateData(tAgencyDaily * agency, float avg, int year, int month, int day);
static void freeQuery4(tAgencyDaily * agency);
static void freeQuery4YearsRec(tYear * year);

void freeCity(cityADT city)
{
    freeQuery1(city);
    freeQuery2(city->firstAgency);
    freeQuery3(city);
    freeQuery4(city -> firstAgencyDaily);
    free(city);
}

static void freeQuery4(tAgencyDaily * agency){
    if(agency == NULL){
        return;
    }
    freeQuery4YearsRec(agency -> firstYear);
    freeQuery4(agency -> next);
    free(agency);
}

static void freeQuery4YearsRec(tYear * year){
    if(year == NULL){
        return;
    }
    freeQuery4YearsRec(year -> next);
    free(year);
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

static void freeQuery2(tAgencyNode* current) {
    if (current == NULL) {
        return;
    }
    freeQuery2(current->next);
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

    city->firstAgencyDaily = NULL;

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

void addTicketToMakeQuery2(cityADT city, const char* agencyName, const char* plate, int amount) 
{
    if (city == NULL || agencyName == NULL || plate == NULL || amount < 0) {
        return;
    }
    
    tAgencyNode* current = city->firstAgency;
    tAgencyNode* prev = NULL;
    
    while(current != NULL && strcmp(current->name, agencyName) < 0) {
        prev = current;
        current = current->next;
    }
    
    if(current == NULL || strcmp(current->name, agencyName) != 0) {
        tAgencyNode* new = calloc(1, sizeof(tAgencyNode));
        if(new == NULL) {
            return;
        }
        
        strncpy(new->name, agencyName, AGENCY_NAME);
        new->maxTotal = amount;
        strncpy(new->topPlate, plate, PLATE);
        
        if(prev == NULL) {
            new->next = city->firstAgency;
            city->firstAgency = new;
        } else {
            new->next = current;
            prev->next = new;
        }
        current = new;
    } else {
        if(amount > current->maxTotal || 
           (amount == current->maxTotal && strcmp(plate, current->topPlate) < 0)) {
            current->maxTotal = amount;
            strncpy(current->topPlate, plate, PLATE);
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

char * next(cityADT city, int* year, int* tickets) {
    if(!hasNext(city)) {
        return NULL; 
    }

    *year = city->iter->year;
    *tickets = city->iter->total;
    
    char * name;
    dynamic_strcpy(&name, city->iter->name);
    if(name == NULL || errno == ENOMEM) 
    {
        return NULL;  
    }
    city->iter = city->iter->next;
    return name;
}

static void dynamic_strcpy(char ** dest, const char* src)
{   
    errno = 0;
    int i;
    char * aux = NULL;
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

char * getTopInfraction(cityADT city, int month)
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
    char * ans;
    dynamic_strcpy(&ans, currentTopInfractionName);
    return ans;
}

void addTicketToMakeQuery4(cityADT city, char* agencyName, int fineAmount, int year, int month, int day)
{   
    city->firstAgencyDaily = addAgencyDaily(city->firstAgencyDaily, agencyName, fineAmount, year, month, day);
}

static tAgencyDaily* addAgencyDaily(tAgencyDaily* list, char* name, int fineAmount, int year, int month, int day)
{
    errno = 0;
    int c;
    if(list == NULL || (c = strcmp(list->name, name)) > 0)
    {
        // Create a new agency
        tAgencyDaily* aux = calloc(1, sizeof(tAgencyDaily));
        if(aux == NULL || errno == ENOMEM)
        {
            return list;
        }

        // Set its internal data
        strcpy(aux->name, name);
        aux->firstYear = addYear(aux->firstYear, year, month, day, fineAmount);

        // Put the new agency into the list at the corresponding position
        aux->next = list;
        return aux;
    }
    else if(c == 0)
    {
        list->firstYear = addYear(list->firstYear, year, month, day, fineAmount);
        return list;
    }

    list->next = addAgencyDaily(list->next, name, fineAmount, year, month, day);
    return list;
}

static tYear* addYear(tYear* list, int year, int month, int day, int fineAmount)
{
    if(list == NULL || year > list->year)
    {
        tYear* aux = calloc(1, sizeof(tYear));
        if(aux == NULL)
        {
            return list;
        }
        aux->year = year;
        addAmount(aux, fineAmount, month, day);
        aux->next = list;
        return aux;
    }
    else if(year == list->year)
    {
        addAmount(list, fineAmount, month, day);
        return list;
    }
    
    list->next = addYear(list->next, year, month, day, fineAmount);
    return list;
}

static void addAmount(tYear * year, int amount, int month, int day){
    tAmountDay * amountDay = &(year -> dateMtx[month - 1][day - 1]);
    amountDay->totalAmount += amount;
    (amountDay->numFines)++;
}

static void getMinMaxAvg(tAgencyDaily * agency, float * min, float * max){
    * min = agency -> minAvg;
    * max = agency -> maxAvg;
}

static void getDateMinMax(tAgencyDaily * agency, char * maxDailyDate, char * minDailyDate){
    int * minDate = agency -> date[MIN] + 1;
    int * maxDate = agency -> date[MAX] + 1;
    snprintf(maxDailyDate, MAX_DATE_LENGTH, "%d/%d/%d", maxDate[DD], maxDate[MM], maxDate[YY]);
    snprintf(minDailyDate, MAX_DATE_LENGTH, "%d/%d/%d", minDate[DD], minDate[MM], minDate[YY]);
}

static void avgData(tAgencyDaily * agency, tYear * year){
    if(year == NULL){
        return;
    }
    for(int i = 0; i < MONTHS; i++){
        for(int j = 0; j < DAYS; j++){
            int c = year -> dateMtx[i][j].totalAmount;
            int d = year -> dateMtx[i][j].numFines;
            if(c != 0 && d != 0){
                float avg = ((float) c) / d;
                updateData(agency, avg, year -> year, i, j);
            }
        }
    }
    avgData(agency, year -> next);
}

static void updateData(tAgencyDaily * agency, float avg, int year, int month, int day){
    if(agency -> minAvg == 0 || agency -> minAvg > avg){
        agency -> minAvg = avg;
        agency -> date[MIN][DD] = day;
        agency -> date[MIN][MM] = month;
        agency -> date[MIN][YY] = year;
    }
    if(agency -> maxAvg < avg){
        agency -> maxAvg = avg;
        agency -> date[MAX][DD] = day;
        agency -> date[MAX][MM] = month;
        agency -> date[MAX][YY] = year;
    }
    return;       
}

void toBeginQuery4(cityADT city)
{
    city->agencyDailyIter = city->firstAgencyDaily;
}

int hasNextQuery4(cityADT city)
{
    return city->agencyDailyIter != NULL;
}

char * nextQuery4(cityADT city, float * minDailyAvg, float * maxDailyAvg, char * maxDailyDate, char * minDailyDate)
{
    if(!hasNextQuery4(city))
    {
        return NULL;
    }
    avgData(city -> agencyDailyIter, city -> agencyDailyIter -> firstYear);
    getMinMaxAvg(city -> agencyDailyIter, minDailyAvg, maxDailyAvg);
    getDateMinMax(city -> agencyDailyIter, maxDailyDate, minDailyDate);

    char * name;

    dynamic_strcpy(&name, city->agencyDailyIter->name);
    city->agencyDailyIter = city->agencyDailyIter->next;

    return name;
}
