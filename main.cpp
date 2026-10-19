#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QLoggingCategory::setFilterRules("qt.multimedia.*=false");

    QApplication a(argc, argv);

    QString mainQss;
    QFile mainDotQss(":/assets/qss/main.qss");
    if(mainDotQss.open(QFile::OpenModeFlag::ReadOnly))
    {
        QTextStream reader(&mainDotQss);

        mainQss = reader.readAll();
    }
    mainDotQss.close();

    a.setStyleSheet(mainQss);

    MainWindow w;

    QSettings ayarlarFile("ayarlar.ini", QSettings::Format::IniFormat);

    if(ayarlarFile.value("general/arkaPlan").isNull())
    {
        ayarlarFile.setValue("general/arkaPlan", "false");
    }
    else
    {
        if(ayarlarFile.value("general/arkaPlan").value<QString>().contains("false"))
        {
            a.setStyle(QStyleFactory::create("windowsVista"));

            w.setWindowIcon(QIcon(":/medyaKontrol/aydinlik/assets/medyaKontrol/aydinlik/NoMedia.png"));
            a.setWindowIcon(QIcon(":/medyaKontrol/aydinlik/assets/medyaKontrol/aydinlik/NoMedia.png"));
        }
        else
        {
            a.setStyle(QStyleFactory::create("windows11"));

            w.setWindowIcon(QIcon(":/medyaKontrol/karanlik/assets/medyaKontrol/karanlik/NoMedia.png"));
            a.setWindowIcon(QIcon(":/medyaKontrol/karanlik/assets/medyaKontrol/karanlik/NoMedia.png"));
        }
    }
    w.setWindowTitle("Data Music Player");
    //w.show();
    return QApplication::exec();
}
