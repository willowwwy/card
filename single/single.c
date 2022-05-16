#include "single.h"

//����ٿغ���
void single_control()
{
    int choice;
    char date_set[9], time_set[9];
    single_begin = NUM_NOW;
    while (1)
    {
        printf("\nѡ�����:\n\n"
               "1.����\n"
               "2.��ʧ\n"
               "3.���\n"
               "4.����\n"
               "5.����\n"
               "6.��ֵ\n"
               "7.����\n"
               "8.��ѯ\n"
               "9.��������\n"
               "10.�˳�\n\n"
               "�ڴ�����:");
        fflush(stdin);
        scanf("%d", &choice);
        fflush(stdin);
        switch (choice)
        {
        case 1:
        {
            _kh(); //����
            break;
        }
        case 2:
        {
            _gs(); //��ʧ
            break;
        }
        case 3:
        {
            _jg(); //���
            break;
        }
        case 4:
        {
            _bk(); //����
            break;
        }
        case 5:
        {
            _xh(); //����
            break;
        }
        case 6:
        {
            _cz(); //��ֵ
            break;
        }
        case 7:
        {
            _xf(); //����
            break;
        }
        case 8:
        {
            _search(); //ģ����ѯ
            break;
        }
        case 9:
        {
            printf("NOW:%s\n", date_time_set);
            printf("������һ����(8):");
            fflush(stdin);
            scanf("%s", date_set);
            fflush(stdin);
            printf("������һʱ��(8):");
            fflush(stdin);
            scanf("%s", time_set);
            strcpy(date_time_set, date_set);
            strcpy(&date_time_set[8], time_set);
            fflush(stdin);
            date_time_set[16] = '\0';
        }
        case 10:
        {
            return;
        }
        default:
        {
            printf("ERROR!");
            continue;
        }
        }
    }
}

/* ���е��˿��� */
void _kh()
{
    int sum = 0, temp_int;
    char id[8], temp_c[5];

    if (NUM_NOW >= 10000)
    {
        printf("���������ѳ�����!");
        return;
    }
    // init the account ST
    printf("Please input student number(10):");
    fflush(stdin);
    scanf("%s", &ST[NUM_NOW].data.account_id);
    printf("Please input your name:");
    fflush(stdin);
    scanf("%s", &ST[NUM_NOW].data.student_name);
    fflush(stdin);

    ST[NUM_NOW].data.account_sign = 1;
    ST[NUM_NOW].data.card_num = 0;
    ST[NUM_NOW].data.left_money = (float)0;
    strcpy(ST[NUM_NOW].data.last_time.password, "8888");

    // init card
    // calculate and give the card id
    id[0] = '3';
    sprintf(&(id[1]), "%d", serial_number);
    for (int i = 0; i <= 5; i++)
    {
        sum += id[i] - '0';
    }
    id[6] = 9 - sum % 10 + '0';
    id[7] = '\0';

    memcpy(temp_c, &id[2], sizeof(temp_c));
    temp_c[4] = '\0';
    temp_int = atoi(temp_c);

    while (CR[temp_int].if_taken) // find untaken position
    {
        if (temp_int == 9999)
        {
            temp_int = 0;
        }
        else
        {
            temp_int++;
        }
    }
    // link the card and the account
    CR[temp_int].student_account = &ST[NUM_NOW];
    ST[NUM_NOW].card = &CR[temp_int];
    // init card
    CR[temp_int].if_valid = 1;
    CR[temp_int].if_taken = 1;
    CR[temp_int].next = NULL;
    strcpy(CR[temp_int].data.card_id, id);
    strcpy(CR[temp_int].data.valid_time, "20240715");

    fprintf(logp, "%s,%s,%s,%s,�����ɹ�;",
            date_time_set,
            ST[NUM_NOW].data.account_id,
            ST[NUM_NOW].data.student_name,
            id);
    printf("%s,%s,%s,%s,�����ɹ�;",
           date_time_set,
           ST[NUM_NOW].data.account_id,
           ST[NUM_NOW].data.student_name,
           id);

    serial_number++;
    NUM_NOW++;
    return;
}

/* ���е��˹�ʧ */
void _gs()
{
    char key[11];
    int goal;
    printf("Please input your student number(10):");
    fflush(stdin);
    scanf("%s", &key);

    if ((goal = find_account_s(key)) == -1) // unfound
    {
        fprintf(logp, "%s,%s,,δ�ҵ�����,��ʧʧ��;\n",
                date_time_set,
                key);
        printf("%s,%s,,δ�ҵ�����,��ʧʧ��;\n",
               date_time_set,
               key);
    }
    else // found
    {
        if (check_info(&ST[goal]))
        {
            fputs(ST[goal].data.student_name, logp); // record the name in log
            fputs(",", logp);
            fputs(ST[1].card->data.card_id, logp); // record the card ID in log
            if (ST[goal].card->if_valid)
            {
                ST[goal].card->if_valid = 0;
                fprintf(logp, "%s,%s,%s,%s,��ʧ�ɹ�;\n",
                        date_time_set,
                        ST[goal].data.account_id,
                        ST[goal].data.student_name,
                        ST[goal].card->data.card_id);
                printf("%s,%s,%s,%s,��ʧ�ɹ�;\n",
                       date_time_set,
                       ST[goal].data.account_id,
                       ST[goal].data.student_name,
                       ST[goal].card->data.card_id);
            }
            else
            {
                fprintf(logp, "%s,%s,%s,%s,�ÿ��ѹ�ʧ,��ʧʧ��;\n",
                        date_time_set,
                        ST[goal].data.account_id,
                        ST[goal].data.student_name,
                        ST[goal].card->data.card_id);
                printf("%s,%s,%s,%s,�ÿ��ѹ�ʧ,��ʧʧ��;\n",
                       date_time_set,
                       ST[goal].data.account_id,
                       ST[goal].data.student_name,
                       ST[goal].card->data.card_id);
            }
        }
    }
    return;
}

/* ���е��˽�� */
void _jg()
{
    char key[11];
    int goal;
    printf("Please input your student number(10):");
    fflush(stdin);
    scanf("%s", &key);

    if ((goal = find_account_s(key)) == -1) // unfound
    {
        fprintf(logp, "%s,%s,δ�ҵ��û���������,���ʧ��;\n",
                date_time_set,
                key);
        printf("%s,%s,δ�ҵ��û���������,���ʧ��;\n",
               date_time_set,
               key);
    }
    else // found
    {
        if (check_info(&ST[goal]))
        {

            if (!ST[goal].card->if_valid)
            {
                ST[goal].card->if_valid = 1;
                fprintf(logp, "%s,%s,%s,%s,��ҳɹ�;\n",
                        date_time_set,
                        ST[goal].data.account_id,
                        ST[goal].data.student_name,
                        ST[goal].card->data.card_id);
                printf("%s,%s,%s,%s,��ҳɹ�;\n",
                       date_time_set,
                       ST[goal].data.account_id,
                       ST[goal].data.student_name,
                       ST[goal].card->data.card_id);
            }
            else
            {
                fprintf(logp, "%s,%s,%s,%s,���ʧ��,�ÿ�δ��ʧ;\n",
                        date_time_set,
                        ST[goal].data.account_id,
                        ST[goal].data.student_name,
                        ST[goal].card->data.card_id);
                printf("%s,%s,%s,%s,���ʧ��,�ÿ�δ��ʧ;\n",
                       date_time_set,
                       ST[goal].data.account_id,
                       ST[goal].data.student_name,
                       ST[goal].card->data.card_id);
            }
        }
        return;
    }
}

/* ���е��˲��� */
void _bk()
{
    char key[11];
    int goal;
    printf("Please input your student number(10):");
    fflush(stdin);
    scanf("%s", &key);
    fflush(stdin);

    if ((goal = find_account_s(key)) == -1) // unfound
    {
        fprintf(logp, "%s,%s,δ�ҵ��û���������,����ʧ��;\n",
                date_time_set,
                key);
        printf("%s,%s,δ�ҵ��û���������,����ʧ��;\n",
               date_time_set,
               key);
    }
    else // found
    {
        if (check_info(&ST[goal]))
        {
            if (ST[goal].data.card_num > 100)
            {
                fprintf(logp, "%s,%s,%s,%s,����ʧ��,����������������;\n",
                        date_time_set,
                        ST[goal].data.account_id,
                        ST[goal].data.student_name,
                        ST[goal].card->data.card_id);
                printf("%s,%s,%s,%s,����ʧ��,����������������;\n",
                       date_time_set,
                       ST[goal].data.account_id,
                       ST[goal].data.student_name,
                       ST[goal].card->data.card_id);
                // printf("You have exceeded the maximum number of refills!");
                return;
            }
            else
            {
                // establish a new invalid card
                struct invalid_card *p = (struct invalid_card *)malloc(sizeof(struct invalid_card));
                strcpy(p->data.card_id, ST[goal].card->data.card_id);
                strcpy(p->data.valid_time, ST[goal].card->data.valid_time);

                p->next = ST[goal].card->next;
                ST[goal].card->next = NULL;
                ST[goal].data.card_num++;

                // deal with old card
                char reme[8];
                strcpy(reme, ST[goal].card->data.card_id);
                reme[7] = '\0';
                strcpy(ST[goal].card->data.card_id, "\0");
                ST[goal].card->if_taken = 0;
                ST[goal].card->if_valid = 0;
                ST[goal].card->student_account = NULL;

                // distribute a new valid card
                int sum = 0, temp_int;
                char id[8], temp_c[5];
                id[0] = '3';
                sprintf(&(id[1]), "%d", serial_number);
                serial_number++;
                for (int i = 0; i <= 5; i++)
                {
                    sum += id[i] - '0';
                }
                id[6] = 9 - sum % 10 + '0';
                id[7] = '\0';
                strncpy(temp_c, &id[2], sizeof(temp_c));
                temp_int = atoi(temp_c);

                while (CR[temp_int].if_taken) // find untaken position
                {
                    temp_int++;
                    if (temp_int == MAX_STU)
                    {
                        temp_int = 0;
                    }
                }

                CR[temp_int].if_valid = 1; // initialization
                CR[temp_int].if_taken = 1;
                fprintf(logp, "%s,%s,%s,%s �� %s,�����ɹ�;\n",
                        date_time_set,
                        ST[goal].data.account_id,
                        ST[goal].data.student_name,
                        reme,
                        id);
                printf("%s,%s,%s,%s �� %s,�����ɹ�;\n",
                       date_time_set,
                       ST[goal].data.account_id,
                       ST[goal].data.student_name,
                       reme,
                       id);
                strcpy(CR[temp_int].data.card_id, id);
                CR[temp_int].data.card_id[7]='\0';
                strcpy(CR[temp_int].data.valid_time, "20240715");

                CR[temp_int].next = p;
                CR[temp_int].student_account = &ST[goal];
                ST[goal].card = &CR[temp_int];

                printf("�����ɹ�");
            }
        }
        return;
    }
}

/* ���е������� */
void _xh()
{
    char key[11];
    int goal;
    printf("Please input your student number(10):");
    fflush(stdin);
    scanf("%s", &key);
    fflush(stdin);
    if ((goal = find_account_s(key)) == -1) // unfound
    {
        fprintf(logp, "%s,%s,δ�ҵ��û���������,����ʧ��;\n",
                date_time_set,
                key);
        printf("%s,%s,δ�ҵ��û���������,����ʧ��;\n",
               date_time_set,
               key);
    }
    else // found
    {
        if (check_info(&ST[goal]))
        {
            ST[goal].data.account_sign = 0;
            ST[goal].data.left_money = (float)0;

            struct invalid_card *pa = (ST[goal].card)->next;
            if (pa) // exist other invalid cards
            {
                struct invalid_card *pb = pa->next;
                while (pb)
                {
                    free(pa);
                    pa = pb;
                    pb = pb->next;
                }
                free(pa);
                pa = pb = NULL;
            }
            ST[goal].card->if_taken = 0;
            ST[goal].card->if_valid = 0;
            ST[goal].card->next = NULL;
            ST[goal].card->student_account = NULL;
            // Successfully cancle account

            // printf("�����ɹ�;\n");
            fprintf(logp, "%s,%s,%s,�����ɹ�;\n",
                    date_time_set,
                    ST[goal].data.account_id,
                    ST[goal].data.student_name);
            printf("%s,%s,%s,�����ɹ�;\n",
                   date_time_set,
                   ST[goal].data.account_id,
                   ST[goal].data.student_name);
        }
        return;
    }
}

/* ���е��˳�ֵ */
void _cz()
{
    char key[11];
    int goal, add;
    printf("Please input your student number(10):");
    fflush(stdin);
    scanf("%s", &key);
    fflush(stdin);

    printf("\n��ֵ���:");
    fflush(stdin);
    scanf("%d", &add);
    fflush(stdin);
    if ((goal = find_account_s(key)) == -1) // unfound
    {
        fprintf(logp, "%s,%s,δ�ҵ��û���������,��ֵʧ��;\n",
                date_time_set,
                key);
        printf("%s,%s,δ�ҵ��û���������,��ֵʧ��;\n",
               date_time_set,
               key);
    }
    else // found
    {
        if (check_info(&ST[goal]))
        {
            float money = (float)add;
            if (!ST[goal].card->if_valid)
            {
                fprintf(logp, "%s,%s,%s,����ʧЧ, ��ֵʧ��;\n",
                        date_time_set,
                        key,
                        ST[goal].data.student_name);
                printf("%s,%s,%s,����ʧЧ, ��ֵʧ��;\n",
                       date_time_set,
                       key,
                       ST[goal].data.student_name);
            }
            else if (ST[goal].data.left_money + money < (float)1000.0)
            {
                fprintf(logp, "%s,%s,%s,%.2f �� %.2f,��ֵ�ɹ�;\n",
                        date_time_set,
                        ST[goal].data.account_id,
                        ST[goal].data.student_name,
                        ST[goal].data.left_money,
                        ST[goal].data.left_money + money);
                printf("%s,%s,%s,%.2f �� %.2f,��ֵ�ɹ�;\n",
                       date_time_set,
                       ST[goal].data.account_id,
                       ST[goal].data.student_name,
                       ST[goal].data.left_money,
                       ST[goal].data.left_money + money);

                ST[goal].data.left_money += money;
                printf("��ֵ�ɹ�\n");
            }
            else
            {
                fprintf(logp, "%s,%s,%s,%.2f,����Ѵ�����(+%d),��ֵʧ��;\n",
                        date_time_set,
                        ST[goal].data.account_id,
                        ST[goal].data.student_name,
                        ST[goal].data.left_money,
                        add);
                printf("%s,%s,%s,%.2f,����Ѵ�����(+%d),��ֵʧ��;\n",
                       date_time_set,
                       ST[goal].data.account_id,
                       ST[goal].data.student_name,
                       ST[goal].data.left_money,
                       add);

                printf("����Ѵ�����,��ֵʧ��\n");
                return;
            }
        }
        return;
    }
}

/* ���е������� */
void _xf()
{
    int num, goal;
    char key[8], money[6]; // xx.xx
    float sub;
    printf("Please input window number:");
    fflush(stdin);
    scanf("%d", &num);
    fflush(stdin);

    printf("Card ID:");
    fflush(stdin);
    scanf("%s", key);
    fflush(stdin);

    printf("Amount of consumption:");
    fflush(stdin);
    scanf("%f", &sub);
    fflush(stdin);

    if ((goal = find_card_s(key)) == -1) // unfound
    {
        printf("���Ų�����,����ʧ��");
        fprintf(logp, "%s,%s,���Ų�����,����ʧ��;\n",
                date_time_set,
                key);
        printf("%s,%s,���Ų�����,����ʧ��;\n",
               date_time_set,
               key);
        return;
    }
    else // found
    {
        if (CR[goal].if_valid == 0)
        {

            fprintf(logp, "%s,%s,%s,��(%s)��ʧЧ,����ʧ��(-%.2f);\n",
                    date_time_set,
                    ST[goal].data.account_id,
                    ST[goal].data.student_name,
                    key,
                    sub);
            printf("%s,%s,%s,��(%s)��ʧЧ,����ʧ��(-%.2f);\n",
                   date_time_set,
                   ST[goal].data.account_id,
                   ST[goal].data.student_name,
                   key,
                   sub);
            return;
        }
        else if (CR[goal].student_account->data.left_money < sub)
        {
            char temp[10];
            fprintf(logp, "%s,%s,%s,����(%.2f),����ʧ��(-%.2f);\n",
                    date_time_set,
                    CR[goal].data.card_id,
                    CR[goal].student_account->data.student_name,
                    CR[goal].student_account->data.left_money,
                    sub);
            printf("%s,%s,%s,����(%.2f),����ʧ��(-%.2f);\n",
                   date_time_set,
                   CR[goal].data.card_id,
                   CR[goal].student_account->data.student_name,
                   CR[goal].student_account->data.left_money,
                   sub);

            return;
        }
        else
        {
            // ing
            char date_now[5], hour_now[3];
            strncpy(date_now, &date_time_set[4], 4);
            strncpy(hour_now, &date_time_set[8], 2);

            // need to enter password,put in log
            if (((!strcmp(date_now, CR[goal].student_account->data.last_time.last_date)) &&
                 (((strcmp(hour_now, "07") >= 0 && strcmp(CR[goal].student_account->data.last_time.last_hour, "07") >= 0) &&
                   (strcmp(hour_now, "08") <= 0 && strcmp(CR[goal].student_account->data.last_time.last_hour, "08") <= 0)) ||
                  ((strcmp(hour_now, "11") >= 0 && strcmp(CR[goal].student_account->data.last_time.last_hour, "11") >= 0) &&
                   (strcmp(hour_now, "12") <= 0 && strcmp(CR[goal].student_account->data.last_time.last_hour, "12") <= 0)) ||
                  ((strcmp(hour_now, "17") >= 0 && strcmp(CR[goal].student_account->data.last_time.last_hour, "17") >= 0) &&
                   (strcmp(hour_now, "18") <= 0 && strcmp(CR[goal].student_account->data.last_time.last_hour, "18") <= 0)))) ||
                sub >= 20.0)
            {
                if (CR[goal].student_account->data.last_time.add_up + sub >= 20.0)
                {
                    char answer[5];
                    fputc('*', logp);
                    printf("��ǰʱ�����ѳ���20Ԫ,����������:");
                    fflush(stdin);
                    scanf("%s", answer);
                    fflush(stdin);
                    if (!strcmp(answer, CR[goal].student_account->data.last_time.password))
                    {

                        CR[goal].student_account->data.last_time.add_up = 0;
                        goto succ;
                    }
                    else // fail
                    {
                        printf("\n*****�������,����ʧ��!\n");
                        fprintf(logp, "%s,�������,����ʧ��(-%.2f);\n", key, sub);
                        printf("%s,�������,����ʧ��(-%.2f);\n", key, sub);
                        return;
                    }
                }
                else
                {
                    CR[goal].student_account->data.last_time.add_up += sub;
                    goto succ;
                }
            }
            else
            {
            succ:
                fprintf(logp, "%s,%s,%s,%.2f �� %.2f,���ѳɹ�(-%.2f);\n",
                        date_time_set,
                        ST[goal].data.account_id,
                        ST[goal].data.student_name,
                        ST[goal].data.left_money,
                        ST[goal].data.left_money - sub,
                        sub);
                printf("%s,%s,%s,%.2f �� %.2f,���ѳɹ�(-%.2f);\n",
                       date_time_set,
                       ST[goal].data.account_id,
                       ST[goal].data.student_name,
                       ST[goal].data.left_money,
                       ST[goal].data.left_money - sub,
                       sub);

                CR[goal].student_account->data.last_time.add_up = sub;
                CR[goal].student_account->data.left_money -= sub;

                printf("���ѳɹ�,-%.2f", sub);
                // after
                strcpy(CR[goal].student_account->data.last_time.last_date, date_now);
                strcpy(CR[goal].student_account->data.last_time.last_hour, hour_now);
            }

            sprintf(data_win[num][location[num].tail], "%s,%d,%s(%s),%s(-%.2f);\n",
                    date_time_set,
                    num + 1,
                    CR[goal].student_account->data.student_name,
                    CR[goal].student_account->data.account_id,
                    CR[goal].data.card_id,
                    sub);

            set_location_s(num); //����Ŀǰѭ�����е�ͷ��β

            return;
        }
    }
}

/* use the method of binary search to find the student id
���룺Ŀ���ַ���
������Ŀ���ַ����������±� */
int find_account_s(char *key)
{
    int low = 0;
    int high = single_begin - 1;
    int mid;
    while (low <= high)
    {
        mid = (low + high) / 2;
        if (!strcmp((ST[mid]).data.account_id, key)) // equal
        {
            if (ST[mid].data.account_sign)
            {

                return mid;
            }
            else
            {
                printf("���˻�������!\n"); // the account has been cancled
                return ERROR;
            }
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

    for (int i = single_begin; i < NUM_NOW; i++)
    {
        if (!strcmp(key, ST[i].data.account_id))
        {
            return i;
        }
    }

    // the ID has not been serached
    printf("��ѧ�Ų�����!\n");

    return ERROR;
}

/* hash����п��Ų���
���룺Ŀ�꿨��
������Ŀ��ѧ���������±� */
int find_card_s(char *key)
{
    char temp_c[5];
    int temp_int, symbol;

    memcpy(temp_c, &key[2], sizeof(temp_c));
    temp_c[4] = '\0';
    temp_int = atoi(temp_c);
    if(temp_int)
    {
        symbol = temp_int-1;
    }
    else
    {
        symbol = 9999;
    }
    

    while (strcmp(key, CR[temp_int].data.card_id))
    {
        if ((CR[temp_int].if_taken == 0) || (temp_int == symbol))
        {
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

/* ���ѧ������Ϣ�Ƿ���ȷ
���룺ָ����˻��Ľṹ��ָ�� p
��������ѧ����Ϣ�Ƿ���ȷ����ȷ return 1������ return 0 */
bool check_info(struct account *p)
{
    char c;
    while (1)
    {
        printf("\n*Please check your information:\n"
               "*Name : %s \n"
               "*Student Number : %s \n"
               "*card ID : %s \n"
               "if yes,please input y;\n"
               "if not,please input n.\n",
               p->data.student_name, p->data.account_id, p->card->data.card_id);

        fflush(stdin);
        scanf("%c", &c);
        fflush(stdin);
        if (c == 'y')
        {
            return 1;
        }
        else if (c == 'n')
        {
            return 0;
        }
    }
}

/* ����ѧ��ѧ����������ģ����ѯ */
void _search()
{
    char c, z;
    char str_cmp[11], key[11];
    double max_cmp = 0.0, temp;
    int max_sym[20];
    int flag_over = 0;

    printf("����orѧ��?\n"
           "if ����,Input '1';\n"
           "if ѧ��,Input '2';\n"
           "Input Here: ");
    fflush(stdin);
    scanf("%c", &c);
    fflush(stdin);
    printf("Input ����/ѧ��:");
    fflush(stdin);
    scanf("%s", key);
    fflush(stdin);

    for (int i = 0; i < NUM_NOW; i++)
    {
        if (c == '1')
        {
            strcpy(str_cmp, ST[i].data.student_name);
        }
        else if (c == '2')
        {
            strcpy(str_cmp, ST[i].data.account_id);
        }
        else
        {
            return;
        }

        temp = cal_similarity(key, str_cmp);
        if (max_cmp < temp) // better result
        {
            for (int j = 1; j < 20; j++)
            {
                max_sym[j] = -1;
            }
            max_sym[1] = i;
            max_sym[0] = 2; // max_sym[0]�洢�������������±�
            max_cmp = temp;
            flag_over = 0;
        }
        else if (max_cmp == temp) // same
        {
            max_sym[max_sym[0]] = i;
            if (max_sym[0] == 19)
            {
                flag_over = 1;
            }
            else
            {
                max_sym[0]++;
            }
        }
    }

    if (flag_over)
    {
        printf("ƥ��������,�������׼ȷ��ֵ;");
        return;
    }
    for (int i = 1; i < max_sym[0]; i++)
    {
        printf("*%d : %s\t%s\n", i, ST[max_sym[i]].data.student_name, ST[max_sym[i]].data.account_id);
    }

    printf("��ѡ����ȷ�����:");
    fflush(stdin);
    scanf("%d", &z);
    fflush(stdin);

    printf("\n*�������:\n"
           "*Name : %s \n"
           "*Student Number : %s \n"
           "*card ID : %s \n"
           "*Left money : %.2f\n",
           ST[max_sym[z]].data.student_name, ST[max_sym[z]].data.account_id, ST[max_sym[z]].card->data.card_id, ST[max_sym[z]].data.left_money);

    return;
}
/* ��������ִ���ƥ�䴮��ƥ��ȣ��������ַ�����ƥ���
���룺�����ģ���ַ���
�����������ƥ��� */
double cal_similarity(char *a, char *b)
{
    int i, j, max_length = 0;
    int a_length = strlen(a);
    int b_length = strlen(b);
    //����ռ�
    int **temp_array = (int **)malloc((sizeof(int *)) * (a_length + 1));
    assert(temp_array);
    for (i = 0; i < a_length + 1; i++)
    {
        temp_array[i] = (int *)malloc((sizeof(int)) * (b_length + 1));
        assert(temp_array[i]);
    }

    for (i = 0; i < a_length + 1; i++)
        for (j = 0; j < b_length + 1; j++)
            temp_array[i][j] = 0;

    for (i = 1; i < a_length + 1; i++)
        for (j = 1; j < b_length + 1; j++)
            if (a[i - 1] == b[j - 1])
            {
                temp_array[i][j] = temp_array[i - 1][j - 1] + 1;
                max_length = max_length < temp_array[i][j] ? temp_array[i][j] : max_length;
            }

    // free
    for (i = 0; i < a_length + 1; i++)
    {
        free(temp_array[i]);
    }
    free(temp_array);
    return (2 * (double)max_length / (strlen(a) + strlen(b)));
}

/* �������е�ͷ��β����¼λ��
    ���룺Ŀ�괰�ں� */
void set_location_s(int num)
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
            location[num].head = location[num].tail + 1;
        }
    }

    return;
}