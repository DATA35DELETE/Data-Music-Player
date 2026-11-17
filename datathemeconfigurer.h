#ifndef DATATHEMECONFIGURER_H
#define DATATHEMECONFIGURER_H

#include <QApplication>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QDir>
#include <QFile>
#include <QDataStream>
#include <QFileInfo>
#include <QPushButton>
#include <QLabel>
#include <QObject>
#include <QProcess>
#include <QMainWindow>
#include <QMessageBox>
#include <QFileDialog>

enum Themes{
    Black,       // #FFFFFF
    White,       // #111827
    GrayDark,    // #B0B5C4
    GrayLigth,   // #242830
    RedDark,     // #FF1744
    RedLight,    // #B71C1C
    GreenDark,   // #00FF41
    GreenLight,  // #1B5E20
    BlueDark,    // #2979FF
    BlueLight,   // #0D47A1
    CyanDark,    // #00E5FF
    CyanLight,   // #006064
    PurpleDark,  // #D500F9
    PurpleLight, // #4A148C
    YellowDark,  // #FFD600
    YellowLight, // #7F5F00
    OrangeDark,  // #FF6D00
    OrangeLight, // #BF360C
};

class dataThemeConfigurer : public QObject
{
    Q_OBJECT

private:
    Themes theme;

public:
    //dataThemeConfigurer(Themes, QApplication*);
    dataThemeConfigurer();

    void setTheme(Themes);
    Themes getTheme();

    void setApplicationTheme(QApplication*);
    void setApplicationTheme(QApplication*, Themes);

    void setIcon(QPushButton*, QString);
    void setIcon(QLabel*, QString);
    void setIcon(QMainWindow*, QString);
    void setIcon(QMessageBox*, QString);
    void setIcon(QFileDialog*, QString);
    void setIcon(QAction*, QString);
    void setIcon(QString);

Q_SIGNALS:
    void initFinished();
};

#endif // DATATHEMECONFIGURER_H
