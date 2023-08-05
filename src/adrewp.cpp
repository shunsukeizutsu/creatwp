#include "adrewp.hpp"

int main(void)
{
    std::list<wpdata> Alldata;
    int ret;

    /////***********READ WAYPOINT FILE -> list
    char filename[256 / 2] = "../data/test.dat";
    char mode[256 / 2] = "r";
    FILE *fp;
    fp = fopen(filename, mode);
    if (fp == NULL)
    {
        printf("Cannot open the file\n");
        return EXIT_FAILURE;
    }
    wpdata tmp;
    while ((ret = fscanf(fp, "%d %lf %lf %d %s %d %d %lf %d %d", &tmp.ID, &tmp.estPos[0], &tmp.estPos[1], &tmp.dsm_type, tmp.mapname, &tmp.stop_type, &tmp.obst_avoid_flag, &tmp.velocity, &tmp.Area_type, &tmp.Search_Status)) != EOF)
    {
        // printf("%d %lf %lf %d %s %d %d %lf %d %d\n",tmp.ID,tmp.estPos[0],tmp.estPos[1],tmp.dsm_type,tmp.mapname,tmp.stop_type,tmp.obst_avoid_flag,tmp.velocity,tmp.Area_type,tmp.Search_Status);
        Alldata.push_back(tmp);
    }
    fclose(fp);
    ////////////////////////////////////////////////////////////////////////////////////////
    std::list<wpdata>::iterator itr;

    int num;
    printf("1: add data  2: remove data 3: quit\n");
    scanf("%d", &num);
    if (num == 1)
    {
        printf("add data\n");
        wpdata adata;
        adata = ADDdata();
        itr = Alldata.begin();
        for (int i = 1; i < adata.ID; i++)
        {
            itr++;
        }
        Alldata.insert(itr, adata);
    }
    else if (num == 2)
    {
        printf("remove data\n");
        printf("remove ID?\n");
        int reid;
        scanf("%d", &reid);
        itr = Alldata.begin();
        for (int i = 1; i < reid; i++)
        {
            itr++;
        }
        Alldata.erase(itr);
    }
    else
    {
        printf("quit\n");
    }

    /////***********list -> CREATE WAYPOINT FILE
    FILE *fp1;
    fp1 = fopen("../data/creatwp.dat", "w");
    int id_number = 1;
    for (itr = Alldata.begin(); itr != Alldata.end(); itr++)
    {
        fprintf(fp1, "%d %lf %lf %d %s %d %d %lf %d %d\n", id_number, itr->estPos[0], itr->estPos[1], itr->dsm_type, itr->mapname, itr->stop_type, itr->obst_avoid_flag, itr->velocity, itr->Area_type, itr->Search_Status);
        id_number++;
    }
    ///////////////////////////////////////////////////////////////////

    fclose(fp1);
    Terminate();
    return EXIT_SUCCESS;
}
static void Terminate(void)
{
    printf("end\n");
}
wpdata ADDdata(void)
{
    wpdata adata;
    printf("ID?\n");
    scanf("%d", &adata.ID);
    printf("estPos[0]?\n");
    scanf("%lf", &adata.estPos[0]);
    printf("estPos[1]?\n");
    scanf("%lf", &adata.estPos[1]);
    printf("dsm_type?\n");
    scanf("%d", &adata.dsm_type);
    printf("mapname?\n");
    scanf("%s", adata.mapname);
    printf("stop_type?\n");
    scanf("%d", &adata.stop_type);
    printf("obst_avoid_flag?\n");
    scanf("%d", &adata.obst_avoid_flag);
    printf("velocity?\n");
    scanf("%lf", &adata.velocity);
    printf("Area_type?\n");
    scanf("%d", &adata.Area_type);
    printf("Search_Status?\n");
    scanf("%d", &adata.Search_Status);

    return adata;
}
