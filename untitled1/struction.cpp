#include <struction.h>
#include <qDebug>

account *head = NULL;
int person_num = 0;
int total_num = 0;
int exceed = 0 ;
char exceed_name[100];
int searcher_size =0;

void add_node(account *);
void import_(void);
void export_(void);
void insert(char*, char*, char*, int, int, int);
int delete_node(char *, int , int , int , char *, char *, char);
account **statistics(void);
account **search(char*, int, int, char*, char*, int);

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
    for(int i=0;i<100;i++)
        newnode->payTo[i] = 0;
    add_node(newnode);
    total_num ++;
}

void export_(char *file) {
    FILE *fp;
    fp = fopen(file, "w");
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

int import_(char *file) {
    FILE *fp;
    fp = fopen(file, "r");
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

void insert(char* name, char* classes, char* item, int price, int month, int day){
    account *newnode = (account *)malloc(sizeof(account));
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
    for(int i=0;i<100;i++)
        newnode->payTo[i] = 0;
    add_node(newnode);
    total_num ++;
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
                     total_num --;
                 }
                return 1;
            }
        }
    }
    /*2.刪指定帳目*/
    else if (choice == '2') {
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
                        total_num --;
                        return 1;
                    }
                }
            }
        }
    }

    return -1; //刪除錯誤
}

/*Search accounts by name or by month or by class, and put them in a vector searcher.*/
account ** search(char* name, int month, int day, char* classes, char* item, int price){
    if(total_num==0)
        return NULL;

    account** searcher = (account**)malloc(sizeof(account*)*total_num);
    searcher_size = 0;
    qDebug() << "name";
    //Search by name(default name："")
    if(strcmp(name,"")!=0){
        account *ptr1, *ptr2;
        //Find the same name
        for (ptr1 = head; ptr1 != NULL; ptr1 = ptr1->nextperson){
            if(strcmp(ptr1->name, name) == 0){
                //Put all the accounts of the name into the vector searcher.
                for (ptr2 = ptr1; ptr2 != NULL; ptr2 = ptr2->next){
                    qDebug() << ptr2;
                    searcher[searcher_size++] =  ptr2;
                }
                qDebug() << "OK";
                break;
            }
        }
    }

    //Search by date(default month：0/ day：0)
    if(month!=0 && day != 0){
        if(searcher_size==0){ //if searcher.empty()
            account *ptr1, *ptr2;
            for (ptr1 = head; ptr1 != NULL; ptr1 = ptr1->nextperson){
                for (ptr2 = ptr1; ptr2 != NULL; ptr2 = ptr2->next){
                    if(ptr2->month == month && ptr2->day == day){
                        searcher[searcher_size++] = ptr2;
                    }
                }
            }
        }
        else{
            for(int i=0;i<searcher_size;i++){
                if(searcher[i]->month != month || searcher[i]->day != day){
                    for(int j=i+1;j<searcher_size;j++){        //searcher.erase(searcher.begin()+i);
                        searcher[j-1] = searcher[j];
                    }
                    searcher_size--;
                    i--;
                }
            }
        }

    }

    //Search by class(default class："")
    if(strcmp(classes,"") != 0){
        if(searcher_size==0){
            account *ptr1, *ptr2;
            for (ptr1 = head; ptr1 != NULL; ptr1 = ptr1->nextperson){
                for (ptr2 = ptr1; ptr2 != NULL; ptr2 = ptr2->next){
                    if(strcmp(ptr2->classes, classes) == 0){
                        searcher[searcher_size++] = ptr2;
                    }
                }
            }
        }
        else{
            for(int i=0;i<searcher_size;i++){
                if(strcmp(searcher[i]->classes, classes) != 0){
                    for(int j=i+1;j<searcher_size;j++){        //searcher.erase(searcher.begin()+i);
                        searcher[j-1] = searcher[j];
                    }
                    searcher_size--;
                    i--;
                }
            }
        }
    }

    //Search by item(default item："")
    if(strcmp(item,"") != 0){
        if(searcher_size==0){
            account *ptr1, *ptr2;
            for (ptr1 = head; ptr1 != NULL; ptr1 = ptr1->nextperson){
                for (ptr2 = ptr1; ptr2 != NULL; ptr2 = ptr2->next){
                    if(strcmp(ptr2->item, item) == 0){
                        searcher[searcher_size++] = ptr2;
                    }
                }
            }
        }
        else{
            for(int i=0;i<searcher_size;i++){
                if(strcmp(searcher[i]->item, item) != 0){
                    for(int j=i+1;j<searcher_size;j++){        //searcher.erase(searcher.begin()+i);
                        searcher[j-1] = searcher[j];
                    }
                    searcher_size--;
                    i--;
                }
            }
        }
    }

    //Search by price(default price：0)
    if(price != 0){
        if(searcher_size==0){
            account *ptr1, *ptr2;
            for (ptr1 = head; ptr1 != NULL; ptr1 = ptr1->nextperson){
                for (ptr2 = ptr1; ptr2 != NULL; ptr2 = ptr2->next){
                    if(ptr2->price == price){
                        searcher[searcher_size++] = ptr2;
                    }
                }
            }
        }
        else{
            for(int i=0;i<searcher_size;i++){
                if(searcher[i]->price != price){
                    for(int j=i+1;j<searcher_size;j++){        //searcher.erase(searcher.begin()+i);
                        searcher[j-1] = searcher[j];
                    }
                    searcher_size--;
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
    account *head_copy = head;
    person_num = 0;
    exceed = 0;
    for(int i =0;i<20;i++)
        exceed_name[i] = ' ';

    while(head_copy!=NULL){
        head_copy->sum = 0;
        for(int i=0;i<100;i++)
            head_copy->payTo[i] = 0;

        account *head_copy2 = head_copy;
        while(head_copy2!=NULL){
            head_copy->sum += head_copy2->price;
            head_copy2 = head_copy2->next;
        }
        person_num++;
        head_copy = head_copy->nextperson;
    }
    if(person_num==0)
        return NULL;

    account **all_person = (account**)malloc(sizeof(account*)*person_num);
    head_copy = head ;
    int i = 0, avg = 0;
    while(head_copy!=NULL){
        all_person[i++] = head_copy;
        avg += head_copy->sum;
        head_copy = head_copy->nextperson;
    }
    avg /= person_num;

    qsort(all_person, person_num, sizeof(account*), cmp);

    for(int i=0,j=person_num-1;i<j;){
        int diff = avg - all_person[i]->sum;
        int need = all_person[j]->sum - avg;

        if(need<=diff){
             all_person[j]->sum = avg;
             all_person[i]->sum += need;
             all_person[i]->payTo[j] = need;
             j--;
        }
        else{
            all_person[j]->sum -= diff;
            all_person[i]->sum = avg;
            all_person[i]->payTo[j] = diff;
            i++;
        }
    }

    for(int i=0;i<person_num;i++){
        if(all_person[i]->sum != avg){
            exceed = all_person[i]->sum - avg;
            strcpy(exceed_name, all_person[i]->name);
            break;
        }
    }

    return all_person;
}

