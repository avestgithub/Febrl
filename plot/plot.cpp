#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MaxRecordNum 10000

#define StepLength 0.0005
#define Start 0.8
#define End 1.0


double recallMatrix[MaxRecordNum + 10];
int cmpNumMatrix[MaxRecordNum + 10];
int recordNum;


int main()
{
    freopen("dataset10000snm10D.txt","r",stdin);
    freopen("dataset10000snm10DForPlot.txt","w",stdout);
    int cmpNum;
    double recallValue;
    recordNum = 0;
    while (scanf("%d", &cmpNum) != EOF)
    {
        scanf("%lf", &recallValue);
        recallMatrix[recordNum] = recallValue;
        cmpNumMatrix[recordNum] = cmpNum;
        recordNum ++;
    }
    for(double i = Start; i <= End; i += StepLength)
    {
        for(int j = 0; j < recordNum; j++)
        {
            if(recallMatrix[j] >= i && recallMatrix[j] < (i + StepLength))
            {
                printf("%.2lf %d\n", recallMatrix[j]*100.0, cmpNumMatrix[j]);
                break;
            }
        }
    }
}