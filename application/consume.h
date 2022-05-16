#ifndef _CONSUME_H_
#define _CONSUME_H_

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#ifndef _STRUCT_H
#define _STRUCT_H
#include "../management/struct.h"
#endif

#define ERROR -1
#define MAX_STU 10000
#define XF "../../test_data/xf014.txt"
#define WZ "../../test_data/wz003.txt"

#ifndef _STRUCT_LS_
#define _STRUCT_LS_

#define NUM_WIN 99
#define MAX_WIN 99
#define MINKEY "0000000000000000"
#define MAXKEY "9999999999999999"
typedef int LoserTree[NUM_WIN]; //表示非终端结点，由于是完全二叉树，所以可以使用一维数组来表示
typedef struct
{
    char key[17];
} External[NUM_WIN + 1]; // max=58
struct L
{
    int tail;
    int head;
    int if_back;
} location[MAX_WIN] = {0, 0, 0};

#endif

static int offset[NUM_WIN];
static External time_leaf; // loser trees' leaves
char s[NUM_WIN][40];

extern FILE *logp;
extern FILE *win[MAX_WIN]; // windows file
extern FILE *xf_fp;        // data file
extern struct account ST[MAX_STU];
extern struct valid_card CR[MAX_STU];
extern char data_win[MAX_WIN][60000][60];

#ifndef _CONSUME_FUNCTION_
#define _CONSUME_FUNCTION_
void consume_operate( char *);
void Adjust(LoserTree, int);
void sort_in(char *);
bool card_consume(int num, char *);
void set_location(int);
int find_card(char *);
void position_init();
#endif

#endif

// #define MAXKEY 10000
