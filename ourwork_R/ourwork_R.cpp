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
#define ExtractDimesionNum 10
#define ExperimentDimensionNum 2
#define CharNum 2

#define MaxDistance 550
#define FIELD_VALUE_MIN 0
#define FIELD_VALUE_MAX 26


struct people
{
    int id;
    //org==0&&dup==1
    bool type;
    int dupid;

    int record[ExtractDimesionNum][CharNum];
    int field[ExperimentDimensionNum * CharNum];
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
//1w的是16431
//从记录在N条记录中的序号，到记录本身id的映射表
int indexToId[N];
int nowIndex = 0;
long long hitCnt[MaxDistance];
long long dupCnt[MaxDistance];
RTree<int, int, ExperimentDimensionNum * CharNum, float> tree;
int dupRecall;
int RMin[ExperimentDimensionNum * CharNum];
int RMax[ExperimentDimensionNum * CharNum];

//TODO:完成判断函数
int groupInfo[N];
int groupNum;
bool notInGroup[N];

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


//初始化函数
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
    //Init
    memset(indexToId, -1, sizeof(indexToId));
    for (int i = 0; i < (ExperimentDimensionNum * CharNum); i++)
	{
		RMin[i] = FIELD_VALUE_MIN;
		RMax[i] = FIELD_VALUE_MAX;
	}
    memset(hitCnt, 0, sizeof(hitCnt));
    memset(dupCnt, 0, sizeof(dupCnt));

    groupNum = 0;
    memset(groupInfo, -1, sizeof(groupInfo));

    clock_t clockInitEnd = clock();
    printf("\nInit operation spent %lf seconds.\n", (double)(clockInitEnd - clockInitBegin)/1000.0);
    printf("Init finished.\n");
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


void InsertToRTree()
{
    //将结点插入R树中
    clock_t clockInsertBegin = clock();
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
                a[j] = peo[i].field[j];
                //TODO:有时候插入R树时程序会崩溃，需要用随机数去扰乱
                //a[j] = peo[i].field[j] + (rand()%10)/9;
                b[j] = a[j];
			}
		}
        //输出中间结果以供查看
        //printf("%d %d %d\n", peo[i].id, peo[i].type, peo[i].dupid);
        //for (int j = 0; j < DimensionNum * CharNum; j++)
        //{
        //    printf("%d ", a[j]);
        //}
        //puts("");
		tree.Insert(a, b, i);
        indexToId[i] = peo[i].id;
	}
    clock_t clockInsertEnd = clock();
    printf("\nInsert operation spent %lf seconds.\n", (double)(clockInsertEnd - clockInsertBegin)/1000.0);
    printf("Insert into RTree finished.\n");
    return;
}


//计算两条记录间的距离
int CalPeoDistance(people px, people py)
{
	int distance=0;
    for (int i = 0; i < ExperimentDimensionNum * CharNum; i++)
	{
		distance += abs(px.field[i] - py.field[i]);
	}
	return distance;
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
	return true;
}

//TODO:完成函数
//bool SearchCallbackTotal(int index, void* arg)
//{
//    if(nowIndex <= index)
//        return true;
//    int distance = -1;
//    if(groupInfo[index] == -1)
//    {
//        distance = CalPeoDistance(peo[nowIndex], peo[index]);
//        hitCnt[distance]++;
//        if(indexToId[index] == peo[nowIndex].id)
//	    {
//            dupCnt[distance]++;
//            //TODO:考虑一下这种情况可不可能出现
//            if(groupInfo[nowIndex] != -1)
//            {
//                groupInfo[index] = groupInfo[nowIndex];
//            }
//            else
//            {
//                groupInfo[index] = groupNum;
//                groupInfo[nowIndex] = groupNum ++;
//            }
//	    }
//    }
//    else
//    {
//        if(groupInfo[nowIndex] != -1)
//        {
//            if(groupInfo[nowIndex] == groupInfo[index])
//            {
//                //TODO:如果是按照正常程序做的话，这部分可能不能这么写，不用去计算distance了
//                distance = CalPeoDistance(peo[nowIndex], peo[index]);
//                dupCnt[distance]++;
//            }
//        }
//        else
//        {
//            distance = CalPeoDistance(peo[nowIndex], peo[index]);
//            hitCnt[distance]++;
//            if()
//        }
//    }
//	return true;
//}


void SearchInRTree()
{
    clock_t clockSearchBegin = clock();
	for (int i = 0; i < N; i++)
	{
        clock_t clockSearchBegin = clock();
        memset(notInGroup, 0, sizeof(notInGroup));
		nowIndex = i;
        int searchNum = tree.Search(RMin, RMax, SearchCallbackTotal, NULL);
        clock_t clockSearchEnd = clock();
        printf("index = %7d, spent %lf seconds.\n", i, (double)(clockSearchEnd - clockSearchBegin)/1000.0);
        if(i % 100 == 99)
        {
            for(int j = 0; j < MaxDistance; j++)
            {
                printf("now dis = %d, hit = %lld, dup = %lld\n", j, hitCnt[j], dupCnt[j]);
            }
        }
    }
    clock_t clockSearchEnd = clock();
    printf("\nSearch with biggest rectangle spent %lf seconds.\n\n", (double)(clockSearchEnd - clockSearchBegin)/1000.0);
    printf("Search in RTree finished.\n");
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
    return;
}

bool SearchCallbackInDiffRecall(int index, void* arg)
{
	if(nowIndex > index && indexToId[index] == peo[nowIndex].id)
	{
        dupRecall ++;
	}
	return true;
}

void CalSearchTimeInDiffRecall()
{
    int a[ExperimentDimensionNum * CharNum];
    int b[ExperimentDimensionNum * CharNum];
    for(int distance = 0; distance < 30; distance ++)
    {
        clock_t clockSearchBegin = clock();
        dupRecall = 0;
        for (int i = 0; i < N; i++)
	    {
            clock_t clockOneSearchBegin = clock();
		    for (int j = 0; j < (ExperimentDimensionNum * CharNum); j++)
		    {
                a[j] = peo[i].field[j] - distance;
			    b[j] = peo[i].field[j] + distance;
		    }
		    nowIndex = i;
		    int tmp = tree.Search(a, b, SearchCallbackInDiffRecall, NULL);
            clock_t clockOneSearchEnd = clock();
            if(i % 1000 == 0)
                printf("dis = %d, index = %d, spent %lf seconds, now dup = %d\n", distance, i, (double)(clockOneSearchEnd - clockOneSearchBegin)/1000.0), dupRecall;
	    }
        clock_t clockSearchEnd = clock();
        printf("distance in every dimension = %2d, search out = %d, recall = %lf, ", distance, dupRecall, (double)dupRecall/dupCount);
        printf("spent %lf seconds.\n", (double)(clockSearchEnd - clockSearchBegin)/1000.0);
        if(dupRecall == dupCount)
            break;
    }
    return;
}

int main()
{
    freopen(".txt","r",stdin);
    freopen(".txt","w",stdout);

    srand((unsigned)time(NULL));
	Input();
    Init();
    //按照实际id排序
    //降低CountAllTheDup的时间复杂度
	//sort(peo,peo+N,CmpById);
    //找出实际存在的重复记录对数
	//dupCount=CountAllTheDup();
	//printf("dupcount=%d\n",dupCount);
    //插入R树
    InsertToRTree();
    //在R树中搜索
    SearchInRTree();
    //输出结果
    Output();
    //计算不同召回率的搜索时间
    //CalSearchTimeInDiffRecall();
    return 0;
}
