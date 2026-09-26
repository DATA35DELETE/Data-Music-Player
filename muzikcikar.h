#ifndef MUZIKCIKAR_H
#define MUZIKCIKAR_H

#include <QMainWindow>

namespace Ui {
class muzikCikar;
}

class muzikCikar : public QMainWindow
{
    Q_OBJECT

public:
    explicit muzikCikar(QWidget *parent = nullptr);
    ~muzikCikar();

private:
    Ui::muzikCikar *ui;
};

#endif // MUZIKCIKAR_H
