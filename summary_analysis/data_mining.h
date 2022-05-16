#ifndef _DATA_MINING_H
#define _DATA_MINING_H
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <Windows.h>



#ifndef _STRUCT_H
#define _STRUCT_H
#include "../management/struct.h"
#endif
#define MAX_STU 10000

extern int NUM_NOW;
// int single_begin;
extern struct account ST[MAX_STU];

void data_mining(char (*)[60], int );
void find_center_stu(int *score, char *, char (*)[60], int );
void accumulate_stu(int *, char (*)[60], int , char *, char *, char *, int );
int find_student_a(char *, int );
void q_sort(int *, int low, int high, int *);
void output(int *);

#endif