#ifndef ABOUT_H
#define ABOUT_H

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>

namespace Ui {
class about;
}

class about : public QMainWindow
{
    Q_OBJECT

public:

    QPushButton *close;
    QLabel *icon;

    explicit about(QWidget *parent = nullptr);
    ~about();



private:
    Ui::about *ui;
};

#endif // ABOUT_H
