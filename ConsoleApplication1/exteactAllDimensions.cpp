//抽取key信息 
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<algorithm>
using namespace std;


#define N 1000000
#define AllDimensionNum 16
#define MaxCharLength 20

#define ExtractDimensionNum 10
#define ExtractCharNum 2


struct people
{
    int id;
    //org==0&&dup==1
    bool type;
    int dupid;
    int record[AllDimensionNum][MaxCharLength];
}peo[N + 10];


//全部置零
void init()
{
    int i,j,k;
    for(int i = 0; i < N; i++)
    {
        for(int j = 0; j < AllDimensionNum; j++)
        {
            for(int k = 0; k < MaxCharLength; k++)
            {
                peo[i].record[j][k] = -1;
            }
        }
    }
    return;
}


int main()
{
    freopen("dataset2_200000_800000_9_5_5_zipf_all_0_clean.txt","r",stdin);
    freopen("dataset2_200000_800000_9_5_5_zipf_all_0_extract10D2Char.txt","w",stdout);
    

    int cnt = 0;
    char temps[1000];
    char tempc;
	int tempi=-1;
    
    init();

    //old
	//printf("num, typ, dup, culture, sex, age, date_of_birth, title, surname, given_name, state, suburb, postcode, street_number, address_1, address_2, phone_number, soc_sec_id, blocking_number\n");
    //new
    //TODO:补上顺序
    printf("TODO:Fill in the names of dimensions\n");
    for(int i = 0; i < N; i++)
    {
        //rec_id
        scanf("rec-%d-", &peo[i].id);
        scanf("%c", &tempc);
        if(tempc == 'o')
        {
            peo[i].type = 0;
            peo[i].dupid = -1;
			scanf("%s",temps);
        }
        else
        {
            peo[i].type = 1;
            scanf("up-%d", &peo[i].dupid);
        }

        //rec2_id
		//scanf("%s",temps);

        //all 16 dimensions
        for(int j = 0; j < AllDimensionNum; j++)
        {
            scanf("%s", temps);
            int len = strlen(temps);
            //“-1”为空值
            if(temps[0] == '-' || len == 0)
                continue;
            for(int k = 0; k < len; k++)
            {
                if(temps[k] >= '0' && temps[k] <= '9')
                {
                    //数字取后两位，和前期实验保持一致
                    peo[i].record[j][len - k - 1] = temps[k] - '0';
                }
                else if(temps[k] >= 'a' && temps[k] <= 'z')
                {
                    peo[i].record[j][k] = temps[k] - 'a';
                }
            }
        }

        //最后一维
		//family_role
        //没用的，舍弃
		gets(temps);
		
        //output info
        printf("%7d %2d %2d   ",peo[i].id, peo[i].type, peo[i].dupid);
        
        //老的顺序
        /*
        先按照这个顺序排列，然后其他维度再排列
        culture3
        title2
        social_security_ID2
        postcode2
        phone_number2
        address2
        surname2
        given_name2
        */
        //int sorted[16] = {0, 4, 14, 9, 13, 11, 6, 5, 1, 2, 3, 7, 8, 10, 12, 15};


        //新的顺序
        /*
        先按照这个顺序排列，然后其他维度再排列
        given_name2
        surname2
        address2
        phone_number2
        postcode2
        social_security_ID2
        culture3
        title2
        */
        int sorted[16] = {5, 6, 11, 13, 9, 14, 0, 4, 1, 2, 3, 7, 8, 10, 12, 15};

        for(int j = 0; j < ExtractDimensionNum; j++)
        {
            for (int k = 0; k < ExtractCharNum; k++)
            {
                printf("%2d ", peo[i].record[sorted[j]][k]);
            }
            printf(" ");
        }
		printf("\n");
    }
    return 0;
}
