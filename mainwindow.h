#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QShortcut>
#include <QKeySequence>
#include <QDir>
#include <QFile>
#include <QStandardPaths>
#include <QImageReader>
#include <QListWidgetItem>
#include <QHBoxLayout>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QPainter>
#include <QPainterPath>
#include <QMediaMetaData>
#include <QtConcurrent>
#include <QSettings>
#include <QFileDialog>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QDesktopServices>
#include <QDialog>
#include <QKeySequence>

#include <qhotkey.h>

#include <taglib/mpegfile.h>
#include <taglib/id3v2tag.h>
#include <taglib/attachedpictureframe.h>

#include "musiccard.h"
#include "musicremove.h"
#include "playlistadd.h"
#include "about.h"

#ifdef WIN32

#define fileSeparator "\\"
#define ffmpegVersionNumber "ffmpeg-n9.0-latest-win64-lgpl-9.0"
#define TAGLIB_FILENAME(path) ((path).toStdWString().c_str())

#else

#define fileSeparator "/"
#define ffmpegVersionNumber "ffmpeg-n9.0-latest-linux64-lgpl-9.0"
#define TAGLIB_FILENAME(path) ((path).toUtf8().constData())

#endif

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    QNetworkAccessManager *mngr;

    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

    void iconsUpdate();
    void connectors();
    void musicsRead();
    void playlistsRead();
    void readPlaylist(QString);
    void allMusicRead();

    void readFaw();
    void writeFaw();

    void nextMusic(bool);

    void settingsLoad();

    void medyaIndirme(QString);


    Ui::MainWindow *ui;
    musicCard *card;
    musicremove *remove;
    playlistadd *playlistAdd;
    about *aboutWin;

public slots:
    void player_mediaStatusChanged(QMediaPlayer::MediaStatus);
    void player_positionChanged(qint64);
    void player_playingChanged(bool);

    void musicControl_clicked();
    void musicsVolumeButton_clicked();
    void musicVolume_actionTriggered(int);
    void musicMod_clicked();

    void musicProgress_sliderMoved(int);
    void musicProgress_actionTriggered(int);

    void settingsSave_clicked();
    void musicsPathAddingButton_clicked();

    void ytDlpIndirme_clicked();
    void ytDlpIndirme_clicked2();

    void ffmpegIndirme_clicked();

    //void ayarlariKaydet_clicked();
};
#endif // MAINWINDOW_H
