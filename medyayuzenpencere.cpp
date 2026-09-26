#include "medyayuzenpencere.h"
#include "ui_medyayuzenpencere.h"

medyaYuzenPencere::medyaYuzenPencere(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::medyaYuzenPencere)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::WindowType::CoverWindow | Qt::WindowType::WindowStaysOnTopHint);

    medyaYuzenOynatma = ui->medyaYuzenOynatma;
    medyaYuzenKapat = ui->medyaYuzenKapat;
    medyaYuzenBar = ui->medyaYuzenBar;
    medyaYuzenIco = ui->medyaYuzenIco;
    medyaYuzenIsim = ui->medyaYuzenIsim;
    medyaYuzenSanatci = ui->medyaYuzenSanatci;
    medyaYuzenOnceki = ui->medyaYuzenOnceki;
    medyaYuzenSonraki = ui->medyaYuzenSonraki;

}

medyaYuzenPencere::~medyaYuzenPencere()
{
    delete ui;
}

void medyaYuzenPencere::medyaYuzen_baslangic()
{
    this->setVisible(true);
}