#ifndef OYNATMALISTESISILME_H
#define OYNATMALISTESISILME_H

#include <QMainWindow>
#include <QFileDialog>
#include <QCheckBox>
#include <QMessageBox>
#include <QImage>
#include <QImageReader>

#include <filesystem>
#include <algorithm>
#include <fstream>

namespace Ui {
class oynatmaListesiSilme;
}

class oynatmaListesiSilme : public QMainWindow
{
    Q_OBJECT

public:
    explicit oynatmaListesiSilme(QWidget *parent = nullptr);
    ~oynatmaListesiSilme();

    void oynatmaListesiSilmeBitis();
    void oynatmaListesiSilmeBaslangic();

public slots:
    void sil_clicked();

private:
    Ui::oynatmaListesiSilme *ui;
};

#endif // OYNATMALISTESISILME_H
