#ifndef STRUCTION_H
#define STRUCTION_H
#include "widget.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct account {
    char name[100];
    int month;
    int day;
    int price;
    char classes[100];
    char item[100];
    int sum; //個人總花費
    int payTo[100]; //欲付其他人
    struct account *next;         //橫的過去
    struct account *prior;        //橫的回來
    struct account *nextperson;   //直的過去
    struct account *priorperson;  //直的回來
} account;

void add_node(account *);
int import_(char *);
void export_(char *);
void insert(char*, char*, char*, int, int, int);
int delete_node(char *, int , int, int , char *, char *, char );
account **search(char*, int, int, char*, char*, int);
account **statistics(void);

#endif // STRUCTION_H
