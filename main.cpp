#include <iostream>
#include <algorithm>
#include <ctime>
#include <cmath>
#include <cstring>
#include <unistd.h>
#include<vector>
using namespace std;
const int MAX_WORKPIECE_NUM = 100;
const int MAX_MACHINE_NUM = 100;

int n, m;                                     //工件数，机器数
int cost[MAX_WORKPIECE_NUM][MAX_MACHINE_NUM]; //工件加工时间方阵
vector<int> manage;                //调度方案
int restoredManage[MAX_WORKPIECE_NUM];
int sTime[MAX_WORKPIECE_NUM][MAX_MACHINE_NUM];         //第j个工件在机器i上的开始加工时间
int cTime[MAX_WORKPIECE_NUM][MAX_MACHINE_NUM];         //第j个工件在机器i上的完成加工时间
int bestAns = 0x3f3f3f;
vector<int> bestManage; //记录最优加工时间，加工方案

double T_init;
double theta;
int ring;
double T_final;

void Init()
{ //数据读入
    int tmp;
    cin >> n >> m;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            cin >> tmp >> cost[i][j];
        }
    }
    // int M[] = {7,4,2,10,3,1,8,0,6,5,9};
    //初始化调度方案“0，1，2，3，...,n-1”
    for (int i = 0; i < n; i++)
    {
        manage.push_back(i);
    }
}

void Change()
{ //随机交换调度方案中的两个工件顺序
    // int x = rand() % (n - 1);
    // int y = rand() % (n - 1);
    // while (x == y)
    // {
    //     x = rand() % (n - 1);
    // }
    // // memcpy(restoredManage, manage, sizeof(restoredManage));
    // swap(manage[x], manage[y]);
    random_shuffle(manage.begin(),manage.end());
}
void RollBack()
{
    // memcpy(manage, restoredManage, sizeof(manage));
}
int MakeSpan()
{
    for (int j = 0; j < n; j++)
    {
        if (j == 0)
            cTime[j][0] = 0;
        else
        {
            cTime[j][0] = cTime[j - 1][1];
        }

        for (int i = 0; i < m; i++)
        {
            if (i > 0)
            {
                sTime[j][i] = max(cTime[j][i - 1], cTime[j - 1][i]);
            }

            else
            {
                sTime[j][i] = cTime[j][i - 1];
            }
            cTime[j][i] = sTime[j][i] + cost[manage[j]][i];
        }
    }
    return cTime[n - 1][m - 1];
}
void MarkovParam()
{
    cout << "请输入起始温度: ";
    cin >> T_init;
    cout << "\n请输入降温速率: ";
    cin >> theta;
    cout << "\n请输入Markov链长度: ";
    cin >> ring;
    cout << "\n请输入结束温度: ";
    cin >> T_final;
}
void SA()
{
    MarkovParam();
    Change();
    double T = T_init;
    int t = MakeSpan();
    while (T > T_final)
    {
        // cout << "manage: ";
        // for (int i = 0; i < n; i++)
        // {
        //     cout << bestManage[i] << " ";
        // }
        // printf("\tT:%.2f ans:%4d\n", T, bestAns);
        for (int i = 0; i < ring; i++)
        {
            if (t < bestAns)
            {
                bestAns = t;
                bestManage.assign(manage.begin(),manage.end());
            }
            Change();
            int new_t = MakeSpan();
            // printf("new_t:%4d t:%4d best:%4d\n", new_t, t, bestAns);
            // sleep(0.5);
            if (new_t < t)
            {
                t = new_t;
            }
            else
            {
                double p = exp((t - new_t) / T);
                double x = rand() % 10000 / 10000.0;
                if (x <= p)
                {
                    t = new_t;
                }
                // else
                // {
                //     RollBack();
                // }
            }
        }
        T *= theta;
    }
}

int main(int argc, char const *argv[])
{
    srand((unsigned)time(NULL));
    Init();
    SA();
    cout << "the optimal time: " << bestAns << endl;
    cout << "the optimal management is : ";
    
    for (int i = 0; i < n; i++)
    {
        cout << bestManage[i] << " ";
    }
    cout << endl;
    return 0;
}
