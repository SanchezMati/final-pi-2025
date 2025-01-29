#ifndef _DATA_VALIDATION_H
#define _DATA_VALIDATION_H

#include "cityADT.h"
#include <string.h>
#include <stdio.h>

//Devuelve 0 si fileName no es un archivo .csv, 1 en caso contrario
int isCsvFile(const char *fileName);

//Devuelve 1 si es el archivo de NYC, 0 si no
int isNYC(const char * filename);

//Devuelve 1 si es el archivo de CHI, 0 si no
int isCHI(const char * filename);

#endif