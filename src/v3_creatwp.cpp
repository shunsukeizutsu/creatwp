#include "creatwp.hpp"

static unsigned int dT = 100; // 100ms
static SSMApi<ssm::JSData, ssm::JSProperty> *JOY;
static SSMApi<localizer, localizer_property> *LOCALIZER;

int number = 1;

static int printProperty(ssm::JSProperty *joy);
int main(int aArgc, char **aArgv)
{
    if (!setOption(aArgc, aArgv))
        return EXIT_FAILURE;
    FILE *fp;
    fp = fopen("../data/test.dat", "w");

    bool flag_first_loop = true;
    bool start_flag = false;

    SSMApi<ssm::JSData, ssm::JSProperty> joystick(SSM_NAME_JOYSTICK, 0);
    JOY = &joystick;
    SSMApi<localizer, localizer_property> localizer(SNAME_LOCALIZER, 0);
    LOCALIZER = &localizer;
    try
    {
        std::vector<wp> wpdata;
        setupSSM();
        setSigInt();
        bool area_type = false;
        bool avoid_type = false;
        int wp_number = 1;
        double max_velocity = 0.7;
        double pre_x = 0;
        double pre_y = 0;
        printProperty(&joystick.property);

        fprintf(fp, "#Num. of WP\n");
        fprintf(fp, "#ID\tWP_x[m]\tWP_y[m]\tdsm_type\tmap_name\tstop_type\tobst_avoid_flag\tVelocity\tArea_type\tSearch_Status\n");

        bool update[2] = {false};
        SSM_tid update_id[2] = {-1};
#define INDEX_JOYSTICK 0
#define INDEX_LOCALIZER 1
        while (!gShutOff)
        {
            update[INDEX_JOYSTICK] = false;
            update[INDEX_LOCALIZER] = false;

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
            // get latest data for INDEX_JOYSTICK
            if (update_id[INDEX_LOCALIZER] < getTID_top(localizer.getSSMId()))
            {
                localizer.readLast();
                joystick.readLast();
                update[INDEX_LOCALIZER] = true; // 最新情報を読み込む
                update_id[INDEX_LOCALIZER] = localizer.timeId;
            }
            else
            {
                update[INDEX_LOCALIZER] = false;
            }
            if (update[INDEX_JOYSTICK])
            {
                if ((joystick.data.button & ssm::JS_BUTTON_10) == ssm::JS_BUTTON_10)
                {
                    printf("start creat way point\n");
                    start_flag = true;
                }
            }
            if (update[INDEX_LOCALIZER])
            {

                if (flag_first_loop)
                {
                    flag_first_loop = false;
                    pre_x = localizer.data.estPos[0];
                    pre_y = localizer.data.estPos[1];
                    wp_number = CreatWP(wpdata, fp, wp_number, avoid_type, 0, max_velocity, area_type, localizer.data.estPos[0], localizer.data.estPos[1]);
                }
                else
                {
                    if ((joystick.data.button & ssm::JS_BUTTON_1) == ssm::JS_BUTTON_1)
                    { // Area Type
                        printf("Change Area Type %d", area_type);
                        area_type = !area_type;
                    }
                    if ((joystick.data.button & ssm::JS_BUTTON_2) == ssm::JS_BUTTON_2)
                    { // Obst_Avoid_flag
                        printf("Change Avoid Type %d", avoid_type);
                        avoid_type = !avoid_type;
                    }
                    if ((joystick.data.button & ssm::JS_BUTTON_4) == ssm::JS_BUTTON_4)
                    { // Stop flag = 1
                        printf("stop flag\n");
                        wp_number = CreatWP(wpdata, fp, wp_number, avoid_type, 1, 0.5, area_type, localizer.data.estPos[0], localizer.data.estPos[1]);
                        pre_x = localizer.data.estPos[0];
                        pre_y = localizer.data.estPos[1];
                    }else if ((joystick.data.button & ssm::JS_BUTTON_5) == ssm::JS_BUTTON_5)
                    { // Stop flag = 2
                        printf("stop flag\n");
                        wp_number = CreatWP(wpdata, fp, wp_number, avoid_type, 2, 0.5, area_type, localizer.data.estPos[0], localizer.data.estPos[1]);
                        pre_x = localizer.data.estPos[0];
                        pre_y = localizer.data.estPos[1];
                    }else if ((joystick.data.button & ssm::JS_BUTTON_6) == ssm::JS_BUTTON_6)
                    { // Stop flag = 3
                        printf("stop flag\n");
                        wp_number = CreatWP(wpdata, fp, wp_number, avoid_type, 3, 0.5, area_type, localizer.data.estPos[0], localizer.data.estPos[1]);
                        pre_x = localizer.data.estPos[0];
                        pre_y = localizer.data.estPos[1];
                    }else if ((joystick.data.button & ssm::JS_BUTTON_7) == ssm::JS_BUTTON_7)
                    { // Stop flag = 4
                        printf("stop flag\n");
                        wp_number = CreatWP(wpdata, fp, wp_number, avoid_type, 4, 0.5, area_type, localizer.data.estPos[0], localizer.data.estPos[1]);
                        pre_x = localizer.data.estPos[0];
                        pre_y = localizer.data.estPos[1];
                    }

                    double delta_x = localizer.data.estPos[0] - pre_x;
                    double delta_y = localizer.data.estPos[1] - pre_y;

                    double distance = delta_x * delta_x + delta_y * delta_y;

                    if (fabs(localizer.data.estAngvel[2]) > 0.05)
                    {
                        if (fabs(localizer.data.estAngvel[2]) > 0.10)
                        {
                            if (sqrt(distance) > 0.3)
                            {
                                wp_number = CreatWP(wpdata, fp, wp_number, avoid_type, 2, 0.5, area_type, localizer.data.estPos[0], localizer.data.estPos[1]);

                                pre_x = localizer.data.estPos[0];
                                pre_y = localizer.data.estPos[1];
                            }
                        }
                        else
                        {
                            if (sqrt(distance) > 0.7)
                            {

                                wp_number = CreatWP(wpdata, fp, wp_number, avoid_type, 2, 0.5, area_type, localizer.data.estPos[0], localizer.data.estPos[1]);

                                pre_x = localizer.data.estPos[0];
                                pre_y = localizer.data.estPos[1];
                            }
                        }
                    }
                    else
                    {
                        if (sqrt(distance) > 3.0)
                        {
                            wp_number = CreatWP(wpdata, fp, wp_number, avoid_type, 2, 0.5, area_type, localizer.data.estPos[0], localizer.data.estPos[1]);

                            pre_x = localizer.data.estPos[0];
                            pre_y = localizer.data.estPos[1];
                        }
                    }
                }
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
    Terminate(fp);

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
static void Terminate(FILE *fp)
{
    fclose(fp);
    LOCALIZER->release();
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

    //			ssm::JS_BUTTON_0 -> ☓ボタン
    //			ssm::JS_BUTTON_1-> 丸ボタン
    //			ssm::JS_BUTTON_2 -> △ボタン
    //			ssm::JS_BUTTON_3 -> □ボタン
    //			ssm::JS_BUTTON_4 -> L1
    //			ssm::JS_BUTTON_5 -> R1
    //			ssm::JS_BUTTON_6 -> L2
    //			ssm::JS_BUTTON_7 -> R2
    //			ssm::JS_BUTTON_8 -> 小
    //			ssm::JS_BUTTON_9 -> 三
    //			ssm::JS_BUTTON_10 -> HOME
    //			ssm::JS_BUTTON_11 -> LSTICK PUSH
    //			ssm::JS_BUTTON_12 -> RSTICK PUSH
    //          joystick.data.axis[0] = Ljoystick y axis
    //          joystick.data.axis[1] = Ljoystick x axis
    //          joystick.data.axis[3] = Rjoystick y axis
    //          joystick.data.axis[4] = Rjoystick x axis
    //          joystick.data.axis[6] = cross key y axis(right=1.0, left=-1.0)
    //          joystick.data.axis[7] = cross_key x axis(front=-1.0, back=1.0)

    return EXIT_SUCCESS;
}
int CreatWP(vector<wp> wpdata, FILE *fp, int number, bool avoid, bool stop, double velocity, int area, double x, double y)
{
    wp tmp;
    tmp.x = x;
    tmp.y = y;
    tmp.stop_type = stop;
    tmp.avoid_flag = avoid;
    tmp.velocity = velocity;
    tmp.Area_type = area;
    tmp.Search_Status = 1;
    wpdata.push_back(tmp);
    fprintf(fp, "%d\t%f\t%f\t0\tN\t%d\t%d\t%f\t%d\t1\n", number, x, y, stop, avoid, velocity, area);
    
    return number++;
}