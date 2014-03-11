#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<algorithm>
#include<math.h>
#include <time.h>
using namespace std;

#define LineNum 551
#define DocNum 10
#define ExpeNum 4


long long m[DocNum][ExpeNum][LineNum][2];
int dupCount = 2701376;

/*
共DocNum个文件
把记录累加
最后输出
*/


int main()
{
    freopen(".txt","r",stdin);
    freopen(".txt","w",stdout);
    for(int i = 0; i < DocNum; i++)
    {
        for(int j = 0; j < ExpeNum; j++)
        {
            for(int k = 0; k < LineNum; k++)
            {
                for(int x = 0; x < 2; x++)
                {
                    scanf("%lld", &m[i][j][k][x]);
                }
            }
        }
    }
    for(int j = 0; j < ExpeNum; j++)
    {
        for(int k = 0; k < LineNum; k++)
        {
            long long hitNum = 0, dupNum = 0;
            for(int i = 0; i < DocNum; i++)
            {
                hitNum += m[i][j][k][0];
                dupNum += m[i][j][k][1];
            }
            printf("for %lld cmps, find %lld dups, recall is %.8lf\n", hitNum, dupNum, (double)dupNum/dupCount);
            if(dupNum == dupCount)
                break;
        }
        puts("");
    }
    return 0;
}
