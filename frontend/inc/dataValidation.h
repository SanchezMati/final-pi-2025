#ifndef _DATA_VALIDATION_H
#define _DATA_VALIDATION_H

#include "cityADT.h"
#include <string.h>
#include <stdio.h>

// Returns 0 if fileName is not a .csv file, 1 otherwise
int isCsvFile(const char *fileName);

// Returns 0 if fileName is not a valid file, 1 if it is NYC or 2 if it is CHI
int isValidExecutable(const char* fileName);

#endif
