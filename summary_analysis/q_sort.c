#include "q_sort.h"

/* 快速排序
传入：保存记录的消费记录数组arr，数组最小下标low，最大下表high */
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
            while (i < j && strcmp((strncpy(temp, arr[j], 16)), key) >= 0) // 从右向左找第一个小于k的数
            {
                j--;
            }

            if (i < j)
            {
                strcpy(arr[i++], arr[j]);
            }

            while (i < j && strcmp((strncpy(temp, arr[i], 16)), key) < 0) // 从左向右找第一个大于等于k的数
            {
                i++;
            }

            if (i < j)
            {
                strcpy(arr[j--], arr[i]);
            }
        }

        strcpy(arr[i], key);

        // 递归调用
        QSort(arr, low, i - 1);  // 排序k左边
        QSort(arr, i + 1, high); // 排序k右边
    }
}

/* 进行整体的控制与输出
传入：消费记录乱序数组array */
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