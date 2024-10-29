#include <iostream>
#include <vector>
#include <limits>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <math.h>
#include <signal.h>
#include <stdexcept>
#include <linux/input.h>
#include <linux/joystick.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/termios.h>
#include <ypspur.h>
#include <ssm.hpp>
#include <joystick.hpp>
#include "localizer.hpp"

using namespace std;

typedef struct WayPoint
{
    // fprintf(fp, "#ID\tWP_x[m]\tWP_y[m]\tdsm_type\tmap_name\tstop_type\tobst_avoid_flag\tVelocity\tArea_type\tSearch_Status\n");

    int number;
    double x;
    double y;
    int stop_type;
    int avoid_flag;
    double velocity;
    bool Area_type;
    bool Search_Status;

}wp;

static int gShutOff = 0;
static void setSigInt(void);
static void printShortHelp(const char *programName);
static bool setOption(int aArgc, char *aArgv[]);
static void setupSSM(void);
static void Terminate(FILE *fp);
int CreatWP(vector<wp> wpdata, FILE *fp, int number, bool avoid, bool stop, double velocity, int area, double x, double y);
