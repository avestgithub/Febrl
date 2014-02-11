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
#define N 10000
#define AllDimesionNum 16
#define DimensionNum 4
#define CharNum 2
#define MaxLength 20

#define FIELD_VALUE_MIN -1
#define FIELD_VALUE_MAX 26

//TODO:将两个index放在一个index的前五和后五位
//因为目前MySearchCallback函数只能传进一个参数id
//需要改变这个做法
#define MULTIPLY_NUMBER 100000

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
bool MySearchCallback(int id, void* arg);
void SearchInRTree();
void Output();


//全局变量
int dupCount;
int nowIndex = 0;
int hitCnt[1000];
int dupCnt[1000];
clock_t clockBegin, clockInsertEnd, clockEnd;
RTree<int, int, DimensionNum * CharNum, float> tree;


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
                if(peo[i].record[j][k] > 25)
                    printf("%d\n", peo[i].record[j][k]);
            }
        }
	}
}


//初始化函数
//将record中的原始信息选取导入到field中
void RecordInit()
{
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
		if(px.field[i] != -1 && py.field[i] != -1)
		{
			distance += abs(px.field[i] - py.field[i]);
		}
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
                a[j] = peo[i].field[j] + rand()%2;
                b[j] = a[j];
			}
		}
        //printf("%d %d %d\n", peo[i].id, peo[i].type, peo[i].dupid);
        //for (int j = 0; j < DimensionNum * CharNum; j++)
        //{
        //    printf("%d ", a[j]);
        //}
        //puts("");
		tree.Insert(a, b, (peo[i].id*MULTIPLY_NUMBER + i));
	}
    clockInsertEnd = clock();
    return;
}

bool MySearchCallback(int id, void* arg)
{
	int index = id % MULTIPLY_NUMBER;
	int num = id / MULTIPLY_NUMBER;

	int distance = CalPeoDistance(peo[nowIndex], peo[index]);
    //printf("id = %d nowindex = %d index = %d distance = %d\n", id, nowIndex, index, distance);
    if(nowIndex > index)
	    hitCnt[distance]++;
	if(num == peo[nowIndex].id && nowIndex > index)
	{
		dupCnt[distance]++;
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
		int a[DimensionNum * CharNum];
		int b[DimensionNum * CharNum];
		for (int j = 0; j < (DimensionNum * CharNum); j++)
		{
			a[j] = FIELD_VALUE_MIN;
			b[j] = FIELD_VALUE_MAX;
		}
		nowIndex = i;
		int tmp = tree.Search(a, b, MySearchCallback, NULL);
	}
    clockEnd = clock();
    return;
}

void Output()
{
    for(int i = 0; i <= 900; i++)
	{
		hitCnt[i] += hitCnt[i-1];
		dupCnt[i] += dupCnt[i-1];
		printf("for %8d comparasion , we find %8d dups", hitCnt[i], dupCnt[i]);
		printf(" , the recall is %9.8lf\n",(double)dupCnt[i]/dupCount);
	}
    printf("\nInsert operation spent %lf seconds.\n", (float)(clockInsertEnd - clockBegin)/1000.0);
    printf("\nALL search operations spent %lf seconds.\n", (float)(clockEnd - clockInsertEnd)/1000.0);
    return;
}

int main()
{
    freopen("dataset10000extractALLDimensions.txt","r",stdin);
    freopen("dataset10000R4D.txt","w",stdout);
    srand((unsigned)time(NULL));
    
	int i,j;
	Input();
    RecordInit();
	sort(peo,peo+N,CmpById);
    //find all existed dups
	dupCount=CountAllTheDup();
	printf("dupcount=%d\n",dupCount);
    //插入R树
    InsertToRTree();
    //在R树中搜索
    SearchInRTree();
    //输出结果
    Output();
    return 0;   
}
