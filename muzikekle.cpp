#include "muzikekle.h"
#include "ui_muzikekle.h"

muzikEkle::muzikEkle(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::muzikEkle)
{
    ui->setupUi(this);
}

muzikEkle::~muzikEkle()
{
    delete ui;
}
