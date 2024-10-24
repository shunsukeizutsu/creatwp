#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <stdexcept>
#include <getopt.h>
#include <ssm.hpp>
#include "ssm-log.hpp"
#include "localizer.hpp"
#include <math.h>
#include "creatfig.hpp"

static int gShutOff = 0;
static void setSigInt(void);
static int printShortHelp(const char *programName);
static bool setOption(int aArgc, char *aArgv[]);
static void Terminate(void);
int countID(void);

static char path[256 / 2] = "/home/haselab15/TC2023_log";
static char log_name[256 / 2] = "localizer.log";
static char filename[265] = "../data/test1";
static unsigned int dT = 100; // 100ms
int number = 1;
int main(int aArgc, char **aArgv)
{
    PlotData PD(1);
    FILE *fp;
    fp = fopen("../data/test.dat", "w");
    if (!setOption(aArgc, aArgv))
        return EXIT_FAILURE;

    SSMLog<localizer, localizer_property> localizer;

    // Logファイルをオープン
    char LogName[256];
    sprintf(LogName, "%s/%s", path, log_name);
    if (!localizer.open(LogName))
    {
        fprintf(stderr, "Error! Cannot open %s\n", LogName);
        exit(EXIT_FAILURE);
    }
    bool flag_first_loop = true;
    double pre_x = 0;
    double pre_y = 0;
    try
    {
        setSigInt();

        printf("Start log2wp\n");
        fprintf(fp, "#Num. of WP\n");
        fprintf(fp, "#ID    WP_x[m]    WP_x[m]    WP_y[m]    dsm_type    map_name    stop_type    obst_avoid_flag    Velocity    Area_type    Search_Status\n");

        while (!gShutOff)
        {

            if (localizer.read())
            {
                //printf("%f %f\n", localizer.data().estPos[0], localizer.data().estPos[1]);
                if (flag_first_loop)
                {
                    flag_first_loop = false;
                    pre_x = localizer.data().estPos[0];
                    pre_y = localizer.data().estPos[1];
                    fprintf(fp, "1    %f    %f    0    Toyosumap2022/Toyosu2022_1.pcd    0    0    0.7    0    1\n", localizer.data().estPos[0], localizer.data().estPos[1]);
                    PD.SaveData2D(localizer.data().estPos[0], localizer.data().estPos[1]);
                }
                else
                {
                    double delta_x = localizer.data().estPos[0] - pre_x;
                    double delta_y = localizer.data().estPos[1] - pre_y;

                    if (localizer.data().estAngvel[2] > 0.1 || localizer.data().estAngvel[2] < -0.1)
                    {
                        if (localizer.data().estAngvel[2] > 0.2 || localizer.data().estAngvel[2] < -0.2)
                        {
                            if (sqrt(delta_x * delta_x + delta_y * delta_y) > 0.3)
                            {
                                int i = countID();
                                fprintf(fp, "%d    %f    %f    0    N    0    0    0.3    0    1\n", i, localizer.data().estPos[0], localizer.data().estPos[1]);
                                PD.SaveData2D(localizer.data().estPos[0], localizer.data().estPos[1]);
                                pre_x = localizer.data().estPos[0];
                                pre_y = localizer.data().estPos[1];
                            }
                        }
                        else
                        {
                            if (sqrt(delta_x * delta_x + delta_y * delta_y) > 1.5)
                            {
                                int i = countID();
                                fprintf(fp, "%d    %f    %f    0    N    0    0    0.5    0    1\n", i, localizer.data().estPos[0], localizer.data().estPos[1]);
                                PD.SaveData2D(localizer.data().estPos[0], localizer.data().estPos[1]);
                                pre_x = localizer.data().estPos[0];
                                pre_y = localizer.data().estPos[1];
                            }
                        }
                    }

                    else
                    {
                        if (sqrt(delta_x * delta_x + delta_y * delta_y) > 3.0)
                        {
                            int i = countID();
                            fprintf(fp, "%d    %f    %f    0    N    0    0    0.7    0    1\n", i, localizer.data().estPos[0], localizer.data().estPos[1]);
                            PD.SaveData2D(localizer.data().estPos[0], localizer.data().estPos[1]);
                            pre_x = localizer.data().estPos[0];
                            pre_y = localizer.data().estPos[1];
                        }
                    }
                    // printf("%f\n", localizer.data.estAngvel[2]);
                }
            }
            else
                break;
            //			usleep( dT * 1000 );
        }
        PD.PrintFig2D();
        PD.SaveFigure(filename);
    }
    catch (std::runtime_error const &error)
    {
        std::cout << error.what() << std::endl;
    }
    catch (...)
    {
        std::cout << "An unknown fatal error has occured. Aborting." << std::endl;
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
    fprintf(stderr, "\t$ %s -p %s\n", programName, path);
    fputs("OPTION\n", stderr);
    //	printf( "\t-t | --s_time   TIME     : Sampling time (default=%dms)\n", dT );
    printf("\t-f | --log_name FILE     : Set log file name (default=%s)\n", log_name);
    printf("\t-p | --path     PATH     : Set path for log file (default=%s)\n", path);
    return EXIT_SUCCESS;
}
static bool setOption(int aArgc, char *aArgv[])
{
    int opt, optIndex = 0;
    struct option longOpt[] = {
        {"s_time", 1, 0, 't'},
        {"log_name", 1, 0, 'f'},
        {"path", 1, 0, 'p'},
        {"help", 0, 0, 'h'},
        {0, 0, 0, 0}};

    while ((opt = getopt_long(aArgc, aArgv, "f:t:p:h", longOpt, &optIndex)) != -1)
    {
        switch (opt)
        {
        case 't':
            dT = atoi(optarg);
            break;
        case 'f':
            sprintf(log_name, "%s", optarg);
            break;
        case 'p':
            sprintf(path, "%s", optarg);
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
    printf("\nend\n");
}
int countID(void)
{
    number++;
    return number;
}
