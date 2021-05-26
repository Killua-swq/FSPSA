#include <iostream>
#include <algorithm>
#include <ctime>
#include <cmath>
#include <cstring>
#include <unistd.h>
#include <vector>
using namespace std;
const int MAX_WORKPIECE_NUM = 100;
const int MAX_MACHINE_NUM = 100;

int n, m;                                     //工件数，机器数
int cost[MAX_WORKPIECE_NUM][MAX_MACHINE_NUM]; //工件加工时间方阵
vector<int> manage;                           //调度方案
int restoredManage[MAX_WORKPIECE_NUM];
int sTime[MAX_WORKPIECE_NUM][MAX_MACHINE_NUM]; //第j个工件在机器i上的开始加工时间
int cTime[MAX_WORKPIECE_NUM][MAX_MACHINE_NUM]; //第j个工件在机器i上的完成加工时间
int bestAns = 0x3f3f3f;                        //记录最优时间
vector<int> bestManage;                        //记录最优加工方案

double T_init;
double theta;
int ring = 200;
double T_final = 1;

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
    //初始化调度方案“0，1，2，3，...,n-1”
    for (int i = 0; i < n; i++)
    {
        manage.push_back(i);
    }
}

void Change()
{ //从n个工件的加工顺序中随机选择一个
    random_shuffle(manage.begin(), manage.end());
}
void InitManage()
{
    vector<int>().swap(manage);
    //初始化调度方案“0，1，2，3，...,n-1”
    for (int i = 0; i < n; i++)
    {
        manage.push_back(i);
    }
}
int MakeSpan() //解的评估函数,前向计算
{
    for (int j = 0; j < n; j++)
    {
        if (j == 0)
            cTime[j][0] = 0; // 方案中第一个零件在第一台机器上开始加工时间为0
        else
        {
            cTime[j][0] = cTime[j - 1][1];
            //方案中的第j个零件在第一台机器上的开始加工时间是上一个零件完成第一台机器加工的时间
        }
        //计算方案中第j个工件的在每台机器上的开始加工时间和完成加工时间
        for (int i = 0; i < m; i++)
        {
            if (j != 0) //当目前机器上的零件不是方案中的第一个时
            //它的开始时间便从上一个零件的完成时间和自己在上一个机器的完成时间中选择最大的
            {
                sTime[j][i] = max(cTime[j][i - 1], cTime[j - 1][i]);
            }

            else //当目前机器上加工方案中的第一个零件是,
            //它的开始加工时间就是它在上一个机器完成加工的时间
            {
                sTime[j][i] = cTime[j][i - 1];
            }
            //计算当前零件的完成加工时间
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
    MarkovParam(); //参数设置
    Change();      //随机产生一个解
    double T = T_init;
    int t = MakeSpan(); //评估初始解
    /*
    模拟退火主体
    */
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
            if (t < bestAns) //记录最优解,最优方案
            {
                bestAns = t;
                bestManage.assign(manage.begin(), manage.end());
            }
            Change();               //扰动,产生新解
            int new_t = MakeSpan(); //评估新解
            // printf("new_t:%4d t:%4d best:%4d\n", new_t, t, bestAns);
            // sleep(0.5);
            if (new_t < t)
            {
                t = new_t;
            }
            else
            {
                //Metropli法则
                double p = exp((t - new_t) / T);
                double x = rand() % 10000 / 10000.0;
                if (x <= p)
                {
                    t = new_t;
                }
            }
        }
        T *= theta;
    }
}

int main(int argc, char const *argv[])
{
    clock_t startTime, endTime; //运行时间计算
    startTime = clock();
    srand((unsigned)time(NULL));
    Init(); //初始化数据
    // int ans = 0;
    // for (int i = 0; i < 100; i++)
    // {
    // InitManage();
    SA();
    // cout << bestAns << endl;
    // ans += bestAns;
    // }

    cout << "the optimal time: " << bestAns << endl;
    cout << "the optimal management is : ";
    //调度方案输出
    for (int i = 0; i < n; i++)
    {
        cout << bestManage[i] << " ";
    }
    cout << endl;
    endTime = clock();
    cout << "the run time is : " << (double)(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;
    return 0;
}
