#ifndef MUSICCARD_H
#define MUSICCARD_H

#include <QMainWindow>
#include <QLabel>
#include <QProgressBar>
#include <QPushButton>
#include <QCloseEvent>

namespace Ui {
class musicCard;
}

class musicCard : public QMainWindow
{
    Q_OBJECT

public:
    explicit musicCard(QWidget *parent = nullptr);
    ~musicCard();

    QLabel *musicCardThumbnails;
    QPushButton *musicCardAfter;
    QPushButton *musicCardControl;
    QPushButton *musicCardBefore;
    QPushButton *musicCardClose;
    QProgressBar *musicCardProgress;

    void start();

    void closeEvent(QCloseEvent*) override;

public slots:
    void end();

private:
    Ui::musicCard *ui;
};

#endif // MUSICCARD_H
