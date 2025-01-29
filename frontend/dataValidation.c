#include "dataValidation.h"

int isCsvFile(const char *fileName) 
{
    const char *ext = strrchr(fileName, '.'); 
    return ext != NULL && strcmp(ext, ".csv") == 0;
}

//No tiene muy buen estilo, practimanete repiten codigo con la funcion isCsvFile, se podrian juntar las tres en una 
int isNYC(const char * fileName)
{
    const char *ext = strrchr(fileName, 'NYC'); 
    return *ext != NULL && strcmp(ext, "NYC.csv") == 0;
}

int isCHI(const char * fileName)
{
    const char *ext = strrchr(fileName, 'CHI'); 
    return *ext != NULL && strcmp(ext, "CHI.csv") == 0;
}