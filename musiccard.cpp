#include "musiccard.h"
#include "ui_musiccard.h"

musicCard::musicCard(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::musicCard)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::WindowType::SubWindow | Qt::WindowType::WindowStaysOnTopHint);

    musicCardThumbnails = ui->musicCardThumbnails;

    musicCardAfter = ui->musicCardAfter;
    musicCardBefore = ui->musicCardBefore;
    musicCardControl = ui->musicCardControl;
    musicCardClose = ui->musicCardClose;

    musicCardProgress = ui->musicCardProgress;

    connect(musicCardClose, &QPushButton::clicked, this, &musicCard::end);
}

musicCard::~musicCard()
{
    delete ui;
}

void musicCard::closeEvent(QCloseEvent* event)
{
    event->accept();

    end();
}
