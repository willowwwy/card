#ifndef _ACCOUNT_OPEN_H
#define _ACCOUNT_OPEN_H 

#include <stdio.h>
#include <assert.h>
#include <malloc.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#ifndef _STRUCT_H
#define _STRUCT_H
#include "../management/struct.h"
#endif

#define ERROR -1
#define MAX_STU 10000
#define KH "../../test_data/kh001.txt"

extern int serial_number;
extern FILE *logp;
extern struct account ST[MAX_STU];
extern struct valid_card CR[MAX_STU];
extern int NUM_NOW;

void open_account();
bool open_card(struct account *);

#endif