#include "../management/account_open.c"
#include "../management/card_operate.c"
#include "../application/consume.c"
#include "../summary_analysis/merge_gather.c"
#include "../summary_analysis/q_sort.c"
// #include "../summary_analysis/data_mining.c"
#include "../single/single.c"

// #pragma comment(lib, "winmm.lib")
#include <time.h>
#ifdef _WIN32
#include <Windows.h>
#else
#include <unistd.h>
#endif

#define LOG "../../record_files/log/log.txt"


int serial_number;
FILE *logp;
FILE *win[MAX_WIN]; // windows file
FILE *xf_fp;        // XF data file
FILE *cz_fp;        // CZ data file
struct account ST[MAX_STU];
struct valid_card CR[MAX_STU];
int NUM_NOW;
int single_begin;
char date_time_set[17];
char data_win[MAX_WIN][60000][60];

void open_file();
void close_file();
void group_control();
void gather_control();
void input_win();
char *which_todo(char *s);
void date_gather(char (*)[60]);
