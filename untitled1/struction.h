#ifndef STRUCTION_H
#define STRUCTION_H
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "widget.h"

typedef struct account {
    char name[100];
    int month;
    int day;
    int price;
    char classes[100];  
    char item[100];
    int sum;                      // personal expense
    int payTo[100];               // pay others bills
    struct account *next;         // horizontal past--->
    struct account *prior;        // horizontal back<---
    struct account *nextperson;   // straight past\|/
    struct account *priorperson;  // straight back/|\ //
} account;

void add_node(account *);
int import_(char *);
void export_(char *);
void insert(char *, char *, char *, int, int, int);
int delete_node(char *, int, int, int, char *, char *, char);
account **search(char *, int, int, char *, char *, int);
account **statistics(void);        // split bills

#endif  // STRUCTION_H
