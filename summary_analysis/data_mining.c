/* 未debug完，无法正常运行 */

#include "data_mining.h"

/* 进行数据挖掘的控制函数 */
void data_mining(char (*arr)[60], int length)
{
    while (1)
    {
        char key[11];
        // int *score=(int*)malloc(sizeof(int)*MAX_STU)
        int st_score[MAX_STU] = {0};
        int *score = st_score;
        int record[MAX_STU] = {0};
        for (int i = 0; i < NUM_NOW; i++)
        {
            record[i] = i;
        }

        printf("\n输入目标学号:\n");
        fflush(stdin);
        scanf("%s", key);
        fflush(stdin);
        if (find_student_a(key, 1) == -1)
        {
            continue;
        }
        find_center_stu(score, key, arr, length);
        // q_sort(score, 0, NUM_NOW - 1, record);
        output(score);

        { //界面
            char c;
            printf("*******\n是否退出? （退出请输入'y',继续输入任意键）\n ");
            fflush(stdin);
            scanf("%c", &c);
            fflush(stdin);
            if (c == 'y')
            {
                return;
            }
        }
    }
    return;
}

/* 找到目标学生 */
void find_center_stu(int *score, char *key_id, char (*array)[60], long int length)
{
    for (long int i = 0; i < length; i++)
    {
        char temp[11];
        strncpy(temp, &array[i][24], 10);
        if (!strcmp(temp, key_id))
        {
            char key_date[5], key_time[5], key_win[3];
            strncpy(key_date, &array[i][4], 4);
            strncpy(key_time, &array[i][8], 4);
            strncpy(key_win, &array[i][20], 2);
            accumulate_stu(score, array, i, key_date, key_time, key_win, length); //比较窗口与时间
        }
    }
    return;
}

/* 计算各个相邻同学的积分，按照加权累计
传入参数：各个同学的得分，总共消费记录，目标学生的数组下标，目标日期，目标时间，目标窗口号，消费记录总长度 */
void accumulate_stu(int *score, char (*array)[60], long int key_i, char *key_date, char *key_time, char *key_win, int max_leng)
{
    int before = key_i, after = key_i; //向前与向后探索
    int goal_date = atoi(key_date);    // goal
    int goal_time = atoi(key_time);    // goal
    int goal_win = atoi(key_win);      // goal

    int partner_date = goal_date; // partner
    int partner_time = goal_time; // partner
    int partner_win = goal_win;   // partner

    while (before >= 0 &&
           (partner_date == goal_date) &&
           abs(goal_time - partner_time) < 10)
    {
        char partner_id[11];
        char s_date[5], s_time[5], s_win[3];

        //输入新值
        strncpy(partner_id, &array[before][24], 10);
        strncpy(s_date, &array[before][4], 4);
        strncpy(s_time, &array[before][8], 4);
        strncpy(s_win, &array[before][20], 2);
        int partner_date = atoi(s_date);
        int partner_time = atoi(s_time);
        int partnerl_win = atoi(s_win);

        if (abs(goal_win - partner_win) < 5)
        {
            int j = find_student_a(partner_id, 0);
            score[j] += (15 - abs(goal_time - partner_time) - abs(goal_win - partner_win));
        }
        before--;
    }

    while (after <= max_leng - 1 &&
           (partner_date == goal_date) &&
           abs(goal_time - partner_time) < 10)//时间与目标差异小于10min
    {
        char partner_id[11];
        char s_date[5], s_time[5], s_win[3];

        //输入新值
        strncpy(partner_id, &array[before][24], 10);
        strncpy(s_date, &array[before][4], 4);
        strncpy(s_time, &array[before][8], 4);
        strncpy(s_win, &array[before][20], 2);
        int partner_date = atoi(s_date);
        int partner_time = atoi(s_time);
        int partnerl_win = atoi(s_win);

        if (abs(goal_win - partner_win) < 5)
        {
            int j = find_student_a(partner_id, 0);
            score[j] += (15 - abs(goal_time - partner_time) - abs(goal_win - partner_win));
        }
        after++;
    }
    return;
}

// use the method of binary search to find the student id
int find_student_a(char *key, int flag_f)
{
    int low = 0;
    int high = NUM_NOW - 1;
    int mid;
    while (low <= high)
    {
        mid = (low + high) / 2;
        if (!strcmp((ST[mid]).data.account_id, key)) // equal
        {
            return mid;
        }
        else if (strcmp((ST[mid]).data.account_id, key) > 0)
        {
            high = mid - 1;
        }
        else
        {
            low = mid + 1;
        }
    }
    if (flag_f) //是否用来检验输入值
    {
        printf("The student's ID %s not exists!\n", key); // the ID has not been serached
    }

    return ERROR;
}

/* 排序得分函数
传入：得分数组，最低下标，最高下标，记录下标数组 */
void q_sort(int *score, int low, int high, int *record)
{
    if (low < high)
    {
        int i = low;
        int j = high;
        int k = score[low];
        while (i < j)
        {
            while (i < j && score[j] >= k) // 从右向左找第一个小于k的数
            {
                j--;
            }

            if (i < j)
            {
                score[i++] = score[j];
                record[i - 1] = record[j];
            }

            while (i < j && score[i] < k) // 从左向右找第一个大于等于k的数
            {
                i++;
            }

            if (i < j)
            {
                score[j--] = score[i];
                record[j + 1] = record[i];
            }
        }
        score[i] = k;
        record[i] = record[low];

        // 递归调用
        q_sort(score, low, i - 1, record);  // 排序k左边
        q_sort(score, i + 1, high, record); // 排序k右边
    }
}

/* 输出函数 */
void output(int *record)
{
    printf("***************************\n");
    for (int i = 0; i < 9; i++)
    {
        printf("%s\n", ST[record[i]].data.account_id);
    }
    printf("***************************\n");
    return;
}
/* void output(int *score)
{
    const int n = NUM_NOW;
    int flag[MAX_STU];
    for (int j = 0; j < n; j++)
    {
        int max = 0, min = 0;
        for (int i = 0; i < n; i++)
        {
            if (score[i] > score[max])
            {
                max = i;
            }
            if (score[i] < score[min])
            {
                min = i;
            }
        }
        flag[j] = max;
        score[max] = score[min] - 10;
    }
    printf("***************************\n");
    for (int i = 0; i < 10; i++)
    {
        printf("%d ", flag[i]);
    }
    printf("***************************\n");
} */