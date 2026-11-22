#include "datathemeconfigurer.h"

/*dataThemeConfigurer::dataThemeConfigurer(Themes userTheme, QApplication* appPtr)
{
    QDir().mkdir("dataTheme");

    QDir dataThemeFolder("dataTheme");
    dataThemeFolder.setFilter(QDir::Filter::Files);

    for (auto i : dataThemeFolder.entryList()) {
        QFile::remove(i.prepend("dataTheme/"));
    }

    QProcess* unzipProcess = new QProcess(this);

    QNetworkAccessManager* mngr = new QNetworkAccessManager(this);
    connect(mngr, &QNetworkAccessManager::finished, this,
    [=](QNetworkReply* rp)
    {
        QFile downloanded("dataTheme/it.zip");
        if(downloanded.open(QFile::OpenModeFlag::WriteOnly))
        {
            QDataStream stream(&downloanded);
            stream << rp->readAll();
        }
        downloanded.close();

        QStringList args;

        args << "dataTheme/it.zip"
             << "-d" << "dataTheme";

        unzipProcess->start("unzip", args);

        mngr->deleteLater();
    });

    connect(unzipProcess, &QProcess::finished, this,
    [=](int){
        QFile::remove("dataTheme/it.zip");

        QDir themesFiles("dataTheme/Data-s-QSS-1.2v");
        themesFiles.setFilter(QDir::Filter::Files);

        for (auto i : themesFiles.entryList()) {
            if(i.contains("LICENSE"))
                QFile::remove(QFileInfo(QString(i).prepend("dataTheme/Data-s-QSS-1.2v/")).filePath());

            QFile::copy(QFileInfo(QString(i).prepend("dataTheme/Data-s-QSS-1.2v/")).filePath(), QFileInfo(QString(i).prepend("dataTheme/")).filePath());
            QFile::remove(QFileInfo(QString(i).prepend("dataTheme/Data-s-QSS-1.2v/")).filePath());
        }

        QDir().mkdir("dataTheme/icons");

        QDir iconsFiles("dataTheme/Data-s-QSS-1.2v/icons");
        iconsFiles.setFilter(QDir::Filter::Files);

        for (auto i : iconsFiles.entryList()) {
            QFile::copy(QFileInfo(QString(i).prepend("dataTheme/Data-s-QSS-1.2v/icons/")).filePath(), QFileInfo(QString(i).prepend("dataTheme/icons/")).filePath());
            QFile::remove(QFileInfo(QString(i).prepend("dataTheme/Data-s-QSS-1.2v/icons/")).filePath());
        }

        QDir().rmdir("dataTheme/Data-s-QSS-1.2v/icons");

        QDir().rmdir("dataTheme/Data-s-QSS-1.2v");

        setTheme(userTheme);
        setApplicationTheme(appPtr);

        initFinished();

        unzipProcess->deleteLater();
    });

    mngr->get(QNetworkRequest(QUrl("https://github.com/DATA35DELETE/Data-s-QSS/archive/refs/tags/1.2v.zip")));
}*/
dataThemeConfigurer::dataThemeConfigurer(bool){ emit initFinished();}
dataThemeConfigurer::dataThemeConfigurer()
{
    QDir().mkdir("dataTheme");

    QDir dataThemeFolder("dataTheme");
    dataThemeFolder.setFilter(QDir::Filter::Files);

    for (auto i : dataThemeFolder.entryList()) {
        QFile::remove(i.prepend("dataTheme/"));
    }

    QProcess* unzipProcess = new QProcess(this);

    QNetworkAccessManager* mngr = new QNetworkAccessManager(this);
    connect(mngr, &QNetworkAccessManager::finished, this,
            [=](QNetworkReply* rp)
            {
                QFile downloanded("dataTheme/it.zip");
                if(downloanded.open(QFile::OpenModeFlag::WriteOnly))
                {
                    QDataStream stream(&downloanded);
                    stream << rp->readAll();
                }
                downloanded.close();

                QStringList args;
                QString program;

#ifdef Q_OS_WIN
                program = "tar";
                args << "-xf" << "dataTheme/it.zip" << "-C" << "dataTheme";
#else
                program = "unzip";
                args << "-o" << "dataTheme/it.zip" << "-d" << "dataTheme";
#endif

                unzipProcess->start(program, args);
                mngr->deleteLater();
            });

    connect(unzipProcess, &QProcess::finished, this,
            [=](int){
                QFile::remove("dataTheme/it.zip");

                QDir themesFiles("dataTheme/Data-s-QSS-1.2v");
                themesFiles.setFilter(QDir::Filter::Files);

                for (auto i : themesFiles.entryList()) {
                    if(i.contains("LICENSE"))
                        QFile::remove(QFileInfo(QString(i).prepend("dataTheme/Data-s-QSS-1.2v/")).filePath());

                    QFile::copy(QFileInfo(QString(i).prepend("dataTheme/Data-s-QSS-1.2v/")).filePath(), QFileInfo(QString(i).prepend("dataTheme/")).filePath());
                    QFile::remove(QFileInfo(QString(i).prepend("dataTheme/Data-s-QSS-1.2v/")).filePath());
                }

                QDir().mkdir("dataTheme/icons");

                QDir iconsFiles("dataTheme/Data-s-QSS-1.2v/icons");
                iconsFiles.setFilter(QDir::Filter::Files);

                for (auto i : iconsFiles.entryList()) {
                    QFile::copy(QFileInfo(QString(i).prepend("dataTheme/Data-s-QSS-1.2v/icons/")).filePath(), QFileInfo(QString(i).prepend("dataTheme/icons/")).filePath());
                    QFile::remove(QFileInfo(QString(i).prepend("dataTheme/Data-s-QSS-1.2v/icons/")).filePath());
                }

                QDir().rmdir("dataTheme/Data-s-QSS-1.2v/icons");

                QDir().rmdir("dataTheme/Data-s-QSS-1.2v");

                emit initFinished();

                unzipProcess->deleteLater();
            });

    mngr->get(QNetworkRequest(QUrl("https://github.com/DATA35DELETE/Data-s-QSS/archive/refs/tags/1.2v.zip")));
}

void dataThemeConfigurer::setTheme(Themes userTheme) {theme = userTheme;};
Themes dataThemeConfigurer::getTheme() {return theme;};

void dataThemeConfigurer::setApplicationTheme(QApplication* appPtr)
{
    QFile* qssFile;
    QString qss;

    if(theme == Themes::Black)
    {
        qssFile = new QFile("dataTheme/black.qss");
    }
    else if(theme == Themes::White)
    {
        qssFile = new QFile("dataTheme/white.qss");
    }
    else if(theme == Themes::GrayDark)
    {
        qssFile = new QFile("dataTheme/gray-dark.qss");
    }
    else if(theme == Themes::GrayLigth)
    {
        qssFile = new QFile("dataTheme/gray-light.qss");
    }
    else if(theme == Themes::RedDark)
    {
        qssFile = new QFile("dataTheme/red-dark.qss");
    }
    else if(theme == Themes::RedLight)
    {
        qssFile = new QFile("dataTheme/red-light.qss");
    }
    else if(theme == Themes::GreenDark)
    {
        qssFile = new QFile("dataTheme/green-dark.qss");
    }
    else if(theme == Themes::GreenLight)
    {
        qssFile = new QFile("dataTheme/green-light.qss");
    }
    else if(theme == Themes::BlueDark)
    {
        qssFile = new QFile("dataTheme/blue-dark.qss");
    }
    else if(theme == Themes::BlueLight)
    {
        qssFile = new QFile("dataTheme/blue-light.qss");
    }
    else if(theme == Themes::CyanDark)
    {
        qssFile = new QFile("dataTheme/cyan-dark.qss");
    }
    else if(theme == Themes::CyanLight)
    {
        qssFile = new QFile("dataTheme/cyan-light.qss");
    }
    else if(theme == Themes::PurpleDark)
    {
        qssFile = new QFile("dataTheme/purple-dark.qss");
    }
    else if(theme == Themes::PurpleLight)
    {
        qssFile = new QFile("dataTheme/purple-light.qss");
    }
    else if(theme == Themes::YellowDark)
    {
        qssFile = new QFile("dataTheme/yellow-dark.qss");
    }
    else if(theme == Themes::YellowLight)
    {
        qssFile = new QFile("dataTheme/yellow-light.qss");
    }
    else if(theme == Themes::OrangeDark)
    {
        qssFile = new QFile("dataTheme/orange-dark.qss");
    }
    else if(theme == Themes::OrangeLight)
    {
        qssFile = new QFile("dataTheme/orange-light.qss");
    }
    else if(theme == Themes::Default)
    {
        qssFile = new QFile();
    }

    if(qssFile->open(QFile::OpenModeFlag::ReadOnly))
    {
        qss = QTextStream(qssFile).readAll();

        qssFile->close();
    }

    if(theme == Themes::White || theme == Themes::BlueLight || theme == Themes::RedLight || theme == Themes::GreenLight || theme == Themes::GrayLigth || theme == Themes::CyanLight || theme == Themes::YellowLight || theme == Themes::OrangeLight || theme == Themes::PurpleLight)
    {
        qApp->setStyle("windowsVista");
    }
    else
    {
#ifdef WIN32
        qApp->setStyle("windows11");
#else
        qApp->setStyle("fusion");
#endif
    }

    QFile downArrow("dataTheme/icons/downArrow.svg");
    QString color = "";

    if(theme == Themes::Black) color = "#FFFFFF";
    else if(theme == Themes::White) color = "#111827";
    else if(theme == Themes::GrayDark) color = "#B0B5C4";
    else if(theme == Themes::GrayLigth) color = "#242830";
    else if(theme == Themes::RedDark) color = "#FF1744";
    else if(theme == Themes::RedLight) color = "#B71C1C";
    else if(theme == Themes::GreenDark) color = "#00FF41";
    else if(theme == Themes::GreenLight) color = "#1B5E20";
    else if(theme == Themes::BlueDark) color = "#2979FF";
    else if(theme == Themes::BlueLight) color = "#0D47A1";
    else if(theme == Themes::CyanDark) color = "#00E5FF";
    else if(theme == Themes::CyanLight) color = "#006064";
    else if(theme == Themes::PurpleDark) color = "#D500F9";
    else if(theme == Themes::PurpleLight) color = "#4A148C";
    else if(theme == Themes::YellowDark) color = "#FFD600";
    else if(theme == Themes::YellowLight) color = "#7F5F00";
    else if(theme == Themes::OrangeDark) color = "#FF6D00";
    else if(theme == Themes::OrangeLight) color = "#BF360C";

    if(downArrow.open(QFile::WriteOnly))
    {
        QFile baseDownArrow("dataTheme/icons/baseDownArrow.svg");
        if(baseDownArrow.open(QFile::ReadOnly))
        {
            QTextStream(&downArrow) << QTextStream(&baseDownArrow).readAll().replace("currentColor",color);
        }
        baseDownArrow.close();
    }
    downArrow.close();

    QFile upArrow("dataTheme/icons/upArrow.svg");

    if(theme == Themes::Black) color = "#FFFFFF";
    else if(theme == Themes::White) color = "#111827";
    else if(theme == Themes::GrayDark) color = "#B0B5C4";
    else if(theme == Themes::GrayLigth) color = "#242830";
    else if(theme == Themes::RedDark) color = "#FF1744";
    else if(theme == Themes::RedLight) color = "#B71C1C";
    else if(theme == Themes::GreenDark) color = "#00FF41";
    else if(theme == Themes::GreenLight) color = "#1B5E20";
    else if(theme == Themes::BlueDark) color = "#2979FF";
    else if(theme == Themes::BlueLight) color = "#0D47A1";
    else if(theme == Themes::CyanDark) color = "#00E5FF";
    else if(theme == Themes::CyanLight) color = "#006064";
    else if(theme == Themes::PurpleDark) color = "#D500F9";
    else if(theme == Themes::PurpleLight) color = "#4A148C";
    else if(theme == Themes::YellowDark) color = "#FFD600";
    else if(theme == Themes::YellowLight) color = "#7F5F00";
    else if(theme == Themes::OrangeDark) color = "#FF6D00";
    else if(theme == Themes::OrangeLight) color = "#BF360C";

    if(upArrow.open(QFile::WriteOnly))
    {
        QFile baseUpArrow("dataTheme/icons/baseDownupArrow.svg");
        if(baseUpArrow.open(QFile::ReadOnly))
        {
            QTextStream(&upArrow) << QTextStream(&baseUpArrow).readAll().replace("currentColor",color);
        }
        baseUpArrow.close();
    }
    upArrow.close();

    appPtr->setStyleSheet(qss);
}
void dataThemeConfigurer::setApplicationTheme(QApplication* appPtr, Themes userTheme){
    QFile* qssFile;
    QString qss;

    if(userTheme == Themes::Black)
    {
        qssFile = new QFile("dataTheme/black.qss");
    }
    else if(userTheme == Themes::White)
    {
        qssFile = new QFile("dataTheme/white.qss");
    }
    else if(userTheme == Themes::GrayDark)
    {
        qssFile = new QFile("dataTheme/gray-dark.qss");
    }
    else if(userTheme == Themes::GrayLigth)
    {
        qssFile = new QFile("dataTheme/gray-light.qss");
    }
    else if(userTheme == Themes::RedDark)
    {
        qssFile = new QFile("dataTheme/red-dark.qss");
    }
    else if(userTheme == Themes::RedLight)
    {
        qssFile = new QFile("dataTheme/red-light.qss");
    }
    else if(userTheme == Themes::GreenDark)
    {
        qssFile = new QFile("dataTheme/green-dark.qss");
    }
    else if(userTheme == Themes::GreenLight)
    {
        qssFile = new QFile("dataTheme/green-light.qss");
    }
    else if(userTheme == Themes::BlueDark)
    {
        qssFile = new QFile("dataTheme/blue-dark.qss");
    }
    else if(userTheme == Themes::BlueLight)
    {
        qssFile = new QFile("dataTheme/blue-light.qss");
    }
    else if(userTheme == Themes::CyanDark)
    {
        qssFile = new QFile("dataTheme/cyan-dark.qss");
    }
    else if(userTheme == Themes::CyanLight)
    {
        qssFile = new QFile("dataTheme/cyan-light.qss");
    }
    else if(userTheme == Themes::PurpleDark)
    {
        qssFile = new QFile("dataTheme/purple-dark.qss");
    }
    else if(userTheme == Themes::PurpleLight)
    {
        qssFile = new QFile("dataTheme/purple-light.qss");
    }
    else if(userTheme == Themes::YellowDark)
    {
        qssFile = new QFile("dataTheme/yellow-dark.qss");
    }
    else if(userTheme == Themes::YellowLight)
    {
        qssFile = new QFile("dataTheme/yellow-light.qss");
    }
    else if(userTheme == Themes::OrangeDark)
    {
        qssFile = new QFile("dataTheme/orange-dark.qss");
    }
    else if(userTheme == Themes::OrangeLight)
    {
        qssFile = new QFile("dataTheme/orange-dark.qss");
    }
    else if(userTheme == Themes::Default)
    {
        qssFile = new QFile();
    }

    if(qssFile->open(QFile::OpenModeFlag::ReadOnly))
    {
        qss = QTextStream(qssFile).readAll();
    }
    qssFile->close();

    if(userTheme == Themes::White || userTheme == Themes::BlueLight || userTheme == Themes::RedLight || userTheme == Themes::GreenLight || userTheme == Themes::GrayLigth || userTheme == Themes::CyanLight || userTheme == Themes::YellowLight || userTheme == Themes::OrangeLight || userTheme == Themes::PurpleLight)
    {
        qApp->setStyle("windowsVista");
    }
    else
    {
#ifdef WIN32
        qApp->setStyle("windows11");
#else
        qApp->setStyle("fusion");
#endif
    }

    QFile downArrow("dataTheme/icons/downArrow.svg");
    QString color = "";

    if(userTheme == Themes::Black) color = "#FFFFFF";
    else if(userTheme == Themes::White) color = "#111827";
    else if(userTheme == Themes::GrayDark) color = "#B0B5C4";
    else if(userTheme == Themes::GrayLigth) color = "#242830";
    else if(userTheme == Themes::RedDark) color = "#FF1744";
    else if(userTheme == Themes::RedLight) color = "#B71C1C";
    else if(userTheme == Themes::GreenDark) color = "#00FF41";
    else if(userTheme == Themes::GreenLight) color = "#1B5E20";
    else if(userTheme == Themes::BlueDark) color = "#2979FF";
    else if(userTheme == Themes::BlueLight) color = "#0D47A1";
    else if(userTheme == Themes::CyanDark) color = "#00E5FF";
    else if(userTheme == Themes::CyanLight) color = "#006064";
    else if(userTheme == Themes::PurpleDark) color = "#D500F9";
    else if(userTheme == Themes::PurpleLight) color = "#4A148C";
    else if(userTheme == Themes::YellowDark) color = "#FFD600";
    else if(userTheme == Themes::YellowLight) color = "#7F5F00";
    else if(userTheme == Themes::OrangeDark) color = "#FF6D00";
    else if(userTheme == Themes::OrangeLight) color = "#BF360C";
    else if(userTheme == Themes::Default) color = "#FFFFFF";

    if(downArrow.open(QFile::WriteOnly))
    {
        QFile baseDownArrow("dataTheme/icons/baseDownArrow.svg");
        if(baseDownArrow.open(QFile::ReadOnly))
        {
            QTextStream(&downArrow) << QTextStream(&baseDownArrow).readAll().replace("currentColor",color);
        }
        baseDownArrow.close();
    }
    downArrow.close();

    QFile upArrow("dataTheme/icons/upArrow.svg");

    if(userTheme == Themes::Black) color = "#FFFFFF";
    else if(userTheme == Themes::White) color = "#111827";
    else if(userTheme == Themes::GrayDark) color = "#B0B5C4";
    else if(userTheme == Themes::GrayLigth) color = "#242830";
    else if(userTheme == Themes::RedDark) color = "#FF1744";
    else if(userTheme == Themes::RedLight) color = "#B71C1C";
    else if(userTheme == Themes::GreenDark) color = "#00FF41";
    else if(userTheme == Themes::GreenLight) color = "#1B5E20";
    else if(userTheme == Themes::BlueDark) color = "#2979FF";
    else if(userTheme == Themes::BlueLight) color = "#0D47A1";
    else if(userTheme == Themes::CyanDark) color = "#00E5FF";
    else if(userTheme == Themes::CyanLight) color = "#006064";
    else if(userTheme == Themes::PurpleDark) color = "#D500F9";
    else if(userTheme == Themes::PurpleLight) color = "#4A148C";
    else if(userTheme == Themes::YellowDark) color = "#FFD600";
    else if(userTheme == Themes::YellowLight) color = "#7F5F00";
    else if(userTheme == Themes::OrangeDark) color = "#FF6D00";
    else if(userTheme == Themes::OrangeLight) color = "#BF360C";
    else if(userTheme == Themes::Default) color = "#FFFFFF";


    if(upArrow.open(QFile::WriteOnly))
    {
        QFile baseUpArrow("dataTheme/icons/baseDownupArrow.svg");
        if(baseUpArrow.open(QFile::ReadOnly))
        {
            QTextStream(&upArrow) << QTextStream(&baseUpArrow).readAll().replace("currentColor",color);
        }
        baseUpArrow.close();
    }
    upArrow.close();

    appPtr->setStyleSheet(qss);
}

void dataThemeConfigurer::setIcon(QPushButton* buttonPtr, QString path)
{
    QFile originalImage(path);
    QString originalContext = "";

    if(originalImage.open(QFile::OpenModeFlag::ReadOnly))
    {
        originalContext = QTextStream(&originalImage).readAll();
    }
    originalImage.close();

    QString color = "";

    if(theme == Themes::Black) color = "#FFFFFF";
    else if(theme == Themes::White) color = "#111827";
    else if(theme == Themes::GrayDark) color = "#B0B5C4";
    else if(theme == Themes::GrayLigth) color = "#242830";
    else if(theme == Themes::RedDark) color = "#FF1744";
    else if(theme == Themes::RedLight) color = "#B71C1C";
    else if(theme == Themes::GreenDark) color = "#00FF41";
    else if(theme == Themes::GreenLight) color = "#1B5E20";
    else if(theme == Themes::BlueDark) color = "#2979FF";
    else if(theme == Themes::BlueLight) color = "#0D47A1";
    else if(theme == Themes::CyanDark) color = "#00E5FF";
    else if(theme == Themes::CyanLight) color = "#006064";
    else if(theme == Themes::PurpleDark) color = "#D500F9";
    else if(theme == Themes::PurpleLight) color = "#4A148C";
    else if(theme == Themes::YellowDark) color = "#FFD600";
    else if(theme == Themes::YellowLight) color = "#7F5F00";
    else if(theme == Themes::OrangeDark) color = "#FF6D00";
    else if(theme == Themes::OrangeLight) color = "#BF360C";
    else if(theme == Themes::Default) color = "#FFFFFF";

    originalContext.replace("currentColor", color);

    QFile copyImage(QString("dataTheme/icons/").append(QFileInfo(path).fileName()).append(".svg"));
    if(copyImage.open(QFile::WriteOnly))
    {
        QTextStream(&copyImage) << originalContext;
    }
    copyImage.close();

    buttonPtr->setIcon(QIcon(QString("dataTheme/icons/").append(QFileInfo(path).fileName()).append(".svg")));
}

void dataThemeConfigurer::setIcon(QLabel* labelPtr, QString path)
{
    QFile originalImage(path);
    QString originalContext = "";

    if (originalImage.open(QIODevice::ReadOnly)) {
        originalContext = QString::fromUtf8(originalImage.readAll());
        originalImage.close();
    }

    originalImage.close();

    QString color = "";

    if(theme == Themes::Black) color = "#FFFFFF";
    else if(theme == Themes::White) color = "#111827";
    else if(theme == Themes::GrayDark) color = "#B0B5C4";
    else if(theme == Themes::GrayLigth) color = "#242830";
    else if(theme == Themes::RedDark) color = "#FF1744";
    else if(theme == Themes::RedLight) color = "#B71C1C";
    else if(theme == Themes::GreenDark) color = "#00FF41";
    else if(theme == Themes::GreenLight) color = "#1B5E20";
    else if(theme == Themes::BlueDark) color = "#2979FF";
    else if(theme == Themes::BlueLight) color = "#0D47A1";
    else if(theme == Themes::CyanDark) color = "#00E5FF";
    else if(theme == Themes::CyanLight) color = "#006064";
    else if(theme == Themes::PurpleDark) color = "#D500F9";
    else if(theme == Themes::PurpleLight) color = "#4A148C";
    else if(theme == Themes::YellowDark) color = "#FFD600";
    else if(theme == Themes::YellowLight) color = "#7F5F00";
    else if(theme == Themes::OrangeDark) color = "#FF6D00";
    else if(theme == Themes::OrangeLight) color = "#BF360C";
    else if(theme == Themes::Default) color = "#FFFFFF";

    originalContext.replace("currentColor", color);

    QFile copyImage(QString("dataTheme/icons/").append(QFileInfo(path).fileName()).append(".svg"));
    if(copyImage.open(QFile::WriteOnly))
    {
        QTextStream(&copyImage) << originalContext;
    }
    copyImage.close();

    labelPtr->setPixmap(QPixmap(QString("dataTheme/icons/").append(QFileInfo(path).fileName()).append(".svg")));
}

void dataThemeConfigurer::setIcon(QMainWindow* mainWindowPtr, QString path)
{
    QFile originalImage(path);
    QString originalContext = "";

    if (originalImage.open(QIODevice::ReadOnly)) {
        originalContext = QString::fromUtf8(originalImage.readAll());
        originalImage.close();
    }

    originalImage.close();

    QString color = "";

    if(theme == Themes::Black) color = "#FFFFFF";
    else if(theme == Themes::White) color = "#111827";
    else if(theme == Themes::GrayDark) color = "#B0B5C4";
    else if(theme == Themes::GrayLigth) color = "#242830";
    else if(theme == Themes::RedDark) color = "#FF1744";
    else if(theme == Themes::RedLight) color = "#B71C1C";
    else if(theme == Themes::GreenDark) color = "#00FF41";
    else if(theme == Themes::GreenLight) color = "#1B5E20";
    else if(theme == Themes::BlueDark) color = "#2979FF";
    else if(theme == Themes::BlueLight) color = "#0D47A1";
    else if(theme == Themes::CyanDark) color = "#00E5FF";
    else if(theme == Themes::CyanLight) color = "#006064";
    else if(theme == Themes::PurpleDark) color = "#D500F9";
    else if(theme == Themes::PurpleLight) color = "#4A148C";
    else if(theme == Themes::YellowDark) color = "#FFD600";
    else if(theme == Themes::YellowLight) color = "#7F5F00";
    else if(theme == Themes::OrangeDark) color = "#FF6D00";
    else if(theme == Themes::OrangeLight) color = "#BF360C";
    else if(theme == Themes::Default) color = "#FFFFFF";

    originalContext.replace("currentColor", color);

    QFile copyImage(QString("dataTheme/icons/").append(QFileInfo(path).fileName()).append(".svg"));
    if(copyImage.open(QFile::WriteOnly))
    {
        QTextStream(&copyImage) << originalContext;
    }
    copyImage.close();

    mainWindowPtr->setWindowIcon(QIcon(QString("dataTheme/icons/").append(QFileInfo(path).fileName()).append(".svg")));
}
void dataThemeConfigurer::setIcon(QAction* actionPtr, QString path)
{
    QFile originalImage(path);
    QString originalContext = "";

    if (originalImage.open(QIODevice::ReadOnly)) {
        originalContext = QString::fromUtf8(originalImage.readAll());
        originalImage.close();
    }

    originalImage.close();

    QString color = "";

    if(theme == Themes::Black) color = "#FFFFFF";
    else if(theme == Themes::White) color = "#111827";
    else if(theme == Themes::GrayDark) color = "#B0B5C4";
    else if(theme == Themes::GrayLigth) color = "#242830";
    else if(theme == Themes::RedDark) color = "#FF1744";
    else if(theme == Themes::RedLight) color = "#B71C1C";
    else if(theme == Themes::GreenDark) color = "#00FF41";
    else if(theme == Themes::GreenLight) color = "#1B5E20";
    else if(theme == Themes::BlueDark) color = "#2979FF";
    else if(theme == Themes::BlueLight) color = "#0D47A1";
    else if(theme == Themes::CyanDark) color = "#00E5FF";
    else if(theme == Themes::CyanLight) color = "#006064";
    else if(theme == Themes::PurpleDark) color = "#D500F9";
    else if(theme == Themes::PurpleLight) color = "#4A148C";
    else if(theme == Themes::YellowDark) color = "#FFD600";
    else if(theme == Themes::YellowLight) color = "#7F5F00";
    else if(theme == Themes::OrangeDark) color = "#FF6D00";
    else if(theme == Themes::OrangeLight) color = "#BF360C";
    else if(theme == Themes::Default) color = "#FFFFFF";

    originalContext.replace("currentColor", color);

    QFile copyImage(QString("dataTheme/icons/").append(QFileInfo(path).fileName()).append(".svg"));
    if(copyImage.open(QFile::WriteOnly))
    {
        QTextStream(&copyImage) << originalContext;
    }
    copyImage.close();

    actionPtr->setIcon(QIcon(QString("dataTheme/icons/").append(QFileInfo(path).fileName()).append(".svg")));
}
void dataThemeConfigurer::setIcon(QString path)
{
    QFile originalImage(path);
    QString originalContext = "";

    if (originalImage.open(QIODevice::ReadOnly)) {
        originalContext = QString::fromUtf8(originalImage.readAll());
        originalImage.close();
    }

    originalImage.close();

    QString color = "";

    if(theme == Themes::Black) color = "#FFFFFF";
    else if(theme == Themes::White) color = "#111827";
    else if(theme == Themes::GrayDark) color = "#B0B5C4";
    else if(theme == Themes::GrayLigth) color = "#242830";
    else if(theme == Themes::RedDark) color = "#FF1744";
    else if(theme == Themes::RedLight) color = "#B71C1C";
    else if(theme == Themes::GreenDark) color = "#00FF41";
    else if(theme == Themes::GreenLight) color = "#1B5E20";
    else if(theme == Themes::BlueDark) color = "#2979FF";
    else if(theme == Themes::BlueLight) color = "#0D47A1";
    else if(theme == Themes::CyanDark) color = "#00E5FF";
    else if(theme == Themes::CyanLight) color = "#006064";
    else if(theme == Themes::PurpleDark) color = "#D500F9";
    else if(theme == Themes::PurpleLight) color = "#4A148C";
    else if(theme == Themes::YellowDark) color = "#FFD600";
    else if(theme == Themes::YellowLight) color = "#7F5F00";
    else if(theme == Themes::OrangeDark) color = "#FF6D00";
    else if(theme == Themes::OrangeLight) color = "#BF360C";
    else if(theme == Themes::Default) color = "#FFFFFF";

    originalContext.replace("currentColor", color);

    QFile copyImage(QString("dataTheme/icons/").append(QFileInfo(path).fileName()).append(".svg"));
    if(copyImage.open(QFile::WriteOnly))
    {
        QTextStream(&copyImage) << originalContext;
    }

    qDebug() << QFileInfo(copyImage).filePath();

    copyImage.close();
}
void dataThemeConfigurer::setIcon(QMessageBox* messageBoxPtr, QString path)
{
    QFile originalImage(path);
    QString originalContext = "";

    if (originalImage.open(QIODevice::ReadOnly)) {
        originalContext = QString::fromUtf8(originalImage.readAll());
        originalImage.close();
    }

    originalImage.close();

    QString color = "";

    if(theme == Themes::Black) color = "#FFFFFF";
    else if(theme == Themes::White) color = "#111827";
    else if(theme == Themes::GrayDark) color = "#B0B5C4";
    else if(theme == Themes::GrayLigth) color = "#242830";
    else if(theme == Themes::RedDark) color = "#FF1744";
    else if(theme == Themes::RedLight) color = "#B71C1C";
    else if(theme == Themes::GreenDark) color = "#00FF41";
    else if(theme == Themes::GreenLight) color = "#1B5E20";
    else if(theme == Themes::BlueDark) color = "#2979FF";
    else if(theme == Themes::BlueLight) color = "#0D47A1";
    else if(theme == Themes::CyanDark) color = "#00E5FF";
    else if(theme == Themes::CyanLight) color = "#006064";
    else if(theme == Themes::PurpleDark) color = "#D500F9";
    else if(theme == Themes::PurpleLight) color = "#4A148C";
    else if(theme == Themes::YellowDark) color = "#FFD600";
    else if(theme == Themes::YellowLight) color = "#7F5F00";
    else if(theme == Themes::OrangeDark) color = "#FF6D00";
    else if(theme == Themes::OrangeLight) color = "#BF360C";
    else if(theme == Themes::Default) color = "#FFFFFF";

    originalContext.replace("currentColor", color);

    QFile copyImage(QString("dataTheme/icons/").append(QFileInfo(path).fileName()).append(".svg"));
    if(copyImage.open(QFile::WriteOnly))
    {
        QTextStream(&copyImage) << originalContext;
    }
    copyImage.close();

    messageBoxPtr->setWindowIcon(QIcon(QString("dataTheme/icons/").append(QFileInfo(path).fileName()).append(".svg")));
}
void dataThemeConfigurer::setIcon(QFileDialog* fileDialogPtr, QString path)
{
    QFile originalImage(path);
    QString originalContext = "";

    if (originalImage.open(QIODevice::ReadOnly)) {
        originalContext = QString::fromUtf8(originalImage.readAll());
        originalImage.close();
    }

    originalImage.close();

    QString color = "";

    if(theme == Themes::Black) color = "#FFFFFF";
    else if(theme == Themes::White) color = "#111827";
    else if(theme == Themes::GrayDark) color = "#B0B5C4";
    else if(theme == Themes::GrayLigth) color = "#242830";
    else if(theme == Themes::RedDark) color = "#FF1744";
    else if(theme == Themes::RedLight) color = "#B71C1C";
    else if(theme == Themes::GreenDark) color = "#00FF41";
    else if(theme == Themes::GreenLight) color = "#1B5E20";
    else if(theme == Themes::BlueDark) color = "#2979FF";
    else if(theme == Themes::BlueLight) color = "#0D47A1";
    else if(theme == Themes::CyanDark) color = "#00E5FF";
    else if(theme == Themes::CyanLight) color = "#006064";
    else if(theme == Themes::PurpleDark) color = "#D500F9";
    else if(theme == Themes::PurpleLight) color = "#4A148C";
    else if(theme == Themes::YellowDark) color = "#FFD600";
    else if(theme == Themes::YellowLight) color = "#7F5F00";
    else if(theme == Themes::OrangeDark) color = "#FF6D00";
    else if(theme == Themes::OrangeLight) color = "#BF360C";
    else if(theme == Themes::Default) color = "#FFFFFF";

    originalContext.replace("currentColor", color);

    QFile copyImage(QString("dataTheme/icons/").append(QFileInfo(path).fileName()).append(".svg"));
    if(copyImage.open(QFile::WriteOnly))
    {
        QTextStream(&copyImage) << originalContext;
    }
    copyImage.close();

    fileDialogPtr->setWindowIcon(QIcon(QString("dataTheme/icons/").append(QFileInfo(path).fileName()).append(".svg")));
}