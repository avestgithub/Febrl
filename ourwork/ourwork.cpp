//ourwork
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
#define FIELD 17


struct people
{
    /*
    culture3
    title2
    social_security_ID2
    postcode2
    phone_number2
    address2
    surname2
    given_name2
    */
    int num;
    bool type;//org==0&&dup==1
    int dupid;
    
    double field[FIELD];
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

		for(int j = 0; j < FIELD; j++)
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

/*
bool cmpByKey(people px, people py)
{
	return strcmp(px.cul , py.cul) < 0;
	return strcmp(px.title , py.title) < 0;
	return px.id < py.id;
	return px.post < py.post;
	return px.phone < py.phone;
	return strcmp(px.add , py.add) < 0;
	return strcmp(px.sur , py.sur) < 0;
	return strcmp(px.given , py.given) < 0;
}
*/

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

int main()
{

    freopen("dataset2000extract2.txt","r",stdin);
    freopen("dataset2000ourwork.txt","w",stdout);

	int i, j;
	input();
	sort(peo,peo+N,cmpDup);
	int dupCount=CountAllTheDup();
	printf("dupcount=%d\n",dupCount);
	//output();
	

	//int callBack[N+10];
	//memset(callBack,sizeof(callBack),0);


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

	int callBack = 0;
	int compareCount = 0;

	for(int Size = 0; Size < N; Size++)
	{
		for(i=0;i<N;i++)
		{

			//i < peoDis[i][j].num保证不会重复计算
			if(i < peoDis[i][Size].num)
			{
				if(peo[i].num==peo[peoDis[i][Size].num].num )
				{
					callBack++;
				}
				compareCount ++;
			}
		}
		//还没算上 * (int)log((double)N)
        //printf("%lf,%9.8lf\n",log10((double)compareCount),(double)callBack/dupCount);
		printf("for %8d comparasion , we find %8d dups",compareCount,callBack);
		printf(" , the recall is %9.8lf\n",(double)callBack/dupCount);
	}

    return 0;
}
