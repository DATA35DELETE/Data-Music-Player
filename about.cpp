#include "about.h"
#include "ui_about.h"

about::about(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::about)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::WindowType::SubWindow | Qt::WindowType::WindowStaysOnTopHint);

    close = ui->appClose;
    icon = ui->appIcon;
}

about::~about()
{
    delete ui;
}
