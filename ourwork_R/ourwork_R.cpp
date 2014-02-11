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

//��¼���������ظ���¼��
#define N 10000
#define AllDimesionNum 16
#define DimensionNum 4
#define CharNum 2
#define MaxLength 20

#define MaxDistance 900
#define FIELD_VALUE_MIN -1
#define FIELD_VALUE_MAX 26

//TODO:������index����һ��int����ǰ��ͺ���λ
//��ΪĿǰMySearchCallback����ֻ�ܴ���һ������id
//��Ҫ�ı��������
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


//�����б�
void Input();
void RecordInit();
int CountAllTheDup();
bool CmpById(people px, people py);
int CalPeoDistance(people px, people py);
void InsertToRTree();
bool SearchCallbackTotal(int id, void* arg);
void SearchInRTree();
void Output();


//ȫ�ֱ���
int dupCount;
int nowIndex = 0;
int hitCnt[1000];
int dupCnt[1000];
clock_t clockBegin, clockInsertEnd, clockEnd;
RTree<int, int, DimensionNum * CharNum, float> tree;
int dupRecall;


//���뺯��
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


//��ʼ������
//��record�е�ԭʼ��Ϣѡȡ���뵽field��
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


//�ҳ�����ʵ�ʴ��ڵ��ظ���¼��
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


//�Լ�¼����ʵ��id��������
bool CmpById(people px, people py)
{
    if(px.id != py.id)
	    return px.id < py.id;
    if(px.type != py.type)
	    return px.type < py.type;
    if(px.dupid != py.dupid)
	    return px.dupid < py.dupid;
}


//����������¼��ľ���
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
    //��������R����
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
                //TODO:��ʱ�����R��ʱ������������Ҫ�������ȥ����
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

bool SearchCallbackTotal(int id, void* arg)
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
		int tmp = tree.Search(a, b, SearchCallbackTotal, NULL);
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
		printf("for %8d comparasion , we find %8d dups", hitCnt[i], dupCnt[i]);
		printf(" , the recall is %9.8lf\n",(double)dupCnt[i]/dupCount);
        if(dupCnt[i] == dupCount)
            break;
	}
    printf("\nInsert operation spent %lf seconds.\n", (float)(clockInsertEnd - clockBegin)/1000.0);
    printf("\nALL search operations spent %lf seconds.\n", (float)(clockEnd - clockInsertEnd)/1000.0);
    return;
}

bool SearchCallbackInDiffRecall(int mutiId, void* arg)
{
	int index = mutiId % MULTIPLY_NUMBER;
	int id = mutiId / MULTIPLY_NUMBER;
    
	if(id == peo[nowIndex].id && nowIndex > index)
	{
        dupRecall ++;
	}
	return true;
}

void CalSearchTimeInDiffRecall()
{
    //double recallList[3] = {90.0, 95.0, 100.0};
    //for(int i = 0; i < 3; i++)
    //{
    //    int windowWidth = -1;
    //    for(int distance = 0; distance < MaxDistance; distance++)
    //    {
    //        if((double)(dupCnt[distance]/dupCount) >= recallList[i])
    //        {
    //            windowWidth = distance;
    //            break;
    //        }
    //    }
    //}
    for(int distance = 0; distance < 30; distance ++)
    {
        clock_t clockSearchBegin = clock();
        dupRecall = 0;
        for (int i = 0; i < N; i++)
	    {
		    int a[DimensionNum * CharNum];
		    int b[DimensionNum * CharNum];
		    for (int j = 0; j < (DimensionNum * CharNum); j++)
		    {
                a[j] = peo[i].field[j] - distance;
			    b[j] = peo[i].field[j] + distance;
		    }
		    nowIndex = i;
		    int tmp = tree.Search(a, b, SearchCallbackInDiffRecall, NULL);
	    }
        clock_t clockSearchEnd = clock();
        printf("distance = %2d, recall = %lf, ", distance, (double)dupRecall/dupCount);
        printf("spent %lf seconds.\n", (float)(clockSearchEnd - clockSearchBegin)/1000.0);
    }
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
    //����ʵ��id����
    //TODO:���ô���
	sort(peo,peo+N,CmpById);
    //�ҳ�ʵ�ʴ��ڵ��ظ���¼����
	dupCount=CountAllTheDup();
	printf("dupcount=%d\n",dupCount);
    //����R��
    InsertToRTree();
    //��R��������
    SearchInRTree();
    //������
    Output();
    //���㲻ͬ�ٻ��ʵ�����ʱ��
    CalSearchTimeInDiffRecall();
    return 0;
}
