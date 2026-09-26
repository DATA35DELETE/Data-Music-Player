#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QScroller>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QThread>
#include <QImageReader>
#include <QMediaMetaData>
#include <QImage>
#include <QFile>
#include <QFileDialog>
#include <QProgressDialog>
#include <QShortcut>
#include <QListWidgetItem>
#include <QLoggingCategory>
#include <QDirIterator>

#include <filesystem>
#include <vector>
#include <algorithm>
#include <thread>
#include <random>
#include <iterator>
#include <fstream>

#include "oynatmalistesiolusturma.h"
#include "oynatmalistesisilme.h"
#include "medyayuzenpencere.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

    void medyalariListele();
    void medyaCal(bool);

    void medyalariListele(std::wstring);

    void favorileriListele();
    void favorileriYaz();

    void oynatici_mediaStatusChanged_altSistem();

public slots:
    void medyaOynatmaKontrol_clicked();
    void medyaSlider_positionChanged(qint64);
    void medyaSlider_sliderMoved(int);
    void medyaSlider_actionTriggered(int);
    void medyaSesSeviyesi_sliderMoved(int);
    void medyaMod_clicked();
    void oynatici_mediaStatusChanged(QMediaPlayer::MediaStatus);
    void medyaArama_textChanged(QString);
    void medyalar_itemActivated(QListWidgetItem*);
    void oynatmaListeleri_clicked();
    void actionDosya_Ekle_triggered(bool);

private:
    Ui::MainWindow *ui;
    oynatmaListesiOlusturma *oynatmaListesiCreate;
    oynatmaListesiSilme *oynatmaListesiDelete;
    medyaYuzenPencere *yuzenPencere;
};
#endif // MAINWINDOW_H
