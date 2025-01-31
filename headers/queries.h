#ifndef __QUERIES_H__
#define __QUERIES_H__

#include "cityADT.h"
#include <stdio.h>
#include <stdlib.h>

#define FILE_ERROR NULL
#define ERROR 0
#define SUCCESS 1

#define DELIMITER ";"

#define QUERY1 "query1.csv"

#define QUERY2 "query2.csv"

#define QUERY3 "query3.csv"

#define QUERY4 "query4.csv"

void handleQueryError(cityADT city, int queryNum);
int makeQuery1(cityADT city);

#endif