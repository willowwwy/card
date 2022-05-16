#include "single.h"

//总体操控函数
void single_control()
{
    int choice;
    char date_set[9], time_set[9];
    single_begin = NUM_NOW;
    while (1)
    {
        printf("\n选择操作:\n\n"
               "1.开户\n"
               "2.挂失\n"
               "3.解挂\n"
               "4.补卡\n"
               "5.销户\n"
               "6.充值\n"
               "7.消费\n"
               "8.查询\n"
               "9.更改日期\n"
               "10.退出\n\n"
               "在此输入:");
        fflush(stdin);
        scanf("%d", &choice);
        fflush(stdin);
        switch (choice)
        {
        case 1:
        {
            _kh(); //开户
            break;
        }
        case 2:
        {
            _gs(); //挂失
            break;
        }
        case 3:
        {
            _jg(); //解挂
            break;
        }
        case 4:
        {
            _bk(); //补卡
            break;
        }
        case 5:
        {
            _xh(); //销户
            break;
        }
        case 6:
        {
            _cz(); //充值
            break;
        }
        case 7:
        {
            _xf(); //消费
            break;
        }
        case 8:
        {
            _search(); //模糊查询
            break;
        }
        case 9:
        {
            printf("NOW:%s\n", date_time_set);
            printf("输入下一日期(8):");
            fflush(stdin);
            scanf("%s", date_set);
            fflush(stdin);
            printf("输入下一时间(8):");
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

/* 进行单人开户 */
void _kh()
{
    int sum = 0, temp_int;
    char id[8], temp_c[5];

    if (NUM_NOW >= 10000)
    {
        printf("开户人数已超上限!");
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

    fprintf(logp, "%s,%s,%s,%s,开户成功;",
            date_time_set,
            ST[NUM_NOW].data.account_id,
            ST[NUM_NOW].data.student_name,
            id);
    printf("%s,%s,%s,%s,开户成功;",
           date_time_set,
           ST[NUM_NOW].data.account_id,
           ST[NUM_NOW].data.student_name,
           id);

    serial_number++;
    NUM_NOW++;
    return;
}

/* 进行单人挂失 */
void _gs()
{
    char key[11];
    int goal;
    printf("Please input your student number(10):");
    fflush(stdin);
    scanf("%s", &key);

    if ((goal = find_account_s(key)) == -1) // unfound
    {
        fprintf(logp, "%s,%s,,未找到卡号,挂失失败;\n",
                date_time_set,
                key);
        printf("%s,%s,,未找到卡号,挂失失败;\n",
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
                fprintf(logp, "%s,%s,%s,%s,挂失成功;\n",
                        date_time_set,
                        ST[goal].data.account_id,
                        ST[goal].data.student_name,
                        ST[goal].card->data.card_id);
                printf("%s,%s,%s,%s,挂失成功;\n",
                       date_time_set,
                       ST[goal].data.account_id,
                       ST[goal].data.student_name,
                       ST[goal].card->data.card_id);
            }
            else
            {
                fprintf(logp, "%s,%s,%s,%s,该卡已挂失,挂失失败;\n",
                        date_time_set,
                        ST[goal].data.account_id,
                        ST[goal].data.student_name,
                        ST[goal].card->data.card_id);
                printf("%s,%s,%s,%s,该卡已挂失,挂失失败;\n",
                       date_time_set,
                       ST[goal].data.account_id,
                       ST[goal].data.student_name,
                       ST[goal].card->data.card_id);
            }
        }
    }
    return;
}

/* 进行单人解挂 */
void _jg()
{
    char key[11];
    int goal;
    printf("Please input your student number(10):");
    fflush(stdin);
    scanf("%s", &key);

    if ((goal = find_account_s(key)) == -1) // unfound
    {
        fprintf(logp, "%s,%s,未找到用户或已销户,解挂失败;\n",
                date_time_set,
                key);
        printf("%s,%s,未找到用户或已销户,解挂失败;\n",
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
                fprintf(logp, "%s,%s,%s,%s,解挂成功;\n",
                        date_time_set,
                        ST[goal].data.account_id,
                        ST[goal].data.student_name,
                        ST[goal].card->data.card_id);
                printf("%s,%s,%s,%s,解挂成功;\n",
                       date_time_set,
                       ST[goal].data.account_id,
                       ST[goal].data.student_name,
                       ST[goal].card->data.card_id);
            }
            else
            {
                fprintf(logp, "%s,%s,%s,%s,解挂失败,该卡未挂失;\n",
                        date_time_set,
                        ST[goal].data.account_id,
                        ST[goal].data.student_name,
                        ST[goal].card->data.card_id);
                printf("%s,%s,%s,%s,解挂失败,该卡未挂失;\n",
                       date_time_set,
                       ST[goal].data.account_id,
                       ST[goal].data.student_name,
                       ST[goal].card->data.card_id);
            }
        }
        return;
    }
}

/* 进行单人补卡 */
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
        fprintf(logp, "%s,%s,未找到用户或已销户,补卡失败;\n",
                date_time_set,
                key);
        printf("%s,%s,未找到用户或已销户,补卡失败;\n",
               date_time_set,
               key);
    }
    else // found
    {
        if (check_info(&ST[goal]))
        {
            if (ST[goal].data.card_num > 100)
            {
                fprintf(logp, "%s,%s,%s,%s,补卡失败,补卡次数超过上限;\n",
                        date_time_set,
                        ST[goal].data.account_id,
                        ST[goal].data.student_name,
                        ST[goal].card->data.card_id);
                printf("%s,%s,%s,%s,补卡失败,补卡次数超过上限;\n",
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
                fprintf(logp, "%s,%s,%s,%s → %s,补卡成功;\n",
                        date_time_set,
                        ST[goal].data.account_id,
                        ST[goal].data.student_name,
                        reme,
                        id);
                printf("%s,%s,%s,%s → %s,补卡成功;\n",
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

                printf("补卡成功");
            }
        }
        return;
    }
}

/* 进行单人销户 */
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
        fprintf(logp, "%s,%s,未找到用户或已销户,销户失败;\n",
                date_time_set,
                key);
        printf("%s,%s,未找到用户或已销户,销户失败;\n",
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

            // printf("销户成功;\n");
            fprintf(logp, "%s,%s,%s,销户成功;\n",
                    date_time_set,
                    ST[goal].data.account_id,
                    ST[goal].data.student_name);
            printf("%s,%s,%s,销户成功;\n",
                   date_time_set,
                   ST[goal].data.account_id,
                   ST[goal].data.student_name);
        }
        return;
    }
}

/* 进行单人充值 */
void _cz()
{
    char key[11];
    int goal, add;
    printf("Please input your student number(10):");
    fflush(stdin);
    scanf("%s", &key);
    fflush(stdin);

    printf("\n充值金额:");
    fflush(stdin);
    scanf("%d", &add);
    fflush(stdin);
    if ((goal = find_account_s(key)) == -1) // unfound
    {
        fprintf(logp, "%s,%s,未找到用户或已销户,充值失败;\n",
                date_time_set,
                key);
        printf("%s,%s,未找到用户或已销户,充值失败;\n",
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
                fprintf(logp, "%s,%s,%s,卡已失效, 充值失败;\n",
                        date_time_set,
                        key,
                        ST[goal].data.student_name);
                printf("%s,%s,%s,卡已失效, 充值失败;\n",
                       date_time_set,
                       key,
                       ST[goal].data.student_name);
            }
            else if (ST[goal].data.left_money + money < (float)1000.0)
            {
                fprintf(logp, "%s,%s,%s,%.2f → %.2f,充值成功;\n",
                        date_time_set,
                        ST[goal].data.account_id,
                        ST[goal].data.student_name,
                        ST[goal].data.left_money,
                        ST[goal].data.left_money + money);
                printf("%s,%s,%s,%.2f → %.2f,充值成功;\n",
                       date_time_set,
                       ST[goal].data.account_id,
                       ST[goal].data.student_name,
                       ST[goal].data.left_money,
                       ST[goal].data.left_money + money);

                ST[goal].data.left_money += money;
                printf("充值成功\n");
            }
            else
            {
                fprintf(logp, "%s,%s,%s,%.2f,金额已达上限(+%d),充值失败;\n",
                        date_time_set,
                        ST[goal].data.account_id,
                        ST[goal].data.student_name,
                        ST[goal].data.left_money,
                        add);
                printf("%s,%s,%s,%.2f,金额已达上限(+%d),充值失败;\n",
                       date_time_set,
                       ST[goal].data.account_id,
                       ST[goal].data.student_name,
                       ST[goal].data.left_money,
                       add);

                printf("金额已达上限,充值失败\n");
                return;
            }
        }
        return;
    }
}

/* 进行单人消费 */
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
        printf("卡号不存在,消费失败");
        fprintf(logp, "%s,%s,卡号不存在,消费失败;\n",
                date_time_set,
                key);
        printf("%s,%s,卡号不存在,消费失败;\n",
               date_time_set,
               key);
        return;
    }
    else // found
    {
        if (CR[goal].if_valid == 0)
        {

            fprintf(logp, "%s,%s,%s,卡(%s)已失效,消费失败(-%.2f);\n",
                    date_time_set,
                    ST[goal].data.account_id,
                    ST[goal].data.student_name,
                    key,
                    sub);
            printf("%s,%s,%s,卡(%s)已失效,消费失败(-%.2f);\n",
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
            fprintf(logp, "%s,%s,%s,余额不足(%.2f),消费失败(-%.2f);\n",
                    date_time_set,
                    CR[goal].data.card_id,
                    CR[goal].student_account->data.student_name,
                    CR[goal].student_account->data.left_money,
                    sub);
            printf("%s,%s,%s,余额不足(%.2f),消费失败(-%.2f);\n",
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
                    printf("当前时段消费超过20元,请输入密码:");
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
                        printf("\n*****密码错误,消费失败!\n");
                        fprintf(logp, "%s,密码错误,消费失败(-%.2f);\n", key, sub);
                        printf("%s,密码错误,消费失败(-%.2f);\n", key, sub);
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
                fprintf(logp, "%s,%s,%s,%.2f → %.2f,消费成功(-%.2f);\n",
                        date_time_set,
                        ST[goal].data.account_id,
                        ST[goal].data.student_name,
                        ST[goal].data.left_money,
                        ST[goal].data.left_money - sub,
                        sub);
                printf("%s,%s,%s,%.2f → %.2f,消费成功(-%.2f);\n",
                       date_time_set,
                       ST[goal].data.account_id,
                       ST[goal].data.student_name,
                       ST[goal].data.left_money,
                       ST[goal].data.left_money - sub,
                       sub);

                CR[goal].student_account->data.last_time.add_up = sub;
                CR[goal].student_account->data.left_money -= sub;

                printf("消费成功,-%.2f", sub);
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

            set_location_s(num); //调整目前循环队列的头与尾

            return;
        }
    }
}

/* use the method of binary search to find the student id
传入：目标字符串
传出：目标字符串的数组下标 */
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
                printf("该账户已销户!\n"); // the account has been cancled
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
    printf("该学号不存在!\n");

    return ERROR;
}

/* hash表进行卡号查找
传入：目标卡号
传出：目标学生的数组下标 */
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

/* 检查学生的信息是否正确
传入：指向该账户的结构体指针 p
传出：该学生信息是否正确，正确 return 1，否则 return 0 */
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

/* 进行学生学号与姓名的模糊查询 */
void _search()
{
    char c, z;
    char str_cmp[11], key[11];
    double max_cmp = 0.0, temp;
    int max_sym[20];
    int flag_over = 0;

    printf("名字or学号?\n"
           "if 名字,Input '1';\n"
           "if 学号,Input '2';\n"
           "Input Here: ");
    fflush(stdin);
    scanf("%c", &c);
    fflush(stdin);
    printf("Input 姓名/学号:");
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
            max_sym[0] = 2; // max_sym[0]存储结果中最大数组下标
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
        printf("匹配结果过多,请输入更准确的值;");
        return;
    }
    for (int i = 1; i < max_sym[0]; i++)
    {
        printf("*%d : %s\t%s\n", i, ST[max_sym[i]].data.student_name, ST[max_sym[i]].data.account_id);
    }

    printf("请选择正确的序号:");
    fflush(stdin);
    scanf("%d", &z);
    fflush(stdin);

    printf("\n*结果如下:\n"
           "*Name : %s \n"
           "*Student Number : %s \n"
           "*card ID : %s \n"
           "*Left money : %.2f\n",
           ST[max_sym[z]].data.student_name, ST[max_sym[z]].data.account_id, ST[max_sym[z]].card->data.card_id, ST[max_sym[z]].data.left_money);

    return;
}
/* 计算查找字串与匹配串的匹配度，并返回字符串的匹配度
传入：输入的模糊字符串
传出：计算的匹配度 */
double cal_similarity(char *a, char *b)
{
    int i, j, max_length = 0;
    int a_length = strlen(a);
    int b_length = strlen(b);
    //分配空间
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

/* 调整队列的头与尾，记录位置
    传入：目标窗口号 */
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