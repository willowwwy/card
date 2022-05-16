#include "q_sort.h"

/* ��������
���룺�����¼�����Ѽ�¼����arr��������С�±�low������±�high */
void QSort(char (*arr)[60], int low, int high)
{
    if (low < high)
    {
        int i = low;
        int j = high;
        char key[60];
        strcpy(key, arr[low]);
        char temp[17];
        while (i < j)
        {
            while (i < j && strcmp((strncpy(temp, arr[j], 16)), key) >= 0) // ���������ҵ�һ��С��k����
            {
                j--;
            }

            if (i < j)
            {
                strcpy(arr[i++], arr[j]);
            }

            while (i < j && strcmp((strncpy(temp, arr[i], 16)), key) < 0) // ���������ҵ�һ�����ڵ���k����
            {
                i++;
            }

            if (i < j)
            {
                strcpy(arr[j--], arr[i]);
            }
        }

        strcpy(arr[i], key);

        // �ݹ����
        QSort(arr, low, i - 1);  // ����k���
        QSort(arr, i + 1, high); // ����k�ұ�
    }
}

/* ��������Ŀ��������
���룺���Ѽ�¼��������array */
int QuickSort(char (*array)[60])
{ // data_win[num][][]
    FILE *qall_fp = NULL;
    qall_fp = fopen(QALL, "w");
    assert(qall_fp);

    int length = 0;

    // input
    for (int i = 0; i < MAX_WIN; i++)
    {
        for (int j = 0; j < MAX_L; j++)
        {
            if (data_win[i][j][0] == '2')
            {
                strcpy(array[length], data_win[i][j]);
                length++;
            }
        }
    }
    // sort
    QSort(array, 0, length - 1);
    // output

    for (int i = 0; i < length; i++)
    {
        fputs(array[i], qall_fp);
    }

    fclose(qall_fp); // close the file

    return length;
}