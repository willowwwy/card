#include "card_operate.h"

/* identify differnt operation */
void operate_card(int if_only_charge, char *limit_time)
{

    int choice;
    int row = 2;
    static int if_first = 1;
    if (if_first)
    {
        // check if the file is correct
        if (fgetc(cz_fp) != 'C' || fgetc(cz_fp) != 'Z' || fgetc(cz_fp) != '\n')
        {
            fputs("文件名错误,读取信息失败\n", logp);
            return;
        }
        if_first = 0;
    }

    while (fgetc(cz_fp) != EOF) // next row
    {
        char time_cmp[17];
        char key[11];
        char key_oper[5];
        int goal;
        fseek(cz_fp, -1, SEEK_CUR);
        fgets(time_cmp, 17, cz_fp); // read the time

        if (if_end_time(time_cmp, limit_time)) // whether the time is over 9.3-9.6
        {
            fseek(cz_fp, -16, SEEK_CUR);
            return;
        }

        fputs(time_cmp, logp); // read the time in log
        fputc(',', logp);

        if (fgetc(cz_fp) != ',') // check the format
        {
            goto exit;
        }

        // decide what to operate
        if (!if_only_charge)
        {
            fgets(key_oper, 5, cz_fp); // get the operation
            if (!strcmp(key_oper, "挂失"))
            {
                choice = 1;
            }
            else if (!strcmp(key_oper, "解挂"))
            {
                choice = 2;
            }
            else if (!strcmp(key_oper, "补卡"))
            {
                choice = 3;
            }
            else if (!strcmp(key_oper, "销户"))
            {
                choice = 4;
            }
            else if (!strcmp(key_oper, "充值"))
            {
                choice = 5;
            }
            else
            {
                goto exit;
            }
        }
        else
        {
            fgets(key_oper, 5, cz_fp); // get the operation
            choice = 5;
        }

        if (fgetc(cz_fp) != ',') // check the format
        {
            goto exit;
        }

        fgets(key, 11, cz_fp); // read the student number

        // record the student number in log
        fputs(key, logp);
        fputc(',', logp);

        goal = find_account(key, &row); // find the student account

        switch (choice)
        {
        case 1: // report loss
        {
            if (goal == -1)
            {
                fputs(",挂失失败;\n", logp);
                READ_END_EXIT;
                continue;
            }
            fputs(ST[goal].data.student_name, logp); // record the name in log
            fputs(",", logp);

            report_loss_card(goal);
            break;
        }
        case 2: // cancle the loss report
        {
            if (goal == -1)
            {
                fputs(",解挂失败;\n", logp);
                READ_END_EXIT;
                continue;
            }
            fputs(ST[goal].data.student_name, logp); // record the name in log
            fputs(",", logp);

            cancle_loss_card(goal);
            break;
        }
        case 3: // Reapply for a new card
        {
            if (goal == -1)
            {
                fputs(",补卡失败;\n", logp);
                READ_END_EXIT;
                continue;
            }
            fputs(ST[goal].data.student_name, logp); // record the name in log
            fputs(",", logp);

            reissue_card(goal);
            break;
        }
        case 4: // Cancle account
        {
            if (goal == -1)
            {
                fputs(",销户失败;\n", logp);
                READ_END_EXIT;
                continue;
            }
            fputs(ST[goal].data.student_name, logp); // record the name in log
            fputs(",", logp);

            cancle_account(goal);
            break;
        }
        case 5: // recharge
        {
            char temp[4];
            int add = 0;
            if (fgetc(cz_fp) != ',')
            {
                goto exit;
            }
            fgets(temp, 4, cz_fp); // read the money
            add = atoi(temp);

            if (goal == -1)
            {
                fputs(",解挂失败;\n", logp);
                READ_END_EXIT;
                continue;
            }
            fputs(ST[goal].data.student_name, logp); // record the name in log
            fputs(",", logp);

            recharge_card(goal, add);
            break;
        }
        }

        READ_END_EXIT; // check the format
        row++;         // record the row
    }

    return;
exit:
    printf("The format of the file is wrong!");
    return;
}

/*report the loss and make the card invalid
 *the address of the all system
 *the location of the student*/
void report_loss_card(int goal)
{
    fputs(ST[goal].card->data.card_id, logp); // record the card ID in log
    if (ST[goal].card->if_valid)
    {
        ST[goal].card->if_valid = 0;
        fputs(",挂失成功;\n", logp);
    }
    else
    {
        fputs(",该卡已挂失,挂失失败;\n", logp);
    }
    return;
}

/* cancle the operation of reporting loss and regain the valid card
 *the address of the all system
 *the location of the student*/
void cancle_loss_card(int goal)
{

    fputs(ST[goal].card->data.card_id, logp); // record the card ID in log
    if (!ST[goal].card->if_valid)
    {
        ST[goal].card->if_valid = 1;
        fputs(",解挂成功;\n", logp);
    }
    else
    {
        fputs(",解挂失败,该卡未挂失;\n", logp);
    }

    return;
}

/*Reapply for a new card
 *the address of the all system
 *the location of the student*/
void reissue_card(int goal)
{
    // whether the number of cards has reached the upper limit
    if (ST[goal].data.card_num > 100)
    {
        fputs(",补卡失败,补卡次数超过上限;\n", logp);
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
        fputs(ST[goal].card->data.card_id, logp);
        fputs(" → ", logp); // record the old card ID
        strcpy(ST[goal].card->data.card_id, "\0");
        ST[goal].card->if_taken = 0;
        ST[goal].card->if_valid = 0;
        ST[goal].card->student_account = NULL;

        // distribute a new valid card
        int sum = 0, temp_int;
        char id[8], temp_c[5];
        id[0] = '3';
        sprintf(&(id[1]), "%d", serial_number);
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
            if (temp_int >= MAX_STU)
            {
                temp_int = 0;
            }
        }

        CR[temp_int].if_valid = 1; // initialization
        CR[temp_int].if_taken = 1;
        strcpy(CR[temp_int].data.card_id, id);
        strcpy(CR[temp_int].data.valid_time, "20240715");

        CR[temp_int].next = p;
        CR[temp_int].student_account = &ST[goal];
        ST[goal].card = &CR[temp_int];

        fputs(ST[goal].card->data.card_id, logp); // record the new card ID
        fputs(",补卡成功;\n", logp);
        serial_number++;
    }
    return;
}

/*log out account
 *the address of the all system
 *the location of the student*/
void cancle_account(int goal)
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
    fputs("销户成功;\n", logp);

    return;
}

/*recharge for the account
 *the address of the all system
 *the location of the student
 *how much to recharge*/
void recharge_card(int goal, int add)
{
    float money = (float)add;
    if (!ST[goal].card->if_valid)
    {
        fputs("卡已失效,充值失败;\n", logp);
    }
    else if (ST[goal].data.left_money + money < (float)1000.0)
    {
        char temp1[7], temp3[7];
        sprintf(temp1, "%.2f", ST[goal].data.left_money);
        fputs(temp1, logp);

        ST[goal].data.left_money += money;
        fputs(" → ", logp);

        sprintf(temp3, "%.2f", ST[goal].data.left_money);
        fputs(temp3, logp);
        fputs(",充值成功;\n", logp);
    }
    else
    {
        char temp1[7], temp2[7];
        sprintf(temp1, "%.2f", ST[goal].data.left_money);
        fputs(temp1, logp);
        fputs(",金额已达上限(+", logp);

        sprintf(temp2, "%d", add);
        fputs(temp2, logp);

        fputs("),充值失败;\n", logp);
        return;
    }

    return;
}

// use the methods of binary search to find the student id
int find_account(char *key, int *row)
{
    int low = 0;
    int high = NUM_NOW - 1;
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
                // printf("The student's ID %s in %d not exists!\n" , key, row);//the account has been cancled
                fputs("该账户已销户", logp);
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

    printf("The student's ID %s in %d not exists!\n", key, *row); // the ID has not been serached
    fputs("该学号不存在", logp);

    return ERROR;
}

bool if_end_time(char *now_time, char *limit_time)
{
    if (strcmp(now_time, limit_time) > 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}