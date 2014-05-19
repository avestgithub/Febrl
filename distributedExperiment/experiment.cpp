//ourwork
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<algorithm>
#include<math.h>
#include <time.h>
#include "RTree.h"
#include <iostream>
#include <string>
#include <set>
using namespace std;
#define INF 2100000000

//记录条数（含重复记录）
#define N 100000
#define ExtractDimesionNum 10
#define ExperimentDimensionNum 10
#define CharNum 2
#define MachineNum 3

#define FIELD_VALUE_MIN -1
#define FIELD_VALUE_MAX 25

//切记多看参数是否正确
#define Probability 3
#define MinDistance 16
#define MaxDistance 16
#define NMin 0
#define NMax N

struct people
{
    int id;
    //org==0&&dup==1
    bool type;
    int dupid;

    int record[ExtractDimesionNum][CharNum];
    int field[ExperimentDimensionNum * CharNum];
}peo[N + 10];

//全局变量
int dupCount = 269466;
//100w:2701376
//10w_9_1_1:268929
//10w_9_5_5:269466
//1w:16431
//从记录在N条记录中的序号，到记录本身id的映射表
int indexToId[N];
int nowIndex = 0;

RTree<int, int, ExperimentDimensionNum * CharNum, float> tree[MachineNum];

int RMin[ExperimentDimensionNum * CharNum];
int RMax[ExperimentDimensionNum * CharNum];

long long machineSetSize[MachineNum];
long long entireSetSize;
long long foundDupSize;
long long realDupSize;

set<long long> entireSet;
set<long long> machineSet;



//输入
void Input()
{
    clock_t clockInputBegin = clock();
	int i;
    int cnt = 0;
    char temps[1000];
    char tempc = '\0';
	int tempi=-1;

    for(i = 0; i < N; i++)
    {
		scanf("%d",&peo[i].id);
		scanf("%d",&peo[i].type);
		scanf("%d", &peo[i].dupid);

        for(int j = 0; j < ExtractDimesionNum; j++)
        {
            for(int k = 0; k < CharNum; k++)
            {
                scanf("%d", &peo[i].record[j][k]);
                //验证没有超过25的字段
                if(peo[i].record[j][k] > 25)
                    printf("%d\n", peo[i].record[j][k]);
            }
        }
	}
    clock_t clockInputEnd = clock();
    printf("\nInput operation spent %lf seconds.\n", (double)(clockInputEnd - clockInputBegin)/1000.0);
    printf("Input finished.\n");
    return;
}

//初始化
void Init()
{
    clock_t clockInitBegin = clock();
    //将record中的原始信息选取导入到field中
    for(int i = 0; i < N; i++)
    {
        for(int j = 0; j < ExperimentDimensionNum; j++)
        {
            for(int k = 0; k < CharNum; k++)
            {
                peo[i].field[j * CharNum + k] = peo[i].record[j][k];
            }
        }
    }
    memset(indexToId, -1, sizeof(indexToId));
    for (int i = 0; i < (ExperimentDimensionNum * CharNum); i++)
	{
		RMin[i] = FIELD_VALUE_MIN;
		RMax[i] = FIELD_VALUE_MAX;
	}

    clock_t clockInitEnd = clock();
    printf("\nInit operation spent %lf seconds.\n", (double)(clockInitEnd - clockInitBegin)/1000.0);
    printf("Init finished.\n");
    return;
}

void InsertToRTree(int machineId)
{
    //将结点插入R树中
    clock_t clockInsertBegin = clock();
    tree[machineId].RemoveAll();
	for (int i = 0; i < N; i++)
	{
        int a[ExperimentDimensionNum * CharNum];
        int b[ExperimentDimensionNum * CharNum];
		for (int j = 0; j < (ExperimentDimensionNum * CharNum); j++)
		{
			if(peo[i].field[j] == -1)
			{
                a[j] = FIELD_VALUE_MIN;
				b[j] = FIELD_VALUE_MAX;
			}
			else
			{
                //a[j] = peo[i].field[j];
                int dupRand = rand()%100;
                if(dupRand < Probability)
                {
                    a[j] = rand() % 26;
                }
                else
                {
                    a[j] = peo[i].field[j];
                }
                b[j] = a[j];
			}
		}
        tree[machineId].Insert(a, b, i);
        indexToId[i] = peo[i].id;
	}
    clock_t clockInsertEnd = clock();
    printf("Insert machine %d spent %lf seconds.\n", machineId, (double)(clockInsertEnd - clockInsertBegin)/1000.0);
    return;
}

bool SearchCallbackInDiffRecall(int index, void* arg)
{
    if(nowIndex > index)
	{
        long long tmp = (long long)nowIndex * N + index;
        machineSet.insert(tmp);
	}
	return true;
}

void CalSearchTimeInDiffRecall()
{
    int a[ExperimentDimensionNum * CharNum];
    int b[ExperimentDimensionNum * CharNum];
    for(int distance = MinDistance; distance <= MaxDistance; distance ++)
    {
        printf("\nbegin, distance = %d\n", distance);
        for(int machineId = 0; machineId < MachineNum; machineId ++)
        {
            InsertToRTree(machineId);
        }
        //两个计算器初始化
        memset(machineSetSize, 0, sizeof(machineSetSize));
        entireSetSize = 0;
        foundDupSize = 0;
        realDupSize = 0;
        for (int i = NMin; i < NMax; i++)
	    {
            entireSet.clear();
            nowIndex = i;
		    for (int j = 0; j < (ExperimentDimensionNum * CharNum); j++)
		    {
                a[j] = peo[i].field[j] - distance;
			    b[j] = peo[i].field[j] + distance;
		    }
            for (int j = 0; j < i; j++)
            {
                if(peo[i].id == peo[j].id)
                {
                    realDupSize ++;
                }
            }
            for(int machineId = 0; machineId < MachineNum; machineId ++)
            {
                machineSet.clear();
                int tmp = tree[machineId].Search(a, b, SearchCallbackInDiffRecall, NULL);
                machineSetSize[machineId] += machineSet.size();
                if(machineId == 0)
                {
                    entireSet = machineSet;
                }
                else
                {
                    set<long long> tempSet = entireSet;
                    entireSet.clear();
                    set_intersection(tempSet.begin(), tempSet.end(), machineSet.begin(), machineSet.end(), inserter(entireSet, entireSet.begin()));
                    tempSet.clear();
                }
            }
            set<long long>::const_iterator itor ;
            for(itor = entireSet.begin(); itor != entireSet.end(); ++itor)
            {
                long long item = *itor;
                
                long long index1 = (long long)(item / N);
                long long index2 = (long long)(item % N);
                
                if(indexToId[(int)index2] == peo[(int)index1].id)
                {
                    //printf("%lld\n", item);
                    foundDupSize ++;
                }
            }
            entireSetSize += entireSet.size();
            if(i % 100 == 99)
            {
                printf("index = %d\n", i);
                for(int machineId = 0; machineId < MachineNum; machineId ++)
                {
                    printf("machine %d, set = %lld\n", machineId, machineSetSize[machineId]);
                }
                printf("entire set =     %lld\n", entireSetSize);
                printf("foundDupSize =   %lld\n", foundDupSize);
                printf("realDupSize =    %lld\n\n", realDupSize);
            }
	    }
    }
    return;
}

int main()
{
    freopen("dataset2_20000_80000_9_1_1_zipf_all_0_extract10D2Char.txt","r",stdin);
    freopen("316_1_1_10machine.txt","w",stdout);
    srand((unsigned)time(NULL));
	Input();
    Init();
    CalSearchTimeInDiffRecall();
    return 0;
}