#include "merge_gather.h"

/* merge sort ���ƺ��� */
void merge_gather_operation()
{
    FILE *all_fp = fopen(M_ALL, "w");
    assert(all_fp);
    LoserTree ls;
    int now[MAX_WIN];
    //��ʼ��  53687  head
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
    //����������
    strcpy(node[NUM_WIN].key, MINKEY);
    //��ʼ��
    for (int i = 0; i < NUM_WIN; i++)
    {
        ls[i] = NUM_WIN;
    }
    //������������ʼֵ
    for (int i = NUM_WIN - 1; i >= 0; i--)
    {
        Adjust_1(ls, i);
    }

    //���յ�ʤ�ߴ洢�� node[0]�У�����ֵΪ MAXKEYʱ��֤��5����ʱ�ļ��鲢����
    while ((strcmp(node[ls[0]].key, MAXKEY)) < 0)
    {
        //�������,�����д
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
        //�����¶���ļ�¼�Ĺؼ��ֵ�ֵ�����µ������������ҳ����յ�ʤ��
        Adjust_1(ls, ls[0]);
    }

    fclose(all_fp);
    return;
}

/* �ش�Ҷ�ӽ��time[s]�������ls[0]��·������������
������������������������飬��һ��ʤ�ߵ��±� n */
void Adjust_1(LoserTree ls, int n)
{
    int t = (n + NUM_WIN) / 2;
    while (t > 0)
    {
        //�ж�ÿһ��Ҷ�ӽ��ͬ��˫�׽���м�¼�İ��ߵ�ֵ��Ƚϣ��������ߵ�ֵ������ n һֱ��ʾ�Ķ���ʤ��
        if (strcmp(node[n].key, node[ls[t]].key) > 0)
        {
            int swap = n;
            n = ls[t];
            ls[t] = swap;
        }
        t = t / 2;
    }
    //���ս�ʤ�ߵ�ֵ���� node[0]
    ls[0] = n;
    return;
}