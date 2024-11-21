#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <getopt.h>
#include <signal.h>
#include <stdexcept>
#include <unistd.h>
#include <string.h>
#include <vector>

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

static void setSigInt(void);
static bool setOption(int aArgc, char *aArgv[]);
static void ctrlC(int aStatus);
static void Terminate(void);
static int CreatWP(FILE *fp, int number, bool avoid, const char map[32], int stop, double velocity, int area, double x, double y, int Search_status);

static char filename[256] = "/home/haselab17/TC2024-master/param/2024/01_wp_test.dat";
static char filename2[256] = "/home/haselab17/TC2024-master/param/2024/01_wp_test_v2.dat";

int main(int aArgc, char *aArgv[])
{
    if (!setOption(aArgc, aArgv))
        return EXIT_FAILURE;

    FILE *fp = fopen(filename, "r");
    if (fp == NULL)
    {
        perror("Failed to open input file");
        return EXIT_FAILURE;
    }

    FILE *fp2 = fopen(filename2, "w");
    if (fp2 == NULL)
    {
        perror("Failed to open output file");
        fclose(fp);
        return EXIT_FAILURE;
    }

    std::vector<wp> wpdata;
    int wpnum = 1;
    char buffer[1024];

    try
    {
        fprintf(fp2, "#Num. of WP\n");
        fprintf(fp2, "#ID\tWP_x[m]\tWP_y[m]\tdsm_type\tmap_name\tstop_type\tobst_avoid_flag\tVelocity\tArea_type\tSearch_Status\n");

        // Waypointデータの読み込み
        while (fgets(buffer, sizeof(buffer), fp) != NULL)
        {
            if (buffer[0] == '#') // コメント行のスキップ
                continue;

            wp data;
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
            data.map_name[sizeof(data.map_name) - 1] = '\0'; // Null終端

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

            wpdata.push_back(data);
        }

        // Waypointデータの書き込み
        for (const auto &data : wpdata)
        {
            wpnum = CreatWP(fp2, wpnum, data.obst_avoid_flag, data.map_name, data.stop_type, data.Velocity, data.Area_type, data.x, data.y, data.Search_Status);
        }
    }
    catch (const std::runtime_error &error)
    {
        std::cerr << "Runtime error: " << error.what() << std::endl;
    }
    catch (...)
    {
        std::cerr << "An unknown error occurred." << std::endl;
    }

    fclose(fp);
    fclose(fp2);
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
    std::cerr << "HELP\n";
    std::cerr << "\t$ " << programName << " -p <input_path> -s <output_path>\n";
    return EXIT_SUCCESS;
}

static bool setOption(int aArgc, char *aArgv[])
{
    int opt;
    struct option longOpt[] = {
        {"filename", required_argument, 0, 'p'},
        {"savefile", required_argument, 0, 's'},
        {"help", no_argument, 0, 'h'},
        {0, 0, 0, 0}};

    while ((opt = getopt_long(aArgc, aArgv, "p:s:h", longOpt, nullptr)) != -1)
    {
        switch (opt)
        {
        case 'p':
            strncpy(filename, optarg, sizeof(filename) - 1);
            filename[sizeof(filename) - 1] = '\0';
            break;
        case 's':
            strncpy(filename2, optarg, sizeof(filename2) - 1);
            filename2[sizeof(filename2) - 1] = '\0';
            break;
        case 'h':
            printShortHelp(aArgv[0]);
            return false;
        default:
            fprintf(stderr, "help : %s -h\n", aArgv[0]);
            return false;
        }
    }
    return true;
}

static void Terminate(void)
{
    std::cout << "Program terminated successfully.\n";
}

int CreatWP(FILE *fp, int number, bool avoid, const char map[32], int stop, double velocity, int area, double x, double y, int Search_status)
{
    fprintf(fp, "%d\t%.3f\t%.3f\t0\t%s\t%d\t%d\t%.3f\t%d\t%d\n", number, x, y, map, stop, avoid, velocity, area, Search_status);
    number = number+1;
    return number;
}
