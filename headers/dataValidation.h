#ifndef _DATA_VALIDATION_H
#define _DATA_VALIDATION_H

#include "cityADT.h"
#include <string.h>
#include <stdio.h>

//Devuelve 0 si fileName no es un archivo .csv, 1 en caso contrario
int isCsvFile(const char *fileName);

/*Devuelve 0 si fileName no es un archivo valido
  1 si es NYC o 2 si es CHI*/
int isValidExecutable(const char* fileName);

#endif