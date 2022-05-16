#include "merge_gather.h"

/* merge sort 控制函数 */
void merge_gather_operation()
{
    FILE *all_fp = fopen(M_ALL, "w");
    assert(all_fp);
    LoserTree ls;
    int now[MAX_WIN];
    //初始化  53687  head
    for (int i = 0; i < MAX_WIN; i++)
    {
        now[i] = (location[i]).head;
        if (data_win[i][(location[i].head)][0] == '\n')
        {
            strcpy(node[i].key, MAXKEY);
        }
        else
        {
            strncpy((node[i].key), data_win[i][(location[i]).head], 16);
            node[i].key[16] = '\0';
        }
    }
    //创建败者树
    strcpy(node[NUM_WIN].key, MINKEY);
    //初始化
    for (int i = 0; i < NUM_WIN; i++)
    {
        ls[i] = NUM_WIN;
    }
    //调整败者树初始值
    for (int i = NUM_WIN - 1; i >= 0; i--)
    {
        Adjust_1(ls, i);
    }

    //最终的胜者存储在 node[0]中，当其值为 MAXKEY时，证明5个临时文件归并结束
    while ((strcmp(node[ls[0]].key, MAXKEY)) < 0)
    {
        //输出过程,向外存写
        fputs(data_win[ls[0]][now[ls[0]]], all_fp);

        // now[ls[0]]++
        if (now[ls[0]] == 59999)
        {
            now[ls[0]] = 0;
        }
        else
        {
            (now[ls[0]])++;
        }

        // The input file x END
        if (data_win[ls[0]][now[ls[0]]][0] != '2' ||
            now[ls[0]] == (location[ls[0]]).tail)
        {
            strcpy(node[ls[0]].key, MAXKEY);
        }
        else
        {
            strncpy(node[ls[0]].key, data_win[ls[0]][now[ls[0]]], 16);
            node[ls[0]].key[16] = '\0';
        }
        //根据新读入的记录的关键字的值，重新调整败者树，找出最终的胜者
        Adjust_1(ls, ls[0]);
    }

    fclose(all_fp);
    return;
}

/* 沿从叶子结点time[s]到根结点ls[0]的路径调整败者树
传入参数：败者树的整型数组，上一次胜者的下标 n */
void Adjust_1(LoserTree ls, int n)
{
    int t = (n + NUM_WIN) / 2;
    while (t > 0)
    {
        //判断每一个叶子结点同其双亲结点中记录的败者的值相比较，调整败者的值，其中 n 一直表示的都是胜者
        if (strcmp(node[n].key, node[ls[t]].key) > 0)
        {
            int swap = n;
            n = ls[t];
            ls[t] = swap;
        }
        t = t / 2;
    }
    //最终将胜者的值赋给 node[0]
    ls[0] = n;
    return;
}