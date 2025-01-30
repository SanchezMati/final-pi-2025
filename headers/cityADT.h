#ifndef __CITY_ADT_H__
#define __CITY_ADT_H__

#include <errno.h>

typedef struct cityCDT* cityADT;

//TODO: Comentar lo que hace cada funcion

cityADT newCity(void);

void freeCity(cityADT city);
#endif