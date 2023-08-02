#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <stdexcept>
#include <getopt.h>
#include <iostream>
#include <ssm.hpp>
#include <math.h>
#include "localizer.hpp"
#include "creatfig.hpp"

static int gShutOff = 0;
static void setSigInt(void);
static void ctrlC(int aStatus);
static void setupSSM(void);
static void Terminate(void);
int countID(void);

static SSMApi<localizer, localizer_property> *LOCALIZER;

static unsigned int dT = 100;
int number = 1;
char filename[265] = "../data/test1";


int main(int aArgc, char *aArgv[])
{
    PlotData PD(1);
    FILE *fp;
    fp = fopen("../data/test.dat", "w");

    // if (!setOption(aArgc, aArgv))
    //    return EXIT_FAILURE;

    SSMApi<localizer, localizer_property> localizer_fs(SNAME_LOCALIZER, 0);
    LOCALIZER = &localizer_fs;

    bool update = false;
    SSM_tid update_id = -1;
    bool flag_first_loop = true;
    double pre_x = 0;
    double pre_y = 0;
    try
    {
        setupSSM();
        setSigInt();
        fprintf(fp, "#Num. of WP\n");
        fprintf(fp, "#ID    WP_x[m]    WP_x[m]    WP_y[m]    dsm_type    map_name    stop_type    obst_avoid_flag    Velocity    Area_type    Search_Status\n");
        while (!gShutOff)
        {
            update = false;
            // get latest data for INDEX_LOCALIZER
            if (update_id < getTID_top(localizer_fs.getSSMId()))
            {
                localizer_fs.readLast();
                update = true; // 最新情報を読み込む
                update_id = localizer_fs.timeId;
            }
            else
            {
                update = false;
            }

            if (update)
            {
                if (flag_first_loop)
                {
                    flag_first_loop = false;
                    pre_x = localizer_fs.data.estPos[0];
                    pre_y = localizer_fs.data.estPos[1];
                    fprintf(fp, "1    %f    %f    0    N    0    0    0.7    0    1\n", localizer_fs.data.estPos[0], localizer_fs.data.estPos[1]);
                    PD.SaveData2D(localizer_fs.data.estPos[0], localizer_fs.data.estPos[1]);
                }
                else
                {
                    double delta_x = localizer_fs.data.estPos[0] - pre_x;
                    double delta_y = localizer_fs.data.estPos[1] - pre_y;

                    if (localizer_fs.data.estAngvel[2] > 0.2 || localizer_fs.data.estAngvel[2] < -0.2)
                    {
                        if (localizer_fs.data.estAngvel[2] > 0.35 || localizer_fs.data.estAngvel[2] < -0.35)
                        {
                            if (sqrt(delta_x * delta_x + delta_y * delta_y) > 1.0)
                            {
                                int i = countID();
                                fprintf(fp, "%d    %f    %f    0    N    0    0    0.7    0    1\n", i, localizer_fs.data.estPos[0], localizer_fs.data.estPos[1]);
                                PD.SaveData2D(localizer_fs.data.estPos[0], localizer_fs.data.estPos[1]);
                                pre_x = localizer_fs.data.estPos[0];
                                pre_y = localizer_fs.data.estPos[1];
                            }
                        }
                        else
                        {
                            if (sqrt(delta_x * delta_x + delta_y * delta_y) > 2.0)
                            {
                                int i = countID();
                                fprintf(fp, "%d    %f    %f    0    N    0    0    0.7    0    1\n", i, localizer_fs.data.estPos[0], localizer_fs.data.estPos[1]);
                                PD.SaveData2D(localizer_fs.data.estPos[0], localizer_fs.data.estPos[1]);
                                pre_x = localizer_fs.data.estPos[0];
                                pre_y = localizer_fs.data.estPos[1];
                            }
                        }
                    }

                    else
                    {
                        if (sqrt(delta_x * delta_x + delta_y * delta_y) > 5.0)
                        {
                            int i = countID();
                            fprintf(fp, "%d    %f    %f    0    N    0    0    0.7    0    1\n", i, localizer_fs.data.estPos[0], localizer_fs.data.estPos[1]);
                            PD.SaveData2D(localizer_fs.data.estPos[0], localizer_fs.data.estPos[1]);
                            pre_x = localizer_fs.data.estPos[0];
                            pre_y = localizer_fs.data.estPos[1];
                        }
                    }
                    //printf("%f\n", localizer_fs.data.estAngvel[2]);
                }
                //PD.PrintFig2D();
            }
            else
            {
                usleepSSM(dT * 1000);
            }
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
static void setupSSM(void)
{
    std::cerr << "initializing ssm ... ";
    if (!initSSM())
        throw std::runtime_error("[\033[1m\033[31mERROR\033[30m\033[0m]:fail to initialize ssm.");
    else
        std::cerr << "OK.\n";

    // localizerを開く
    std::cerr << "open  localizer ... ";
    if (!LOCALIZER->open(SSM_READ))
    {
        throw std::runtime_error("[\033[1m\033[31mERROR\033[30m\033[0m]:fail to open spur_global on ssm.\n");
    }
    else
    {
        std::cerr << "OK.\n";
    }
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
static void Terminate(void)
{
    endSSM();
    printf("end\n");
}
int countID(void)
{
    number++;
    return number;
}