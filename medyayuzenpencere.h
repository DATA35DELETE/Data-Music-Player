#ifndef MEDYAYUZENPENCERE_H
#define MEDYAYUZENPENCERE_H

#include <QMainWindow>
#include <QPushButton>
#include <QProgressBar>
#include <QLabel>

namespace Ui {
class medyaYuzenPencere;
}

class medyaYuzenPencere : public QMainWindow
{
    Q_OBJECT

public:
    explicit medyaYuzenPencere(QWidget *parent = nullptr);
    ~medyaYuzenPencere();

    QPushButton *medyaYuzenOynatma;
    QPushButton *medyaYuzenKapat;
    QProgressBar *medyaYuzenBar;
    QLabel *medyaYuzenIco;
    QLabel *medyaYuzenIsim;
    QLabel *medyaYuzenSanatci;

    QPushButton *medyaYuzenOnceki;
    QPushButton *medyaYuzenSonraki;

    void medyaYuzen_baslangic();

private:
    Ui::medyaYuzenPencere *ui;
};

#endif // MEDYAYUZENPENCERE_H
