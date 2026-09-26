#include "muzikcikar.h"
#include "ui_muzikcikar.h"

muzikCikar::muzikCikar(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::muzikCikar)
{
    ui->setupUi(this);
}

muzikCikar::~muzikCikar()
{
    delete ui;
}
