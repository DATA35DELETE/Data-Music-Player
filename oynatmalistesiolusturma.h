#ifndef OYNATMALISTESIOLUSTURMA_H
#define OYNATMALISTESIOLUSTURMA_H

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
class oynatmaListesiOlusturma;
}

class oynatmaListesiOlusturma : public QMainWindow
{
    Q_OBJECT

public:
    explicit oynatmaListesiOlusturma(QWidget *parent = nullptr);
    ~oynatmaListesiOlusturma();

    void oynatmaListesiOlusturmaBaslangic();
    void oynatmaListesiOlusturmaBitis();

public slots:
    void oynatmaListesi_fotoSec();
    void ekle_clicked();

private:
    Ui::oynatmaListesiOlusturma *ui;
};

#endif // OYNATMALISTESIOLUSTURMA_H
