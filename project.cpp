#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
typedef struct account {
    char name[100];
    int month;
    int day;
    int price;
    char classes[100];
    char item[100];
    struct account *next;         //橫向
    struct account *prior;        //橫向
    struct account *NextPerson;   //直向
    struct account *PriorPerson;  //直向
} account;

account *head = NULL;
account *sortHead = NULL;
account *sortPrice = NULL;
account *sortName = NULL;
account *sortDate = NULL;

void add_node(account *);
int check(account *);
void text_input(void);
void text_output(void);
void test_PRINT(void);
void traverse(void);
void sortLink(void);
void sortedPrice(account* newnode);
void insertionPrice(void);
void sort_traverse(void);
void insert(void);
void delete_choice(void);
void delete_node(char *, int, int, int, char *, char *, char);
std::vector<account *> search(char *, int, int, char *);

int main(void) {
    printf("Please inter the acount data:\n");
    text_input();
    // insert();
    // insert();
    // insert();
    int cmd;
    printf("Please inter the command (1:Delete 2:Traverse 3:Sort 4:End): ");
    while(scanf("%d",&cmd)!=4){
        if(cmd==1) delete_choice();
        if(cmd==2) traverse();
        if(cmd==3) 
        {
            sortLink();
            insertionPrice();
            sort_traverse();
        }
        if (cmd < 1 ||cmd>4){
            printf("\033[031mERROR!!Can't execute the command.\033[m\n");
            printf("\033[032mPlease enter the cmd again.\033[m\n");
        }
        if(cmd==4) break;
        else printf("Please inter the command (1:Delete 2:Traverse 4:End) :");
    }
    printf("SYSTEM OFF\n");
    text_output();

    /*測試search*/
    // char name[15];
    // char classes[15];
    // std::vector<account *> v;
    // strcpy(name, "Troye");
    // strcpy(classes, "");
    // v = search(name, 0, 0, classes);
    // for (int i = 0; i < v.size(); i++){
    //     printf("%s %d/%d %s ->", v[i]->name, v[i]->month, v[i]->day, v[i]->item);
    // }
    // printf("\n");
    // strcpy(name, "");
    // v = search(name, 4, 24, classes);
    // for (int i = 0; i < v.size(); i++){
    //     printf("%s %d/%d %s ->", v[i]->name, v[i]->month, v[i]->day, v[i]->item);
    // }
    // printf("\n");
    // strcpy(classes, "drink");
    // v = search(name, 0, 0, classes);
    // for (int i = 0; i < v.size(); i++){
    //     printf("%s %d/%d %s ->", v[i]->name, v[i]->month, v[i]->day, v[i]->item);
    // }

    return 0;
}

void test_PRINT(void) {
    account *root = head;
    while (root != NULL) {
        account *person = root;
        while (person != NULL) {
            printf("%s %s %d/%d -> ", person->name, person->item, person->month, person->day);
            person = person->next;
        }
        printf("\n\n\n");
        root = root->NextPerson;
    }
}

/*將StrLine切割出所需的變數放進NewNode*/
void slice(char *StrLine, char *delimiter, int n) {
    account *NewNode = (account *)malloc(sizeof(account));
    char *token = strtok(StrLine, delimiter);
    int i = 0;

    while (token != NULL) {
        if (i == 0)
            strcpy(NewNode->name, token);
        else if (i == 1)
            strcpy(NewNode->classes, token);
        else if (i == 2)
            strcpy(NewNode->item, token);
        else if (i == 3)
            NewNode->price = atoi(token);
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
            NewNode->month = m;
            NewNode->day = d;
        }
        token = strtok(NULL, delimiter);
        i++;
    }
    NewNode->next = NULL;
    NewNode->prior = NULL;
    NewNode->NextPerson = NULL;
    NewNode->PriorPerson = NULL;
  
    if(n == 1)
        add_node(NewNode);
    else if(n == 2)
    {
        if(check(NewNode))
            add_node(NewNode);
    else
    {
        printf("\033[032mThe input format is incorrect, please insert again.\033[m\n");
        insert();
    }
    }
}

void text_output(void) {
    FILE *fp;
    fp = fopen("data_output.csv", "w");
    account *root = head; //root記錄人
    char StrLine[1500];
    while (root != NULL) {
        account *person = root; //person記錄某一人的不同筆帳目
        while (person != NULL) {
            char tmp[100];
            strcpy(StrLine, person->name);
            strcat(StrLine, ",");
            strcat(StrLine, person->classes);
            strcat(StrLine, ",");
            strcat(StrLine, person->item);
            strcat(StrLine, ",");
            strcat(StrLine, itoa(person->price, tmp, 10));
            strcat(StrLine, ",");
            strcat(StrLine, itoa(person->month, tmp, 10));
            strcat(StrLine, "/");
            strcat(StrLine, itoa(person->day, tmp, 10));
            fprintf(fp, "%s\n", StrLine);
            person = person->next;
        }
        root = root->NextPerson;
    }
    fclose(fp);
}

/*File_Input*/
void text_input(void) {
    FILE *fp;
    fp = fopen("data.csv", "r"); //將data.csv讀進程式
    char StrLine[1500];
    while (fgets(StrLine, 1500, fp)) {
        slice(StrLine, ",", 1);
    }
    fclose(fp);
}

/*使用者手動輸入帳目*/
void insert(void) {
    char StrLine[1500];
    fgets(StrLine, 1500, stdin);
    slice(StrLine, " ", 2);
    return;
}

/*將新增帳目加入linked list*/
void add_node(account *NewNode) {
    if (head == NULL)
    {
        head = NewNode;
        sortHead = head;
    }
    else {
        account *current = head, *previous = NULL;
        while (current != NULL) {
            previous = current;
            if (strcmp(current->name, NewNode->name) == 0) {
                while (current->next != NULL) current = current->next;
                current->next = NewNode;
                current->next->prior = current;
                return;
            }
            current = current->NextPerson;
        }

        previous->NextPerson = NewNode;
        previous->NextPerson->PriorPerson = previous;
    }
}

/*刪除選項*/
void delete_choice(void) {
    char choice;            //選項
    char DeleteName[100];   //要刪的名字
    int price = 0, month, day;
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
            int find = 0;       //初始是沒找到人
            account *cur;
            for (cur = head; cur != NULL; cur = cur->NextPerson) {
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
            printf("class\titem\tdollar\n");  //印出此日期下有的類別/品項/金額____可以考慮要不要變顏色??
            for (cur = head; cur != NULL; cur = cur->NextPerson) {
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

        printf("\nPlease enter the class, the item and the price you want to delete.\n");
        printf("\033[032mFormat: class item dollar\033[m\n");
        
        scanf("%s %s %d", classes, item, &price);
        delete_node(DeleteName, month, day, price, classes, item, choice);
        return;
    }
}

/*進行刪除的動作*/
void delete_node(char *DeleteName, int month, int day, int price, char *classes, char *item, char choice) {
    struct account *cur, *prev, *p, *ptr;
    /*1.刪掉整個帳戶*/
    if (choice == '1') { 
        for (cur = head; cur != NULL; cur = cur->NextPerson) {
            if (strcmp(cur->name, DeleteName) == 0) {  //有找到人
                if (cur == head) {
                    head = cur->NextPerson;
                } else {
                    cur->PriorPerson->NextPerson = cur->NextPerson;
                    if (cur->NextPerson != NULL) {
                        cur->NextPerson->PriorPerson = cur->PriorPerson;
                    }
                }
                //free這個人cur指到的帳目及接在之後的所有帳目
                p = cur; 
                while (p != NULL) {
                    account *del = p;
                    p = p->next;
                    free(del);
                }
                return;
            }
        }
        printf("\033[031mERROR!!Can't find the person.\033[m\n");  //錯誤訊息
        return;
    /*2.刪指定帳目*/
    } else if (choice == '2') {
        for (cur = head, prev = NULL;
        cur != NULL;
        prev = cur, cur = cur->NextPerson) {
            if (strcmp(cur->name, DeleteName) == 0) {
                for (ptr = cur; ptr != NULL; ptr = ptr->next) {
                    if ((ptr->month == month) && (ptr->day == day) 
                        && (strcmp(ptr->classes, classes) == 0) 
                        && (strcmp(ptr->item, item) == 0) && (ptr->price == price)) {
                        if (ptr == cur && ptr == head) {
                            head = cur->next;
                            head->NextPerson = cur->NextPerson;                          
                        }else if (ptr == cur && ptr != head) {
                            account *buf = cur->NextPerson;
                            prev->NextPerson = cur->next;
                            cur = cur->next;
                            cur->NextPerson = buf;                           
                        } else {
                            ptr->prior->next = ptr->next;
                            if (ptr->next != NULL) ptr->next->prior = ptr->prior;
                        }
                        free(ptr);
                        return;
                    }
                }
            }
        } 
        printf("\033[031mERROR!!Can't find an account.\033[m\n");  //錯誤訊息
        return;
    }
    return;
}

void traverse(void) {
    account *cur, *curPerson;
    printf("\n");
    for (curPerson = head; curPerson != NULL; curPerson = curPerson->NextPerson) {
        printf("%s: ", curPerson->name);
        for (cur = curPerson; cur != NULL; cur = cur->next) {
            printf("%s %s %d/%d %d", cur->classes, cur->item, cur->month, cur->day, cur->price);
            if(cur->next !=NULL)
                printf("  ->  ");
        }
        printf("\n\n");
    }
}

/*Search accounts by name or by month or by class, and put them in a vector searcher.*/
std::vector<account *> search(char* name, int month, int day, char* classes){
    std::vector<account*> searcher; 
    //Search by name(default name："")
    if(strcmp(name,"")!=0){ 
        account *ptr1, *ptr2;
        //Find the same name 
        for (ptr1 = head; ptr1 != NULL; ptr1 = ptr1->NextPerson){
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
    else if(month!=0 && day != 0){
        account *ptr1, *ptr2;
        for (ptr1 = head; ptr1 != NULL; ptr1 = ptr1->NextPerson){
            for (ptr2 = ptr1; ptr2 != NULL; ptr2 = ptr2->next){
                if(ptr2->month == month && ptr2->day == day){
                    searcher.push_back(ptr2);
                }
            }
        }
    }
    //Search by class(default class："")
    else if(strcmp(classes,"") != 0){
        account *ptr1, *ptr2;
        for (ptr1 = head; ptr1 != NULL; ptr1 = ptr1->NextPerson){
            for (ptr2 = ptr1; ptr2 != NULL; ptr2 = ptr2->next){
                if(strcmp(ptr2->classes, classes) == 0){
                    searcher.push_back(ptr2);
                }
            }
        }
    }

    return searcher;
}

int check(account *NewNode)
{
  int flag = 1;
  account *cur, *curperson;
  for (curperson = head; curperson != NULL; curperson = curperson->NextPerson) {
        for (cur = curperson; cur != NULL; cur = cur->next) {
            if(NewNode -> name != cur -> name)
              continue;
            if(strcmp(NewNode -> classes, cur -> classes) == 0 && strcmp(NewNode -> item, cur -> item) == 0 && NewNode -> month == cur -> month && NewNode -> classes == cur -> classes)
              flag = 0;
        }
    }
  return flag;
}

void sortLink(void)
{
    account* current = head;
    account* currentlink = head;
    while(currentlink -> NextPerson != NULL)
    {
        while(current -> next != NULL)
            current = current -> next;
        current -> next = currentlink -> NextPerson;
        current = current -> next;
        currentlink = currentlink -> NextPerson;
    }
}

void sortedPrice(account* newnode)
{
    //printf("近來\n");
	if (sortPrice == NULL || sortPrice->price >= newnode->price) {
		newnode->next = sortPrice;
		sortPrice = newnode;
        printf("進來1\n");
	}
	else {
        printf("進來2\n");
		account* current = sortPrice;
		
		while (current->next != NULL
			&& current->next->price < newnode->price) {
			current = current->next;
		}
		newnode->next = current->next;
		current->next = newnode;
        printf("進來3\n");
	}
}

void insertionPrice(void)
{
	account* current = sortHead;

	while (current != NULL) {
        printf("1\n");
		account* next = current->next;
        printf("2\n");
		sortedPrice(current);
        printf("3\n");
		current = next;
        printf("4\n");
	}
	sortHead = sortPrice;
}

void sort_traverse(void)
{
    account* current = sortHead;
    while(current != NULL)
    {
        printf("%s %s %s %d/%d %d\n", current -> name, current -> classes, current -> item, current -> month, current -> day, current -> price);
        current = current -> next;
    }
}