#ifndef _SINGLE_H_
#define _SINGLE_H_
#include <stdio.h>
#include <assert.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#ifndef _STRUCT_H
#define _STRUCT_H
#include "../management/struct.h"
#define ERROR -1
#endif

#define MAX_STU 10000


extern int NUM_NOW;
extern int serial_number;
extern FILE *logp;
extern struct account ST[MAX_STU];
extern struct valid_card CR[MAX_STU];
extern char date_time_set[17];
extern char data_win[MAX_WIN][60000][60];
extern int single_begin;
// extern struct L location[MAX_WIN];




void _kh();
void _gs();
void _jg();
void _bk();
void _xh();
void _cz();
void _xf();
void _search();
int find_account_s(char *);
int find_card_s(char *);
bool check_info(struct account *);
double cal_similarity(char *a, char *b);
void set_location_s(int );

#endif