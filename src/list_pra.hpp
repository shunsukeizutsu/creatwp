#include <iostream>
#include <string>
#include <list>
#include <stdio.h>




typedef struct Data
{
	int ID;
    double estPos[2];
    int dsm_type;
    char mapname[256];
    int stop_type;
    int obst_avoid_flag;
    double velocity;
    int Area_type;
    int Search_Status;

} wpdata;

static void Terminate(void);
wpdata ADDdata(void);


