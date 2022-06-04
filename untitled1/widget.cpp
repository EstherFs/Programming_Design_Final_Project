#include "widget.h"
#include "./ui_widget.h"
#include <struction.h>
#include <windows.h>


extern account *head;
extern int person_num;
extern int exceed;
extern char exceed_name[20];

QMediaPlaylist *playlist = new QMediaPlaylist();
QMediaPlayer *background_music = new QMediaPlayer();
QMediaPlayer *cash_music = new QMediaPlayer();


class Items: public QTableWidgetItem
{

 public:
     Items(QString StrLine):QTableWidgetItem(StrLine){};


     bool operator< (const QTableWidgetItem &other) const
     {
         // TODO: To be safe, check weather conversion to int is possible.
         int idx1 = this->text().indexOf('/');
         int idx2 = other.text().indexOf('/');
         if(idx1==-1 && idx2==-1)
            return (this->text().toInt() < other.text().toInt());
         else
         {
            int month1 = this->text().mid(0,idx1).toInt(), month2 =other.text().mid(0,idx2).toInt();
            int day1 = this->text().mid(idx1+1,this->text().size()-idx1-1).toInt(), day2 =other.text().mid(idx2+1,other.text().size()-idx2-1).toInt();
            if(month1!=month2)
                return month1 < month2;
            else
                return day1 < day2;
         }
     }
 };


Widget::Widget(QWidget *parent): QWidget(parent), ui(new Ui::Widget)
{
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);

    QPixmap bkgnd(":/img/rsz_background.jpg");
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Background, bkgnd);
    this->setPalette(palette);
    background_music->setMedia(QUrl("qrc:/sound_effect/background_music.mp3"));
    cash_music->setMedia(QUrl("qrc:/sound_effect/cash.mp3"));
    background_music->play();

    connect(background_music, &QMediaPlayer::stateChanged, [&](QMediaPlayer::State state) {
            if (state == QMediaPlayer::State::StoppedState)
            {
                background_music->play();
            }
        });

    ui->tableWidget->setColumnWidth(0,85);
    ui->tableWidget->setColumnWidth(1,115);
    ui->tableWidget->setColumnWidth(2,115);
    ui->tableWidget->setColumnWidth(3,60);
    ui->tableWidget->setColumnWidth(4,55);

    ui->tableWidget_2->setColumnWidth(0,85);
    ui->tableWidget_2->setColumnWidth(1,115);
    ui->tableWidget_2->setColumnWidth(2,115);
    ui->tableWidget_2->setColumnWidth(3,60);
    ui->tableWidget_2->setColumnWidth(4,55);
}


Widget::~Widget()
{
    delete ui;
}


void Widget::TableWidgetDisplay()
{

    QTableWidget *table = ui->tableWidget;
    table->setRowCount(0);
    account *root = head;
    int i = 0;
    while(root!=NULL){
        account *cur = root;
        while(cur!=NULL)
        {
            table->insertRow(i);
            table->setItem(i,0,new QTableWidgetItem(cur->name));
            table->setItem(i,1,new QTableWidgetItem(cur->classes));
            table->setItem(i,2,new QTableWidgetItem(cur->item));
            table->setItem(i,3,new Items(QString::number(cur->price)));
            QString date;
            date = (QString::number(cur->month)+'/'+QString::number(cur->day));
            table->setItem(i,4,new Items(date));
            i++;
            cur = cur->next;

        }
        root = root->nextperson;
    }
    table->setSortingEnabled(true);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
}


void Widget::on_pushButton_clicked()//import
{
    QString file = "";
    file = QFileDialog::getOpenFileName(this, tr("Open File"),
                                      "",
                                      tr("CSV files (*.csv)"));
    if(file=="")
        return;

    int status = text_input(file.toLocal8Bit().data());
    if(status==1)
    {
        background_music->stop();
        cash_music->play();
        background_music->play();

        QMessageBox msgBox;
        msgBox.setText("Success Import!! Your import file is: \n"+file);
        msgBox.setWindowTitle("Success Import");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();

        TableWidgetDisplay();
    }
    else if(status==-1)
    {
        QMessageBox::warning(this,tr("Import Error"), tr("Can't find the import file"),QMessageBox::Ok);
    }

}


void Widget::on_pushButton_5_clicked() //export
{
    QString file = "";
    file = QFileDialog::getSaveFileName(this, tr("Save File"),
                                       "./data.csv",
                                       tr("CSV files (*.csv)"));
    if(file=="")
        return;

    text_output(file.toLocal8Bit().data());
    QMessageBox msgBox;
    msgBox.setText("Success Export!! Your export path is: \n"+file);
    msgBox.setWindowTitle("Success Export");
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.exec();
}


void Widget::on_toolButton_clicked() //set background
{
    QString image = "";
    image = QFileDialog::getOpenFileName(this, tr("Open File"),
                                            image,
                                            tr("Images (*.png *.xpm *.jpg)"));
    if(image=="")
        return;
    QPixmap bkgnd(image);
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Background, bkgnd);
    this->setPalette(palette);
}


void Widget::on_pushButton_6_clicked() //search data
{
    QString name="",classs="",date="";
    int month=0,day=0;
    name = ui->lineEdit->text();
    classs = ui->lineEdit_2->text();
    date = ui->lineEdit_3->text();
    int idx = date.indexOf('/');
    if(idx!=-1){
        month = date.mid(0,idx).toInt();
        day = date.mid(idx+1,date.size()-idx-1).toInt();
    }
    std::vector<account*> searcher = search(name.toLocal8Bit().data(), month, day, classs.toLocal8Bit().data());

    if(searcher.empty()){
        QMessageBox::warning(this,tr("Search Error"), tr("Can't find the selected data"),QMessageBox::Ok);
        return;
    }

    QTableWidget *table = ui->tableWidget_2;
    table->setRowCount(0);
    for(int i=0;i<(int)searcher.size();i++){
        table->insertRow(i);
        table->setItem(i,0,new QTableWidgetItem(searcher[i]->name));
        table->setItem(i,1,new QTableWidgetItem(searcher[i]->classes));
        table->setItem(i,2,new QTableWidgetItem(searcher[i]->item));
        table->setItem(i,3,new Items(QString::number(searcher[i]->price)));
        QString date2;
        date2 = (QString::number(searcher[i]->month)+'/'+QString::number(searcher[i]->day));
        table->setItem(i,4,new Items(date2));
    }

}


void Widget::on_pushButton_9_clicked() //insert
{
    QString name="",classes="",item="",date="";
    int month=0, day=0, price=0;
    name = ui->lineEdit_4->text();
    classes = ui->lineEdit_5->text();
    item = ui->lineEdit_6->text();
    price = ui->lineEdit_7->text().toInt();
    date = ui->lineEdit_8->text();

    int idx = date.indexOf('/');
    if(idx!=-1){
        month = date.mid(0,idx).toInt();
        day = date.mid(idx+1,date.size()-idx-1).toInt();
    }
    if(name=="" || classes=="" || item=="" || date=="" ){
        QMessageBox::warning(this,tr("Insert Error"), tr("Please fill out all entrys"),QMessageBox::Ok);
    }
    else if(month<=0||day<=0||month>12||day>31||(month==2&&day>29)||((month==4||month==6||month==9||month==11)&&day>30)){
        QMessageBox::warning(this,tr("Insert Error"), tr("Please fill the date correctly"),QMessageBox::Ok);
    }
    else if(price<=0){
        QMessageBox::warning(this,tr("Insert Error"), tr("Please fill the price correctly"),QMessageBox::Ok);
    }
    else{
        background_music->stop();
        cash_music->play();
        background_music->play();
        QMessageBox msgBox;
        msgBox.setText("Inserted sucessesfully!!");
        msgBox.setWindowTitle("Inserted Sucessesfully");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();

        insert_node(name.toLocal8Bit().data(), classes.toLocal8Bit().data(), item.toLocal8Bit().data(), price, month, day);
        TableWidgetDisplay();

    }

}


void Widget::on_pushButton_10_clicked() //delete account
{
    QString name="",classes="",item="",date="";
    int month=0, day=0, price=0;
    name = ui->lineEdit_9->text();
    classes = ui->lineEdit_10->text();
    item = ui->lineEdit_11->text();
    price = ui->lineEdit_12->text().toInt();
    date = ui->lineEdit_13->text();
    int idx = date.indexOf('/');
    if(idx!=-1){
        month = date.mid(0,idx).toInt();
        day = date.mid(idx+1,date.size()-idx-1).toInt();
    }
    int status = delete_node(name.toLocal8Bit().data(), month, day, price, classes.toLocal8Bit().data(), item.toLocal8Bit().data(), '2');
    if(status==1){
        QMessageBox msgBox;
        msgBox.setText("Deleted account sucessesfully!!");
        msgBox.setWindowTitle("Deleted Sucessesfully");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();

        TableWidgetDisplay();
    }
    else if(status==-1){
        QMessageBox::warning(this,tr("Delete Error"), tr("Can't find the account"),QMessageBox::Ok);
    }
}


void Widget::on_pushButton_12_clicked() //delete person
{
    QString name="",classes="",item="",date="";
    int month=0, day=0, price=0;
    name = ui->lineEdit_9->text();
    classes = ui->lineEdit_10->text();
    item = ui->lineEdit_11->text();
    price = ui->lineEdit_12->text().toInt();
    date = ui->lineEdit_13->text();

    int idx = date.indexOf('/');
    if(idx!=-1){
        month = date.mid(0,idx).toInt();
        day = date.mid(idx+1,date.size()-idx-1).toInt();
    }

    int status = delete_node(name.toLocal8Bit().data(), month, day, price, classes.toLocal8Bit().data(), item.toLocal8Bit().data(), '1');
    if(status==1){
        QMessageBox msgBox;
        msgBox.setText("Deleted person sucessesfully!!");
        msgBox.setWindowTitle("Deleted Sucessesfully");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();

        TableWidgetDisplay();
    }
    else if(status==-1){
        QMessageBox::warning(this,tr("Delete Error"), tr("Can't find the person"),QMessageBox::Ok);
    }
}


void Widget::on_pushButton_2_clicked() //go to search
{
    ui->stackedWidget->setCurrentIndex(2);
}


void Widget::on_pushButton_7_clicked() //search back
{
    ui->stackedWidget->setCurrentIndex(0);
}


void Widget::on_pushButton_8_clicked() // insert back
{
    ui->stackedWidget->setCurrentIndex(0);
}


void Widget::on_pushButton_4_clicked() // go to insert
{
    ui->stackedWidget->setCurrentIndex(1);
}


void Widget::on_pushButton_11_clicked() // delete back
{
    ui->stackedWidget->setCurrentIndex(0);
}


void Widget::on_pushButton_3_clicked() //go to delete
{
    ui->stackedWidget->setCurrentIndex(3);
}


void Widget::on_pushButton_13_clicked() //got to statistic
{
    ui->stackedWidget->setCurrentIndex(4);

    QListWidget *list = ui->listWidget;

    account **all_person = statistics(); //call statistic function and return to all_person array

    QStringList vHeader, hHeader;
    list->clear(); //clean

    /*build the table*/
    list->setEditTriggers(QAbstractItemView::NoEditTriggers);
    QFont font = list->font();
    font.setPointSize(12);
    list->setFont(font);
    for(int i=0;i<person_num;i++){ //i:row ,j:column
        for(int j=0;j<=person_num-1;j++){
            if(all_person[i]->payTo[j]!=0){
                QString a(all_person[i]->name), b(all_person[j]->name);
                QString str = a + " --> " + b + " ($" + QString::number(all_person[i]->payTo[j]) + ")";
                QListWidgetItem *relation = new QListWidgetItem(str);
                list->addItem(relation);//no need to pay for people ahead of that perso
            }
        }
    }

    int total_sum = 0;
    for(int i=0; i<person_num; i++){
        total_sum += all_person[i]->sum;
    }

    int total_avg = (person_num==0)?0:total_sum/person_num;
    QString Qexceed_name(exceed_name);
    ui->label_15->setText(QString::number(total_sum));
    ui->label_17->setText(QString::number(total_avg));
    ui->label_19->setText(Qexceed_name + " ($" + QString::number(exceed) +")");
}

void Widget::on_pushButton_14_clicked() //statistic back
{
    ui->stackedWidget->setCurrentIndex(0);
}


void Widget::on_checkBox_stateChanged(int arg1) //hide table
{
    if(arg1)
        ui->tableWidget->setVisible(false);
    else
        ui->tableWidget->setVisible(true);
}

