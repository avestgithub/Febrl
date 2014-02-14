//抽取key信息 
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<algorithm>
using namespace std;

//dataset2_1000_1000_3_2_4_zipf_all_1
//dataset2_5000_5000_9_5_5_zipf_all_1

#define N 10000


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
}peo[N + 10];

bool CmpById(people px, people py)
{
    if(px.num != py.num)
	    return px.num < py.num;
    if(px.type != py.type)
	    return px.type < py.type;
    if(px.dupid != py.dupid)
	    return px.dupid < py.dupid;
}

int main()
{
    freopen("dataset10000clean.txt","r",stdin);
    freopen("dataset10000extract.txt","w",stdout);
    

    int i, j;
    int cnt = 0;
    char temps[1000];
    char tempc;
	int tempi=-1;
    
	for(i=0;i<N;i++)
	{
		peo[i].cul[0]='*';
		peo[i].cul[1]='*';
		peo[i].cul[2]='*';
		peo[i].title[0]='*';
		peo[i].title[1]='*';
		peo[i].id=-1;
		peo[i].post=-1;
		peo[i].phone=-1;
		peo[i].add[0]='*';
		peo[i].add[1]='*';
		peo[i].sur[0]='*';
		peo[i].sur[1]='*';
		peo[i].given[0]='*';
		peo[i].given[1]='*';
	}

	printf("num typ dup cul tit id pos pho add sur giv\n");


    for(i = 0; i < N; i++)
    {
        scanf("rec-%d-", &peo[i].num);
        scanf("%c", &tempc);
		
        if(tempc == 'o')
        {
            peo[i].type = 0;
			scanf("%s",temps);
        }
        else
        {
            peo[i].type = 1;
            scanf("up-%d", &peo[i].dupid);
        }

		scanf("%s",temps);//rec2
		scanf("%s",temps);//cul
		if(temps[0]!='-')
		{
			peo[i].cul[0]=temps[0];
			peo[i].cul[1]=temps[1];
			peo[i].cul[2]=temps[2];
		}
		scanf("%s",temps);//sex
		scanf("%s",temps);//age
		scanf("%s",temps);//date_of_birth, 

		scanf("%s",temps);//title, //char2
		if(temps[0]!='-')
		{
			peo[i].title[0]=temps[0];
			peo[i].title[1]=temps[1];
		}

		scanf("%s",temps);//given_name, //char2
		if(temps[0]!='-')
		{
			peo[i].given[0]=temps[0];
			peo[i].given[1]=temps[1];
		}
		scanf("%s",temps);//surname, //char2
		if(temps[0]!='-')
		{
			peo[i].sur[0]=temps[0];
			peo[i].sur[1]=temps[1];
		}
		scanf("%s",temps);//state, 
		
		scanf("%s",temps);//suburb, 

		scanf("%d",&tempi);//postcode, //int2
		if(tempi!=-1)
		{
			peo[i].post=tempi%100;
		}
		scanf("%s",temps);//street_number, 

		scanf("%s",temps);//address_1, //char2
		if(temps[0]!='-')
		{
			peo[i].add[0]=temps[0];
			peo[i].add[1]=temps[1];
		}
		scanf("%s",temps);//address_2, 

		scanf("%d",&tempi);//phone_number, //int2
		if(tempi!=-1)
		{
			peo[i].phone=tempi%100;
		}
		scanf("%d",&tempi);//soc_sec_id, //int2
		if(tempi!=-1)
		{
			peo[i].id=tempi%100;
		}
		scanf("%s",temps);//blocking_number, 

		//scanf("%s",temps);//family_role
		gets(temps);
    }

    //sort(peo, peo + N, CmpById);

    for(int i = 0; i < N; i++)
    {
        printf("%3d %3d %3d ",peo[i].num, peo[i].type, peo[i].dupid);
		printf("%3s %3s ",peo[i].cul,peo[i].title);
		printf("%3d %3d %3d ",peo[i].id, peo[i].post, peo[i].phone);
		printf("%3s %3s %3s ",peo[i].add, peo[i].sur, peo[i].given);
		printf("\n");
    }

    //统计各个维度的扰乱程度
    //int disturbNum[8];
    //memset(disturbNum, 0, sizeof(disturbNum));
    //for(int i = 0; i < N; i++)
    //{
    //    for(int j = 0; j < N; j++)
    //    {
    //        if(peo[i].num == peo[j].num && peo[i].type == 0 && peo[j].type == 1)
    //        {
    //            printf("111\n");
    //            if(strcmp(peo[i].cul, peo[j].cul) != 0)
    //                disturbNum[0]++;
    //            if(strcmp(peo[i].title, peo[j].title) != 0)
    //                disturbNum[1]++;
    //            if(peo[i].id != peo[j].id)
    //                disturbNum[2]++;
    //            if(peo[i].post != peo[j].post)
    //                disturbNum[3]++;
    //            if(peo[i].phone != peo[j].phone)
    //                disturbNum[4]++;
    //            if(strcmp(peo[i].add, peo[j].add) != 0)
    //                disturbNum[5]++;
    //            if(strcmp(peo[i].sur, peo[j].sur) != 0)
    //                disturbNum[6]++;
    //            if(strcmp(peo[i].given, peo[j].given) != 0)
    //                disturbNum[7]++;
    //        }
    //    }
    //}
    //for (int i = 0; i < 8; i++)
    //{
    //    printf("disturb %d = %d\n", i, disturbNum[i]);
    //}


    return 0;
}
