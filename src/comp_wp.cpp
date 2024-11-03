#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <getopt.h>
#include <signal.h>
#include <stdexcept>
#include <unistd.h>
#include <string.h>
#include <vector>
#include "creatfig.hpp"

typedef struct waypoint
{
    int number;
    double x;
    double y;
    bool dsm_type;
    char map_name[32];
    int stop_type;
    bool obst_avoid_flag;
    double Velocity;
    bool Area_type;
    bool Search_Status;
} wp;

static int gShutOff = 0;
static int dT = 100;

static void setSigInt(void);
static bool setOption(int aArgc, char *aArgv[]);
static void ctrlC(int aStatus);
static void Terminate(void);

static char filename[128] = "/home/haselab17/TC2024-master/param/2024/revice_izutsu_v7.dat";
static char filename2[128] = "/home/haselab17/TC2024-master/param/wp2023/wp_2023_v9_rtk1r.dat";


int main(int aArgc, char *aArgv[])
{
    if (!setOption(aArgc, aArgv))
        return EXIT_FAILURE;

    PlotData PD(1);

    FILE *fp;
    FILE *fp2;
    fp = fopen(filename, "r");
    if (fp == NULL)
    {
        fprintf(stderr, "ファイルを開けませんでした。\n");
        return EXIT_FAILURE;
    }
    fp2 = fopen(filename2, "r");
    if (fp2 == NULL)
    {
        fprintf(stderr,"waypointファイルを開くことができない\n");
        return EXIT_FAILURE;
    }

    char buffer[1024];
    std::vector<wp> wpdata;
    std::vector<wp> wpdata2;

    try
    {
        while (fgets(buffer, sizeof(buffer), fp) != NULL)
        {
            wp data;
            // Skip lines that start with '#'
            if (buffer[0] == '#')
                continue;

            // Tokenize the line
            char *token = strtok(buffer, ", \t\n");
            if (token == NULL)
                continue;
            data.number = atoi(token);

            token = strtok(NULL, ", \t\n");
            if (token == NULL)
                continue;
            data.x = atof(token);

            token = strtok(NULL, ", \t\n");
            if (token == NULL)
                continue;
            data.y = atof(token);

            token = strtok(NULL, ", \t\n");
            if (token == NULL)
                continue;
            data.dsm_type = atoi(token);

            token = strtok(NULL, ", \t\n");
            if (token == NULL)
                continue;
            strncpy(data.map_name, token, sizeof(data.map_name) - 1);
            data.map_name[sizeof(data.map_name) - 1] = '\0'; // Ensure null termination

            token = strtok(NULL, ", \t\n");
            if (token == NULL)
                continue;
            data.stop_type = atoi(token);

            token = strtok(NULL, ", \t\n");
            if (token == NULL)
                continue;
            data.obst_avoid_flag = atoi(token);

            token = strtok(NULL, ", \t\n");
            if (token == NULL)
                continue;
            data.Velocity = atof(token);

            token = strtok(NULL, ", \t\n");
            if (token == NULL)
                continue;
            data.Area_type = atoi(token);

            token = strtok(NULL, ", \t\n");
            if (token == NULL)
                continue;
            data.Search_Status = atoi(token);

            PD.SaveData2Dx3_1(data.x, data.y);

            // Add parsed data to vector
            wpdata.push_back(data);

            // Output parsed values for verification
            // printf("Parsed values:\n");
            // printf("Number: %d, X: %.2f, Y: %.2f, DSM Type: %d, Map Name: %s, Stop Type: %d, Obstacle Avoid Flag: %d, Velocity: %.2f, Area Type: %d, Search Status: %d\n",
            //       data.number, data.x, data.y, data.dsm_type, data.map_name, data.stop_type, data.obst_avoid_flag, data.Velocity, data.Area_type, data.Search_Status);
        }
        while (fgets(buffer, sizeof(buffer), fp2) != NULL)
        {
            wp data2;
            // Skip lines that start with '#'
            if (buffer[0] == '#')
                continue;

            // Tokenize the line
            char *token = strtok(buffer, ", \t\n");
            if (token == NULL)
                continue;
            data2.number = atoi(token);

            token = strtok(NULL, ", \t\n");
            if (token == NULL)
                continue;
            data2.x = atof(token);

            token = strtok(NULL, ", \t\n");
            if (token == NULL)
                continue;
            data2.y = atof(token);

            token = strtok(NULL, ", \t\n");
            if (token == NULL)
                continue;
            data2.dsm_type = atoi(token);

            token = strtok(NULL, ", \t\n");
            if (token == NULL)
                continue;
            strncpy(data2.map_name, token, sizeof(data2.map_name) - 1);
            data2.map_name[sizeof(data2.map_name) - 1] = '\0'; // Ensure null termination

            token = strtok(NULL, ", \t\n");
            if (token == NULL)
                continue;
            data2.stop_type = atoi(token);

            token = strtok(NULL, ", \t\n");
            if (token == NULL)
                continue;
            data2.obst_avoid_flag = atoi(token);

            token = strtok(NULL, ", \t\n");
            if (token == NULL)
                continue;
            data2.Velocity = atof(token);

            token = strtok(NULL, ", \t\n");
            if (token == NULL)
                continue;
            data2.Area_type = atoi(token);

            token = strtok(NULL, ", \t\n");
            if (token == NULL)
                continue;
            data2.Search_Status = atoi(token);

            PD.SaveData2Dx3_2(data2.x, data2.y);

            // Add parsed data to vector
            wpdata2.push_back(data2);

            // Output parsed values for verification
            // printf("Parsed values:\n");
            // printf("Number: %d, X: %.2f, Y: %.2f, DSM Type: %d, Map Name: %s, Stop Type: %d, Obstacle Avoid Flag: %d, Velocity: %.2f, Area Type: %d, Search Status: %d\n",
            //      data2.number, data2.x, data2.y, data.dsm_type, data.map_name, data.stop_type, data.obst_avoid_flag, data.Velocity, data.Area_type, data.Search_Status);
        }
    }
    catch (std::runtime_error const &error)
    {
        std::cout << error.what() << std::endl;
    }
    catch (...)
    {
        std::cout << "An unknown fatal error has occurred. Aborting." << std::endl;
    }
    fclose(fp2);
    fclose(fp);
    PD.PrintFig2Dx3();
    while (!gShutOff)
    {
        usleep(dT * 1000);
    }
    Terminate();
    return EXIT_SUCCESS;
}

static void ctrlC(int aStatus)
{
    signal(SIGINT, NULL);
    gShutOff = 1;
}

static void setSigInt()
{
    struct sigaction sig;
    memset(&sig, 0, sizeof(sig));
    sig.sa_handler = ctrlC;
    sigaction(SIGINT, &sig, NULL);
}
static int printShortHelp(const char *programName)
{
    fputs("HELP\n", stderr);
    fprintf(stderr, "\t$ %s -p %s\n", programName, filename);
    fputs("OPTION\n", stderr);
    //	printf( "\t-t | --s_time   TIME     : Sampling time (default=%dms)\n", dT );
    printf("\t-p | --path     PATH     : Set path for log file (default=%s)\n", filename);
    printf("\t-s | --savefile SAVE     : Set path for log file \n");
    return EXIT_SUCCESS;
}
static bool setOption(int aArgc, char *aArgv[])
{
    int opt, optIndex = 0;
    struct option longOpt[] = {
        {"filename", 1, 0, 'p'},
        {"help", 0, 0, 'h'},
        {0, 0, 0, 0}};

    while ((opt = getopt_long(aArgc, aArgv, "p:h", longOpt, &optIndex)) != -1)
    {
        switch (opt)
        {
        case 'p':
            sprintf(filename, "%s", optarg);
            break;
        case 'h':
            printShortHelp(aArgv[0]);
            return false;
            break;
        default:
            fprintf(stderr, "help : %s -h\n", aArgv[0]);
            return false;
            break;
        }
    }
    return true;
}

static void Terminate(void)
{
    printf("end\n");
}
