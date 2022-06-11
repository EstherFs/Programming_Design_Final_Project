#include <QApplication>

#include "widget.h"

#ifdef QT_STATIC
#include <QtCore/QtPlugin>
Q_IMPORT_PLUGIN(DSServicePlugin)
#endif

int main(int argc, char *argv[]) 
{
    QApplication a(argc, argv);
    Widget w;
    w.setWindowTitle("Accounting With U");
    w.setWindowIcon(QIcon(":/img/icon.png"));
    w.show();
    return a.exec();
}
