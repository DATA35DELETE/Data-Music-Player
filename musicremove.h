#ifndef MUSICREMOVE_H
#define MUSICREMOVE_H

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QStringList>
#include <QDir>
#include <QCheckBox>

namespace Ui {
class musicremove;
}

class musicremove : public QMainWindow
{
    Q_OBJECT

public:
    explicit musicremove(QWidget *parent = nullptr);
    ~musicremove();

    QPushButton *closeButton;
    QPushButton *remove;

    QLabel *removeIcon;

    void readMusicPath();

    void start(QStringList);
    void end(QStringList);

public slots:

    void musicPathList_activated(int);
    void musicRemoveSearch_textChanged(const QString&);

private:
    Ui::musicremove *ui;
};

#endif // MUSICREMOVE_H
