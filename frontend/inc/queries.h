#ifndef __QUERIES_H__
#define __QUERIES_H__

#include "cityADT.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define FILE_ERROR NULL
#define ERROR 0
#define SUCCESS 1

#define DELIMITER ";"

#define QUERY1 "query1.csv"
#define HEADER1 "infraction;year;tickets"

#define QUERY2 "query2.csv"
#define HEADER2 "agency;topPlate;topTotal"

#define QUERY3 "query3.csv"
#define HEADER3 "month;topInfraction"

#define QUERY4 "query4.csv"
#define HEADER4 "agency;maxDailyAvg;maxDailyDate;minDailyAvg;minDailyDate;spread"

void handleQueryError(cityADT city, int queryNum);
int makeQuery1(cityADT city);
int makeQuery2(cityADT city);
int makeQuery3(cityADT city);
int makeQuery4(cityADT city);

char* intToMonth(int number);

#endif
