//ourwork
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<algorithm>
#include<math.h>
#include <time.h>
#include "RTree.h"
using namespace std;
#define INF 2100000000

//记录条数（含重复记录）
#define N 1000000
#define AllDimesionNum 16
#define DimensionNum 2
#define CharNum 2
#define MaxLength 20

#define MaxDistance 900
#define FIELD_VALUE_MIN 0
#define FIELD_VALUE_MAX 25


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
int CalPeoDistance(people px, people py);
void InsertToRTree();
bool SearchCallbackTotal(int id, void* arg);
void SearchInRTree();
void Output();


//全局变量
int dupCount = 2701376;
//100w数据集是2701376
//从记录在N条记录中的序号，到记录本身id的映射表
int indexToId[N + 10];
int nowIndex = 0;
long long hitCnt[1000];
long long dupCnt[1000];
clock_t clockBegin, clockInsertEnd, clockEnd;
RTree<int, int, DimensionNum * CharNum, float> tree;
int dupRecall;
int RMax[DimensionNum * CharNum];
int RMin[DimensionNum * CharNum];


//输入函数
void Input()
{
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
                    printf("%d\n", peo[i].record[j][k]);
            }
        }
	}
}


//初始化函数
void Init()
{
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
    return;
}


//找出所有实际存在的重复记录数
int CountAllTheDup()
{
	int i,j;
	int dupCount=0;
	for(i=0;i<N;i++)
	{
		for(j=i+1;j<N;j++)
		{
			if(peo[i].id ==peo[j].id)
			{
				dupCount++;
			}
            else
            {
                break;
            }
		}
	}
	return dupCount;
}


//对记录按照实际id进行排序
bool CmpById(people px, people py)
{
    if(px.id != py.id)
	    return px.id < py.id;
    if(px.type != py.type)
	    return px.type < py.type;
    if(px.dupid != py.dupid)
	    return px.dupid < py.dupid;
}


//计算两条记录间的距离
int CalPeoDistance(people px, people py)
{
	int distance=0;
    for (int i = 0; i < DimensionNum * CharNum; i++)
	{
		distance += abs(px.field[i] - py.field[i]);
	}
	return distance;
}

void InsertToRTree()
{
    //将结点插入R树中
    clockBegin = clock();
	for (int i = 0; i < N; i++)
	{
        int a[DimensionNum * CharNum];
        int b[DimensionNum * CharNum];
		for (int j = 0; j < (DimensionNum * CharNum); j++)
		{
			if(peo[i].field[j] == -1)
			{
                a[j] = FIELD_VALUE_MIN;
				b[j] = FIELD_VALUE_MAX;
			}
			else
			{
                //a[j] = peo[i].field[j];
                //TODO:有时候插入R树时程序会崩溃，需要用随机数去扰乱
                a[j] = peo[i].field[j] + (rand()%100)/99;
                b[j] = a[j];
			}
		}
        //printf("%d %d %d\n", peo[i].id, peo[i].type, peo[i].dupid);
        //for (int j = 0; j < DimensionNum * CharNum; j++)
        //{
        //    printf("%d ", a[j]);
        //}
        //puts("");
		tree.Insert(a, b, i);
        indexToId[i] = peo[i].id;
	}
    clockInsertEnd = clock();
    return;
}

bool SearchCallbackTotal(int index, void* arg)
{
    if(nowIndex > index)
    {
        int distance = CalPeoDistance(peo[nowIndex], peo[index]);
        hitCnt[distance]++;
        if(indexToId[index] == peo[nowIndex].id)
	    {
		    dupCnt[distance]++;
	    }
    }
    //keep going
	return true;
}

void SearchInRTree()
{
    memset(hitCnt, 0, sizeof(hitCnt));
	memset(dupCnt, 0, sizeof(dupCnt));
	for (int i = 0; i < N; i++)
	{
        clock_t clockSearchBegin = clock();
		nowIndex = i;
        int searchNum = tree.Search(RMax, RMin, SearchCallbackTotal, NULL);
        printf("index = %d\n", i);
        clock_t clockSearchEnd = clock();
        printf("spent %lf seconds.\n", (float)(clockSearchEnd - clockSearchBegin)/1000.0);
    }
    clockEnd = clock();
    return;
}

void Output()
{
    for(int i = 0; i <= MaxDistance; i++)
	{
		hitCnt[i] += hitCnt[i-1];
		dupCnt[i] += dupCnt[i-1];
		printf("for %8lld comparasion , we find %8lld dups", hitCnt[i], dupCnt[i]);
		printf(" , the recall is %9.8lf\n",(double)dupCnt[i]/dupCount);
        if(dupCnt[i] == dupCount)
            break;
	}
    printf("\nInsert operation spent %lf seconds.\n", (float)(clockInsertEnd - clockBegin)/1000.0);
    printf("\nSearch with biggest rectangle spent %lf seconds.\n\n", (float)(clockEnd - clockInsertEnd)/1000.0);
    return;
}

bool SearchCallbackInDiffRecall(int index, void* arg)
{
	if(indexToId[index] == peo[nowIndex].id && nowIndex > index)
	{
        dupRecall ++;
	}
	return true;
}

void CalSearchTimeInDiffRecall()
{
    for(int distance = 0; distance < 30; distance ++)
    {
        clock_t clockSearchBegin = clock();
        dupRecall = 0;
        for (int i = 0; i < N; i++)
	    {
            clock_t clockOneSearchBegin = clock();
		    int a[DimensionNum * CharNum];
		    int b[DimensionNum * CharNum];
		    for (int j = 0; j < (DimensionNum * CharNum); j++)
		    {
                a[j] = peo[i].field[j] - distance;
			    b[j] = peo[i].field[j] + distance;
		    }
		    nowIndex = i;
		    int tmp = tree.Search(a, b, SearchCallbackInDiffRecall, NULL);
            clock_t clockOneSearchEnd = clock();
            //printf("dis = %2d, index = %d, spent %lf seconds.\n", distance, i, (float)(clockOneSearchEnd - clockOneSearchBegin)/1000.0);
	    }
        clock_t clockSearchEnd = clock();
        printf("distance in every dimension = %2d, search out = %d, recall = %lf, ", distance, dupRecall, (double)dupRecall/dupCount);
        printf("spent %lf seconds.\n", (float)(clockSearchEnd - clockSearchBegin)/1000.0);
        if(dupRecall == dupCount)
            break;
    }
    return;
}

int main()
{
    freopen("dataset2_200000_800000_9_5_5_zipf_all_0_extractALLDimensions.txt","r",stdin);
    freopen("dataset2_200000_800000_9_5_5_zipf_all_0_R2D_BigRect.txt","w",stdout);
    srand((unsigned)time(NULL));
	int i,j;
	Input();
    printf("Input finished.\n");
    Init();
    printf("Init finished.\n");
    //按照实际id排序
    //降低CountAllTheDup的时间复杂度
	//sort(peo,peo+N,CmpById);
    //找出实际存在的重复记录对数
	//dupCount=CountAllTheDup();
	//printf("dupcount=%d\n",dupCount);
    //插入R树
    InsertToRTree();
    printf("Insert into RTree finished.\n");
    //在R树中搜索
    SearchInRTree();
    printf("Search in RTree finished.\n");
    //输出结果
    Output();
    //计算不同召回率的搜索时间
    //CalSearchTimeInDiffRecall();
    return 0;
}
