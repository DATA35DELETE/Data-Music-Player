#include "mainwindow.h"
#include "datathemeconfigurer.h"
#include "ui_mainwindow.h"

dataThemeConfigurer *configurer = nullptr;

QStringList muzikYollari;
QStringList playlists;

QStringList playlistsMusics;
QStringList musics;
QStringList favoriler;

int iMusic = 0;
int mod = 0;
// 0: Loops
// 1: Repeat
// 2: Shuffle

bool userControl = false;
bool isPlaylists = false;
bool isMusicCard = false;

QMediaPlayer *player;
QAudioOutput *output;

MainWindow *mainWindow;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    configurer = new dataThemeConfigurer();
    card = new musicCard;
    remove = new musicremove;
    playlistAdd = new playlistadd;
    aboutWin = new about;

    mainWindow = this;

    player = new QMediaPlayer(this);

    output = new QAudioOutput(this);
    output->setVolume((float)ui->musicVolume->value() / 100.0f);

    player->setAudioOutput(output);

    QDir().mkdir("../thumbnails");
    QDir().mkdir("../playlists");
    QDir().mkdir("../thumbnailsPlaylists");
    QDir().mkdir("../ffmpeg");
    QDir().mkdir("../yt-dlp");
    QDir().mkdir("../downloads");

    QFile fawFiles("../playlists/Favoriler.txt");
    if(!fawFiles.exists())
        if(fawFiles.open(QFile::OpenModeFlag::WriteOnly));
    fawFiles.close();

    QFile muzikYollariFile("../musicPath.sett");
    if(!muzikYollariFile.exists())
    {
        if(muzikYollariFile.open(QFile::OpenModeFlag::WriteOnly))
        {
            QTextStream(&muzikYollariFile) <<                                             QStandardPaths::writableLocation(QStandardPaths::MusicLocation) << "\n" << QString::fromStdString(QDir("../downloads").absolutePath().toStdString());
        }
    }
    muzikYollariFile.close();

    musicsRead();
    playlistsRead();
    readFaw();

    connectors();
}

void MainWindow::iconsUpdate()
{
    configurer->setIcon(ui->musicControl, QString::fromUtf8(":/assets/icons/pause.svg"));
    configurer->setIcon(ui->musicAfter, QString::fromUtf8(":/assets/icons/skip-forward.svg"));
    configurer->setIcon(ui->musicBefore, QString::fromUtf8(":/assets/icons/skip-back.svg"));
    configurer->setIcon(ui->musicMod, QString::fromUtf8(":/assets/icons/repeat.svg"));
    configurer->setIcon(ui->musicVolumeButton, QString::fromUtf8(":/assets/icons/volume-2.svg"));
    configurer->setIcon(this, QString::fromUtf8(":/assets/icons/nota.svg"));
    if(player->isAvailable())
        if(player->metaData().value(QMediaMetaData::CoverArtImage).isNull())
        {
            configurer->setIcon(ui->musicsThumbnails, QString::fromUtf8(":/assets/icons/nota.svg"));
            configurer->setIcon(card->musicCardThumbnails, QString::fromUtf8(":/assets/icons/nota.svg"));
        }
    configurer->setIcon(ui->searcIcon, QString::fromUtf8(":/assets/icons/search.svg"));
    configurer->setIcon(ui->musicCardButton, QString::fromUtf8(":/assets/icons/airplay.svg"));
    configurer->setIcon(ui->label_4, QString::fromUtf8(":/assets/icons/search.svg"));
    configurer->setIcon(ui->musicReloader, QString::fromUtf8(":/assets/icons/reset.svg"));
    configurer->setIcon(ui->playlistReloader, QString::fromUtf8(":/assets/icons/reset.svg"));
    configurer->setIcon(card->musicCardControl, QString::fromUtf8(":/assets/icons/pause.svg"));
    configurer->setIcon(card->musicCardAfter, QString::fromUtf8(":/assets/icons/skip-forward.svg"));
    configurer->setIcon(card->musicCardBefore, QString::fromUtf8(":/assets/icons/skip-back.svg"));
    configurer->setIcon(card->musicCardClose, QString::fromUtf8(":/assets/icons/x.svg"));
    configurer->setIcon(card, QString::fromUtf8(":/assets/icons/musicCard.svg"));
    if(ui->ytDlpTools->text().contains("Yt-Dlp Aracını İndir"))
        configurer->setIcon(ui->ytDlpTools, QString::fromUtf8(":/assets/icons/download.svg"));
    else
        configurer->setIcon(ui->ytDlpTools, QString::fromUtf8(":/assets/icons/reset.svg"));
    configurer->setIcon(ui->ffmpegTools, QString::fromUtf8(":/assets/icons/download.svg"));
    configurer->setIcon(ui->nodejsTools, QString::fromUtf8(":/assets/icons/download.svg"));
    configurer->setIcon(ui->downloadButton, QString::fromUtf8(":/assets/icons/download.svg"));
    configurer->setIcon(remove->closeButton, QString::fromUtf8(":/assets/icons/x.svg"));
    configurer->setIcon(remove->removeIcon, QString::fromUtf8(":/assets/icons/search.svg"));
    configurer->setIcon(playlistAdd->closeButton, QString::fromUtf8(":/assets/icons/x.svg"));
    configurer->setIcon(playlistAdd->searchIcon, QString::fromUtf8(":/assets/icons/search.svg"));
    if(playlistAdd->toolTip().isEmpty())
        configurer->setIcon(playlistAdd->playlistIconButton, QString::fromUtf8(":/assets/icons/nota.svg"));
    configurer->setIcon(aboutWin->close, QString::fromUtf8(":/assets/icons/x.svg"));
    configurer->setIcon(aboutWin->icon, QString::fromUtf8(":/assets/icons/nota.svg"));
}

void MainWindow::connectors()
{
    connect(configurer, &dataThemeConfigurer::initFinished, this,
            [=]()
            {
                settingsLoad();

                iconsUpdate();

                this->show();

                this->activateWindow();
            });

    connect(new QShortcut(QKeySequence("Alt+M"), ui->centralwidget), &QShortcut::activated, this,
            [=](){
                if(!ui->menubar->isVisible())
                    ui->menubar->setVisible(true);
                else
                    ui->menubar->setVisible(false);
            });

    /* Menubar Tema */
    connect(ui->actionSiyah, &QAction::triggered, this,
            [=](){
                configurer->setApplicationTheme(qApp, Themes::Black);
                configurer->setTheme(Themes::Black);

                QSettings settFile("./config.ini", QSettings::IniFormat);
                settFile.setValue("general/theme", "black");

                settingsLoad();

                iconsUpdate();
            });
    connect(ui->actionByeaz, &QAction::triggered, this,
    [=](){
        configurer->setApplicationTheme(qApp, Themes::White);
        configurer->setTheme(Themes::White);

        QSettings settFile("./config.ini", QSettings::IniFormat);
        settFile.setValue("general/theme", "white");

        settingsLoad();

        iconsUpdate();
    });
    connect(ui->actionGri_Karanl_k, &QAction::triggered, this,
            [=](){
                configurer->setApplicationTheme(qApp, Themes::GrayDark);
                configurer->setTheme(Themes::GrayDark);

                QSettings settFile("./config.ini", QSettings::IniFormat);
                settFile.setValue("general/theme", "grayDark");

                settingsLoad();

                iconsUpdate();
            });
    connect(ui->actionGri_Ayd_nl_k, &QAction::triggered, this,
            [=](){
                configurer->setApplicationTheme(qApp, Themes::GrayLigth);
                configurer->setTheme(Themes::GrayLigth);

                QSettings settFile("./config.ini", QSettings::IniFormat);
                settFile.setValue("general/theme", "grayLight");

                settingsLoad();

                iconsUpdate();
            });
    connect(ui->actionK_rm_z_Karanl_k, &QAction::triggered, this,
            [=](){
                configurer->setApplicationTheme(qApp, Themes::RedDark);
                configurer->setTheme(Themes::RedDark);

                QSettings settFile("./config.ini", QSettings::IniFormat);
                settFile.setValue("general/theme", "redDark");

                settingsLoad();

                iconsUpdate();
            });
    connect(ui->actionK_rm_z_Ayd_nl_k, &QAction::triggered, this,
            [=](){
                configurer->setApplicationTheme(qApp, Themes::RedLight);
                configurer->setTheme(Themes::RedLight);

                QSettings settFile("./config.ini", QSettings::IniFormat);
                settFile.setValue("general/theme", "redLight");

                settingsLoad();

                iconsUpdate();
            });
    connect(ui->actionYe_il_Karanl_k, &QAction::triggered, this,
            [=](){
                configurer->setApplicationTheme(qApp, Themes::GreenDark);
                configurer->setTheme(Themes::GreenDark);

                QSettings settFile("./config.ini", QSettings::IniFormat);
                settFile.setValue("general/theme", "greenDark");

                settingsLoad();

                iconsUpdate();
            });
    connect(ui->actionYe_il_Ayd_nl_k, &QAction::triggered, this,
            [=](){
                configurer->setApplicationTheme(qApp, Themes::GreenLight);
                configurer->setTheme(Themes::GreenLight);

                QSettings settFile("./config.ini", QSettings::IniFormat);
                settFile.setValue("general/theme", "greenLight");

                settingsLoad();

                iconsUpdate();
            });
    connect(ui->actionMavi_Ayd_nl_k, &QAction::triggered, this,
            [=](){
                configurer->setApplicationTheme(qApp, Themes::BlueLight);
                configurer->setTheme(Themes::BlueLight);

                QSettings settFile("./config.ini", QSettings::IniFormat);
                settFile.setValue("general/theme", "blueDark");

                settingsLoad();

                iconsUpdate();
            });
    connect(ui->actionMavi_Karanl_k, &QAction::triggered, this,
            [=](){
                configurer->setApplicationTheme(qApp, Themes::BlueDark);
                configurer->setTheme(Themes::BlueDark);

                QSettings settFile("./config.ini", QSettings::IniFormat);
                settFile.setValue("general/theme", "blueLight");

                settingsLoad();

                iconsUpdate();
            });
    connect(ui->actionCamg_be_i_Karanl_k, &QAction::triggered, this,
            [=](){
                configurer->setApplicationTheme(qApp, Themes::CyanDark);
                configurer->setTheme(Themes::CyanLight);

                QSettings settFile("./config.ini", QSettings::IniFormat);
                settFile.setValue("general/theme", "cyanDark");

                settingsLoad();

                iconsUpdate();
            });
    connect(ui->actionCamg_be_i_Ayd_nl_k, &QAction::triggered, this,
            [=](){
                configurer->setApplicationTheme(qApp, Themes::CyanLight);
                configurer->setTheme(Themes::CyanLight);

                QSettings settFile("./config.ini", QSettings::IniFormat);
                settFile.setValue("general/theme", "cyanLight");

                settingsLoad();

                iconsUpdate();
            });
    connect(ui->actionTuruncu_Karanl_k, &QAction::triggered, this,
            [=](){
                configurer->setApplicationTheme(qApp, Themes::OrangeDark);
                configurer->setTheme(Themes::OrangeDark);

                QSettings settFile("./config.ini", QSettings::IniFormat);
                settFile.setValue("general/theme", "orangeDark");

                settingsLoad();

                iconsUpdate();
            });
    connect(ui->actionTuruncu_Ayd_nl_k, &QAction::triggered, this,
            [=](){
                configurer->setApplicationTheme(qApp, Themes::OrangeLight);
                configurer->setTheme(Themes::OrangeLight);

                QSettings settFile("./config.ini", QSettings::IniFormat);
                settFile.setValue("general/theme", "orangeLight");


                settingsLoad();
                iconsUpdate();
            });
    connect(ui->actionSar_Karanl_k, &QAction::triggered, this,
            [=](){
                configurer->setApplicationTheme(qApp, Themes::YellowDark);
                configurer->setTheme(Themes::YellowDark);

                QSettings settFile("./config.ini", QSettings::IniFormat);
                settFile.setValue("general/theme", "yellowDark");

                settingsLoad();

                iconsUpdate();
            });
    connect(ui->actionSar_Ayd_nl_k, &QAction::triggered, this,
            [=](){
                configurer->setApplicationTheme(qApp, Themes::YellowLight);
                configurer->setTheme(Themes::YellowLight);

                QSettings settFile("./config.ini", QSettings::IniFormat);
                settFile.setValue("general/theme", "yellowLight");

                settingsLoad();

                iconsUpdate();
            });
    connect(ui->actionMor_Karanl_k, &QAction::triggered, this,
            [=](){
                configurer->setApplicationTheme(qApp, Themes::PurpleDark);
                configurer->setTheme(Themes::PurpleDark);

                QSettings settFile("./config.ini", QSettings::IniFormat);
                settFile.setValue("general/theme", "purpleDark");

                settingsLoad();

                iconsUpdate();
            });
    connect(ui->actionMor_Ayd_nl_k, &QAction::triggered, this,
            [=](){
                configurer->setApplicationTheme(qApp, Themes::PurpleLight);
                configurer->setTheme(Themes::PurpleLight);

                QSettings settFile("./config.ini", QSettings::IniFormat);
                settFile.setValue("general/theme", "purpleLight");

                settingsLoad();

                iconsUpdate();
            });
    connect(ui->actionGiri, &QAction::triggered, this,
            [=](){
                ui->pages->setCurrentWidget(ui->homePage);
            });
    connect(ui->actionT_m_ark_lar, &QAction::triggered, this,
            [=](){
                isPlaylists = false;
                player->pause();
                iMusic = 0;

                allMusicRead();

                ui->pages->setCurrentWidget(ui->allMusicsPage);
            });


    connect(ui->action_alma_Listeleri, &QAction::triggered, this,
            [=](){
                isPlaylists = true;
                player->pause();
                iMusic = 0;

                ui->pages->setCurrentWidget(ui->playlists);

                playlistsRead();
            });

    connect(ui->actionTekli_Ekle, &QAction::triggered, this,
            [=](){
                QFileDialog fd;
                fd.setAcceptMode(QFileDialog::AcceptOpen);
                fd.setNameFilter(QString("Müzik Dosyaları (*.mp3)"));

                fd.exec();

                if(!fd.selectedFiles().isEmpty())
                {
                    QDialog d;
                    d.setWindowIconText("Müzik Ekleme");
                    QVBoxLayout boxLayout;

                    QLabel label;
                    label.setText("Eklenicek olan müzik dosyasının hangi müzik yoluna kayıt edilceğini seçiniz.");
                    boxLayout.addWidget(&label);

                    QComboBox comboBox;
                    for (auto i : muzikYollari) {
                        comboBox.addItem(i);
                    }
                    boxLayout.addWidget(&comboBox);
                    d.setLayout(&boxLayout);

                    QPushButton button;
                    button.setText("Ekle");
                    connect(&button, &QPushButton::clicked, this,
                    [=, &fd, &comboBox, &d](){
                        for (auto i : fd.selectedFiles()) {
                            qDebug() << QFile::copy(i, comboBox.currentText().append("/").append(QFileInfo(i).fileName()));
                        }

                        QMessageBox(QMessageBox::Icon::Information, "Başarılı!", "Seçilen müzikler eklendi.").exec();

                        isPlaylists = false;
                        player->pause();
                        iMusic = 0;

                        musicsRead();
                        allMusicRead();

                        ui->pages->setCurrentWidget(ui->allMusicsPage);

                        d.close();
                    });
                    boxLayout.addWidget(&button);

                    d.exec();
                }
                else
                {
                    QMessageBox(QMessageBox::Warning, "Müzik Seçilmedi", "Hiç bir müzik dosyası seçilmedi.").exec();
                }
            });
    connect(ui->actionTekli_kar, &QAction::triggered, this,
    [=](){
        remove->start(muzikYollari);
    });
    connect(ui->actionYolu_kar2, &QAction::triggered, this,
            [=](){
                QDialog d;
                d.setWindowIconText("Müzik Yolu Silme");
                QVBoxLayout boxLayout;

                QLabel label;
                label.setText("Silinecek olan müzik yolunu seçiniz.");
                boxLayout.addWidget(&label);

                QComboBox comboBox;
                for (auto i : muzikYollari) {
                    comboBox.addItem(i);
                }
                boxLayout.addWidget(&comboBox);
                d.setLayout(&boxLayout);

                QPushButton button;
                button.setText("Sil");
                connect(&button, &QPushButton::clicked, this,
                        [=, &comboBox, &d](){
                            QFile musicsPath("../musicPath.sett");
                            if(musicsPath.open(QFile::OpenModeFlag::ReadWrite))
                            {
                                QTextStream stream(&musicsPath);
                                QStringList context;

                                while(!stream.atEnd())
                                {
                                    context.append(stream.readLine());
                                }

                                context.remove(comboBox.currentIndex());

                                stream.seek(0);

                                for (auto i : context) {
                                    stream << i << '\n';
                                }
                            }

                            settingsLoad();

                            musicsRead();
                            allMusicRead();

                            iconsUpdate();
                        });
                boxLayout.addWidget(&button);

                d.exec();
            });

    connect(ui->actionEkle, &QAction::triggered, this, [=](){playlistAdd->start(muzikYollari);});

    connect(ui->actionKald_r, &QAction::triggered, this,
    [=](){
                QDialog d;
                d.setWindowIconText("Oynatma Listesi Silme");
                QVBoxLayout boxLayout;

                QLabel label;
                label.setText("Silinecek olan oynatma listesini seçiniz.");
                boxLayout.addWidget(&label);

                QComboBox comboBox;
                for (auto i : playlists) {
                    if(i.contains("Favoriler.txt"))
                        continue;

                    comboBox.addItem(QString(i).remove(".txt"));
                }
                boxLayout.addWidget(&comboBox);
                d.setLayout(&boxLayout);

                QPushButton button;
                button.setText("Sil");
                connect(&button, &QPushButton::clicked, this,
                        [=, &comboBox, &d](){
                            QFile::remove(QString(comboBox.currentText()).append(".txt").prepend("../playlists/"));

                            QFile::remove(QString(comboBox.currentText()).append(".jpeg").prepend("../thumbnailsPlaylists/"));

                            d.setVisible(false);

                            QMessageBox(QMessageBox::Icon::Information, "Başarılı!", "Başarılı bir şekilde oynatma listesi silindi.").exec();

                            mainWindow->settingsLoad();

                            mainWindow->musicsRead();
                            mainWindow->allMusicRead();

                            mainWindow->iconsUpdate();
                        });
                boxLayout.addWidget(&button);

                d.exec();
    });
    connect(ui->actionS_r_m, &QAction::triggered, this,
    [=](){
        aboutWin->setVisible(true);
    });
    connect(aboutWin->close, &QPushButton::clicked, this,
            [=](){
                aboutWin->setVisible(false);
            });


    /* Menubar Tema */

    connect(ui->homeButton, &QPushButton::clicked, this,
    [=](){
        ui->pages->setCurrentWidget(ui->homePage);
    });
    connect(ui->musicsDownloadButton, &QPushButton::clicked, this,
    [=](){
        ui->pages->setCurrentWidget(ui->musicDonwloadPage);
    });
    connect(ui->allMusicsButton, &QPushButton::clicked, this,
    [=](){
        isPlaylists = false;
        player->pause();
        iMusic = 0;

        allMusicRead();

        ui->pages->setCurrentWidget(ui->allMusicsPage);
    });
    connect(ui->playlistButton, &QPushButton::clicked, this,
    [=](){
        isPlaylists = true;
        player->pause();
        iMusic = 0;

        ui->pages->setCurrentWidget(ui->playlists);

        playlistsRead();
    });
    connect(ui->settingsButton, &QPushButton::clicked, this,
    [=](){
        settingsLoad();

        ui->pages->setCurrentWidget(ui->settingsPage);
    });

    connect(player, &QMediaPlayer::mediaStatusChanged, this, &MainWindow::player_mediaStatusChanged);
    connect(player, &QMediaPlayer::positionChanged, this, &MainWindow::player_positionChanged);
    connect(player, &QMediaPlayer::playingChanged, this, &MainWindow::player_playingChanged);

    connect(ui->musicControl, &QPushButton::clicked, this, &MainWindow::musicControl_clicked);
    connect(ui->musicVolumeButton, &QPushButton::clicked, this, &MainWindow::musicsVolumeButton_clicked);
    connect(ui->musicVolume, &QSlider::actionTriggered, this, &MainWindow::musicVolume_actionTriggered);
    connect(ui->musicMod, &QPushButton::clicked, this, &MainWindow::musicMod_clicked);

    connect(ui->musicProgres, &QSlider::sliderMoved, this, &MainWindow::musicProgress_sliderMoved);
    connect(ui->musicProgres, &QSlider::actionTriggered, this, &MainWindow::musicProgress_actionTriggered);

    connect(ui->musicAfter, &QPushButton::clicked, this,
    [=]()
    {
        if(mod == 1)
        {
            mod = 0;

            nextMusic(true);

            mod = 1;
        }
        else
            nextMusic(true);
    });
    connect(ui->musicBefore, &QPushButton::clicked, this,
    [=]()
    {
        if(mod == 1)
        {
            mod = 0;

            nextMusic(false);

            mod = 1;
        }
        else
            nextMusic(false);
    });
    connect(card->musicCardAfter, &QPushButton::clicked, this,
            [=]()
            {
                if(mod == 1)
                {
                    mod = 0;

                    nextMusic(true);

                    mod = 1;
                }
                else
                    nextMusic(true);
            });
    connect(card->musicCardBefore, &QPushButton::clicked, this,
            [=]()
            {
                if(mod == 1)
                {
                    mod = 0;

                    nextMusic(false);

                    mod = 1;
                }
                else
                    nextMusic(false);
            });
    connect(card->musicCardControl, &QPushButton::clicked, this, &MainWindow::musicControl_clicked);

    connect(ui->allMusicsList, &QListWidget::currentRowChanged, this,
    [=](int i){

        if(i < 0)
            return;

        iMusic = i;

        player->stop();

        player->setSource(QUrl::fromLocalFile(musics.at(iMusic)));

        player->play();
    });
    connect(ui->listWidget, &QListWidget::currentRowChanged, this,
    [=](int i){

        if(i < 0)
            return;

        iMusic = i;

        player->stop();

        player->setSource(QUrl::fromLocalFile(playlistsMusics.at(iMusic)));

        player->play();
    });
    connect(ui->musicsPathList, &QListWidget::currentRowChanged, this,
            [=](int i){
                if(i < 0)
                    return;

                QFile musicsPath("../musicPath.sett");
                if(musicsPath.open(QFile::OpenModeFlag::ReadWrite))
                {
                    QTextStream stream(&musicsPath);
                    QStringList context;

                    while(!stream.atEnd())
                    {
                        context.append(stream.readLine());
                    }

                    context.remove(i);

                    stream.seek(0);

                    for (auto i : context) {
                        stream << i << '\n';
                    }
                }

                settingsLoad();

                musicsRead();
                allMusicRead();

                iconsUpdate();
            });
    connect(ui->playlistsList, &QListWidget::currentRowChanged, this,
    [=](int i){

        if(i < 0)
            return;

        isPlaylists = true;
        player->pause();
        iMusic = 0;

        readPlaylist(QString(ui->playlistsList->itemWidget(ui->playlistsList->item(i))->toolTip()).prepend("../playlists/"));

        allMusicRead();

        ui->playlistIcon->setPixmap(ui->playlistsList->itemWidget(ui->playlistsList->item(i))->findChild<QLabel*>("iconLabel")->pixmap());
        ui->playlistName->setText(ui->playlistsList->itemWidget(ui->playlistsList->item(i))->findChild<QLabel*>("nameLabel")->text());
        ui->playlistMusicCount->setText(QString::number(playlistsMusics.count()).append(" Şarkı"));

        ui->pages->setCurrentWidget(ui->playlistsPage);
    });

    connect(ui->musicsSearcher, &QLineEdit::textChanged, this,
            [=](const QString &text){
                QListWidgetItem *item;

        for (int i = 0; i < ui->allMusicsList->count(); ++i) {
            item = ui->allMusicsList->item(i);

            item->setHidden(false);
        }

        for (int i = 0; i < ui->allMusicsList->count(); ++i) {
            item = ui->allMusicsList->item(i);

            if(ui->allMusicsList->itemWidget(item)->toolTip().contains(text, Qt::CaseInsensitive))
                item->setHidden(false);
            else
                item->setHidden(true);
        }
    });
    connect(ui->playlistSearcher, &QLineEdit::textChanged, this,
            [=](const QString &text){
                QListWidgetItem *item;

                for (int i = 0; i < ui->listWidget->count(); ++i) {
                    item = ui->listWidget->item(i);

                    item->setHidden(false);
                }

                for (int i = 0; i < ui->listWidget->count(); ++i) {
                    item = ui->listWidget->item(i);

                    if(ui->listWidget->itemWidget(item)->toolTip().contains(text, Qt::CaseInsensitive))
                        item->setHidden(false);
                    else
                        item->setHidden(true);
                }
            });


    connect(ui->musicReloader, &QPushButton::clicked, this,
    [=](){
        isPlaylists = false;
        player->pause();
        iMusic = 0;

        allMusicRead();
    });
    connect(ui->playlistReloader, &QPushButton::clicked, this,
    [=](){
        isPlaylists = true;
        player->pause();
        iMusic = 0;

        allMusicRead();
    });

    connect(ui->musicCardButton, &QPushButton::clicked, this, [=](){card->start();});

    connect(ui->settingsSave, &QPushButton::clicked, this, &MainWindow::settingsSave_clicked);
    connect(ui->musicsPathAddingButton, &QPushButton::clicked, this, &MainWindow::musicsPathAddingButton_clicked);

#ifdef WIN32
    if(!QFile::exists("../yt-dlp/yt-dlp.exe"))
        connect(ui->ytDlpTools, &QPushButton::clicked, this, &MainWindow::ytDlpIndirme_clicked);
    else
    {
        ui->ytDlpTools->setText("Yt-Dlp Aracını Güncelle");
        configurer->setIcon(ui->ytDlpTools, QString::fromUtf8(":/assets/icons/reset.svg"));
        connect(ui->ytDlpTools, &QPushButton::clicked, this, &MainWindow::ytDlpIndirme_clicked2);
    }

    if(!QFile::exists("../ffmpeg/ffmpeg/bin/ffmpeg.exe"))
        connect(ui->ffmpegTools, &QPushButton::clicked, this, &MainWindow::ffmpegIndirme_clicked);
    else
        ui->ffmpegTools->setEnabled(false);

    if(QFile::exists("../yt-dlp/yt-dlp.exe") && QFile::exists("../ffmpeg/ffmpeg/bin/ffmpeg.exe"))
    {
        connect(ui->musicDownloadUrl, &QLineEdit::returnPressed , this,
                [this]()
                {
                    medyaIndirme(ui->musicDownloadUrl->text());
                });
        connect(ui->downloadButton, &QPushButton::clicked , this,
                [this]()
                {
                    medyaIndirme(ui->musicDownloadUrl->text());
                });
    }

#else
    if(!QFile::exists("../yt-dlp/yt-dlp"))
        connect(ui->ytDlpTools, &QPushButton::clicked, this, &MainWindow::ytDlpIndirme_clicked);
    else
    {
        ui->ytDlpTools->setText("Yt-Dlp Aracını Güncelle");
        configurer->setIcon(ui->ytDlpTools, QString::fromUtf8(":/assets/icons/reset.svg"));
        connect(ui->ytDlpTools, &QPushButton::clicked, this, &MainWindow::ytDlpIndirme_clicked2);
    }

    if(!QFile::exists("../ffmpeg/ffmpeg/bin/ffmpeg"))
        connect(ui->ffmpegTools, &QPushButton::clicked, this, &MainWindow::ffmpegIndirme_clicked);
    else
        ui->ffmpegTools->setEnabled(false);

    if(QFile::exists("../yt-dlp/yt-dlp") && QFile::exists("../ffmpeg/ffmpeg/bin/ffmpeg"))
    {
        connect(ui->downloadButton, &QLineEdit::returnPressed , this,
                [this]()
                {
                    medyaIndirme(ui->musicDownloadUrl->text());
                });
    }
#endif
    connect(ui->nodejsTools, &QPushButton::clicked, this,
    [this]()
    {
        QDesktopServices::openUrl(QUrl("https://nodejs.org/tr/download"));
    });

    connect(ui->actionGithub, &QAction::triggered, this,
            [this]()
            {
                QDesktopServices::openUrl(QUrl("https://github.com/DATA35DELETE/Data-Music-Player"));
            });

    connect(ui->actionLinkedln, &QAction::triggered, this,
            [this]()
            {
                QDesktopServices::openUrl(QUrl("https://www.linkedin.com/in/muhammet-enes-karabo%C4%9Fa-065777385/"));
            });
}

void MainWindow::musicsRead()
{
    ui->musicsPathList->clear();
    muzikYollari.clear();

    QFile muzikYollariFile("../musicPath.sett");
    if(muzikYollariFile.open(QFile::OpenModeFlag::ReadOnly))
    {
        QTextStream akis(&muzikYollariFile);

        while(!akis.atEnd())
        {
            QString path = akis.readLine().replace("\\", "/");

            muzikYollari.append(path);

            ui->musicsPathList->addItem(path);
        }
    }
    muzikYollariFile.close();

    musics.clear();

    for (auto i : muzikYollari) {
        QDir muzikYollariDir(i);
        muzikYollariDir.setSorting(QDir::SortFlag::Time);
        muzikYollariDir.setFilter(QDir::Filter::Files);

        for (auto ii : muzikYollariDir.entryList()) {
            if(!ii.endsWith(".mp3"))
                continue;

            if(!isPlaylists)
                musics.append(ii.prepend(QString(i).append(QDir::separator())));
            else
                playlistsMusics.append(ii.prepend(QString(i).append(QDir::separator())));
        }

        //qDebug() << i;
    }

    if(!isPlaylists)
        ui->musicsCount->setText(QString::number(musics.count()).append(" Şarkı"));

    if(isPlaylists)
        return;

    for (auto i : musics) {
        TagLib::MPEG::File file(i.toStdWString().c_str());
        if (!file.isValid()) {
            qDebug() << "Hata: Geçersiz MP3 dosyası." << '\n';
            return;
        }

        // 2. ID3v2 etiketini al (kapak resimleri sadece ID3v2'de bulunur)
        TagLib::ID3v2::Tag* tag = file.ID3v2Tag();
        if (!tag) {
            qDebug() << "Hata: ID3v2 etiketi bulunamadı." << '\n';
            return;
        }

        // 3. "APIC" (Attached Picture) çerçevelerini listele
        TagLib::ID3v2::FrameList frames = tag->frameList("APIC");
        if (frames.isEmpty()) {
            qDebug() << "Bilgi: Dosyada gömülü kapak resmi yok." << '\n';
            continue;
        }

        // 4. İlk resim çerçevesini al (genellikle ön kapaktır)
        auto* picFrame = dynamic_cast<TagLib::ID3v2::AttachedPictureFrame*>(frames.front());
        if (!picFrame) {
            return;
        }

        // 5. Görsel verisini al ve dosyaya yaz
        TagLib::ByteVector imageData = picFrame->picture();

        // QFileInfo(i).fileName().replace(".mp3",".jpeg").prepend("../thumbnails/")
        QFile thumbnailsFotoFile("../tempImage");
        if(thumbnailsFotoFile.open(QFile::OpenModeFlag::WriteOnly))
        {
            thumbnailsFotoFile.write(imageData.data(), imageData.size());
        }
        thumbnailsFotoFile.close();

        QImageReader rd("../tempImage");
        rd.setClipRect(QRect((rd.size().width() - rd.size().height()) / 2, 0,rd.size().height(),rd.size().height()));
        rd.setScaledSize(QSize(40, 40));

        rd.read().save(QFileInfo(i).fileName().replace(".mp3",".jpeg").prepend("../thumbnails/"), "JPEG");
    }
}

void MainWindow::playlistsRead()
{
    ui->playlistsList->clear();

    QDir playlistsDir("../playlists");
    playlistsDir.setFilter(QDir::Filter::Files);
    playlistsDir.setSorting(QDir::SortFlag::Time);

    for (auto i : playlistsDir.entryList()) {
        QListWidgetItem *item = new QListWidgetItem(ui->playlistsList);

        QWidget *anaWidget = new QWidget(this);
        anaWidget->setToolTip(i);
        QHBoxLayout *anaLayout = new QHBoxLayout(anaWidget);
        anaWidget->setLayout(anaLayout);

        QLabel *iconLabel = new QLabel;
        iconLabel->setObjectName("iconLabel");
        iconLabel->setScaledContents(true);
        iconLabel->setMaximumSize(QSize(40, 40));
        iconLabel->setMinimumSize(QSize(40, 40));
        if(i.contains("Favoriler.txt"))
            configurer->setIcon(iconLabel, QString::fromUtf8(":/assets/icons/heart.svg"));
        else if(QFileInfo::exists(QString(i).replace(".txt",".jpeg").prepend("../thumbnailsPlaylists/")))
            iconLabel->setPixmap(QPixmap(QString(i).replace(".txt",".jpeg").prepend("../thumbnailsPlaylists/")));
        else
            configurer->setIcon(iconLabel, QString::fromUtf8(":/assets/icons/nota.svg"));
        anaLayout->addWidget(iconLabel);

        QLabel *nameLabel = new QLabel;
        nameLabel->setObjectName("nameLabel");
        nameLabel->setText(QString(i).remove(".txt"));
        nameLabel->setWordWrap(true);
        anaLayout->addWidget(nameLabel);

        item->setSizeHint(anaWidget->sizeHint());

        ui->playlistsList->setItemWidget(item, anaWidget);

        playlists.append(i);
    }
}

void MainWindow::readPlaylist(QString playlistPath)
{
    playlistsMusics.clear();

    QFile playlistFile(playlistPath);
    if(playlistFile.open(QFile::OpenModeFlag::ReadOnly))
    {
        QTextStream akis(&playlistFile);

        QString line;
        while(!akis.atEnd())
        {
            line = akis.readLine();

            if(line.contains("\n"))
                continue;

            playlistsMusics.append(line);
        }
    }
}

void MainWindow::allMusicRead()
{
    if(!isPlaylists)
    {
        ui->allMusicsList->clear();
    }
    else
    {
        ui->listWidget->clear();
    }

    int count = 0;

    if(!isPlaylists)
        count = musics.count();
    else
        count = playlistsMusics.count();

    for (int i = 0; i < count; i++) {
        QListWidgetItem *item;

        if(!isPlaylists)
            item = new QListWidgetItem(ui->allMusicsList);
        else
            item = new QListWidgetItem(ui->listWidget);

        QWidget *anaWidget = new QWidget(this);
        if(!isPlaylists)
            anaWidget->setToolTip(musics.at(i));
        else
            anaWidget->setToolTip(playlistsMusics.at(i));
        anaWidget->setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);

        if(!isPlaylists)
        {
            connect(anaWidget, &QWidget::customContextMenuRequested, this,[=](QPoint pos){
                QMenu contextMenu(this);

                QAction *favori = contextMenu.addAction("Favori");
                if(favoriler.contains(musics.at(i)))
                {
                    configurer->setIcon(favori, QString::fromUtf8(":/assets/icons/check.svg"));
                }
                else
                {
                    configurer->setIcon(favori, QString::fromUtf8(":/assets/icons/x.svg"));
                }
                connect(favori, &QAction::triggered, this,
                [=](){
                    if(favoriler.contains(musics.at(i)))
                    {
                        configurer->setIcon(favori, QString::fromUtf8(":/assets/icons/x.svg"));

                        favoriler.removeOne(musics.at(i));
                        writeFaw();
                    }
                    else
                    {
                        configurer->setIcon(favori, QString::fromUtf8(":/assets/icons/check.svg"));

                        favoriler.append(musics.at(i));
                        writeFaw();
                    }
                });

                QMenu *playlistMenu = contextMenu.addMenu("Oynatma Listesine Ekle");
                for (auto ii : playlists) {
                    if(ii.contains("Favoriler.txt"))
                        continue;

                    connect(playlistMenu->addAction(QFileInfo(ii).fileName().remove(".txt")), &QAction::triggered, this,
                    [=](){
                        QFile playlistFile(ii);

                        if(playlistFile.open(QFile::OpenModeFlag::Append))
                        {
                            QTextStream(&playlistFile) << '\n' << musics.at(i);
                        }
                        playlistFile.close();
                    });
                }

                contextMenu.exec(anaWidget->mapToGlobal(pos));
            });
        }

        QHBoxLayout *anaLayout = new QHBoxLayout(anaWidget);
        anaWidget->setLayout(anaLayout);

        QLabel *iconLabel = new QLabel;
        iconLabel->setScaledContents(true);
        iconLabel->setMaximumSize(QSize(30, 30));
        iconLabel->setMinimumSize(QSize(30, 30));
        if(QFile(QFileInfo(musics.at(i)).fileName().replace(".mp3",".jpeg").prepend("../thumbnails/")).exists())
        {
            if(!isPlaylists)
                iconLabel->setPixmap(QPixmap(QFileInfo(musics.at(i)).fileName().replace(".mp3",".jpeg").prepend("../thumbnails/")));
            else
                iconLabel->setPixmap(QPixmap(QFileInfo(playlistsMusics.at(i)).fileName().replace(".mp3",".jpeg").prepend("../thumbnails/")));
        }
        else
        {
            configurer->setIcon(iconLabel, QString::fromUtf8(":/assets/icons/nota.svg"));
        }
        anaLayout->addWidget(iconLabel);

        QLabel *nameLabel = new QLabel;
        if(!isPlaylists)
            nameLabel->setText(QFileInfo(musics.at(i)).fileName().remove(".mp3"));
        else
            nameLabel->setText(QFileInfo(playlistsMusics.at(i)).fileName().remove(".mp3"));
        nameLabel->setWordWrap(true);
        anaLayout->addWidget(nameLabel);

        QPushButton *playButton = new QPushButton;
        playButton->setText("Oynat");
        connect(playButton, &QPushButton::clicked, this,
        [=](){
            iMusic = i;

            if(!isPlaylists)
                player->setSource(QUrl::fromLocalFile(musics.at(i)));
            else
                player->setSource(QUrl::fromLocalFile(playlistsMusics.at(i)));

            player->play();
        });
        playButton->setMaximumSize(QSize(100, 1287365));
        anaLayout->addWidget(playButton);

        item->setSizeHint(anaWidget->sizeHint());

        if(!isPlaylists)
            ui->allMusicsList->setItemWidget(item, anaWidget);
        else
            ui->listWidget->setItemWidget(item, anaWidget);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::player_mediaStatusChanged(QMediaPlayer::MediaStatus status)
{

    if(status != QMediaPlayer::MediaStatus::BufferedMedia)
        return;

    TagLib::MPEG::File *file;

    if(!isPlaylists)
    {
        file = new TagLib::MPEG::File(musics.at(iMusic).toStdWString().c_str());
        if (!file->isValid()) {
            qDebug() << "Hata: Geçersiz MP3 dosyası." << '\n';
            return;
        }
    }
    else
    {
        file = new TagLib::MPEG::File(playlistsMusics.at(iMusic).toStdWString().c_str());
        if (!file->isValid()) {
            qDebug() << "Hata: Geçersiz MP3 dosyası." << '\n';
            return;
        }
    }

        // 2. ID3v2 etiketini al (kapak resimleri sadece ID3v2'de bulunur)
    TagLib::ID3v2::Tag* tag = file->ID3v2Tag();
    if (!tag) {
        qDebug() << "Hata: ID3v2 etiketi bulunamadı." << '\n';
        return;
    }

        // 3. "APIC" (Attached Picture) çerçevelerini listele
    TagLib::ID3v2::FrameList frames = tag->frameList("APIC");
    if (frames.isEmpty()) {
        if(!isMusicCard)
            configurer->setIcon(ui->musicsThumbnails, QString::fromUtf8(":/assets/icons/nota.svg"));
        else
            configurer->setIcon(card->musicCardThumbnails, QString::fromUtf8(":/assets/icons/nota.svg"));

        if(!isPlaylists)
            ui->musicsName->setText(QFileInfo(musics.at(iMusic)).fileName().remove(".mp3"));
        else
            ui->musicsName->setText(QFileInfo(playlistsMusics.at(iMusic)).fileName().remove(".mp3"));

        ui->musicsAuthor->setText((player->metaData().value(QMediaMetaData::ContributingArtist).value<QString>().isEmpty()) ? QString("Belirtilmemiş") : player->metaData().value(QMediaMetaData::ContributingArtist).value<QString>());

        int s = player->duration() / 1000;
        int m = 0,h = 0;

        while(s >= 60)
        {
            m++;

            s -= 60;
        }
        while(m >= 60)
        {
            h++;

            m -= 60;
        }

        QString prompt = "";

        if(h < 10)
            prompt.append("0");
        prompt.append(QString::number(h)).append(":");
        if(m < 10)
            prompt.append("0");
        prompt.append(QString::number(m)).append(":");
        if(s < 10)
            prompt.append("0");
        prompt.append(QString::number(s));

        ui->musicDuration->setText(prompt);

        if(!isPlaylists)
            ui->allMusicsList->setCurrentRow(iMusic);
        else
            ui->listWidget->setCurrentRow(iMusic);

        if(!isPlaylists)
            ui->statusbar->showMessage(QFileInfo(musics.at(iMusic)).fileName().remove(".mp3").append(" | ").append((player->metaData().value(QMediaMetaData::ContributingArtist).value<QString>().isEmpty()) ? QString("Belirtilmemiş") : player->metaData().value(QMediaMetaData::ContributingArtist).value<QString>()));
        else
            ui->statusbar->showMessage(QFileInfo(playlistsMusics.at(iMusic)).fileName().remove(".mp3").append(" | ").append((player->metaData().value(QMediaMetaData::ContributingArtist).value<QString>().isEmpty()) ? QString("Belirtilmemiş") : player->metaData().value(QMediaMetaData::ContributingArtist).value<QString>()));

        delete file;

        return;
    }

        // 4. İlk resim çerçevesini al (genellikle ön kapaktır)
    auto* picFrame = dynamic_cast<TagLib::ID3v2::AttachedPictureFrame*>(frames.front());
    if (!picFrame) {
        return;
    }

        // 5. Görsel verisini al ve dosyaya yaz
    TagLib::ByteVector imageData = picFrame->picture();

        // QFileInfo(i).fileName().replace(".mp3",".jpeg").prepend("../thumbnails/")
    QFile thumbnailsFotoFile("../tempImage2");
    if(thumbnailsFotoFile.open(QFile::OpenModeFlag::WriteOnly))
    {
        thumbnailsFotoFile.write(imageData.data(), imageData.size());
    }
    thumbnailsFotoFile.close();

    QImageReader rd("../tempImage2");
    rd.setClipRect(QRect((rd.size().width() - rd.size().height()) / 2, 0,rd.size().height(),rd.size().height()));
    rd.setScaledSize(QSize(200,200));

    QImage readBoku = rd.read();

    QPixmap yuvarlak(readBoku.size());
    yuvarlak.fill(Qt::transparent);

    QPainter cizici(&yuvarlak);
    cizici.setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    QPainterPath ciziciPath;
    ciziciPath.addRoundedRect(yuvarlak.rect(), 40, 40);

    cizici.setClipPath(ciziciPath);
    cizici.drawPixmap(0,0, QPixmap::fromImage(readBoku));

    if(!isMusicCard)
        ui->musicsThumbnails->setPixmap(yuvarlak);
    else
        card->musicCardThumbnails->setPixmap(yuvarlak);

    if(!isPlaylists)
        ui->musicsName->setText(QFileInfo(musics.at(iMusic)).fileName().remove(".mp3"));
    else
        ui->musicsName->setText(QFileInfo(playlistsMusics.at(iMusic)).fileName().remove(".mp3"));
    ui->musicsAuthor->setText((player->metaData().value(QMediaMetaData::ContributingArtist).value<QString>().isEmpty()) ? QString("Belirtilmemiş") : player->metaData().value(QMediaMetaData::ContributingArtist).value<QString>());

    int s = player->duration() / 1000;
    int m = 0,h = 0;

    while(s >= 60)
    {
        m++;

        s -= 60;
    }
    while(m >= 60)
    {
        h++;

        m -= 60;
    }

    QString prompt = "";

    if(h < 10)
        prompt.append("0");
    prompt.append(QString::number(h)).append(":");
    if(m < 10)
        prompt.append("0");
    prompt.append(QString::number(m)).append(":");
    if(s < 10)
        prompt.append("0");
    prompt.append(QString::number(s));

    ui->musicDuration->setText(prompt);

    if(!isPlaylists)
        ui->allMusicsList->setCurrentRow(iMusic);
    else
        ui->listWidget->setCurrentRow(iMusic);
    if(!isPlaylists)
        ui->statusbar->showMessage(QFileInfo(musics.at(iMusic)).fileName().remove(".mp3").append(" | ").append((player->metaData().value(QMediaMetaData::ContributingArtist).value<QString>().isEmpty()) ? QString("Belirtilmemiş") : player->metaData().value(QMediaMetaData::ContributingArtist).value<QString>()));
    else
        ui->statusbar->showMessage(QFileInfo(playlistsMusics.at(iMusic)).fileName().remove(".mp3").append(" | ").append((player->metaData().value(QMediaMetaData::ContributingArtist).value<QString>().isEmpty()) ? QString("Belirtilmemiş") : player->metaData().value(QMediaMetaData::ContributingArtist).value<QString>()));

    delete file;
}
void MainWindow::player_positionChanged(qint64 position)
{
    int s = position / 1000;
    int m = 0,h = 0;

    while(s >= 60)
    {
        m++;

        s -= 60;
    }
    while(m >= 60)
    {
        h++;

        m -= 60;
    }

    QString prompt = "";

    if(h < 10)
        prompt.append("0");
    prompt.append(QString::number(h)).append(":");
    if(m < 10)
        prompt.append("0");
    prompt.append(QString::number(m)).append(":");
    if(s < 10)
        prompt.append("0");
    prompt.append(QString::number(s));

    ui->musicPosition->setText(prompt);

    if(!isMusicCard)
    {
        if(!userControl)
            ui->musicProgres->setValue((float)position / (float)player->duration() * 100);
    }
    else
    {
        card->musicCardProgress->setValue((float)position / (float)player->duration() * 100);
    }

    if(position == player->duration())
        nextMusic(true);
}
void MainWindow::player_playingChanged(bool playing)
{
    if(playing)
    {
        if(!isMusicCard)
            configurer->setIcon(ui->musicControl, QString::fromUtf8(":/assets/icons/play.svg"));
        else
            configurer->setIcon(card->musicCardControl, QString::fromUtf8(":/assets/icons/play.svg"));
    }
    else
    {
        if(!isMusicCard)
            configurer->setIcon(ui->musicControl, QString::fromUtf8(":/assets/icons/pause.svg"));
        else
            configurer->setIcon(card->musicCardControl, QString::fromUtf8(":/assets/icons/pause.svg"));
    }
}

void MainWindow::musicControl_clicked()
{
    if(player->isPlaying())
        player->pause();
    else
        player->play();
}
void MainWindow::musicsVolumeButton_clicked()
{
    if(output->isMuted())
    {
        configurer->setIcon(ui->musicVolumeButton, QString::fromUtf8(":/assets/icons/volume-2.svg"));

        output->setMuted(false);
    }
    else
    {
        configurer->setIcon(ui->musicVolumeButton, QString::fromUtf8(":/assets/icons/volume-x.svg"));

        output->setMuted(true);
    }
}
void MainWindow::musicVolume_actionTriggered(int)
{
    output->setVolume((float)ui->musicVolume->value() / 100.0f);
}

void MainWindow::musicProgress_sliderMoved(int deger)
{
    userControl = true;

    player->setPosition(((player->duration() * 1) / 100 ) * deger);
    //if(mod == 1) isPlayPosition = ((player->duration() * 1) / 100 ) * deger;

    userControl = false;
}
void MainWindow::musicProgress_actionTriggered(int deger)
{
    auto _ = QtConcurrent::run(
    [this]()
    {
        userControl = true;

        player->setPosition(((player->duration() * 1) / 100 ) * ui->musicProgres->value());

            //if(mod == 1) isPlayPosition = ((player->duration() * 1) / 100 ) * ui->musicProgres->value();

        QThread::msleep(500);

        userControl = false;
    });
}
void MainWindow::musicMod_clicked()
{
    if(mod == 0)
    {
        mod = 1;

        configurer->setIcon(ui->musicMod, QString::fromUtf8(":/assets/icons/repeat-1.svg"));
    }
    else if(mod == 1)
    {
        mod = 2;

        configurer->setIcon(ui->musicMod, QString::fromUtf8(":/assets/icons/shuffle.svg"));
    }
    else if(mod == 2)
    {
        mod = 0;

        configurer->setIcon(ui->musicMod, QString::fromUtf8(":/assets/icons/repeat.svg"));
    }
}

void MainWindow::nextMusic(bool isNext)
{
    if(isNext)
    {
        if(mod == 0)
        {
            iMusic++;

            if(iMusic == musics.count() && !isPlaylists)
            {
                iMusic = 0;
            }
            else if(iMusic == playlistsMusics.count() && isPlaylists)
            {
                iMusic = 0;
            }

            player->stop();

            if(!isPlaylists)
                player->setSource(QUrl::fromLocalFile(musics.at(iMusic)));
            else
                player->setSource(QUrl::fromLocalFile(playlistsMusics.at(iMusic)));

            player->play();
        }
        else if(mod == 1)
        {
            player->setLoops(2);
        }
        else if(mod == 2)
        {
            iMusic = QRandomGenerator::global()->bounded(musics.count());

            player->stop();

            if(!isPlaylists)
                player->setSource(QUrl::fromLocalFile(musics.at(iMusic)));
            else
                player->setSource(QUrl::fromLocalFile(playlistsMusics.at(iMusic)));

            player->play();
        }
    }
    else
    {
        if(mod == 0)
        {
            iMusic--;

            if(iMusic < 0)
            {
                if(!isPlaylists)
                    iMusic = musics.count() - 1;
                else
                    iMusic = playlistsMusics.count() - 1;
            }

            player->stop();

            if(!isPlaylists)
                player->setSource(QUrl::fromLocalFile(musics.at(iMusic)));
            else
                player->setSource(QUrl::fromLocalFile(playlistsMusics.at(iMusic)));

            player->play();
        }
        else if(mod == 1)
        {
            player->setLoops(2);
        }
        else if(mod == 2)
        {
            if(!isPlaylists)
                iMusic = QRandomGenerator::global()->bounded(musics.count());
            else
                iMusic = QRandomGenerator::global()->bounded(playlistsMusics.count());

            player->stop();

            if(!isPlaylists)
                player->setSource(QUrl::fromLocalFile(musics.at(iMusic)));
            else
                player->setSource(QUrl::fromLocalFile(playlistsMusics.at(iMusic)));

            player->play();
        }
    }
}

void MainWindow::readFaw()
{
    favoriler.clear();

    QFile fawFile("../playlists/Favoriler.txt");
    if(fawFile.open(QFile::OpenModeFlag::ReadOnly))
    {
        QTextStream akis(&fawFile);

        QString line;
        while(!akis.atEnd())
        {
            line = akis.readLine();

            if(line.contains("\n"))
                continue;

            favoriler.append(line);
        }
    }
    fawFile.close();
}

void MainWindow::writeFaw()
{
    QFile fawFile("../playlists/Favoriler.txt");

    if(fawFile.open(QFile::OpenModeFlag::WriteOnly))
    {
        for (auto i : favoriler) {
            QTextStream(&fawFile) << i << '\n';
        }
    }
    fawFile.close();

    readFaw();
}

void musicCard::start()
{
    isMusicCard = true;

    mainWindow->setVisible(false);

    mainWindow->ui->musicsThumbnails->clear();

    mainWindow->player_mediaStatusChanged(player->mediaStatus());

    this->setVisible(true);
}
void musicCard::end()
{
    isMusicCard = false;

    this->setVisible(false);

    mainWindow->card->musicCardThumbnails->clear();

    mainWindow->player_mediaStatusChanged(player->mediaStatus());

    mainWindow->setVisible(true);
}

void MainWindow::settingsLoad()
{
    QSettings settFile("./config.ini", QSettings::IniFormat);
    if(settFile.value("general/theme").isNull())
    {
        settFile.setValue("general/theme", "black");

        configurer->setApplicationTheme(qApp, Themes::Black);
        configurer->setTheme(Themes::Black);
    }
    else
    {
        QString theme = settFile.value("general/theme").value<QString>();

        if(theme.contains("black"))
        {
            configurer->setApplicationTheme(qApp, Themes::Black);
            configurer->setTheme(Themes::Black);

            ui->themeColor->setCurrentIndex(1);
        }
        else if(theme.contains("white"))
        {
            configurer->setApplicationTheme(qApp, Themes::White);
            configurer->setTheme(Themes::White);

            ui->themeColor->setCurrentIndex(0);
        }
        else if(theme.contains("grayDark"))
        {
            configurer->setApplicationTheme(qApp, Themes::GrayDark);
            configurer->setTheme(Themes::GrayDark);

            ui->themeColor->setCurrentIndex(2);
        }
        else if(theme.contains("grayLight"))
        {
            configurer->setApplicationTheme(qApp, Themes::GrayLigth);
            configurer->setTheme(Themes::GrayLigth);

            ui->themeColor->setCurrentIndex(3);
        }
        else if(theme.contains("redDark"))
        {
            configurer->setApplicationTheme(qApp, Themes::RedDark);
            configurer->setTheme(Themes::RedDark);

            ui->themeColor->setCurrentIndex(4);
        }
        else if(theme.contains("redLight"))
        {
            configurer->setApplicationTheme(qApp, Themes::RedLight);
            configurer->setTheme(Themes::RedLight);

            ui->themeColor->setCurrentIndex(5);
        }
        else if(theme.contains("greenDark"))
        {
            configurer->setApplicationTheme(qApp, Themes::GreenDark);
            configurer->setTheme(Themes::GreenDark);

            ui->themeColor->setCurrentIndex(6);
        }
        else if(theme.contains("greenLight"))
        {
            configurer->setApplicationTheme(qApp, Themes::GreenLight);
            configurer->setTheme(Themes::GreenLight);

            ui->themeColor->setCurrentIndex(7);
        }
        else if(theme.contains("blueDark"))
        {
            configurer->setApplicationTheme(qApp, Themes::BlueDark);
            configurer->setTheme(Themes::BlueDark);


            ui->themeColor->setCurrentIndex(8);
        }
        else if(theme.contains("blueLight"))
        {
            configurer->setApplicationTheme(qApp, Themes::BlueLight);
            configurer->setTheme(Themes::BlueLight);


            ui->themeColor->setCurrentIndex(9);
        }
        else if(theme.contains("cyanDark"))
        {
            configurer->setApplicationTheme(qApp, Themes::CyanDark);
            configurer->setTheme(Themes::CyanDark);

            ui->themeColor->setCurrentIndex(10);
        }
        else if(theme.contains("cyanLight"))
        {
            configurer->setApplicationTheme(qApp, Themes::CyanLight);
            configurer->setTheme(Themes::CyanLight);

            ui->themeColor->setCurrentIndex(11);
        }
        else if(theme.contains("purpleDark"))
        {
            configurer->setApplicationTheme(qApp, Themes::PurpleDark);
            configurer->setTheme(Themes::PurpleDark);

            ui->themeColor->setCurrentIndex(12);
        }
        else if(theme.contains("purpleLight"))
        {
            configurer->setApplicationTheme(qApp, Themes::PurpleLight);
            configurer->setTheme(Themes::PurpleLight);

            ui->themeColor->setCurrentIndex(13);
        }
        else if(theme.contains("yellowDark"))
        {
            configurer->setApplicationTheme(qApp, Themes::YellowDark);
            configurer->setTheme(Themes::YellowDark);

            ui->themeColor->setCurrentIndex(14);
        }
        else if(theme.contains("yellowLight"))
        {
            configurer->setApplicationTheme(qApp, Themes::YellowLight);
            configurer->setTheme(Themes::YellowLight);

            ui->themeColor->setCurrentIndex(15);
        }
        else if(theme.contains("orangeDark"))
        {
            configurer->setApplicationTheme(qApp, Themes::OrangeDark);
            configurer->setTheme(Themes::OrangeDark);

            ui->themeColor->setCurrentIndex(16);
        }
        else if(theme.contains("orangeLight"))
        {
            configurer->setApplicationTheme(qApp, Themes::OrangeLight);
            configurer->setTheme(Themes::OrangeLight);

            ui->themeColor->setCurrentIndex(17);
        }
    }
}
void MainWindow::settingsSave_clicked()
{
    QSettings settFile("./config.ini", QSettings::IniFormat);
    if(ui->themeColor->currentIndex() == 0)
        settFile.setValue("general/theme", "white");
    else if(ui->themeColor->currentIndex() == 1)
        settFile.setValue("general/theme", "black");
    if(ui->themeColor->currentIndex() == 2)
        settFile.setValue("general/theme", "grayDark");
    else if(ui->themeColor->currentIndex() == 3)
        settFile.setValue("general/theme", "grayLight");
    if(ui->themeColor->currentIndex() == 4)
        settFile.setValue("general/theme", "redDark");
    else if(ui->themeColor->currentIndex() == 5)
        settFile.setValue("general/theme", "redLight");
    if(ui->themeColor->currentIndex() == 6)
        settFile.setValue("general/theme", "greenDark");
    else if(ui->themeColor->currentIndex() == 7)
        settFile.setValue("general/theme", "greenLight");
    if(ui->themeColor->currentIndex() == 8)
        settFile.setValue("general/theme", "blueDark");
    else if(ui->themeColor->currentIndex() == 9)
        settFile.setValue("general/theme", "blueLight");
    if(ui->themeColor->currentIndex() == 10)
        settFile.setValue("general/theme", "cyanDark");
    else if(ui->themeColor->currentIndex() == 11)
        settFile.setValue("general/theme", "cyanLight");
    if(ui->themeColor->currentIndex() == 12)
        settFile.setValue("general/theme", "purpleDark");
    else if(ui->themeColor->currentIndex() == 13)
        settFile.setValue("general/theme", "purpleLight");
    if(ui->themeColor->currentIndex() == 14)
        settFile.setValue("general/theme", "yellowDark");
    else if(ui->themeColor->currentIndex() == 15)
        settFile.setValue("general/theme", "yellowLight");
    if(ui->themeColor->currentIndex() == 16)
        settFile.setValue("general/theme", "orangeDark");
    else if(ui->themeColor->currentIndex() == 17)
        settFile.setValue("general/theme", "orangeLight");


    QFile musicsPath("../musicPath.sett");
    if(musicsPath.open(QFile::OpenModeFlag::WriteOnly))
    {
        QTextStream stream(&musicsPath);

        for (int i = 0; i < ui->musicsPathList->count(); ++i) {
            stream << ui->musicsPathList->item(i)->text() << '\n';
        }
    }

    musicsRead();
    allMusicRead();

    settingsLoad();

    iconsUpdate();
}

void MainWindow::musicsPathAddingButton_clicked()
{
    QFileDialog fd;
    fd.setFileMode(QFileDialog::Directory);
    configurer->setIcon(&fd, QString::fromUtf8(":/assets/icons/nota.svg"));

    fd.exec();

    if(!fd.selectedFiles().isEmpty())
    {
        bool isOkey = false;

        for (auto ii : fd.selectedFiles()) {
            isOkey = true;

            for (int i = 0; i < ui->musicsPathList->count(); ++i) {
                if(ui->musicsPathList->item(i)->text().contains(ii))
                    isOkey = false;
            }

            if(isOkey)
                ui->musicsPathList->addItem(ii);
        }
    }
    else
    {
        QMessageBox msgBox(QMessageBox::Icon::Warning, "Müzik Yolu Seçilmedi!", "Müzik yolu eklenemedi.");

        configurer->setIcon(&msgBox, QString::fromUtf8(":/assets/icons/nota.svg"));

        msgBox.exec();
    }
}
void MainWindow::ytDlpIndirme_clicked()
{
    ui->log->setText(ui->log->toPlainText().append("[Yt-Dlp İndirme]: İndirme için hazırlanılıyor..."));

    mngr = new QNetworkAccessManager;
    connect(mngr, &QNetworkAccessManager::finished, this,
            [this](QNetworkReply *donus){

                ui->log->setText(ui->log->toPlainText().append("[Yt-Dlp İndirme]: İndirilen dosya yazdırılıyor..."));

                QFile gelenZip("../yt-dlp.zip");

                if(gelenZip.open(QFile::OpenModeFlag::WriteOnly))
                {
                    QDataStream akis(&gelenZip);

                    akis << donus->readAll();
                }
                gelenZip.close();

                ui->log->setText(ui->log->toPlainText().append("OK\n"));

                ui->log->setText(ui->log->toPlainText().append("[Yt-Dlp İndirme]: İndirilen dosya ayıklanıyor..."));

#ifdef WIN32
                std::system(qPrintable(QString("tar -xf ..\yt-dlp.zip -C ..\yt-dlp")));
#else
                std::system(qPrintable(QString("unzip ../yt-dlp.zip -d ../yt-dlp")));
#endif
                ui->log->setText(ui->log->toPlainText().append("OK\n"));

                QMessageBox msg(
                    QMessageBox::Icon::Information,
                    "Başaralı!",
                    "Sistem başarılı bir şekilde Yt-Dlp aracını indirdi."
                    );

                msg.exec();

                musicsRead();
                allMusicRead();
            });

    ui->log->setText(ui->log->toPlainText().append("OK\n"));

    mngr->get(QNetworkRequest(QUrl("https://github.com/yt-dlp/yt-dlp/releases/download/2026.08.19/yt-dlp_win.zip")));
}
void MainWindow::ytDlpIndirme_clicked2()
{
    ui->log->setText(ui->log->toPlainText().append("[Yt-Dlp]: Araç güncelleniyor..."));

    QProcess *ytDlpIndirme = new QProcess(this);
    connect(ytDlpIndirme, &QProcess::readyReadStandardOutput,
            this,
            [this, ytDlpIndirme]()
            {
                QString cikti = ytDlpIndirme->readAllStandardOutput();

                ui->log->append(cikti);
            });

    connect(ytDlpIndirme, &QProcess::readyReadStandardError,
            this,
            [this, ytDlpIndirme]()
            {
                QString hata = ytDlpIndirme->readAllStandardError();

                ui->log->append(hata);
            });
#ifdef WIN32
    ytDlpIndirme->startDetached("..\\yt-dlp\\yt-dlp", {"-U"});
#else
    ytDlpIndirme->startDetached("../yt-dlp/yt-dlp", {"-U"});
#endif

    ui->log->setText(ui->log->toPlainText().append("OK\n"));
}

// https://github.com/BtbN/FFmpeg-Builds/releases/download/latest/ffmpeg-n9.0-latest-win64-lgpl-9.0.zip

void MainWindow::ffmpegIndirme_clicked()
{
    ui->log->setText(ui->log->toPlainText().append("[FFmpeg İndirme]: İndirme için hazırlanılıyor..."));

    mngr = new QNetworkAccessManager;
    connect(mngr, &QNetworkAccessManager::finished, this,
            [this](QNetworkReply *donus){

                ui->log->setText(ui->log->toPlainText().append("[FFmpeg İndirme]: İndirilen dosya yazdırılıyor..."));
#ifdef WIN32
                QFile gelenZip("../" ffmpegVersionNumber ".zip");
#else
                QFile gelenZip("../" ffmpegVersionNumber ".tar.xz");
#endif

                if(gelenZip.open(QFile::OpenModeFlag::WriteOnly))
                {
                    QDataStream akis(&gelenZip);

                    akis << donus->readAll();
                }
                gelenZip.close();

                ui->log->setText(ui->log->toPlainText().append("OK\n"));

                ui->log->setText(ui->log->toPlainText().append("[FFmpeg İndirme]: İndirilen dosya ayıklanıyor..."));

#ifdef WIN32
                std::system(qPrintable(QString("tar -xf ../" ffmpegVersionNumber ".zip -C ../ffmpeg")));
#else
                std::system(qPrintable(QString("tar -xf ../" ffmpegVersionNumber ".tar.xz -C ../ffmpeg")));
#endif

                ui->log->setText(ui->log->toPlainText().append("OK\n"));

                QDir ffmpegDir("../ffmpeg");

                ffmpegDir.rename(ffmpegVersionNumber, "ffmpeg");

                QMessageBox msg(
                    QMessageBox::Icon::Information,
                    "Başaralı!",
                    "Sistem başarılı bir şekilde FFmpeg aracını indirdi."
                    );

                msg.exec();

                musicsRead();
                allMusicRead();
            });

    ui->log->setText(ui->log->toPlainText().append("OK\n"));

#ifdef WIN32
    mngr->get(QNetworkRequest(QUrl("https://github.com/BtbN/FFmpeg-Builds/releases/download/latest/ffmpeg-n9.0-latest-win64-lgpl-9.0.zip")));
#else
    mngr->get(QNetworkRequest(QUrl("https://github.com/BtbN/FFmpeg-Builds/releases/download/latest/ffmpeg-n9.0-latest-linux64-lgpl-9.0.tar.xz")));
#endif
}

void MainWindow::medyaIndirme(QString url)
{
    if(url.contains("https://open.spotify.com/"))
    {
        qDebug() << "Spotify aga";
    }
    else
    {
        ui->log->setText(ui->log->toPlainText().append("[Yt-Dlp]: İndiriliyor...\n"));

        QProcess *ytDlpIndirme = new QProcess(this);
        connect(ytDlpIndirme, &QProcess::finished, this,
                [=](int, QProcess::ExitStatus)
                {
                    musicsRead();
                    allMusicRead();
                });
        connect(ytDlpIndirme, &QProcess::readyReadStandardOutput,
                this,
                [this, ytDlpIndirme]()
                {
                    QString cikti = ytDlpIndirme->readAllStandardOutput();

                    ui->log->append(cikti);
                });

        connect(ytDlpIndirme, &QProcess::readyReadStandardError,
                this,
                [this, ytDlpIndirme]()
                {
                    QString hata = ytDlpIndirme->readAllStandardError();

                    ui->log->append(hata);
                });

        QStringList args;

#ifdef WIN32

        args << "--js-runtimes" << "node"
             << "--cookies" << "cookies.txt"
             << "--ffmpeg-location" << "..\\ffmpeg\\ffmpeg\\bin"
             << "-f" << "bestaudio/best"
             << "-x"
             << "--audio-format" << "mp3"
             << "--audio-quality" << "0"
             << "--embed-thumbnail"
             << "--add-metadata"
             << "-P" << "..\\downloads"
             << "-o" << "%(title)s.%(ext)s"
             << url;

        ytDlpIndirme->start("..\\yt-dlp\\yt-dlp.exe", args);

#else

        args << "--js-runtimes" << "node"
             << "--cookies" << "cookies.txt"
             << "--ffmpeg-location" << "../ffmpeg/ffmpeg/bin"
             << "-f" << "bestaudio/best"
             << "-x"
             << "--audio-format" << "mp3"
             << "--audio-quality" << "0"
             << "--embed-thumbnail"
             << "--add-metadata"
             << "-P" << "../downloads"
             << "-o" << "%(title)s.%(ext)s"
             << url;

        ytDlpIndirme->start("../yt-dlp/yt-dlp", args);

#endif
    }
}

void musicremove::end(QStringList list)
{
    this->setVisible(false);

    isPlaylists = false;
    player->pause();
    iMusic = 0;

    for (auto i : list) {
        QFile::remove(i);
    }

    QMessageBox(QMessageBox::Icon::Information, "Başarılı!", "Müzikler başarlı bir şekilde silindi").exec();

    mainWindow->settingsLoad();

    mainWindow->musicsRead();
    mainWindow->allMusicRead();

    mainWindow->iconsUpdate();
}

void playlistadd::end()
{
    this->setVisible(false);

    isPlaylists = false;
    player->pause();
    iMusic = 0;

    mainWindow->settingsLoad();

    mainWindow->musicsRead();
    mainWindow->allMusicRead();

    mainWindow->iconsUpdate();
}