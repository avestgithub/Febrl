//SNM算法
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<algorithm>
#include<math.h>
using namespace std;


#define N 2000
#define DimensionNum 16
#define MaxLength 20

#define Wmin 2
#define Wmax N-1

struct people
{
    int id;
    //org==0&&dup==1
    bool type;
    int dupid;

    int record[DimensionNum][MaxLength];
}peo[N + 10];


//输入函数
void input()
{
    int cnt = 0;
    char temps[1000];
    char tempc = '\0';
	int tempi=-1;
	gets(temps);//the header

    for(int i = 0; i < N; i++)
    {
        scanf("%d",&peo[i].id);
		scanf("%d",&peo[i].type);
		scanf("%d", &peo[i].dupid);

        for(int j = 0; j < DimensionNum; j++)
        {
            for(int k = 0; k < MaxLength; k++)
            {
                scanf("%d", &peo[i].record[j][k]);
            }
        }
	}
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
			if(peo[i].id!=peo[j].id)
			{
				break;
			}
			dupCount++;
		}
	}
	return dupCount;
}


bool cmpByKey(people px, people py)
{
    for(int j = 0; j < DimensionNum; j++)
    {
        for(int k = 0; k < MaxLength; k++)
        {
            //把判断去掉，使得程序和之前同步
            //if(px.record[j][k] != py.record[j][k])
                return px.record[j][k] < py.record[j][k];
        }
    }
    return 0;
}

//对记录按照id进行排序
bool cmpDup(people px, people py)
{
    if(px.id != py.id)
	    return px.id < py.id;
    if(px.type != py.type)
	    return px.type < py.type;
    if(px.dupid != py.dupid)
	    return px.dupid < py.dupid;
}


int compareCount = 0;

int countSnmDup(int winSize)
{
	int i,j;
	int dupCount = 0;
	compareCount = 0;
	for(i = 0; i < N; i++)
	{
		for(j = i + 1; (j < N) && (i + winSize > j); j++)
		{
			compareCount ++;
			if(peo[i].id == peo[j].id)
				dupCount ++;
		}
	}
	return dupCount;
}



int main()
{

    freopen("dataset2000extractALLDimensions.txt","r",stdin);
    freopen("dataset2000snm.txt","w",stdout);

	int i;
	input();
	sort(peo,peo+N,cmpDup);
	int dupCount=CountAllTheDup();
	printf("dupcount=%d\n",dupCount);
	sort(peo,peo+N,cmpByKey);
	for(int winSize = Wmin; winSize <= Wmax; winSize++)
	{
		int snmDupCount = countSnmDup(winSize);
		//printf("%lf,%9.8lf\n",log10((double)compareCount),(double)snmDupCount/dupCount);
		printf("for %8d comparasion , we find %8d dups",compareCount,snmDupCount);
		printf(" , the recall is %9.8lf\n",(double)snmDupCount/dupCount);
	}
    return 0;
}
