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
    struct account *next;         //��V
    struct account *prior;        //��V
    struct account *NextPerson;   //���V
    struct account *PriorPerson;  //���V
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
void sortedPriceup(account* newnode);
void insertionPriceup(void);
void sortedPricedown(account* newnode);
void insertionPricedown(void);
void sortedDateup(account* newnode);
void insertionDateup(void);
void sortedDatedown(account* newnode);
void insertionDatedown(void);
void sortedNameup(account* newnode);
void insertionNameup(void);
void sortedNamedown(account* newnode);
void insertionNamedown(void);
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
    sortLink();
    while(scanf("%d",&cmd)){
        if(cmd==1) delete_choice();
        if(cmd==2) traverse();
        if(cmd == 3) 
        {
            insertionPriceup();
            sort_traverse();
            sortPrice = NULL;
        }
        if(cmd == 4) 
        {
            insertionPricedown();
            sort_traverse();
            sortPrice = NULL;
        }
        if(cmd == 5) 
        {
            insertionDateup();
            sort_traverse();
            sortDate = NULL;
        }
        if(cmd == 6) 
        {
            insertionDatedown();
            sort_traverse();
            sortDate = NULL;
        }
        if(cmd == 7)
        {
            insertionNameup();
            sort_traverse();
            sortName = NULL;
        }
        if(cmd == 8)
        {
            insertionNamedown();
            sort_traverse();
            sortName = NULL;
        }
        if (cmd < 1 ||cmd>9){
            printf("\033[031mERROR!!Can't execute the command.\033[m\n");
            printf("\033[032mPlease enter the cmd again.\033[m\n");
        }
        if(cmd==9) break;
        else printf("Please inter the command (1:Delete 2:Traverse 4:End) :");
    }
    printf("SYSTEM OFF\n");
    text_output();

    /*����search*/
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

/*�NStrLine���ΥX�һݪ��ܼƩ�iNewNode*/
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
            //���ΥX����M��
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
    account *root = head; //root�O���H
    char StrLine[1500];
    while (root != NULL) {
        account *person = root; //person�O���Y�@�H�����P���b��
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
    fp = fopen("data.csv", "r"); //�Ndata.csvŪ�i�{��
    char StrLine[1500];
    while (fgets(StrLine, 1500, fp)) {
        slice(StrLine, ",", 1);
    }
    fclose(fp);
}

/*�ϥΪ̤�ʿ�J�b��*/
void insert(void) {
    char StrLine[1500];
    fgets(StrLine, 1500, stdin);
    slice(StrLine, " ", 2);
    return;
}

/*�N�s�W�b�إ[�Jlinked list*/
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

/*�R���ﶵ*/
void delete_choice(void) {
    char choice;            //�ﶵ
    char DeleteName[100];   //�n�R���W�r
    int price = 0, month, day;
    char classes[100];    
    char item[100];
    /*��J�R���ﶵ(1�R��ӱb��/2�R�Y�ӱb��)*/
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
        /*���T�{���S�����H�A�S���N�~���J�즳*/
        while (1) {
            int find = 0;       //��l�O�S���H
            account *cur;
            for (cur = head; cur != NULL; cur = cur->NextPerson) {
                if (strcmp(cur->name, DeleteName) == 0) {                  
                    find = 1;   //���H
                    break;
                }
            }
            if (find == 0) {
                printf("\033[031mERROR!!Can't find the person.\033[m\n");  //���~�T��
                printf("\033[032mPlease enter the name you want to delete again.\033[m\n");
                scanf("%s", DeleteName);
            } else {
                break;  //���H���Xwhile
            }
        }

        int flag = 0;
        account *cur, *ptr;
        /*�p�G������S���O�A�~���즳�n�R���O�~�������T���*/
        do {
            printf("\nPlease enter the date you want to delete.\n");
            printf("\033[032mFormat: month/day\033[m\n");    //����J���
            scanf("%d/%d", &month, &day);
            printf("\nThe consumption you spent on %d/%d\n", month, day);
            printf("class\titem\tdollar\n");  //�L�X������U�������O/�~��/���B____�i�H�Ҽ{�n���n���C��??
            for (cur = head; cur != NULL; cur = cur->NextPerson) {
                if (strcmp(cur->name, DeleteName) == 0) {
                    for (ptr = cur; ptr != NULL; ptr = ptr->next) {
                        if ((ptr->month == month) && (ptr->day == day)) {  //����쪺���O/�~��/���B
                            printf("%s\t%s\t%d\n", ptr->classes, ptr->item, ptr->price);
                            flag = 1;
                        }
                    }
                }
            }
            if (flag == 0) {
                printf("\033[031mERROR!!Can't find the consumption of %s on %d/%d.\033[m\n", DeleteName, month, day);  //���~�T��
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

/*�i��R�����ʧ@*/
void delete_node(char *DeleteName, int month, int day, int price, char *classes, char *item, char choice) {
    struct account *cur, *prev, *p, *ptr;
    /*1.�R����ӱb��*/
    if (choice == '1') { 
        for (cur = head; cur != NULL; cur = cur->NextPerson) {
            if (strcmp(cur->name, DeleteName) == 0) {  //�����H
                if (cur == head) {
                    head = cur->NextPerson;
                } else {
                    cur->PriorPerson->NextPerson = cur->NextPerson;
                    if (cur->NextPerson != NULL) {
                        cur->NextPerson->PriorPerson = cur->PriorPerson;
                    }
                }
                //free�o�ӤHcur���쪺�b�ؤα��b���᪺�Ҧ��b��
                p = cur; 
                while (p != NULL) {
                    account *del = p;
                    p = p->next;
                    free(del);
                }
                return;
            }
        }
        printf("\033[031mERROR!!Can't find the person.\033[m\n");  //���~�T��
        return;
    /*2.�R���w�b��*/
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
        printf("\033[031mERROR!!Can't find an account.\033[m\n");  //���~�T��
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
    //Search by name(default name�G"")
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
    //Search by date(default month�G0/ day�G0)
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
    //Search by class(default class�G"")
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

void sortedPriceup(account* newnode)
{
	if (sortPrice == NULL || sortPrice->price >= newnode->price) {
		newnode->next = sortPrice;
		sortPrice = newnode;
	}
	else {
		account* current = sortPrice;
		
		while (current->next != NULL && current->next->price < newnode->price) {
			current = current->next;
		}
		newnode->next = current->next;
		current->next = newnode;
	}
}

void insertionPriceup(void)
{
	account* current = sortHead;

	while (current != NULL) {
		account* next = current->next;
		sortedPriceup(current);
		current = next;
	}
	sortHead = sortPrice;
}

void sortedPricedown(account* newnode)
{
	if (sortPrice == NULL || sortPrice->price <= newnode->price) {
		newnode->next = sortPrice;
		sortPrice = newnode;
	}
	else {
		account* current = sortPrice;
		
		while (current->next != NULL && current->next->price > newnode->price) {
			current = current->next;
		}
		newnode->next = current->next;
		current->next = newnode;
	}
}

void insertionPricedown(void)
{
	account* current = sortHead;

	while (current != NULL) {
		account* next = current->next;
		sortedPricedown(current);
		current = next;
	}
	sortHead = sortPrice;
}

void sortedDateup(account* newnode)
{
    if (sortDate == NULL || ((sortDate->month)*100+(sortDate->day)) >= ((newnode->month)*100+(newnode->day))) {
		newnode->next = sortDate;
		sortDate = newnode;
	}
	else {
		account* current = sortDate;
		
		while (current->next != NULL && ((current->next->month)*100+(current->next->day)) < ((newnode->month)*100+(newnode->day))) {
			current = current->next;
		}
		newnode->next = current->next;
		current->next = newnode;
	}
}
void insertionDateup(void)
{
    account* current = sortHead;

	while (current != NULL) {
		account* next = current->next;
		sortedDateup(current);
		current = next;
	}
	sortHead = sortDate;
}



void sortedDatedown(account* newnode)
{
    if (sortDate == NULL || ((sortDate->month)*100+(sortDate->day)) <= ((newnode->month)*100+(newnode->day))) {
		newnode->next = sortDate;
		sortDate = newnode;
	}
	else {
		account* current = sortDate;
		
		while (current->next != NULL && ((current->next->month)*100+(current->next->day)) > ((newnode->month)*100+(newnode->day))) {
			current = current->next;
		}
		newnode->next = current->next;
		current->next = newnode;
	}
}
void insertionDatedown(void)
{
    account* current = sortHead;

	while (current != NULL) {
		account* next = current->next;
		sortedDatedown(current);
		current = next;
	}
	sortHead = sortDate;
}


void sortedNameup(account* newnode)
{
    if (sortName == NULL || (strcmp(sortName -> name, newnode -> name) >= 0)) {
		newnode->next = sortName;
		sortName = newnode;
	}
	else {
		account* current = sortName;
		
		while (current->next != NULL && (strcmp(current->next -> name, newnode -> name) < 0)) {
			current = current->next;
		}
		newnode->next = current->next;
		current->next = newnode;
	}
}
void insertionNameup(void)
{
    account* current = sortHead;

	while (current != NULL) {
		account* next = current->next;
		sortedNameup(current);
		current = next;
	}
	sortHead = sortName;
}



void sortedNamedown(account* newnode)
{
    if (sortName == NULL || strcmp(sortName -> name, newnode -> name) <= 0) {
		newnode->next = sortName;
		sortName = newnode;
	}
	else {
		account* current = sortName;
		
		while (current->next != NULL && strcmp(current->next -> name, newnode -> name) > 0) {
			current = current->next;
		}
		newnode->next = current->next;
		current->next = newnode;
	}
}
void insertionNamedown(void)
{
    account* current = sortHead;

	while (current != NULL) {
		account* next = current->next;
		sortedNamedown(current);
		current = next;
	}
	sortHead = sortName;
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