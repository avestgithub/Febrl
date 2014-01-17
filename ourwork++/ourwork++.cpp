//ourwork++
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<algorithm>
#include<math.h>
using namespace std;
#define INF 2100000000


//dataset2_1000_1000_3_2_4_zipf_all_1
//dataset2_5000_5000_9_5_5_zipf_all_1
#define N 2000
#define THRESHOLD 250
#define FIELD 14


struct people
{
	int num;
    int type;//org==0&&dup==1
    int dupid;

	double field[14];
}peo[N+10];



struct peoDistance
{
	int dis;
	int num;
};


void input()
{
	int i;
    int cnt = 0;
    char temps[1000];
    char tempc = '\0';
	int tempi=-1;
	gets(temps);//the header

    for(i = 0; i < N; i++)
    {
		scanf("%d",&peo[i].num);
		scanf("%d",&peo[i].type);
		scanf("%d", &peo[i].dupid);

		for(int j = 0; j < 14; j++)
		{
			scanf("%lf", &peo[i].field[j]);
		}
	}
}


int CountAllTheDup()
{
	int i,j;
	int dupCount=0;
	for(i=0;i<N;i++)
	{
		for(j=i+1;j<N;j++)
		{
			if(peo[i].num!=peo[j].num)
			{
				continue;
			}
			dupCount++;
		}
	}
	return dupCount;
}


bool cmpByKey(people px, people py)
{
	for(int i = FIELD-1; i >= 0; i--)
	{
		return px.field[i] < py.field[i];
	}
}



bool cmpDup(people px, people py)
{
	return px.num < py.num;
	return px.type < py.type;
	return px.dupid < py.dupid;
}


bool cmpDis(peoDistance dx, peoDistance dy)
{
	return dx.dis < dy.dis;
}


int calPeoDistance(people px, people py)
{
	int distance=0;
	for (int i = 0; i < FIELD; i++)
	{
		distance += abs(px.field[i] - py.field[i]);
	}
	return distance;
}


peoDistance peoDis[N+10][N+10];


struct PeoAve
{
    double field[14];
}peoAve[N + 10] = {0};


double merge(int exist, double oriNums, double newNum)
{
	return (oriNums* ((double)exist) + newNum)/((double)exist + 1);
}


void MergeIntoAve(int index, people p, int exist)
{

	for(int i = 0; i < FIELD; i++)
	{
		if(p.field[i] == -1)
		{
			continue;
		}
		if(peoAve[index].field[i] == 0)
		{
			peoAve[index].field[i] = p.field[i];
		}
		else
		{
			peoAve[index].field[i] = merge(exist, peoAve[index].field[i], p.field[i]);
		}
	}
	return;
}

bool IsNearAve(int index, people p, double thershole)
{
	double dis = 0.0;

	for (int i = 0; i < FIELD; i++)
	{
		if (p.field[i] == -1)
		{
			continue;
		}
		else
		{
			dis += fabs(peoAve[index].field[i] - p.field[i]);
		}
	}
	if(dis < thershole)
		return 1;
	return 0;
}



int main()
{

    freopen("dataset2000extract2.txt","r",stdin);
    freopen("dataset2000ourwork++.txt","w",stdout);

	int i, j;
	input();
	sort(peo,peo+N,cmpDup);
	int dupCount=CountAllTheDup();
	printf("dupcount=%d\n",dupCount);
	
	for(i = 0; i < N; i++)
	{
		for(j = 0; j < N; j++)
		{
            peoDis[i][j].num = j;//这个序号，是目前在数组中的序号，要是重新排序，会变
            if(i == j)
            {
                peoDis[i][j].dis = INF;
            }
			else
			{
			     peoDis[i][j].dis = calPeoDistance(peo[i], peo[j]);
            }
		}
		sort(peoDis[i], peoDis[i]+N, cmpDis);
	}

	memset(peoAve,0,sizeof(peoAve));

	for(int i = 0; i < N; i++)
	{
		MergeIntoAve(i, peo[i], 0);
	}

	int compareCount = 0;
    int callBack = 0;

	int need[N + 10] = {0};
	int cnt[N + 10] = {0};


	for(int Size = 0; Size < N; Size++)
	{
		for(i=0;i<N;i++)
		{
			if(need[i] == 0 && IsNearAve(i, peo[peoDis[i][Size].num], THRESHOLD) == 1)
			{
				//i < peoDis[i].[j].num保证不会重复计算
				if(i < peoDis[i][Size].num)
				{
					if(peo[i].num==peo[peoDis[i][Size].num].num)
					{
						MergeIntoAve(i, peo[peoDis[i][Size].num], cnt[i]+1);
						cnt[i]++;
						callBack++;
					}
					compareCount ++;
				}
				
			}
			else
			{
				need[i] = 1;
			}

		}
        //int compareCount = (N + 1) * Size;
        //printf("%lf,%9.8lf\n",log10((double)compareCount),(double)callBack/dupCount);
		printf("for %8d comparasion , we find %8d dups",compareCount,callBack);
		printf(" , the recall is %9.8lf\n",(double)callBack/dupCount);
	}

    return 0;
}
