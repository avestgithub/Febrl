//DCS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<algorithm>
#include<math.h>
#include <time.h>
using namespace std;


#define N 10000
#define Wmin 2
#define Wmax N-1


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
    
    char cul[4];
    char title[3];

    int id;
    int post;
    int phone;

    char add[3];
    char sur[3];
    char given[3];
}peo[N+10];

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

		scanf("%s",peo[i].cul);//cul, //char3
		scanf("%s",peo[i].title);//title, //char2

		scanf("%d",&peo[i].id);//soc_sec_id, //int2
		scanf("%d",&peo[i].post);//postcode, //int2
		scanf("%d",&peo[i].phone);//phone_number, //int2

		scanf("%s",peo[i].add);//address_1, //char2
		scanf("%s",peo[i].sur);//surname, //char2
		scanf("%s",peo[i].given);//given_name, //char2

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
				break;
			}
			dupCount++;
		}
	}
	return dupCount;
}


bool cmpByKey(people px, people py)
{
    if(strcmp(px.given , py.given) != 0)
        return strcmp(px.given , py.given) < 0;
    if(strcmp(px.sur , py.sur) != 0)
        return strcmp(px.sur , py.sur) < 0;
    //if(strcmp(px.add , py.add) != 0)
    //    return strcmp(px.add , py.add) < 0;
    //if(px.phone != py.phone)
    //    return px.phone < py.phone;
    //if(px.post != py.post)
    //    return px.post < py.post;
    //if(px.id != py.id)
    //    return px.id < py.id;
    //if(strcmp(px.cul , py.cul) != 0)
	   // return strcmp(px.cul , py.cul) < 0;
    //if(strcmp(px.title , py.title) != 0)
	   // return strcmp(px.title , py.title) < 0;
    return 0;
}


bool cmpDup(people px, people py)
{
	return px.num < py.num;
	return px.type < py.type;
	return px.dupid < py.dupid;
}


int compareCount = 0;


int countDcsDup(int winSize)
{
    int i,j;
    int dupCount = 0;
	compareCount = 0;
	double theta = (1/(double)(winSize-1));
    for(j = 0; j < N-2; j++)
    {
        int nowDupCount = 0;
        int nowCompareCount = 0;
        
        for(int k = 1; ;)
        {
            if(peo[j].num == peo[j + k].num)
            {
                dupCount++;
                nowDupCount++;
            }
            compareCount++;
            nowCompareCount++;
            if((j + k) >= (N-1))
            {
                break;
            }
            
            else if((k >= winSize-1) && ((double)((double)nowDupCount/(double)nowCompareCount) >= theta))
            {
                k++;
            }
            else if(k <= winSize-1)
            {
                k++;
            }
            else
            {
                break;
            }
        }
    }
    return dupCount;
}


clock_t clockBegin, clockEnd;

int main()
{

    freopen("dataset10000extract.txt","r",stdin);
    freopen("dataset10000dcs2D.txt","w",stdout);

	int i;
	input();
    clockBegin = clock();
	sort(peo,peo+N,cmpDup);
	int dupCount=CountAllTheDup();
	printf("dupcount=%d\n",dupCount);
	sort(peo,peo+N,cmpByKey);
	for(int winSize = Wmin; winSize <= Wmax; winSize++)
	{
		int dcsDupCount = countDcsDup(winSize);
		//printf("%lf,%9.8lf\n",log10((double)compareCount),(double)dcsDupCount/dupCount);
		printf("for %8d comparasion , we find %8d dups",compareCount,dcsDupCount);
		printf(" , the recall is %9.8lf\n",(double)dcsDupCount/dupCount);
        if(dcsDupCount == dupCount)
            break;
	}
    clockEnd = clock();
    printf("\nSpent %lf seconds.\n", (float)(clockEnd - clockBegin)/1000.0);
    return 0;
}
