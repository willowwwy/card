#include "account_open.h"


/* establish a new account and a card */
void open_account(void)
{
    FILE *kh_fp;
    char c1;
    int i;
    NUM_NOW = 0;
    serial_number = 12346;
    kh_fp = fopen(KH, "r");
    if (!kh_fp)
    {
        printf("读取文件失败");
        return;
    }

    if (fgetc(kh_fp) != 'K' || fgetc(kh_fp) != 'H' || fgetc(kh_fp) != '\n') // check if the file is correct
    {
        printf("Wrong Filename!");
        fputs("文件名错误", logp);
        return;
    }

    for (i = 0; (fgetc(kh_fp) != EOF) && i < MAX_STU; i++) // the next student's information
    {
        struct account *p = &ST[i];
        char c;
        int j = 0;
        fseek(kh_fp, -1, SEEK_CUR);
        fgets(p->data.account_id, 11, kh_fp); // read the student number
        if (fgetc(kh_fp) != ',')
            goto exit; // ,

        fputs(p->data.account_id, logp); // record the student number in log
        fputc(',', logp);

        while ((c = fgetc(kh_fp)) != ';' && j < 9) // read the student's name
        {
            p->data.student_name[j] = c;
            j++;
        }
        p->data.student_name[j] = '\0';

        fputs(p->data.student_name, logp); // record the student name in log
        fputc(',', logp);

        p->data.account_sign = 1;
        p->data.card_num = 0;
        p->data.left_money = (float)0;
        p->data.last_time.add_up=0;
        strcpy(p->data.last_time.last_date,"0000");
        strcpy(p->data.last_time.last_hour,"00");
        strcpy(p->data.last_time.password, "8888");
        if (open_card(p))
        {
            fputs(p->card->data.card_id, logp); // record the card ID in log
            fputs(",开户成功;\n", logp);
            NUM_NOW++;
        }
        else
        {
            fputs("开户失败;\n", logp);
        }

        if (fgetc(kh_fp) != '\n')
            goto exit; // \n
    }

    i--;
    if (i >= 10000) // judge the CORRECT scale
    {
        printf("Out Of Range!");
        goto exit;
    }

    fclose(kh_fp);
    return;
exit:
    printf("The format of the file is wrong!");
    fputs("批量文件格式错误\n", logp);
    fclose(kh_fp);
    return;
}


/*  establish a new card */
bool open_card(struct account *now)
{
    int sum = 0, temp_int;
    char id[8], temp_c[5];

    // calculate and give the card id information
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

    /* 建立hash表方便后续查找 */
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
    CR[temp_int].student_account = now; // link the card and the account
    now->card = &CR[temp_int];

    CR[temp_int].if_valid = 1; // initialization
    CR[temp_int].if_taken = 1;
    CR[temp_int].next = NULL;
    strcpy(CR[temp_int].data.card_id, id);
    strcpy(CR[temp_int].data.valid_time, "20240715");

    serial_number++;
    return 1;
}