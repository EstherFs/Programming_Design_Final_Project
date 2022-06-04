#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<ctype.h>
typedef struct account
{
    char name[100];
    int month;
    int day;
    int price;
    char class[100];
    char item[100];
    struct account *next; //æ©«ç????????
    struct account *prior; //æ©«ç?????ä¾?
    struct account *nextperson; //??´ç????????
    struct account *priorperson; //??´ç?????ä¾?
}account;
//for test
//helloooo00000000
account *head = NULL;
void add_node(account *);
void text_input(void);
void test_PRINT(void);
void insert(void);

int main()
{
    text_input();
    // insert();
    test_PRINT();
    return 0;
}

void test_PRINT(void){
    account *root = head;
    while(root!=NULL){
        account *person = root;
        while(person!=NULL){
            printf("%s %d/%d -> ",person->item,person->month,person->day);
            person = person->next;
        }
        printf("\n\n\n");
        root = root->nextperson;
    }
}
//123456
void slice(char* strLine, char* delimiter){
    account *newnode = (account*)malloc(sizeof(account));
    char *token = strtok(strLine, delimiter);
    int i = 0;

    while(token != NULL){
        if(i==0) strcpy(newnode->name, token);
        else if(i==1) strcpy(newnode->class, token);
        else if(i==2) strcpy(newnode->item, token);
        else if(i==3) newnode->price = atoi(token);
        else if(i==4) {
            int m = 0;
            int itr = 0;
            for(;token[itr]!='/';itr++){
            	m *=10;
            	m +=(token[itr]-'0');
            }
            itr++;
            int d = 0;
            for(;isdigit(token[itr]);itr++){
            	d *=10;
            	d +=(token[itr]-'0');

            }
            newnode -> month = m;
            newnode -> day = d;
        }
        token = strtok(NULL, delimiter);
        i++;
    }
    newnode -> next = NULL;
    newnode -> prior = NULL;
    newnode -> nextperson = NULL;
    newnode -> priorperson = NULL;
    add_node(newnode);
}

void text_input(void){
    FILE *fp;
    fp = fopen("data.csv", "r");
    char strLine[1500];
    while (fgets(strLine, 1500, fp)){
        slice(strLine, ",");
    }
    fclose(fp);
}

void insert(void){
    char strLine[1500];
    fgets(strLine, 1500, stdin);
	slice(strLine, " ");
    return;
}

void add_node(account *newnode){
    if(head == NULL)
        head = newnode;
    else{
        account *current = head;
        while(current->nextperson != NULL){

            if(strcmp(current->name, newnode->name) == 0)
            {
                while(current -> next != NULL)
                    current = current->next;
                current->next = newnode;
                current->next->prior = current;
                return;
            }
            current = current->nextperson;

        }

        current->nextperson = newnode;
        current->nextperson->priorperson = current;
    }
}
