#include <stdio.h>

#define ERROR -1

#define DELIMITER ";"

#define QUERY1 "query1.csv"

#define QUERY2 "query2.csv"

#define QUERY3 "query3.csv"

#define QUERY4 "query4.csv"

static FILE* openFile(const char* query, const char* header)
{
    FILE* file = fopen(query, "w+");
    if(file == NULL) 
    {
        fclose(file);
        fprintf(stderr, "Error al abrir el archivo\n");
        return ERROR;
    }
    fprintf(file, "%s\n", header);

    return file;
}