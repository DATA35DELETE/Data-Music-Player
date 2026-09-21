#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    //QLoggingCategory::setFilterRules("qt.multimedia.*=false");

    QApplication a(argc, argv);

#ifdef WIN32
    a.setStyle("windows11");
#else
    a.setStyle("fusion");
#endif

    MainWindow w;
    w.setWindowIcon(QIcon(":/medyaKontrol/assets/medyaKontrol/NoMedia.png"));
    w.setWindowTitle("Data Music Player");
    //w.show();
    return QApplication::exec();
}
