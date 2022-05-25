#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <struction.h>
#include <vector>
#include<iostream>

account *head = NULL;
int person_num = 0;

void add_node(account *);
void text_input(void);
void text_output(void);
void test_PRINT(void);
void insert(void);
void delete_choice(void);
int delete_node(char *DeleteName, int month, int day, int price, char *classes, char *item, char choice);
account** statistics();

std::vector<account *> search(char* name, int month, int day, char* classes);

void test_PRINT(void) {
    account *root = head;
    while (root != NULL) {
        account *person = root;
        while (person != NULL) {
            printf("%s %s %d/%d -> ", person->name, person->item, person->month, person->day);
            person = person->next;
        }
        printf("\n\n\n");
        root = root->nextperson;
    }
}

void slice(char *strLine, char delimiter) {
    account *newnode = (account *)malloc(sizeof(account));
    char d[2];
    d[0] = delimiter;
    d[1] = '\0';
    char *token = strtok(strLine, d);
    int i = 0;

    while (token != NULL) {
        if (i == 0){
            strcpy(newnode->name, token);
        }
        else if (i == 1){
            strcpy(newnode->classes, token);
        }

        else if (i == 2){
            strcpy(newnode->item, token);
        }
        else if (i == 3){
            newnode->price = atoi(token);
        }
        else if (i == 4) {
            int m = 0;
            int itr = 0;
            //分割出月份和日
            for (; token[itr] != '/'; itr++) {
                m *= 10;
                m += (token[itr] - '0');
            }
            itr++;
            int d = 0;
            for (; isdigit(token[itr]); itr++) {
                d *= 10;
                d += (token[itr] - '0');
            }
            newnode->month = m;
            newnode->day = d;
        }
        token = strtok(NULL, d);
        i++;
    }
    newnode->next = NULL;
    newnode->prior = NULL;
    newnode->nextperson = NULL;
    newnode->priorperson = NULL;
    newnode->sum = 0;
    newnode->pay = 0;
    newnode->last_pay=0;
    newnode->exceed = 0;
    add_node(newnode);
}

void text_output(char *dir) {
    FILE *fp;
    fp = fopen(strcat(dir,"/data.csv"), "w");
    account *root = head;
    char strLine[1500];
    while (root != NULL) {
        account *person = root;
        while (person != NULL) {
            char tmp[100];
            strcpy(strLine, person->name);
            strcat(strLine, ",");
            strcat(strLine, person->classes);
            strcat(strLine, ",");
            strcat(strLine, person->item);
            strcat(strLine, ",");
            strcat(strLine, itoa(person->price, tmp, 10));
            strcat(strLine, ",");
            strcat(strLine, itoa(person->month, tmp, 10));
            strcat(strLine, "/");
            strcat(strLine, itoa(person->day, tmp, 10));
            fprintf(fp, "%s\n", strLine);
            person = person->next;
        }
        root = root->nextperson;
    }
    fclose(fp);
}

int text_input(char *dir) {
    FILE *fp;
    fp = fopen(strcat(dir,"/data.csv"), "r");
    if(fp==NULL){
        return -1;
    }
    char strLine[1500];
    while (fgets(strLine, 1500, fp)) {
        slice(strLine, ',');
    }
    fclose(fp);
    return 1;
}

void insert(void) {
    char strLine[1500];
    fgets(strLine, 1500, stdin);
    slice(strLine, ' ');
    return;
}

void insert_node(char* name, char* classes, char* item, int price, int month, int day){
    account *newnode = (account *)malloc(sizeof(account));
    std::cout<<name;
    newnode->name[0] = '\0';
    newnode->classes[0] = '\0';
    newnode->item[0] = '\0';
    strcat(newnode->name, name);
    strcat(newnode->classes, classes);
    strcat(newnode->item, item);
    newnode->month = month;
    newnode->day = day;
    newnode->price = price;
    newnode->next = NULL;
    newnode->prior = NULL;
    newnode->nextperson = NULL;
    newnode->priorperson = NULL;
    newnode->sum = 0;
    newnode->pay = 0;
    newnode->last_pay=0;
    newnode->exceed = 0;
    add_node(newnode);
}
void add_node(account *newnode) {

        if (head == NULL)
            head = newnode;
        else {
            account *current = head, *previous = NULL;
            while (current != NULL) {
                previous = current;
                if (strcmp(current->name, newnode->name) == 0) {
                    while (current->next != NULL) current = current->next;
                    current->next = newnode;
                    current->next->prior = current;
                    return;
                }
                current = current->nextperson;
            }

            previous->nextperson = newnode;
            previous->nextperson->priorperson = previous;
        }


}

/*刪除選項*/
void delete_choice(void) {
    char choice;            //選項
    char DeleteName[100];   //要刪的名字
    int price = 0, month=1, day = 0;
    char classes[100];
    char item[100];
    /*輸入刪除選項(1刪整個帳戶/2刪某個帳戶)*/
    do {
        printf("Please enter your choice.\n");
        printf("1.remove someone's whole account\n");
        printf("2.delete the desinated accounts\n");
        scanf("%c", &choice);
    } while (choice != '1' && choice != '2');

    printf("Please enter the name you want to delete.\n");
    scanf("%s", DeleteName);
    if (choice == '1') {
        delete_node(DeleteName, month, day, price, classes, item, choice);
        return;
    } else if (choice == '2') {
        /*先確認有沒有找到人，沒有就繼續輸入到有*/
        while (1) {
            int find = 0;       //初始沒找找到人
            account *cur;
            for (cur = head; cur != NULL; cur = cur->nextperson) {
                if (strcmp(cur->name, DeleteName) == 0) {
                    find = 1;   //找到人
                    break;
                }
            }
            if (find == 0) {
                printf("\033[031mERROR!!Can't find the person.\033[m\n");  //錯誤訊息
                printf("\033[032mPlease enter the name you want to delete again.\033[m\n");
                scanf("%s", DeleteName);
            } else {
                break;  //找到人跳出while
            }
        }

        int flag = 0;
        account *cur, *ptr;
        /*如果此日期沒消費，繼續輸到有要刪類別品項的正確日期*/
        do {
            printf("\nPlease enter the date you want to delete.\n");
            printf("\033[032mFormat: month/day\033[m\n");    //先輸入日期
            scanf("%d/%d", &month, &day);
            printf("\nThe consumption you spent on %d/%d\n", month, day);
            printf("classes\titem\tdollar\n");  //印出此日期下有的類別/品項/金額____可以考慮要不要變顏色??
            for (cur = head; cur != NULL; cur = cur->nextperson) {
                if (strcmp(cur->name, DeleteName) == 0) {
                    for (ptr = cur; ptr != NULL; ptr = ptr->next) {
                        if ((ptr->month == month) && (ptr->day == day)) {  //有找到的類別/品項/金額
                            printf("%s\t%s\t%d\n", ptr->classes, ptr->item, ptr->price);
                            flag = 1;
                        }
                    }
                }
            }
            if (flag == 0) {
                printf("\033[031mERROR!!Can't find the consumption of %s on %d/%d.\033[m\n", DeleteName, month, day);  //錯誤訊息
                printf("\033[032mPlease enter again.\033[m\n");
            }
        } while (flag == 0);

        printf("\nPlease enter the classes, the item and the price you want to delete.\n");
        printf("\033[032mFormat: classes item dollar\033[m\n");

        scanf("%s %s %d", classes, item, &price);
        delete_node(DeleteName, month, day, price, classes, item, choice);
        return;
    }
}

/*進行刪除的動作*/
int delete_node(char *DeleteName, int month, int day, int price, char *classes, char *item, char choice) {
    struct account *cur, *prev, *p, *ptr;
    /*1.刪掉整個帳戶*/
    if (choice == '1') {
        for (cur = head; cur != NULL; cur = cur->nextperson) {
            if (strcmp(cur->name, DeleteName) == 0) {  //有找到人
                if (cur == head) {
                    head = cur->nextperson;
                } else {
                    cur->priorperson->nextperson = cur->nextperson;
                    if (cur->nextperson != NULL) {
                        cur->nextperson->priorperson = cur->priorperson;
                    }
                }
                 p = cur;
                 while (p != NULL) {
                     account *del = p;
                     p = p->next;
                     free(del);
                 }
                return 1;
            }
        }
        printf("\033[031mERROR!!Can't find the person.\033[m\n");  //錯誤訊息
        return -1;
    /*2.刪指定帳目*/
    } else if (choice == '2') {
        for (cur = head, prev = NULL;
        cur != NULL;
        prev = cur, cur = cur->nextperson) {
            if (strcmp(cur->name, DeleteName) == 0) {
                for (ptr = cur; ptr != NULL; ptr = ptr->next) {
                    if ((ptr->month == month) && (ptr->day == day)
                        && (strcmp(ptr->classes, classes) == 0)
                        && (strcmp(ptr->item, item) == 0) && (ptr->price == price)) {
                        if (ptr == cur && ptr == head) {
                            head = cur->next;
                            head->nextperson = cur->nextperson;
                        }else if (ptr == cur && ptr != head) {
                            account *buf = cur->nextperson;
                            prev->nextperson = cur->next;
                            cur = cur->next;
                            if(cur!=NULL) cur->nextperson = buf;
                        } else {
                            ptr->prior->next = ptr->next;
                            if (ptr->next != NULL) ptr->next->prior = ptr->prior;
                        }
                        free(ptr);
                        return 1;
                    }
                }
            }
        }
        printf("\033[031mERROR!!Can't find an account.\033[m\n");  //錯誤訊息
        return -1;
    }
    return -1;
}

/*Search accounts by name or by month or by class, and put them in a vector searcher.*/
std::vector<account *> search(char* name, int month, int day, char* classes){
    std::vector<account*> searcher;
    //Search by name(default name："")
    if(strcmp(name,"")!=0){
        account *ptr1, *ptr2;
        //Find the same name
        for (ptr1 = head; ptr1 != NULL; ptr1 = ptr1->nextperson){
            if(strcmp(ptr1->name, name) == 0){
                //Put all the accounts of the name into the vector searcher.
                for (ptr2 = ptr1; ptr2 != NULL; ptr2 = ptr2->next){
                    searcher.push_back(ptr2);
                }
                break;
            }
        }
    }
    //Search by date(default month：0/ day：0)
    if(month!=0 && day != 0){
        if(searcher.empty()){
            account *ptr1, *ptr2;
            for (ptr1 = head; ptr1 != NULL; ptr1 = ptr1->nextperson){
                for (ptr2 = ptr1; ptr2 != NULL; ptr2 = ptr2->next){
                    if(ptr2->month == month && ptr2->day == day){
                        searcher.push_back(ptr2);
                    }
                }
            }
        }
        else{
            for(int i=0;i<(int)searcher.size();i++){
                if(searcher[i]->month != month || searcher[i]->day != day){
                    searcher.erase(searcher.begin()+i);
                    i--;
                }
            }
        }

    }
    //Search by class(default class："")
    if(strcmp(classes,"") != 0){
        if(searcher.empty()){
            account *ptr1, *ptr2;
            for (ptr1 = head; ptr1 != NULL; ptr1 = ptr1->nextperson){
                for (ptr2 = ptr1; ptr2 != NULL; ptr2 = ptr2->next){
                    if(strcmp(ptr2->classes, classes) == 0){
                        searcher.push_back(ptr2);
                    }
                }
            }
        }
        else{
            for(int i=0;i<(int)searcher.size();i++){
                if(strcmp(searcher[i]->classes, classes) != 0){
                    searcher.erase(searcher.begin()+i);
                    i--;
                }
            }
        }
    }

    return searcher;
}

int cmp(const void *a ,const void *b){
    account *A = (*(account**)a);
    account *B = (*(account**)b);
    return A->sum - B->sum;
}

account** statistics(){
    account *tmp = head;
    while(tmp!=NULL){
        account *tmp2 = tmp;
        while(tmp2!=NULL){
            tmp->sum += tmp2->price;
            tmp2 = tmp2->next;
        }
        person_num++;
        tmp = tmp->nextperson;
    }
    if(person_num==0)
        return NULL;

    account **all_person = (account**)malloc(sizeof(account*)*person_num);
    tmp = head ;
    int i = 0, avg = 0;
    while(tmp!=NULL){
        all_person[i++] = tmp;
        avg += tmp->sum;
        tmp = tmp->nextperson;
    }
    avg /= person_num;

    qsort(all_person, person_num, sizeof(account*), cmp);

    for(int i=0;i<person_num-1;i++){
        int diff = avg - all_person[i]->sum;
        all_person[i]->exceed = diff%(person_num-i-1);
        all_person[i]->pay = diff/(person_num-i-1);

        for(int j=i+1;j<person_num;j++){
            all_person[j]->sum -= all_person[i]->pay;
            all_person[i]->sum += all_person[i]->pay;
        }
    }
    for(int i=person_num-2;i>=0;i--){
        all_person[person_num-1]->sum -= all_person[i]->exceed;
        all_person[i]->last_pay = all_person[i]->exceed + all_person[i]->pay;
        all_person[i]->sum  = avg;
    }
    return all_person;

}
