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
#include <map>
using namespace std;
#define INF 2100000000

//��¼���������ظ���¼��
#define N 100000
#define ExtractDimesionNum 10
#define ExperimentDimensionNum 10
#define CharNum 2
#define MachineNum 10

#define MaxDistance 550
#define FIELD_VALUE_MIN -1
#define FIELD_VALUE_MAX 25


struct people
{
    int id;
    //org==0&&dup==1
    bool type;
    int dupid;

    int record[ExtractDimesionNum][CharNum];
    int field[ExperimentDimensionNum * CharNum];
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
int dupCount = 269466;
//100w:2701376
//10w:269466
//1w:16431
//�Ӽ�¼��N����¼�е���ţ�����¼����id��ӳ���
int indexToId[N];
int nowIndex = 0;
long long hitCnt[MaxDistance];
long long dupCnt[MaxDistance];

RTree<int, int, ExperimentDimensionNum * CharNum, float> tree[MachineNum];

int RMin[ExperimentDimensionNum * CharNum];
int RMax[ExperimentDimensionNum * CharNum];

set<long long> entireSet;
set<long long> machineSet;


//TODO:��ɼ�֦����
//int groupInfo[N];
//int groupNum;
//bool notInGroup[N];

//����
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
                //��֤û�г���25���ֶ�
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


//��ʼ��
void Init()
{
    clock_t clockInitBegin = clock();
    //��record�е�ԭʼ��Ϣѡȡ���뵽field��
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
    //�鿴field��ֵ
    //for (int i = 0; i < N; i++)
    //{
    //    printf("%d %d ", peo[i].id, peo[i].dupid);
    //    for (int j = 0; j < ExperimentDimensionNum * CharNum; j++)
    //    {
    //        printf("%d", peo[i].field[j]);
    //    }
    //    puts("");
    //}
    //Init
    memset(indexToId, -1, sizeof(indexToId));
    for (int i = 0; i < (ExperimentDimensionNum * CharNum); i++)
	{
		RMin[i] = FIELD_VALUE_MIN;
		RMax[i] = FIELD_VALUE_MAX;
	}
    memset(hitCnt, 0, sizeof(hitCnt));
    memset(dupCnt, 0, sizeof(dupCnt));

    clock_t clockInitEnd = clock();
    printf("\nInit operation spent %lf seconds.\n", (double)(clockInitEnd - clockInitBegin)/1000.0);
    printf("Init finished.\n");
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
            else
            {
                break;
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


void InsertToRTree(int machineId)
{
    //��������R����
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
                if(dupRand < 3)
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
    printf("\nInsert machine %d spent %lf seconds.\n", machineId, (double)(clockInsertEnd - clockInsertBegin)/1000.0);
    return;
}


//����������¼��ľ���
int CalPeoDistance(people px, people py)
{
	int distance=0;
    for (int i = 0; i < ExperimentDimensionNum * CharNum; i++)
	{
		distance += abs(px.field[i] - py.field[i]);
	}
	return distance;
}

//ʹ�����������R������
//bool SearchCallbackTotal(int index, void* arg)
//{
//    if(nowIndex > index)
//    {
//        int distance = CalPeoDistance(peo[nowIndex], peo[index]);
//        hitCnt[distance]++;
//        if(indexToId[index] == peo[nowIndex].id)
//	    {
//		    dupCnt[distance]++;
//	    }
//    }
//	return true;
//}
//void SearchInRTree()
//{
//    clock_t clockSearchBegin = clock();
//	for (int i = 0; i < N; i++)
//	{
//        clock_t clockSearchBegin = clock();
//		nowIndex = i;
//        int searchNum = tree.Search(RMin, RMax, SearchCallbackTotal, NULL);
//        clock_t clockSearchEnd = clock();
//        printf("index = %7d, spent %lf seconds.\n", i, (double)(clockSearchEnd - clockSearchBegin)/1000.0);
//        //if(i % 1000 == 999)
//        //{
//        //    for(int j = 0; j < MaxDistance; j++)
//        //    {
//        //        printf("now dis = %d, hit = %lld, dup = %lld\n", j, hitCnt[j], dupCnt[j]);
//        //    }
//        //}
//    }
//    clock_t clockSearchEnd = clock();
//    printf("\nSearch with biggest rectangle spent %lf seconds.\n\n", (double)(clockSearchEnd - clockSearchBegin)/1000.0);
//    printf("Search in RTree finished.\n");
//    return;
//}

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
	//if(nowIndex > index && indexToId[index] == peo[nowIndex].id)
    if(nowIndex > index)
	{
        long long tmp = nowIndex * 100000 + index;
        machineSet.insert(tmp);
	}
	return true;
}

void CalSearchTimeInDiffRecall()
{
    int a[ExperimentDimensionNum * CharNum];
    int b[ExperimentDimensionNum * CharNum];
    for(int distance = 16; distance <= 16; distance ++)
    {
        printf("\nbegin, distance = %d\n", distance);
        entireSet.clear();  
        for(int machineId = 0; machineId <= 9; machineId ++)
        {
            InsertToRTree(machineId);
        }
        for(int machineId = 0; machineId <= 9; machineId ++)
        {
            machineSet.clear();
            for (int i = 0; i < N; i++)
	        {
		        for (int j = 0; j < (ExperimentDimensionNum * CharNum); j++)
		        {
                    a[j] = peo[i].field[j] - distance;
			        b[j] = peo[i].field[j] + distance;
		        }
		        nowIndex = i;
                int tmp = tree[machineId].Search(a, b, SearchCallbackInDiffRecall, NULL);
                if(i % 1000 == 999)
                    printf("index = %d, now set = %d\n", i, machineSet.size());
	        }
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
            printf("machine %d has %d candicates\n", machineId, machineSet.size());
            printf("the size of overall entireSet = %d\n",entireSet.size());
            if(machineSet.size() == dupCount)
                break;
        }
    }
    return;
}

int main()
{
    freopen("dataset2_20000_80000_9_1_1_zipf_all_0_extract10D2Char.txt","r",stdin);
    //freopen("0.03_0-25_0.txt","w",stdout);

    srand((unsigned)time(NULL));
	Input();
    Init();
    //����ʵ��id����
    //����CountAllTheDup��ʱ�临�Ӷ�
	sort(peo,peo+N,CmpById);
    //�ҳ�ʵ�ʴ��ڵ��ظ���¼����
	dupCount=CountAllTheDup();
	printf("dupcount=%d\n",dupCount);

    CalSearchTimeInDiffRecall();
    
    //��R��������
    //SearchInRTree();

    //������
    //Output();

    //���㲻ͬ�ٻ��ʵ�����ʱ��
    //CalSearchTimeInDiffRecall();



    return 0;
}


//TODO:��ɼ�֦����
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
//            //TODO:����һ����������ɲ����ܳ���
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
//                //TODO:����ǰ��������������Ļ����ⲿ�ֿ��ܲ�����ôд������ȥ����distance��
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