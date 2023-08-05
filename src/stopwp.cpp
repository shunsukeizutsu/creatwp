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

    FILE *fp;
    fp = fopen("../data/test.dat", "w");

    // if (!setOption(aArgc, aArgv))
    //    return EXIT_FAILURE;

    SSMApi<localizer, localizer_property> localizer_fs(SNAME_LOCALIZER, 0);
    LOCALIZER = &localizer_fs;

    bool update = false;
    SSM_tid update_id = -1;
    bool flag_first_loop = true;
    int stopcount = 0;

    try
    {
        setupSSM();
        setSigInt();

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
                if(localizer_fs.data.estVel_xy > 0)
                {

                }
            }
            else
            {
                usleepSSM(dT * 1000);
            }
        }
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
