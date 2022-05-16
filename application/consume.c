#include "consume.h"
void consume_operate(char *limit_time)
{
    char temp[40], c;
    int i = 0;
    static int if_first = 1;
    if (if_first)
    {
        if (fgetc(xf_fp) != 'X' || fgetc(xf_fp) != 'F' || fgetc(xf_fp) != '\n') // check if the file is correct
        {
            printf("Wrong Filename!");
            fputs("�ļ�������", logp);
            return;
        }
        if_first = 0;

        // printf("%c",fgetc(xf_fp));
        while ((c = fgetc(xf_fp)) != EOF && i < NUM_WIN) // next row����ø����ļ���дָ��׼����д
        {
            if (c == 'W')
            {
                char ct[10];
                fgets(ct, 5, xf_fp);
                offset[i] = ftell(xf_fp); // remember the offset
                fseek(xf_fp, offset[i], SEEK_SET);
                fgets(s[i], 40, xf_fp);
                i++;
            }
            else
            {
                fgets(temp, 40, xf_fp);
            }
        }
        rewind(xf_fp);

        position_init(); //��¼��win�ĳ�ʼλ��
    }

    sort_in(limit_time);

    return;
}

void sort_in(char *limit_time)
{
    static LoserTree ls;
    static int first = 1;
    //��ʼ��
    if (first)
    {
        for (int i = 0; i < NUM_WIN; i++)
        {
            fseek(xf_fp, offset[i], SEEK_SET);
            fgets(s[i], 40, xf_fp); // get a line
            offset[i] = ftell(xf_fp);

            if (s[i][0] != '3' || feof(xf_fp))
            {
                strcpy(time_leaf[i].key, MAXKEY);
            }
            else
            {
                memcpy(&((time_leaf[i].key)[0]), &s[i][8], 8);
                memcpy(&((time_leaf[i].key)[8]), &s[i][17], 8);
                time_leaf[i].key[16] = '\0';
            }
            // get the time
        }

        //����������
        strcpy(time_leaf[NUM_WIN].key, MINKEY);
        //����ls�����а��ߵĳ�ʼֵ
        for (int i = 0; i < NUM_WIN; i++)
        {
            ls[i] = NUM_WIN;
        }
        //����ÿһ��Ҷ�ӽ�㣬�����������з��ն˽���м�¼���ߵ�ֵ
        for (int i = NUM_WIN - 1; i >= 0; i--)
        {
            Adjust(ls, i);
        }
        // card_consume(ls[0], time_leaf[ls[0]].key); //д��log and win
        first = 0;
    }

    //���յ�ʤ�ߴ洢�� ls[0]�У�����ֵΪ MAXKEYʱ��֤����ʱ�ļ��鲢����
    while (strcmp(time_leaf[ls[0]].key, limit_time) <= 0)
    {
        //�������ģ�������д�Ĳ���
        card_consume(ls[0], time_leaf[ls[0]].key); //д��log and win

        //������������ļ�¼
        fseek(xf_fp, offset[ls[0]], SEEK_SET);
        fgets(s[ls[0]], 40, xf_fp); // get a line
        offset[ls[0]] = ftell(xf_fp);
        memcpy(time_leaf[ls[0]].key, &s[ls[0]][8], 8); // get the time
        memcpy(&((time_leaf[ls[0]].key)[8]), &s[ls[0]][17], 8);
        time_leaf[ls[0]].key[16] = '\0';

        if (s[ls[0]][0] != '3' || feof(xf_fp)) // The win file END
        {
            strcpy(time_leaf[ls[0]].key, MAXKEY);
        }
        //�����¶���ļ�¼�Ĺؼ��ֵ�ֵ�����µ������������ҳ����յ�ʤ��
        Adjust(ls, ls[0]);
    }

    return;
}

//�ش�Ҷ�ӽ��time[s]�������ls[0]��·������������
void Adjust(LoserTree ls, int n)
{
    int t = (n + NUM_WIN) / 2;
    while (t > 0)
    {
        //�ж�ÿһ��Ҷ�ӽ��ͬ��˫�׽���м�¼�İ��ߵ�ֵ��Ƚϣ��������ߵ�ֵ������ n һֱ��ʾ�Ķ���ʤ��
        if (strcmp(time_leaf[n].key, time_leaf[ls[t]].key) > 0)
        {
            int swap = n;
            n = ls[t];
            ls[t] = swap;
        }
        t = t / 2;
    }
    //���ս�ʤ�ߵ�ֵ���� ls[0]
    ls[0] = n;
    return;
}

bool card_consume(int num, char *time_goal)
{
    char key[8], money[6]; // xx.xx
    int goal, j;
    float sub;

    // card number
    memcpy(key, s[num], sizeof(key));
    key[7] = '\0';
    // money
    memcpy(money, &s[num][26], sizeof(money));
    for (j = 0; money[j] != ';' && j < sizeof(money); j++)
        ;
    money[j] = '\0';
    sub = (float)atof(money);

    fprintf(logp, "%s,", time_goal);

    goal = find_card(key);
    if (goal == -1)
    {
        fputs(",δ���ҵ�����;\n", logp);
        return 0;
    }
    else if (CR[goal].if_valid == 0)
    {
        fprintf(logp, "%s,����ʧЧ,����ʧ��(-%.2f);\n",
                key,
                sub);
        return 0;
    }
    else if (CR[goal].student_account->data.left_money < sub)
    {
        char temp[10];
        fprintf(logp, "%s,%s,����(%.2f),����ʧ��(-%.2f);\n",
                CR[goal].data.card_id,
                CR[goal].student_account->data.student_name,
                CR[goal].student_account->data.left_money,
                sub);

        return 0;
    }
    else
    {
        char temp1[7], temp2[7];
        char c_sub[7];

        // before
        fputs(CR[goal].student_account->data.account_id, logp);
        fputc(',', logp);
        fputs(CR[goal].student_account->data.student_name, logp);
        fputc(',', logp);
        sprintf(temp1, "%.2f", CR[goal].student_account->data.left_money);
        fputs(temp1, logp);

        // ing
        sprintf(c_sub, "%.2f", sub);
        fputs(" �� ", logp);
        CR[goal].student_account->data.left_money -= sub;
        // after
        sprintf(temp2, "%.2f", CR[goal].student_account->data.left_money);
        fputs(temp2, logp);
        fputs(",���ѳɹ�(-", logp);

        fputs(c_sub, logp);
        fputs(");", logp);

        // if enter password,put in log
        char date_now[5], hour_now[3];
        strncpy(date_now, &time_goal[4], 4);
        strncpy(hour_now, &time_goal[8], 2);
        if ((!strcmp(date_now, CR[goal].student_account->data.last_time.last_date)) &&
            (((strcmp(hour_now, "07") >= 0 && strcmp(CR[goal].student_account->data.last_time.last_hour, "07") >= 0) &&
              (strcmp(hour_now, "08") <= 0 && strcmp(CR[goal].student_account->data.last_time.last_hour, "08") <= 0)) ||
             ((strcmp(hour_now, "11") >= 0 && strcmp(CR[goal].student_account->data.last_time.last_hour, "11") >= 0) &&
              (strcmp(hour_now, "12") <= 0 && strcmp(CR[goal].student_account->data.last_time.last_hour, "12") <= 0)) ||
             ((strcmp(hour_now, "17") >= 0 && strcmp(CR[goal].student_account->data.last_time.last_hour, "17") >= 0) &&
              (strcmp(hour_now, "18") <= 0 && strcmp(CR[goal].student_account->data.last_time.last_hour, "18") <= 0))))
        {
            if (CR[goal].student_account->data.last_time.add_up + sub > 20.0) //>20�ۼ�����
            {
                fputc('*', logp);
                CR[goal].student_account->data.last_time.add_up = 0;
            }
            else
            {
                CR[goal].student_account->data.last_time.add_up += sub; //<20,��������
            }
        }
        else
        {
            CR[goal].student_account->data.last_time.add_up = sub;
        }
        fputc('\n', logp);
        strcpy(CR[goal].student_account->data.last_time.last_date, date_now);
        strcpy(CR[goal].student_account->data.last_time.last_hour, hour_now);

        static int a1 = 0, a2 = 1;
        int b1, b2, correct_2, correct_1;
        char x1[3], x2[2];

        strncpy(x1, &CR[goal].student_account->data.account_id[8], 2);
        strncpy(x2, &CR[goal].data.card_id[6], 1);
        b1 = atoi(x1);                          //ѧ�ź���λ
        b2 = atoi(x2);                          //���ź�һλ
        correct_1 = ((a1 + 1) * (a2 + 1)) % 10; //��һλУ����
        correct_2 = (b1 * b2 + (int)sub) % 10;  //�ڶ�λУ����
        a1 = a2;                                // a1����������������¼��correct_1
        a2 = correct_1;                         // a2����������һ����¼��correct_1
        if(num<9)
        {
            sprintf(data_win[num][location[num].tail], "%s,%d%d,0%d,(%s)%s,%s(-%.2f);\n",
                time_goal,
                correct_1, correct_2,
                num+1,
                CR[goal].student_account->data.account_id,
                CR[goal].student_account->data.student_name,
                CR[goal].data.card_id,
                sub);
        }
        else
        {
            sprintf(data_win[num][location[num].tail], "%s,%d%d,%d,(%s)%s,%s(-%.2f);\n",
                time_goal,
                correct_1, correct_2,
                num+1,
                CR[goal].student_account->data.account_id,
                CR[goal].student_account->data.student_name,
                CR[goal].data.card_id,
                sub);
        }
        

        set_location(num);

        return 1;
    }
}

/* copy the minimum time into the right window
    (array data_win!!!!!) */
void set_location(int num)
{
    // tail
    if (location[num].tail == 59999)
    {
        location[num].tail = 0;
    }
    else
    {
        location[num].tail++;
    }

    // if_back
    if (location[num].tail == location[num].head && !location[num].if_back)
    {
        location[num].if_back = 1; // first rewrite
    }

    // head
    if (location[num].if_back == 1)
    {
        if (location[num].tail == 59999)
        {
            location[num].head = 0;
        }
        else
        {
            location[num].head = location[num].tail;
        }
    }
    return;
}

int find_card(char *key)
{
    char temp_c[5];
    int temp_int, symbol;

    memcpy(temp_c, &key[2], sizeof(temp_c));
    temp_c[4] = '\0';
    temp_int = atoi(temp_c);
    symbol = temp_int;

    while (strcmp(key, CR[temp_int].data.card_id))
    {
        if ((CR[temp_int].if_taken == 0) || (temp_int == symbol))
        {
            fputs(key, logp);
            return ERROR;
        }
        else if (temp_int == 9999)
        {
            temp_int = 0;
        }
        else
        {
            temp_int++;
        }
    }

    return temp_int;
}

/*  get the initial position of the file
    Store records in a static array*/
void position_init()
{
    FILE *wz_fp = fopen(WZ, "r");
    char temp_c[8];
    int num = 0;

    if (!wz_fp)
    {
        printf("��ȡ�ļ�ʧ��");
        return;
    }
    if (fgetc(wz_fp) != 'W' || fgetc(wz_fp) != 'Z' || fgetc(wz_fp) != '\n') // check if the file is correct
    {
        printf("Wrong Filename!2");
        fputs("�ļ�������", logp);
        return;
    }

    while (fgetc(wz_fp) != EOF && num < MAX_WIN) // next window
    {
        int i;

        while (fgetc(wz_fp) != ',')
            ;
        fgets(temp_c, 8, wz_fp);
        for (i = 0; temp_c[i] != ';'; i++)
            ;
        temp_c[i] = '\0';
        location[num].head = location[num].tail = atoi(temp_c) - 1;
        location[num].if_back = 0;

        for (int j = 0; j < 60000; j++)
        {
            strcpy(data_win[num][j], "\n");
        }
        num++;
    }
    fclose(wz_fp);
    return;
}