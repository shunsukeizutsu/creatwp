#include <iostream>
#include <stdio.h>
#include <signal.h>
#include <ssm.h>
#include <sys/time.h>
#include <time.h>
#include <stdlib.h>

#include "localizer.hpp"

static int gShutOff = 0;
static void setSigInt(void);
static void ctrlC(int aStatus);
static void setupSSM(void);
static void Terminate(void);

static SSMApi<localizer, localizser_property> *LOCALIZER;

static usigned int dT =7;
int main(int aArgc, char *aArgv[])
{
    if (!setOption(aArgc, aArgv))
        return EXIT_FAILURE;

    SSMApi<localzier, localizer_property> localizer_fs(SNAME_LOCALIZER, 0);
    LOCALIZER = &localizer_fs;

    bool update = false;
    SSM_tid update_id = -1;
#define INDEX_LOCALIZER 0

    try
    {
        setupSSM();
        setSigInt();
        while (!gShutOff)
        {
            update = false;
            // get latest data for INDEX_URG
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
            int num =0;
            num = localizer_fs.getTID_topBuf(0) - localizer_fs.timeId;
            if(num > 0)
            {

            }
            usleep(dT*1000000);
        }
        Terminate();
    }
    catch (runtime_error const &error)
    {
        cout << error.what() << endl;
    }
    catch (...)
    {
        cout << "An unknown fatal error has occured. Aborting." << endl;
    }

    endSSM();
    return EXIT_SUCCESS;
}