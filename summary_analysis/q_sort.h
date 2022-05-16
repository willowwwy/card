#ifndef _Q_SORT_H_
#define _Q_SORT_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define MAX_L 60000
#define MAX_WIN 99
#define QALL "../../record_files/summary/q_sort_summary.txt"

extern char data_win[MAX_WIN][60000][60];
void QSort(char (*)[60], int, int);
int QuickSort(char (*)[60]);

#endif