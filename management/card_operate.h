#ifndef _ACCOUNT_OPERATION_H 
#define _ACCOUNT_OPERATION_H 

#include<stdio.h>
#include<assert.h>
#include<malloc.h>
#include<string.h>
#include <stdlib.h>
#include <stdbool.h>

#ifndef _STRUCT_H
#define _STRUCT_H
#include "struct.h"
#endif

#define ERROR -1
#define MAX_STU 10000
#define CZ "../../test_data/cz002.txt"
#define READ_END_EXIT   if(fgetc(cz_fp)!=';' || fgetc(cz_fp)!='\n')       \
                        {                                           \
                            goto exit;                              \
                        }            
extern int serial_number;
extern FILE *logp;
extern FILE *cz_fp;
extern struct account ST[MAX_STU];
extern struct valid_card CR[MAX_STU];
extern int NUM_NOW;


void operate_card( int,char *);
void report_loss_card (int );
void cancle_loss_card(int );
void reissue_card(int );
void cancle_account(int );
void recharge_card(int ,int);
int find_account(char *,int *);
bool if_end_time(char *,char *);
#endif