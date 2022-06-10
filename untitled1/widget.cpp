#include "widget.h"

#include <struction.h>

#include "./ui_widget.h"

extern account *head;
extern int person_num;
extern int exceed;
extern char exceed_name[100];
extern int searcher_size;

QString background_path = QDir::temp().relativeFilePath("./background.wav");  // a path in the temporary folder
int noMean = QFile::copy(":/sound_effect/background.wav", background_path);  // Will return true if copied, false if the target already exists
QString cash_path = QDir::temp().relativeFilePath("./cash.wav");  // a path in the temporary folder
int noMean2 = QFile::copy(":/sound_effect/cash.wav", cash_path);  // Will return true if copied, false if the target already exists

class Items : public QTableWidgetItem {
   public:
    Items(QString StrLine) : QTableWidgetItem(StrLine){};

    bool operator<(const QTableWidgetItem &other) const {
        // TODO: To be safe, check weather conversion to int is possible.
        int idx1 = this->text().indexOf('/');
        int idx2 = other.text().indexOf('/');
        if (idx1 == -1 && idx2 == -1)
            return (this->text().toInt() < other.text().toInt());
        else {
            int month1 = this->text().mid(0, idx1).toInt(), month2 = other.text().mid(0, idx2).toInt();
            int day1 = this->text().mid(idx1 + 1, this->text().size() - idx1 - 1).toInt(),
                day2 = other.text().mid(idx2 + 1, other.text().size() - idx2 - 1).toInt();
            if (month1 != month2)
                return month1 < month2;
            else
                return day1 < day2;
        }
    }
};

Widget::Widget(QWidget *parent) : QWidget(parent), ui(new Ui::Widget) {
    ui->setupUi(this);
    ui->stackedWidget->setCurrentIndex(0);

    QPixmap bkgnd(":/img/rsz_background.jpg");
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Background, bkgnd);
    this->setPalette(palette);

    PlaySound((wchar_t *)background_path.utf16(), NULL, SND_ASYNC | SND_LOOP);

    ui->tableWidget->setColumnWidth(0, 85);
    ui->tableWidget->setColumnWidth(1, 115);
    ui->tableWidget->setColumnWidth(2, 115);
    ui->tableWidget->setColumnWidth(3, 60);
    ui->tableWidget->setColumnWidth(4, 55);

    ui->tableWidget_2->setColumnWidth(0, 85);
    ui->tableWidget_2->setColumnWidth(1, 115);
    ui->tableWidget_2->setColumnWidth(2, 115);
    ui->tableWidget_2->setColumnWidth(3, 60);
    ui->tableWidget_2->setColumnWidth(4, 55);

    ui->tableWidget_3->setColumnWidth(0, 85);
    ui->tableWidget_3->setColumnWidth(1, 115);
    ui->tableWidget_3->setColumnWidth(2, 115);
    ui->tableWidget_3->setColumnWidth(3, 60);
    ui->tableWidget_3->setColumnWidth(4, 55);
}

Widget::~Widget() { delete ui; }

void Widget::TableWidgetDisplay() {
    QTableWidget *table = ui->tableWidget;
    table->setRowCount(0);
    account *root = head;
    int i = 0;
    while (root != NULL) {
        account *cur = root;
        while (cur != NULL) {
            table->insertRow(i);
            table->setItem(i, 0, new QTableWidgetItem(cur->name));
            table->setItem(i, 1, new QTableWidgetItem(cur->classes));
            table->setItem(i, 2, new QTableWidgetItem(cur->item));
            table->setItem(i, 3, new Items(QString::number(cur->price)));
            QString date;
            date = (QString::number(cur->month) + '/' + QString::number(cur->day));
            table->setItem(i, 4, new Items(date));
            i++;
            cur = cur->next;
        }
        root = root->nextperson;
    }
    table->setSortingEnabled(true);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void Widget::on_pushButton_clicked()  // import
{
    QString file = "";
    file = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("CSV files (*.csv)"));
    if (file == "") return;

    int status = import_(file.toLocal8Bit().data());
    if (status == 1) {
        QMessageBox msgBox;
        msgBox.setText("Success Import!! Your import file is: \n" + file);
        msgBox.setWindowTitle("Success Import");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();

        TableWidgetDisplay();

        PlaySound((wchar_t *)cash_path.utf16(), NULL, SND_SYNC);
        PlaySound((wchar_t *)background_path.utf16(), NULL, SND_ASYNC | SND_LOOP);

    } else if (status == -1) {
        QMessageBox::about(this, tr("Import Error"), tr("Can't find the import file"));
    }
}

void Widget::on_pushButton_5_clicked()  // export
{
    QString file = "";
    file = QFileDialog::getSaveFileName(this, tr("Save File"), "./data.csv", tr("CSV files (*.csv)"));

    if (file == "") return;

    export_(file.toLocal8Bit().data());
    QMessageBox msgBox;
    msgBox.setText("Success Export!! Your export path is: \n" + file);
    msgBox.setWindowTitle("Success Export");
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.exec();
}

void Widget::on_toolButton_clicked()  // set background
{
    QString image = "";
    image = QFileDialog::getOpenFileName(this, tr("Open File"), "", tr("Images (*.png *.xpm *.jpg)"));

    if (image == "") return;

    QPixmap bkgnd(image);
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Background, bkgnd);
    this->setPalette(palette);
}

void Widget::on_pushButton_6_clicked()  // search data
{
    QString name = "", classs = "", date = "", item = "";
    int month = 0, day = 0;
    name = ui->lineEdit->text();
    classs = ui->lineEdit_2->text();
    date = ui->lineEdit_3->text();
    int idx = date.indexOf('/');
    if (idx != -1) {
        month = date.mid(0, idx).toInt();
        day = date.mid(idx + 1, date.size() - idx - 1).toInt();
    }
    account **searcher = search(name.toLocal8Bit().data(), month, day, classs.toLocal8Bit().data(), item.toLocal8Bit().data(), 0);

    if (searcher_size == 0) {
        QMessageBox::about(this, tr("Search Error"), tr("Can't find the selected data"));
        return;
    }

    QTableWidget *table = ui->tableWidget_2;
    table->setRowCount(0);
    for (int i = 0; i < searcher_size; i++) {
        table->insertRow(i);
        table->setItem(i, 0, new QTableWidgetItem(searcher[i]->name));
        table->setItem(i, 1, new QTableWidgetItem(searcher[i]->classes));
        table->setItem(i, 2, new QTableWidgetItem(searcher[i]->item));
        table->setItem(i, 3, new Items(QString::number(searcher[i]->price)));
        QString date2;
        date2 = (QString::number(searcher[i]->month) + '/' + QString::number(searcher[i]->day));
        table->setItem(i, 4, new Items(date2));
    }

    free(searcher);
}

bool check_exist(QString name, QString classes, QString item, int month, int day, int price) {
    account *head_copy = head;
    while (head_copy != NULL) {
        account *person = head_copy;
        while (person != NULL) {
            if (strcmp(person->name, name.toLocal8Bit().data()) == 0 && person->price == price &&
                strcmp(person->item, item.toLocal8Bit().data()) == 0 && person->month == month &&
                strcmp(person->classes, classes.toLocal8Bit().data()) == 0 && person->day == day) {
                return true;
            }
            person = person->next;
        }
        head_copy = head_copy->nextperson;
    }
    return false;
}

void Widget::on_pushButton_9_clicked()  // insert
{
    QString name = "", classes = "", item = "", date = "";
    int month = 0, day = 0, price = 0;
    name = ui->lineEdit_4->text();
    classes = ui->lineEdit_5->text();
    item = ui->lineEdit_6->text();
    price = ui->lineEdit_7->text().toInt();
    date = ui->lineEdit_8->text();

    int idx = date.indexOf('/');
    if (idx != -1) {
        month = date.mid(0, idx).toInt();
        day = date.mid(idx + 1, date.size() - idx - 1).toInt();
    }
    if (name == "" || classes == "" || item == "" || date == "") {
        QMessageBox::about(this, tr("Insert Error"), tr("Please fill out all entrys"));
    } else if (month <= 0 || day <= 0 || month > 12 || day > 31 || (month == 2 && day > 29) ||
               ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30)) {
        QMessageBox::about(this, tr("Insert Error"), tr("Please fill the date correctly"));
    } else if (price <= 0) {
        QMessageBox::about(this, tr("Insert Error"), tr("Please fill the price correctly"));
    } else {
        QMessageBox::StandardButton reply;
        if (check_exist(name, classes, item, month, day, price) == true) {
            reply = QMessageBox::question(this, "Insert Warn", "The data have already existed, do you still want to insert?",
                                          QMessageBox::Yes | QMessageBox::No);
            if (reply == QMessageBox::No) return;
        }

        QMessageBox msgBox;
        msgBox.setText("Inserted sucessesfully!!");
        msgBox.setWindowTitle("Inserted Sucessesfully");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();

        insert(name.toLocal8Bit().data(), classes.toLocal8Bit().data(), item.toLocal8Bit().data(), price, month, day);
        TableWidgetDisplay();

        ui->lineEdit_4->setText("");
        ui->lineEdit_5->setText("");
        ui->lineEdit_6->setText("");
        ui->lineEdit_7->setText("");
        ui->lineEdit_8->setText("");

        PlaySound((wchar_t *)cash_path.utf16(), NULL, SND_SYNC);
        PlaySound((wchar_t *)background_path.utf16(), NULL, SND_ASYNC | SND_LOOP);
    }
}

void Widget::on_pushButton_10_clicked()  // delete account
{
    QString name = "", classes = "", item = "", date = "";
    int month = 0, day = 0, price = 0;
    name = ui->lineEdit_9->text();
    classes = ui->lineEdit_10->text();
    item = ui->lineEdit_11->text();
    price = ui->lineEdit_12->text().toInt();
    date = ui->lineEdit_13->text();
    int idx = date.indexOf('/');
    if (idx != -1) {
        month = date.mid(0, idx).toInt();
        day = date.mid(idx + 1, date.size() - idx - 1).toInt();
    }
    int status = delete_node(name.toLocal8Bit().data(), month, day, price, classes.toLocal8Bit().data(), item.toLocal8Bit().data(), '2');
    if (status == 1) {
        QMessageBox msgBox;
        msgBox.setText("Deleted account sucessesfully!!");
        msgBox.setWindowTitle("Deleted Sucessesfully");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();

        TableWidgetDisplay();

        ui->lineEdit_9->setText("");
        ui->lineEdit_10->setText("");
        ui->lineEdit_11->setText("");
        ui->lineEdit_12->setText("");
        ui->lineEdit_13->setText("");
        ui->tableWidget_3->setRowCount(0);
    } else if (status == -1) {
        QMessageBox::about(this, tr("Delete Error"), tr("Can't find the account"));
    }
}

void Widget::on_pushButton_12_clicked()  // delete person
{
    QString name = "", classes = "", item = "", date = "";
    int month = 0, day = 0, price = 0;
    name = ui->lineEdit_9->text();
    classes = ui->lineEdit_10->text();
    item = ui->lineEdit_11->text();
    price = ui->lineEdit_12->text().toInt();
    date = ui->lineEdit_13->text();

    int idx = date.indexOf('/');
    if (idx != -1) {
        month = date.mid(0, idx).toInt();
        day = date.mid(idx + 1, date.size() - idx - 1).toInt();
    }

    int status = delete_node(name.toLocal8Bit().data(), month, day, price, classes.toLocal8Bit().data(), item.toLocal8Bit().data(), '1');
    if (status == 1) {
        QMessageBox msgBox;
        msgBox.setText("Deleted person sucessesfully!!");
        msgBox.setWindowTitle("Deleted Sucessesfully");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();

        TableWidgetDisplay();

        ui->lineEdit_9->setText("");
        ui->lineEdit_10->setText("");
        ui->lineEdit_11->setText("");
        ui->lineEdit_12->setText("");
        ui->lineEdit_13->setText("");
        ui->tableWidget_3->setRowCount(0);
    } else if (status == -1) {
        QMessageBox::about(this, tr("Delete Error"), tr("Can't find the person"));
    }
}

void Widget::on_pushButton_2_clicked()  // go to search
{
    ui->stackedWidget->setCurrentIndex(2);
}

void Widget::on_pushButton_7_clicked()  // search back
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->lineEdit->setText("");
    ui->lineEdit_2->setText("");
    ui->lineEdit_3->setText("");
    ui->tableWidget_2->setRowCount(0);
}

void Widget::on_pushButton_8_clicked()  // insert back
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->lineEdit_4->setText("");
    ui->lineEdit_5->setText("");
    ui->lineEdit_6->setText("");
    ui->lineEdit_7->setText("");
    ui->lineEdit_8->setText("");
}

void Widget::on_pushButton_4_clicked()  // go to insert
{
    ui->stackedWidget->setCurrentIndex(1);
}

void Widget::on_pushButton_11_clicked()  // delete back
{
    ui->stackedWidget->setCurrentIndex(0);
    ui->lineEdit_9->setText("");
    ui->lineEdit_10->setText("");
    ui->lineEdit_11->setText("");
    ui->lineEdit_12->setText("");
    ui->lineEdit_13->setText("");
    ui->tableWidget_3->setRowCount(0);
}

void Widget::on_pushButton_3_clicked()  // go to delete
{
    ui->stackedWidget->setCurrentIndex(3);
}

void Widget::on_pushButton_13_clicked()  // got to statistic
{
    ui->stackedWidget->setCurrentIndex(4);

    QListWidget *list = ui->listWidget;

    account **all_person = statistics();  // call statistic function and return to all_person array

    QStringList vHeader, hHeader;
    list->clear();  // clean

    /*build the table*/
    list->setEditTriggers(QAbstractItemView::NoEditTriggers);
    QFont font = list->font();
    font.setPointSize(12);
    list->setFont(font);
    for (int i = 0; i < person_num; i++) {  // i:row ,j:column
        for (int j = 0; j <= person_num - 1; j++) {
            if (all_person[i]->payTo[j] != 0) {
                QString a(all_person[i]->name), b(all_person[j]->name);
                QString str = a + " --> " + b + " ($" + QString::number(all_person[i]->payTo[j]) + ")";
                QListWidgetItem *relation = new QListWidgetItem(str);
                list->addItem(relation);  // no need to pay for people ahead of that perso
            }
        }
    }

    int total_sum = 0;
    for (int i = 0; i < person_num; i++) {
        total_sum += all_person[i]->sum;
    }

    int total_avg = (person_num == 0) ? 0 : total_sum / person_num;
    QString Qexceed_name(exceed_name);
    ui->label_15->setText(QString::number(total_sum));
    ui->label_17->setText(QString::number(total_avg));
    if (Qexceed_name != "                    ")
        ui->label_19->setText(Qexceed_name + " ($" + QString::number(exceed) + ")");

    free(all_person);
}

void Widget::on_pushButton_14_clicked()  // statistic back
{
    ui->stackedWidget->setCurrentIndex(0);
}
void Widget::on_pushButton_15_clicked()  // Guideline
{
    QMessageBox msgBox1;
    msgBox1.setText("Please insert bills or import a CSV file.");
    msgBox1.setWindowTitle("Guideline 1/4");
    msgBox1.setStandardButtons(QMessageBox::Ok);
    msgBox1.exec();

    QMessageBox msgBox2;
    msgBox2.setText("Then you can choose to delete a bill or check the statistic to split the bill.");
    msgBox2.setWindowTitle("Guideline 2/4");
    msgBox2.setStandardButtons(QMessageBox::Ok);
    msgBox2.exec();

    QMessageBox msgBox3;
    msgBox3.setText("When you want to save the database, you should export a CSV file for next time.");
    msgBox3.setWindowTitle("Guideline 3/4");
    msgBox3.setStandardButtons(QMessageBox::Ok);
    msgBox3.exec();
    
    QMessageBox msgBox4;
    msgBox4.setText("By the way, you can hide the table and change the background picture.");
    msgBox4.setWindowTitle("Guideline 4/4");
    msgBox4.setStandardButtons(QMessageBox::Ok);
    msgBox4.exec();
}

void Widget::on_checkBox_stateChanged(int arg1)  // hide table
{
    if (arg1)
        ui->tableWidget->setVisible(false);
    else
        ui->tableWidget->setVisible(true);
}

void Widget::search_for_delete() {
    QString name = "", classes = "", item = "", date = "";
    int month = 0, day = 0, price = 0;
    name = ui->lineEdit_9->text();
    classes = ui->lineEdit_10->text();
    item = ui->lineEdit_11->text();
    price = ui->lineEdit_12->text().toInt();
    date = ui->lineEdit_13->text();
    int idx = date.indexOf('/');
    if (idx != -1) {
        month = date.mid(0, idx).toInt();
        day = date.mid(idx + 1, date.size() - idx - 1).toInt();
    }
    account **searcher = search(name.toLocal8Bit().data(), month, day, classes.toLocal8Bit().data(), item.toLocal8Bit().data(), price);

    QTableWidget *table = ui->tableWidget_3;
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setRowCount(0);
    for (int i = 0; i < searcher_size; i++) {
        table->insertRow(i);
        table->setItem(i, 0, new QTableWidgetItem(searcher[i]->name));
        table->setItem(i, 1, new QTableWidgetItem(searcher[i]->classes));
        table->setItem(i, 2, new QTableWidgetItem(searcher[i]->item));
        table->setItem(i, 3, new Items(QString::number(searcher[i]->price)));
        QString date2;
        date2 = (QString::number(searcher[i]->month) + '/' + QString::number(searcher[i]->day));
        table->setItem(i, 4, new Items(date2));
    }

    free(searcher);
}

void Widget::on_lineEdit_9_editingFinished()  // delete name
{
    search_for_delete();
}

void Widget::on_lineEdit_10_editingFinished()  // delete class
{
    search_for_delete();
}

void Widget::on_lineEdit_11_editingFinished()  // delete item
{
    search_for_delete();
}

void Widget::on_lineEdit_12_editingFinished()  // delete price
{
    search_for_delete();
}

void Widget::on_lineEdit_13_editingFinished()  // delete date
{
    search_for_delete();
}
