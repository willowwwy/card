struct _last
{
    float add_up;      /*Cumulative consumption amount for the current period*/
    char last_date[5]; /*last date*/
    char last_hour[3]; /*last hour*/
    char password[5];  /*password:8888*/
};
struct data_account
{
    char student_name[9];   /* name */
    char account_id[11];    /* ID */
    float left_money;       /* the money left */
    struct _last last_time; /*when last consume*/
    int card_num;           /* the host's cards number */
    int account_sign;       /* whether the account is normal  normal??1 abnormal:0 */
};
struct account
{
    struct data_account data;
    struct valid_card *card;
};

struct data_card
{
    char card_id[8]; /* ID */
    char valid_time[9];
};

struct invalid_card
{
    struct data_card data;
    struct invalid_card *next;
};

struct valid_card
{
    short int if_valid;
    short int if_taken;
    struct account *student_account;
    struct data_card data;
    struct invalid_card *next;
};