#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<algorithm>
#include<math.h>
#include <time.h>
using namespace std;
#define INF 2100000000
#define ABS(a,b) (((a)>(b)) ? ((a)-(b)) : ((b)-(a)))

//记录条数（含重复记录）
#define N 1000000
#define AllDimesionNum 16
#define DimensionNum 10
#define CharNum 2
#define MaxLength 20

#define MaxDistance 550
#define FIELD_VALUE_MIN 0
#define FIELD_VALUE_MAX 26

#define ExpeNum 4


struct people
{
    int id;
    //org==0&&dup==1
    bool type;
    int dupid;

    int record[DimensionNum][MaxLength];
    int field[DimensionNum * CharNum];
}peo[N + 10];


//函数列表
void Input();
void RecordInit();
int CountAllTheDup();
bool CmpById(people px, people py);
void CalPeoDistance(people px, people py);
void Search();
void Output();


//全局变量
int dupCount = 2701376;
//100w数据集是2701376
//从记录在N条记录中的序号，到记录本身id的映射表
int indexToId[N + 10];
int nowIndex = 0;
long long hitCnt[ExpeNum][MaxDistance + 10];
long long dupCnt[ExpeNum][MaxDistance + 10];
int dupRecall;
int distanceList[ExpeNum];



//输入函数
void Input()
{
    clock_t clockInputBegin = clock();
	int i;
    int cnt = 0;
    char temps[1000];
    char tempc = '\0';
	int tempi=-1;
    //the header
	gets(temps);
    for(i = 0; i < N; i++)
    {
		scanf("%d",&peo[i].id);
		scanf("%d",&peo[i].type);
		scanf("%d", &peo[i].dupid);

        for(int j = 0; j < AllDimesionNum; j++)
        {
            for(int k = 0; k < MaxLength; k++)
            {
                scanf("%d", &peo[i].record[j][k]);
                //验证没有超过25的字段
                if(peo[i].record[j][k] > 25)
                    printf("the field is larger than 25: %d\n", peo[i].record[j][k]);
            }
        }
	}
    clock_t clockInputEnd = clock();
    printf("\nInput operation spent %lf seconds.\n", (double)(clockInputEnd - clockInputBegin)/1000.0);
    printf("Input finished.\n");
    return;
}


//初始化函数
void Init()
{
    clock_t clockInitBegin = clock();
    //将record中的原始信息选取导入到field中
    for(int i = 0; i < N; i++)
    {
        for(int j = 0; j < DimensionNum; j++)
        {
            for(int k = 0; k < CharNum; k++)
            {
                peo[i].field[j * CharNum + k] = peo[i].record[j][k];
            }
        }
    }
    //Init
    memset(indexToId, -1, sizeof(indexToId));
    for(int i = 0; i < ExpeNum; i++)
    {
        for(int j = 0; j < MaxDistance; j++)
        {
            hitCnt[i][j] = 0;
            dupCnt[i][j] = 0;
        }
    }
    clock_t clockInitEnd = clock();
    printf("\nInit operation spent %lf seconds.\n", (double)(clockInitEnd - clockInitBegin)/1000.0);
    printf("Init finished.\n");
    return;
}


//计算两条记录间的距离
void CalPeoDistance(people px, people py)
{
	int distance=0;
    for (int i = 0; i < DimensionNum * CharNum; i++)
	{
		distance += ABS(px.field[i], py.field[i]);
        if(i == 1)
            distanceList[0] = distance;
        else if(i == 3)
            distanceList[1] = distance;
        else if(i == 9)
            distanceList[2] = distance;
        else if(i == 11)
            distanceList[3] = distance;
	}
	return;
}


void Search()
{
    clock_t clockSearchBegin = clock();
	for (int i = 0; i < N; i++)
	{
        clock_t clockOneSearchBegin = clock();
        for(int j = i+1; j < N; j++)
        {
            CalPeoDistance(peo[i], peo[j]);
            for(int k = 0; k < ExpeNum; k++)
            {
                hitCnt[k][distanceList[k]]++;
                if(peo[i].id == peo[j].id)
                    dupCnt[k][distanceList[k]]++;
            }
        }
        clock_t clockOneSearchEnd = clock();
        printf("index = %d, spent %lf seconds.\n", i, (double)(clockOneSearchEnd - clockOneSearchBegin)/1000.0);
        if(i % 1000 == 999)
        {
            for (int k = 0; k < ExpeNum; k++)
            {
                printf("Experiment %d:\n", k);
                for(int j = 0; j < MaxDistance; j++)
                {
                    printf("dis = %3d, hit = %15lld, dup = %10lld\n", j, hitCnt[k][j], dupCnt[k][j]);
                }
                puts("");
            }
        }
    }
    clock_t clockSearchEnd = clock();
    printf("\nAll search operation spent %lf seconds.\n\n", (double)(clockSearchEnd - clockSearchBegin)/1000.0);
    printf("Search finished.\n");
    return;
}

void Output()
{
    for(int k = 0; k < ExpeNum; k++)
    {
        printf("Experiment %d:\n", k);
        for(int i = 0; i <= MaxDistance; i++)
	    {
		    hitCnt[k][i] += hitCnt[k][i-1];
		    dupCnt[k][i] += dupCnt[k][i-1];
		    printf("distance = %3d, %15lld comparasion , find %10lld dups", i, hitCnt[k][i], dupCnt[k][i]);
		    printf(" , the recall is %9.8lf\n",(double)dupCnt[k][i]/dupCount);
            if(dupCnt[k][i] == dupCount)
                break;
	    }
        puts("");
    }
    return;
}



int main()
{
    freopen("dataset2_200000_800000_9_5_5_zipf_all_0_extractALLDimensions.txt","r",stdin);
    freopen("dataset2_200000_800000_9_5_5_zipf_all_0_AllDimensions_WithoutRTree.txt","w",stdout);
    srand((unsigned)time(NULL));
	Input();
    Init();
    Search();
    Output();
    return 0;
}
