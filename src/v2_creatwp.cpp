
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <stdexcept>
#include <getopt.h>
#include <math.h>

#include <iostream>
#include <vector>
#include <limits>
#include <fstream>

#include <linux/input.h>
#include <linux/joystick.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/termios.h>
#include <ypspur.h>
#include <ssm.hpp>
#include <joystick.hpp>

static int gShutOff = 0;
static void setSigInt(void);
static void printShortHelp(const char *programName);
static bool setOption(int aArgc, char *aArgv[]);
static void setupSSM(void);
static void Terminate(void);

static unsigned int dT = 100; // 100ms
static SSMApi<ssm::JSData, ssm::JSProperty> *JOY;

static int printProperty(ssm::JSProperty *joy);
int main(int aArgc, char **aArgv)
{
    if (!setOption(aArgc, aArgv))
        return EXIT_FAILURE;

    SSMApi<ssm::JSData, ssm::JSProperty> joystick(SSM_NAME_JOYSTICK, 0);
    JOY = &joystick;
    double max_v = 0.8;
    double max_w = 0.3 * M_PI;
    double v, w;
    try
    {
        setupSSM();
        setSigInt();

        /*        if (Spur_init() < 0)
                {
                    fprintf(stderr, "ERROR : cannot open spur.\n");
                    return -1;
                }

                Spur_set_vel(1);
                Spur_set_accel(1);
                Spur_set_angvel(M_PI / 2.0);
                Spur_set_angaccel(M_PI / 2.0);

                Spur_set_pos_GL(0, 0, 0);
        */
        printProperty(&joystick.property);

        bool update[1] = {false};
        SSM_tid update_id[1] = {-1};
#define INDEX_JOYSTICK 0

        printf("\n\x1b[32m\x1b[1mStart joystick-handler\x1b[39m\x1b[0m\n\n");
        printf("////////   max_v = %f   max_w = %f   ////////\n", max_v, max_w);
        while (!gShutOff)
        {
            update[INDEX_JOYSTICK] = false;
            // get latest data for INDEX_JOYSTICK
            if (update_id[INDEX_JOYSTICK] < getTID_top(joystick.getSSMId()))
            {
                joystick.readLast();
                update[INDEX_JOYSTICK] = true; // 最新情報を読み込む
                update_id[INDEX_JOYSTICK] = joystick.timeId;
            }
            else
            {
                update[INDEX_JOYSTICK] = false;
            }
            //			ssm::JS_BUTTON_0 -> SELECT
            //			ssm::JS_BUTTON_3 -> START
            //			ssm::JS_BUTTON_16 -> PS
            //			ssm::JS_BUTTON_13 -> 丸ボタン
            //			ssm::JS_BUTTON_12 -> △ボタン
            //			ssm::JS_BUTTON_14 -> ☓ボタン
            //			ssm::JS_BUTTON_15 -> □ボタン
            //			0:左、左右(左-1、右1)
            //			1:左、上下(上-1、下1)
            //			2:右、左右(左-1、右1)
            //			3:右、上下(上-1、下1)
            if (update[INDEX_JOYSTICK])
            {
                //				printf( "%lu %lu %lu\n", joystick.data.button & ssm::JS_BUTTON_0, joystick.data.button & ssm::JS_BUTTON_3, joystick.data.button & ssm::JS_BUTTON_16 );
                //				printf( "%lu %lu %lu\n", ssm::JS_BUTTON_0, ssm::JS_BUTTON_3, ssm::JS_BUTTON_16 );
                if ((joystick.data.button & ssm::JS_BUTTON_0) == ssm::JS_BUTTON_0)
                {
                    printf("SELECT\n");
                }
                else if ((joystick.data.button & ssm::JS_BUTTON_3) == ssm::JS_BUTTON_3)
                {
                    printf("START\n");
                }
                else if ((joystick.data.button & ssm::JS_BUTTON_16) == ssm::JS_BUTTON_16)
                {
                    printf("PS\n");
                }
                else if ((joystick.data.button & ssm::JS_BUTTON_13) == ssm::JS_BUTTON_13)
                {
                    printf("丸ボタン\n");
                }
                else if ((joystick.data.button & ssm::JS_BUTTON_14) == ssm::JS_BUTTON_14)
                {
                    printf("☓ボタン\n");
                }
                else if ((joystick.data.button & ssm::JS_BUTTON_12) == ssm::JS_BUTTON_12)
                {
                    printf("△ボタン\n");
                }
                else if ((joystick.data.button & ssm::JS_BUTTON_15) == ssm::JS_BUTTON_15)
                {
                    printf("□ボタン\n");
                }
                else
                {
                    printf("%d\n", joystick.data.button);
                }

                // for( int i = 0 ; i < 4 ; i++ ){
                // printf( "[%d]%f ", i, joystick.data.axis[ i ] );
                //}
                // printf( "\n" );
                /*if ((joystick.data.button & ssm::JS_BUTTON_8) == ssm::JS_BUTTON_8)
                {
                    Spur_unfreeze();
                    printf("unfreeze\n");
                }

                if ((joystick.data.button & ssm::JS_BUTTON_9) == ssm::JS_BUTTON_9)
                {
                    Spur_freeze();
                    printf("freeze\n");
                }

                // Spur_set_vel( -1*joystick.data.axis[ 1 ] );
                // Spur_set_angvel( M_PI * joystick.data.axis[ 3 ] );
                v = -1 * max_v * joystick.data.axis[1];
                w = -1 * max_w * joystick.data.axis[3];
                Spur_vel(v, w);
                printf("v=%f   w=%f\n", v, w);*/
            }
            else
            {
                usleepSSM(dT * 1000); // 100ms
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
static void printShortHelp(const char *programName)
{
    fputs("\nHELP\n", stderr);
    fprintf(stderr, "\t$ %s [   options    ]\n", programName);
    fprintf(stderr, "\t$ %s -t %d\n", programName, dT);
    fputs("OPTION\n", stderr);
    printf("\t-t | --s_time     TIME     : Sampling time (defautl=%dms)\n", dT);
}
static bool setOption(int aArgc, char *aArgv[])
{
    int opt, optIndex = 0;
    struct option longOpt[] = {
        {"s_time", 1, 0, 't'},
        {"help", 0, 0, 'h'},
        {0, 0, 0, 0}};

    while ((opt = getopt_long(aArgc, aArgv, "t:h", longOpt, &optIndex)) != -1)
    {
        switch (opt)
        {
        case 't':
            dT = atoi(optarg);
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
static void setupSSM(void)
{
    std::cerr << "initializing ssm ... ";
    if (!initSSM())
        throw std::runtime_error("[\033[1m\033[31mERROR\033[30m\033[0m]:fail to initialize ssm.");
    else
        std::cerr << "OK.\n";

    // get joystick.property
    std::cerr << "\n"
              << "get joystick.property ... ";
    if (!JOY->getProperty())
        throw std::runtime_error("[\033[1m\033[31mERROR\033[30m\033[0m]:fail to get joystick.property .");
    else
        std::cerr << "OK.\n";

    // joystick を開く
    std::cerr << "open joystick ... ";
    if (!JOY->open(SSM_READ))
        throw std::runtime_error("[\033[1m\033[31mERROR\033[30m\033[0m]:fail to open joystick on ssm.\n");
    else
        std::cerr << "OK.\n";
}
static void Terminate(void)
{
    JOY->release();
    endSSM();
    printf("\n\x1b[32m\x1b[1mend\x1b[39m\x1b[0m\n\n");
}
static int printProperty(ssm::JSProperty *joy)
{
    printf("\n<JOYSTICK INFO>\n");
    printf("product  : %s\n", joy->name);
    printf("version  : %d\n", joy->version);

    printf("\n");
    printf("%8d # NUMBER OF AXIS\n", joy->axisNum);
    printf("%8d # NUMBER OF BOTTON\n", joy->buttonNum);

    return EXIT_SUCCESS;
}
