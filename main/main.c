#include "main.h"

int main()
{
    char date_set[9], time_set[9];
    char in_or_gr; // individual or group

    open_file();

    printf("****Welcome!****\n");
    printf("����i or ����g?\nInput here: ");
    scanf("%c", &in_or_gr);

    if (in_or_gr == 'i')
    {
        //���˽���ѡ��single
        printf("���뵱ǰ����(��λ):");
        fflush(stdin);
        scanf("%s", date_set);
        printf("���뵱ǰʱ��(��λ):");
        fflush(stdin);
        scanf("%s", time_set);
        strcpy(date_time_set, date_set);
        strcpy(&date_time_set[8], time_set);
        fflush(stdin);
        date_time_set[16] = '\0';

        group_control();  //����ǰ�涨ʱ��֮ǰ����������
        single_control(); //������˲���ϵͳ���е�������
    }
    else if (in_or_gr == 'g') //����group����
    {
        strcpy(date_time_set, "9999999999999999");
        group_control();  //���Ƹ���������ʱ��˳��
        gather_control(); //�������������Ļ��ܷ��������򣨹鲢�����ţ�
    }

    close_file();
    system("pause");
    return 0;
}

/* open files--log,cz,xf */
void open_file()
{
    logp = fopen(LOG, "w"); // open file log
    assert(logp);

    cz_fp = fopen(CZ, "r"); // open file CZ
    assert(cz_fp);

    xf_fp = fopen(XF, "r"); // open file XF
    assert(xf_fp);

    for (int i = 1; i <= MAX_WIN; i++) // open file win[n]
    {
        char name[42];
        int right = 35;
        strcpy(name, "../../record_files/expense_log/win_xx.txt");

        if (i < 10)
        {
            name[right] = '0';
            name[right + 1] = i + '0';
        }
        else
        {
            char temp[3];
            itoa(i, temp, 10);
            strncpy(&name[right], temp, 2);
        }
        win[i - 1] = fopen(name, "w");
        assert(win[i - 1]);
    }

    return;
}
/* close files -- log,cz,xf */
void close_file()
{
    fclose(logp);
    fclose(cz_fp);
    fclose(xf_fp);
    for (int i = 0; i < MAX_WIN; i++)
    {
        fclose(win[i]);
    }
    return;
}

/* ��ʱ��˳���ȡ����������ļ��еĲ��� */
void group_control()
{
    clock_t start_t, end_t;
    double diff_t;

    // 9.1 all day open the account
    printf("9.1 open account runtime:  ");
    start_t = clock();
    open_account();
    end_t = clock();
    diff_t = (end_t - start_t) / CLOCKS_PER_SEC;
    printf("%f\n", diff_t);

    // 9.3-9.6  report loss/cancle account/cancle report loss/reissue_card/recharge
    printf("9.3-9.6 operate_card:  ");
    start_t = clock();
    operate_card(0, which_todo("2021090700000000"));
    end_t = clock();
    diff_t = (end_t - start_t) / CLOCKS_PER_SEC;
    printf("%lf\n", diff_t);

    // 9.23-10.13 consume
    printf("9.23-10.13 consume_operate:  ");
    start_t = clock();
    consume_operate(which_todo("2021101400000000"));
    end_t = clock();
    diff_t = (end_t - start_t) / CLOCKS_PER_SEC;
    printf("%lf\n", diff_t);
    //��
    // 10.14
    printf("10.14 operate_card:  ");
    start_t = clock();
    operate_card(1, which_todo("2021101406999999"));
    end_t = clock();
    diff_t = (end_t - start_t) / CLOCKS_PER_SEC;
    printf("%lf\n", diff_t);

    // 10.14-11.03
    printf("10.14-11.03 consume_operate:  ");
    start_t = clock();
    consume_operate(which_todo("2021110400000000"));
    end_t = clock();
    diff_t = (end_t - start_t) / CLOCKS_PER_SEC;
    printf("%lf\n", diff_t);

    // 11.4
    printf("11.4 operate_card:  ");
    start_t = clock();
    operate_card(1, which_todo("2021110406999999"));
    end_t = clock();
    diff_t = (end_t - start_t) / CLOCKS_PER_SEC;
    printf("%lf\n", diff_t);

    // 11.4-11.24
    printf("11.4-11.24 consume_operate:  ");
    start_t = clock();
    consume_operate(which_todo("2021112500000000"));
    end_t = clock();
    diff_t = (end_t - start_t) / CLOCKS_PER_SEC;
    printf("%lf\n", diff_t);

    // 11.25
    printf("11.25 operate_card:  ");
    start_t = clock();
    operate_card(1, which_todo("2021112506999999"));
    end_t = clock();
    diff_t = (end_t - start_t) / CLOCKS_PER_SEC;
    printf("%lf\n", diff_t);

    // 11.25-12.15
    printf("11.25-12.15 consume_operate:  ");
    start_t = clock();
    consume_operate(which_todo("2021121600000000"));
    end_t = clock();
    diff_t = (end_t - start_t) / CLOCKS_PER_SEC;
    printf("%lf\n", diff_t);

    // 12.16
    printf("12.16 operate_card:  ");
    start_t = clock();
    operate_card(1, which_todo("2021121606999999"));
    end_t = clock();
    diff_t = (end_t - start_t) / (float)CLOCKS_PER_SEC;
    printf("%lf\n", diff_t);

    // 12.16-...
    printf("12.16-... consume_operate:  ");
    start_t = clock();
    consume_operate(which_todo("2022000000000000"));
    end_t = clock();
    diff_t = (end_t - start_t) / CLOCKS_PER_SEC;
    printf("%lf\n", diff_t);

    // input into the win_file
    printf("write in windows' file:  ");
    start_t = clock();
    input_win();
    end_t = clock();
    diff_t = (end_t - start_t) / CLOCKS_PER_SEC;
    printf("%lf\n", diff_t);

    return;
}

/* �����������л��ܷ��� */
void gather_control()
{
    clock_t start_t, end_t;
    double diff_t;
    //�鲢
    printf("merge_gather information runtime:  ");
    start_t = clock();
    merge_gather_operation();
    end_t = clock();
    diff_t = (end_t - start_t) / CLOCKS_PER_SEC;
    printf("%lf\n", diff_t);
    //����
    printf("quick_sort_gather information runtime:  ");
    start_t = clock();
    char(*array)[60] = (char(*)[60])malloc((60 * sizeof(char)) * (MAX_L * MAX_WIN));
    int num_csm = QuickSort(array);
    end_t = clock();
    diff_t = (end_t - start_t) / CLOCKS_PER_SEC;
    printf("%lf\n", diff_t);
    //�ھ�
    // data_mining(array,num_csm);��bugδ����

    //ͳ��
    date_gather(array);

    free(array);
    return;
}

/*���ڴ�������д���ļ�*/
void input_win()
{
    int i, j;
    for (i = 0; i < MAX_WIN; i++)
    {
        for (j = 0; j < 60000; j++)
        {
            fputs(data_win[i][j], win[i]);
        }
    }
    return;
}

/* ����ʱ�䣬ִ�е�ǰ�涨ʱ��֮ǰ�Ĳ���
����ò������������ʱ��
���ص�ǰʱ�����ֹʱ���С��*/
char *which_todo(char *s)
{
    return (strcmp(date_time_set, s) < 0 ? date_time_set : s);
}

/* ������ֹ�����ڵ��ܼ����ѽ��
����������ܹ����Ѽ�¼���� */
void date_gather(char (*arr)[60])
{
    char start[9], end[9];
    char temp[9] = {"'\0'"};
    int i;
    double sum = 0;
    printf("���뿪ʼ����");
    fflush(stdin);
    scanf("%s", start);

    printf("�����������");
    fflush(stdin);
    scanf("%s", end);

    for (i = 0; strcmp(temp, start) < 0; i++)
    {
        strncpy(temp, arr[i], 8);
        temp[8] = '\0';
    }

    while (strcmp(temp, end) <= 0)
    {
        int j = 49;
        for (; arr[i][j] != '-'; j++)
            ;
        j++;
        char add_m[6];
        int count = 0;
        for (int k = j; arr[i][k] == '.' || (arr[i][k] >= '0') && (arr[i][k] <= '9'); k++, count++)
        {
            add_m[count] = arr[i][k];
        }
        add_m[++count] = '\0';
        sum += atof(add_m);
        i++;
        strncpy(temp, arr[i], 8);
        temp[8] = '\0';
    }

    printf("�ܼƽ�%.2lf", sum);

    return;
}