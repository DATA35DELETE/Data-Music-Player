#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QLoggingCategory::setFilterRules("qt.multimedia.*=false");

    QApplication a(argc, argv);
    MainWindow w;
    //w.show();
    return QApplication::exec();
}
