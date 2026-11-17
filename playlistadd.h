#ifndef PLAYLISTADD_H
#define PLAYLISTADD_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QDir>
#include <QCheckBox>
#include <QFileDialog>
#include <QImageReader>
#include <QMessageBox>

namespace Ui {
class playlistadd;
}

class playlistadd : public QMainWindow
{
    Q_OBJECT

public:
    QLabel *searchIcon;
    QPushButton *closeButton;
    QPushButton *playlistIconButton;

    explicit playlistadd(QWidget *parent = nullptr);
    ~playlistadd();

    void start(QStringList);
    void end();

public slots:
    void playlistAddMusic_textChanged(const QString&);
    void playlistAddIcon_clicked();
    void playlistAddButton_clicked();

private:
    Ui::playlistadd *ui;
};

#endif // PLAYLISTADD_H
