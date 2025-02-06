#include "dataValidation.h"

enum fileType {INVALID = 0, NYC, CHI};

int isCsvFile(const char *fileName) 
{
    const char* ext = strrchr(fileName, '.'); 
    return ext != NULL && strcmp(ext, ".csv") == 0;
}

static int isNYC(const char * fileName)
{
    const char* ext = strstr(fileName, "NYC"); 
    return ext != NULL;
}

static int isCHI(const char * fileName)
{
    const char* ext = strstr(fileName, "CHI"); 
    return ext != NULL;
}

int isValidExecutable(const char* fileName)
{
    if(isNYC(fileName))
    {
        return NYC;
    }
    else if(isCHI(fileName))
    {
        return CHI;
    }

    return INVALID;
}