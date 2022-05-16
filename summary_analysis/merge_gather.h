#ifndef _MERGE_GATHER_H
#define _MERGE_GATHER_H
#include <stdio.h>
#include <string.h>

#define M_ALL "../../record_files/summary/merge_summary.txt"
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

extern char data_win[MAX_WIN][60000][60];
extern struct L location[MAX_WIN];
External node;

void merge_gather_operation();
void Adjust_1(LoserTree ls, int n);
#endif