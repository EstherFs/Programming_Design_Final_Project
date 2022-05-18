#ifndef STRUCTION_H
#define STRUCTION_H
#include "widget.h"
typedef struct account {
    char name[100];
    int month;
    int day;
    int price;
    char classes[100];
    char item[100];
    struct account *next;         //橫的過去
    struct account *prior;        //橫的回來
    struct account *nextperson;   //直的過去
    struct account *priorperson;  //直的回來
} account;

void add_node(account *);
int text_input(char *);
void text_output(char *);
void test_PRINT(void);
void insert(void);
void delete_choice(void);
int delete_node(char *, int , int, int , char *, char *, char );
void insert_node(char*, char* , char*, int, int, int);
std::vector<account *> search(char*, int , int, char* );
#endif // STRUCTION_H
