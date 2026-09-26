#ifndef MUZIKEKLE_H
#define MUZIKEKLE_H

#include <QMainWindow>

namespace Ui {
class muzikEkle;
}

class muzikEkle : public QMainWindow
{
    Q_OBJECT

public:
    explicit muzikEkle(QWidget *parent = nullptr);
    ~muzikEkle();

private:
    Ui::muzikEkle *ui;
};

#endif // MUZIKEKLE_H
