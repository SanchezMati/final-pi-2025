#ifndef __QUERIES_H__
#define __QUERIES_H__

#include "cityADT.h"
#include <stdio.h>
#include <stdlib.h>

#define FILE_ERROR NULL

#define DELIMITER ";"

#define QUERY1 "query1.csv"
#define HEADER1 "infraction;year;tickets"

#define QUERY2 "query2.csv"
#define HEADER2 "agency;topPlate;topTotal"

#define QUERY3 "query3.csv"

#define QUERY4 "query4.csv"

void handleQueryError(cityADT city, int queryNum);
int makeQuery1(cityADT city);
int makeQuery2(cityADT city);

#endif
