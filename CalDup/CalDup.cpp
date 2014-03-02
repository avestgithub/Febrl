#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<algorithm>
#include<math.h>
#include <time.h>
using namespace std;

#define LineNum 551
#define ExpeNum 4
#define DocNum 5

long long m[DocNum][ExpeNum][LineNum][3];
int dupCount = 2701376;

/*
共5个文件doc0-4
doc1=doc0-doc1
doc1每行记录累加
再把doc1到doc4累加
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
                for(int x = 0; x < 3; x++)
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
            m[1][j][k][1] = m[0][j][k][1] - m[1][j][k][1];
            m[1][j][k][2] = m[0][j][k][2] - m[1][j][k][2];
            if(k > 0)
            {
                m[1][j][k][1] += m[1][j][k-1][1];
                m[1][j][k][2] += m[1][j][k-1][2];
            }
        }
    }
    for(int j = 0; j < ExpeNum; j++)
    {
        for(int k = 0; k < LineNum; k++)
        {
            long long hitNum = 0, dupNum = 0;
            for(int i = 1; i < DocNum; i++)
            {
                hitNum += m[i][j][k][1];
                dupNum += m[i][j][k][2];
            }
            printf("for %lld cmps, find %lld dups, recall is %lf\n", hitNum, dupNum, (double)dupNum/dupCount);
            if(dupNum == dupCount)
                break;
        }
        puts("");
    }
    return 0;
}
