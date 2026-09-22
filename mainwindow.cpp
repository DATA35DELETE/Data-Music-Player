#include "mainwindow.h"
#include "./ui_mainwindow.h"

QVector<std::wstring> medyalar;
QVector<std::wstring> sanacilar;

QVector<std::wstring> playlistsMedyalar;
QVector<std::wstring> playlistsSanacilar;
bool isPlaylists = false;

QMediaPlayer *oynatici;
QAudioOutput *cikis;
int gecerliIndex = 0;
bool kullaniciKontrol = false;
int mod = 0;
// 0 = Döngüsel
// 1 = Döngü
// 2 = Karışık

QProgressDialog *yuklenmeEkrani;

int oynaticiSayici = 0;
bool isReset = false;

MainWindow *mw2;

MainWindow::MainWindow(QWidget * parent)
    : QMainWindow(parent),
ui(new Ui::MainWindow) {
    ui->setupUi(this);

    mw2 = this;

    oynatmaListesiCreate = new oynatmaListesiOlusturma;
    oynatmaListesiDelete = new oynatmaListesiSilme;

    oynatici = new QMediaPlayer;
    cikis = new QAudioOutput;

    oynatici->setAudioOutput(cikis);

    cikis->setVolume(ui->medyaSesSeviyesi->value());

    medyalariListele();

    QWidget *girisSecenegi = new QWidget(this);
    QHBoxLayout *girisSecenegiLayout = new QHBoxLayout(girisSecenegi);
    girisSecenegi->setMinimumSize(QSize(0, 40));

    QPushButton *giris = new QPushButton("Giriş");
    girisSecenegiLayout->addWidget(giris);

    QListWidgetItem *girisItem = new QListWidgetItem(ui->secenekler);
    girisItem->setSizeHint(girisSecenegi->sizeHint());

    ui->secenekler->setItemWidget(girisItem, girisSecenegi);

    QWidget *tumSarkilarSecenegi = new QWidget(this);
    QHBoxLayout *tumSarkilarSecenegiLayout = new QHBoxLayout(tumSarkilarSecenegi);
    tumSarkilarSecenegi->setMinimumSize(QSize(0, 40));

    QPushButton *tumSarkilar = new QPushButton("Tüm Şarkılar");
    tumSarkilarSecenegiLayout->addWidget(tumSarkilar);

    QListWidgetItem *tumSarkilarItem = new QListWidgetItem(ui->secenekler);
    tumSarkilarItem->setSizeHint(tumSarkilarSecenegi->sizeHint());

    ui->secenekler->setItemWidget(tumSarkilarItem, tumSarkilarSecenegi);

    QWidget *oynatmaListeleriSecenegi = new QWidget(this);
    QHBoxLayout *oynatmaListeleriSecenegiLayout = new QHBoxLayout(oynatmaListeleriSecenegi);
    oynatmaListeleriSecenegi->setMinimumSize(QSize(0, 40));

    QPushButton *oynatmaListeleri = new QPushButton("Oynatma Listeleri");
    oynatmaListeleriSecenegiLayout->addWidget(oynatmaListeleri);

    QListWidgetItem *oynatmaListeleriItem = new QListWidgetItem(ui->secenekler);
    oynatmaListeleriItem->setSizeHint(oynatmaListeleriSecenegi->sizeHint());

    ui->secenekler->setItemWidget(oynatmaListeleriItem, oynatmaListeleriSecenegi);

    connect(ui->medyaOynatmaKontrol, &QPushButton::clicked, this,&MainWindow::medyaOynatmaKontrol_clicked);
    connect(oynatici, &QMediaPlayer::positionChanged, this, &MainWindow::medyaSlider_positionChanged);
    connect(ui->medyaSlider, &QSlider::sliderMoved, this, &MainWindow::medyaSlider_sliderMoved);
    connect(ui->medyaSlider, &QSlider::actionTriggered, this, &MainWindow::medyaSlider_actionTriggered);
    connect(ui->medyaOnceki, &QPushButton::clicked, this, [this](){
        if(mod == 1)
        {
            mod = 0;

            medyaCal(false);

            mod = 1;
        }
        else
        {
            medyaCal(false);
        }
    });
    connect(ui->medyaSonraki, &QPushButton::clicked, this, [this](){
        if(mod == 1)
        {
            mod = 0;

            medyaCal(true);

            mod = 1;
        }
        else
        {
            medyaCal(true);
        }
    });
    connect(ui->medyaSesSeviyesi, &QSlider::sliderMoved, this, &MainWindow::medyaSesSeviyesi_sliderMoved);
    connect(ui->medyaMod, &QPushButton::clicked, this, &MainWindow::medyaMod_clicked);
    connect(oynatici, &QMediaPlayer::mediaStatusChanged, this, &MainWindow::oynatici_mediaStatusChanged,Qt::UniqueConnection);
    QShortcut *playPauseShortcut = new QShortcut(QKeySequence("Toggle Media Play/Pause"), this);
    connect(playPauseShortcut, &QShortcut::activated, this, [this](){
        if(oynatici->isPlaying())
        {
            ui->medyaOynatmaKontrol->setIcon(QIcon(":/medyaKontrol/assets/medyaKontrol/baslatma.png"));
            oynatici->pause();
        }
        else
        {
            ui->medyaOynatmaKontrol->setIcon(QIcon(":/medyaKontrol/assets/medyaKontrol/duraklatma.png"));
            oynatici->play();
        }
    });
    QShortcut *nextShortcut = new QShortcut(QKeySequence(Qt::Key_MediaNext), this);
    connect(nextShortcut, &QShortcut::activated, this, [this](){
        if(mod == 1)
        {
            mod = 0;

            medyaCal(true);

            mod = 1;
        }
        else
        {
            medyaCal(true);
        }
    });
    QShortcut *prevShortcut = new QShortcut(QKeySequence(Qt::Key_MediaPrevious), this);
    connect(prevShortcut, &QShortcut::activated, this, [this](){
        if(mod == 1)
        {
            mod = 0;

            medyaCal(false);

            mod = 1;
        }
        else
        {
            medyaCal(false);
        }
    });
    connect(tumSarkilar, &QPushButton::clicked, this, [this](){
        ui->anaPencereleri->setCurrentWidget(ui->medyalarPage);
        isPlaylists = false;
    });
    connect(ui->medyaArama, &QLineEdit::textChanged, this, &MainWindow::medyaArama_textChanged);
    connect(ui->medyalar, &QListWidget::itemActivated, this, &MainWindow::medyalar_itemActivated);
    connect(ui->medyaYenile, &QPushButton::clicked, this, [this](){ui->medyalar->clear();

            isReset = true;
            isPlaylists = false;

            medyalar.clear();
            sanacilar.clear();
            oynaticiSayici = 0;

            connect(
                oynatici,
                &QMediaPlayer::mediaStatusChanged,
                this,
                &MainWindow::oynatici_mediaStatusChanged,
                Qt::UniqueConnection
                );
            oynatici->stop();
            gecerliIndex = 0;
            kullaniciKontrol = false;
            mod = 0;

            this->setVisible(false);

            medyalariListele();

            isReset = false;});
    connect(giris, &QPushButton::clicked, this, [this](){ui->anaPencereleri->setCurrentWidget(ui->girisPage);});
    connect(oynatmaListeleri, &QPushButton::clicked, this, &MainWindow::oynatmaListeleri_clicked);
    connect(ui->playlistsArama, &QLineEdit::textChanged, this, &MainWindow::medyaArama_textChanged);
    connect(ui->actionEkle, &QAction::triggered, this, [this](){this->setVisible(false); oynatmaListesiCreate->oynatmaListesiOlusturmaBaslangic();});
    connect(ui->actionKald_r, &QAction::triggered, this, [this](){this->setVisible(false); oynatmaListesiDelete->oynatmaListesiSilmeBaslangic();});
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::medyaOynatmaKontrol_clicked()
{
    if(oynatici->isPlaying())
    {
        ui->medyaOynatmaKontrol->setIcon(QIcon(":/medyaKontrol/assets/medyaKontrol/baslatma.png"));
        oynatici->pause();
    }
    else
    {
        ui->medyaOynatmaKontrol->setIcon(QIcon(":/medyaKontrol/assets/medyaKontrol/duraklatma.png"));
        oynatici->play();
    }

}

void MainWindow::medyalariListele()
{
    medyalar.clear();
    sanacilar.clear();
    oynaticiSayici = 0;

    std::vector<long long> medyalarZaman;
    int medyalarZamanCount = 0;

    for(auto i: std::filesystem::directory_iterator("../musics"))
    {
        medyalarZaman.emplace_back(i.last_write_time().time_since_epoch().count());

        medyalarZamanCount++;
    }

    delete yuklenmeEkrani;
    yuklenmeEkrani = new QProgressDialog("Yükleniyor...", "İptal", 0, medyalarZamanCount);
    connect(yuklenmeEkrani, &QProgressDialog::canceled, this, [](){exit(0);});

    std::sort(medyalarZaman.rbegin(), medyalarZaman.rend());

    for(auto i: medyalarZaman)
    {
        for (auto ii : std::filesystem::directory_iterator("../musics")) {
            if(i == ii.last_write_time().time_since_epoch().count())
            {
                medyalar.emplace_back(ii.path());

                break;
            }
        }
    }

    int gecmisMedyaSayisi = 0;

    QFile iniFile("../info.ini");
    if(iniFile.open(QFile::OpenModeFlag::ReadOnly))
    {
        QTextStream st(&iniFile);
        st.setEncoding(QStringConverter::Utf8);

        gecmisMedyaSayisi = st.readLine().toInt();

        iniFile.close();
    }

    if(medyalarZamanCount == gecmisMedyaSayisi)
    {
        oynatici->setSource(QUrl::fromLocalFile(QString::fromStdWString(medyalar.at(0)).replace("../musics\\","../musics/")));
    }
    else
    {
        QFile iniFile2("../info.ini");

        if(iniFile2.open(QFile::OpenModeFlag::WriteOnly))
        {
            iniFile2.write(QString::number(medyalarZamanCount).toStdString().c_str());

            iniFile2.close();
        }

        for (auto i : std::filesystem::directory_iterator("../thumbnails/")) {
            QFile::remove(i.path());
        }

        oynatici->setSource(QUrl::fromLocalFile(QString::fromStdWString(medyalar.at(0)).replace("../musics\\","../musics/")));
    }

}

void MainWindow::medyaSlider_positionChanged(qint64 deger)
{
    if(!kullaniciKontrol && !isReset)
    {
        long long maxMusicSaniyesi = oynatici->duration();

        double musicYuzdeHesaplama = (double)deger / (double)maxMusicSaniyesi;

        ui->medyaSlider->setProperty("value", (int)(musicYuzdeHesaplama * 100));

        int guncelSaniye = (int)((float)oynatici->position() / 1000);

        int saat = 0;
        int dakika = 0;

        while(guncelSaniye >= 60)
        {
            guncelSaniye -= 60;
            dakika++;
        }
        while(dakika >= 60)
        {
            dakika -= 60;
            saat++;
        }

        std::string prompt;

        if(saat < 10)
        {
            prompt += "0";
            prompt += saat + 48;
        }
        else
        {
            prompt += (saat / 10) + 48;
            prompt += ((((float)saat / (float)10) - ((int)saat / (int)10)) * 10) + 48;
        }
        prompt += ":";
        if(dakika < 10)
        {
            prompt += "0";
            prompt += dakika + 48;
        }
        else
        {
            prompt += (dakika / 10) + 48;
            prompt += ((((float)dakika / (float)10) - ((int)dakika / (int)10)) * 10) + 48;
        }
        prompt += ":";
        if(guncelSaniye < 10)
        {
            prompt += "0";
            prompt += guncelSaniye + 48;
        }
        else
        {
            prompt += (guncelSaniye / 10) + 48;
            prompt += ((((float)guncelSaniye / (float)10) - ((int)guncelSaniye / (int)10)) * 10) + 48;
        }

        ui->medyaGuncelSure->setProperty("text", prompt.c_str());

        guncelSaniye = (int)((float)oynatici->duration() / 1000);

        saat = 0;
        dakika = 0;

        while(guncelSaniye >= 60)
        {
            guncelSaniye -= 60;
            dakika++;
        }
        while(dakika >= 60)
        {
            dakika -= 60;
            saat++;
        }

        prompt = "";

        if(saat < 10)
        {
            prompt += "0";
            prompt += saat + 48;
        }
        else
        {
            prompt += (saat / 10) + 48;
            prompt += ((((float)saat / (float)10) - ((int)saat / (int)10)) * 10) + 48;
        }
        prompt += ":";
        if(dakika < 10)
        {
            prompt += "0";
            prompt += dakika + 48;
        }
        else
        {
            prompt += (dakika / 10) + 48;
            prompt += ((((float)dakika / (float)10) - ((int)dakika / (int)10)) * 10) + 48;
        }
        prompt += ":";
        if(guncelSaniye < 10)
        {
            prompt += "0";
            prompt += guncelSaniye + 48;
        }
        else
        {
            prompt += (guncelSaniye / 10) + 48;
            prompt += ((((float)guncelSaniye / (float)10) - ((int)guncelSaniye / (int)10)) * 10) + 48;
        }

        ui->medyaSuresi->setProperty("text", prompt.c_str());

        if(oynatici->position() == oynatici->duration())
        {
            medyaCal(true);
        }

        if(oynatici->metaData().value(QMediaMetaData::ThumbnailImage).isNull())
        {
            ui->medyaICon->setPixmap(QPixmap(":/medyaKontrol/assets/medyaKontrol/NoMedia.png"));
        }
        else
        {
            ui->medyaICon->setPixmap(QPixmap::fromImage(oynatici->metaData().value(QMediaMetaData::ThumbnailImage).value<QImage>()));
        }

        if(!isPlaylists)
        {
            ui->medyaIsim->setText(QString::fromStdWString(medyalar.at(gecerliIndex)).remove("../musics\\").remove(".mp3"));
            ui->medyaSanatci->setText(QString::fromStdWString(sanacilar.at(gecerliIndex)));
            ui->medyalar->setCurrentRow(gecerliIndex);
        }
        else
        {
            ui->medyaIsim->setText(QString::fromStdWString(playlistsMedyalar.at(gecerliIndex)).remove("../musics\\").remove(".mp3"));
            ui->medyaSanatci->setText(QString::fromStdWString(playlistsSanacilar.at(gecerliIndex)));
            ui->playlistsList->setCurrentRow(gecerliIndex);
        }
    }
}

void MainWindow::medyaSlider_sliderMoved(int deger)
{
    kullaniciKontrol = true;

    oynatici->setPosition(((oynatici->duration() * 1) / 100 ) * deger);

    kullaniciKontrol = false;
}
void MainWindow::medyaSlider_actionTriggered(int deger)
{
    std::thread *medyaSlider_actionTriggered_2 = new std::thread(
    [this]()
    {
        kullaniciKontrol = true;

        oynatici->setPosition(((oynatici->duration() * 1) / 100 ) * ui->medyaSlider->value());

        QThread::msleep(500);

        kullaniciKontrol = false;
    });
}

void MainWindow::medyaCal(bool ileriMi)
{
    if(!isPlaylists)
    {
        if(mod == 0)
        {
            if(ileriMi)
            {
                if(gecerliIndex >= medyalar.count() - 1)
                {
                    gecerliIndex = 0;
                }
                else
                {
                    gecerliIndex++;
                }
            }
            if(!ileriMi)
            {
                if(gecerliIndex <= 0)
                {
                    gecerliIndex = medyalar.count() - 1;
                }
                else
                {
                    gecerliIndex--;
                }
            }
        }
        else if(mod == 2)
        {
            std::random_device cihaz;
            std::mt19937 mt(cihaz());
            std::uniform_int_distribution<int> randomer(1, medyalar.count() - 1);

            gecerliIndex = randomer(mt);
        }

        oynatici->setSource(QUrl::fromLocalFile(QString::fromStdWString(medyalar.at(gecerliIndex)).replace("../musics\\","../musics/")));

        oynatici->play();
    }
    else
    {
        if(mod == 0)
        {
            if(ileriMi)
            {
                if(gecerliIndex >= playlistsMedyalar.count() - 1)
                {
                    gecerliIndex = 0;
                }
                else
                {
                    gecerliIndex++;
                }
            }
            if(!ileriMi)
            {
                if(gecerliIndex <= 0)
                {
                    gecerliIndex = playlistsMedyalar.count() - 1;
                }
                else
                {
                    gecerliIndex--;
                }
            }
        }
        else if(mod == 2)
        {
            std::random_device cihaz;
            std::mt19937 mt(cihaz());
            std::uniform_int_distribution<int> randomer(1, playlistsMedyalar.count() - 1);

            gecerliIndex = randomer(mt);
        }

        oynatici->setSource(QUrl::fromLocalFile(QString::fromStdWString(playlistsMedyalar.at(gecerliIndex)).replace("../musics\\","../musics/")));

        oynatici->play();
    }

}

void MainWindow::medyaSesSeviyesi_sliderMoved(int deger)
{
    cikis->setVolume((float)ui->medyaSesSeviyesi->value() / (float)100);
}

void MainWindow::medyaMod_clicked()
{
    if(mod == 0)
    {
        ui->medyaMod->setIcon(QIcon(":/medyaKontrol/assets/medyaKontrol/tekrar.png"));

        mod = 1;
    }
    else if(mod == 1)
    {
        ui->medyaMod->setIcon(QIcon(":/medyaKontrol/assets/medyaKontrol/karisik.png"));

        mod = 2;
    }
    else if(mod == 2)
    {
        ui->medyaMod->setIcon(QIcon(":/medyaKontrol/assets/medyaKontrol/dongu.png"));

        mod = 0;
    }

}

void MainWindow::oynatici_mediaStatusChanged(QMediaPlayer::MediaStatus status)
{

    if(status != QMediaPlayer::LoadedMedia)
        return;

    // Güvenlik kontrolü
    if(!isPlaylists)
    {
        if(oynaticiSayici < 0 ||
            oynaticiSayici >= medyalar.count())
        {
            return;
        }
    }
    else
    {
        if(oynaticiSayici < 0 ||
            oynaticiSayici >= playlistsMedyalar.count())
        {
            return;
        }
    }


    if(status == QMediaPlayer::MediaStatus::LoadedMedia)
    {
        if(!isPlaylists)
        {
            if(!oynatici->metaData().value(QMediaMetaData::ThumbnailImage).isNull())
            {
                oynatici->metaData().value(QMediaMetaData::ThumbnailImage).value<QImage>().save("../temp.jpeg");
                QImageReader rd = QImageReader("../temp.jpeg");
                rd.setScaledSize(QSize(26,26));
                rd.read().save(QString::fromStdWString(medyalar.at(oynaticiSayici)).replace("../musics\\","../thumbnails/").replace(".mp3", ".jpeg"));
            }
            else
            {
                QFile::remove(QString::fromStdWString(medyalar.at(oynaticiSayici)).replace("../musics\\", "../thumbnails/").replace(".mp3", ".png"));
                QFile::copy(":/medyaKontrol/assets/medyaKontrol/NoMedia.png", QString::fromStdWString(medyalar.at(oynaticiSayici)).replace("../musics\\", "../thumbnails/").replace(".mp3", ".png"));
            }
            if(!oynatici->metaData().value(QMediaMetaData::ContributingArtist).isNull())
            {
                sanacilar.append(oynatici->metaData().value(QMediaMetaData::ContributingArtist).value<QString>().toStdWString());
            }
            else
            {
                sanacilar.append(L"Belirtilmemiş");
            }

            yuklenmeEkrani->setValue(yuklenmeEkrani->value() + 1);
            yuklenmeEkrani->setWindowIcon(QIcon(":/medyaKontrol/assets/medyaKontrol/NoMedia.png"));

            oynaticiSayici++;

            if(oynaticiSayici < medyalar.count())
            {
                oynatici->setSource(QUrl::fromLocalFile(QString::fromStdWString(medyalar.at(oynaticiSayici)).replace("../musics\\","../musics/")));
            }
            else
            {
                oynatici_mediaStatusChanged_altSistem();
            }
        }
        else
        {
            if(!oynatici->metaData().value(QMediaMetaData::ThumbnailImage).isNull())
            {
                oynatici->metaData().value(QMediaMetaData::ThumbnailImage).value<QImage>().save("../temp.jpeg");
                QImageReader rd = QImageReader("../temp.jpeg");
                rd.setScaledSize(QSize(26,26));
                rd.read().save(QString::fromStdWString(playlistsMedyalar.at(oynaticiSayici)).replace("../musics\\","../playlistTumbnails/").replace(".mp3", ".jpeg"));
            }
            else
            {
                QFile::remove(QString::fromStdWString(playlistsMedyalar.at(oynaticiSayici)).replace("../musics\\", "../playlistTumbnails/").replace(".mp3", ".png"));
                QFile::copy(":/medyaKontrol/assets/medyaKontrol/NoMedia.png", QString::fromStdWString(playlistsMedyalar.at(oynaticiSayici)).replace("../musics\\", "../playlistTumbnails/").replace(".mp3", ".png"));
            }

            if(!oynatici->metaData().value(QMediaMetaData::ContributingArtist).isNull())
            {
                playlistsSanacilar.append(oynatici->metaData().value(QMediaMetaData::ContributingArtist).value<QString>().toStdWString());
            }
            else
            {
                playlistsSanacilar.append(L"Belirtilmemiş");
            }

            yuklenmeEkrani->setValue(yuklenmeEkrani->value() + 1);
            yuklenmeEkrani->setWindowIcon(QIcon(":/medyaKontrol/assets/medyaKontrol/NoMedia.png"));

            oynaticiSayici++;

            if(oynaticiSayici < playlistsMedyalar.count())
            {
                oynatici->setSource(QUrl::fromLocalFile(QString::fromStdWString(playlistsMedyalar.at(oynaticiSayici)).replace("../musics\\","../musics/")));
            }
            else
            {
                oynatici_mediaStatusChanged_altSistem();
            }
        }
    }
}

void MainWindow::oynatici_mediaStatusChanged_altSistem()
{
    disconnect(oynatici, &QMediaPlayer::mediaStatusChanged, this, &MainWindow::oynatici_mediaStatusChanged);

    //int imageCount = 0;
    int medyaSayisi = 0;

    if(!isPlaylists)
    {
        for (auto i : std::filesystem::directory_iterator("../thumbnails/")) {
            QListWidgetItem *anaItem = new QListWidgetItem(ui->medyalar);

            QWidget *anaWidget = new QWidget(this);
            anaWidget->setToolTip(QString::fromStdWString(medyalar.at(medyaSayisi)));
            anaWidget->setObjectName("medyaInfo");
            QHBoxLayout *anaLayout = new QHBoxLayout(anaWidget);
            anaWidget->setMinimumSize(QSize(0, 40));

            QLabel *medyaIcon = new QLabel();
            medyaIcon->setPixmap(QPixmap::fromImage(QImage(QString::fromStdWString(medyalar.at(medyaSayisi)).replace("../musics\\","../thumbnails/").replace(".mp3", ".jpeg"))));
            medyaIcon->setScaledContents(true);
            medyaIcon->setMaximumSize(QSize(26,26));
            medyaIcon->setMinimumSize(QSize(26,26));
            anaLayout->addWidget(medyaIcon);

            QLabel *medyaIsim = new QLabel();
            medyaIsim->setText(QString::fromStdWString(medyalar.at(medyaSayisi)).remove("../musics\\").remove(".mp3") + "\n  " + QString::fromStdWString(sanacilar.at(medyaSayisi)));
            anaLayout->addWidget(medyaIsim);

            QPushButton *medyaOynat = new QPushButton();
            medyaOynat->setText("Oynat");
            medyaOynat->setMaximumSize(QSize(100,50));
            connect(medyaOynat, &QPushButton::clicked, this,
            [this, medyaSayisi]()
            {
                 oynatici->setSource(QUrl::fromLocalFile(QString::fromStdWString(medyalar.at(medyaSayisi)).replace("../musics\\","../musics/")));

                oynatici->play();

                gecerliIndex = medyaSayisi;
            });
            anaLayout->addWidget(medyaOynat);

            anaItem->setSizeHint(anaWidget->sizeHint());

            ui->medyalar->setItemWidget(anaItem, anaWidget);

            medyaSayisi++;
        }
    }
    else
    {
        for (auto i : std::filesystem::directory_iterator("../playlistTumbnails/")) {
            QListWidgetItem *anaItem = new QListWidgetItem(ui->playlistsList);

            QWidget *anaWidget = new QWidget(this);
            anaWidget->setToolTip(QString::fromStdWString(playlistsMedyalar.at(medyaSayisi)));
            anaWidget->setObjectName("medyaInfo");
            QHBoxLayout *anaLayout = new QHBoxLayout(anaWidget);
            anaWidget->setMinimumSize(QSize(0, 40));

            QLabel *medyaIcon = new QLabel();
            medyaIcon->setPixmap(QPixmap::fromImage(QImage(QString::fromStdWString(playlistsMedyalar.at(medyaSayisi)).replace("../musics\\","../playlistTumbnails/").replace(".mp3", ".jpeg"))));
            medyaIcon->setScaledContents(true);
            medyaIcon->setMaximumSize(QSize(26,26));
            medyaIcon->setMinimumSize(QSize(26,26));
            anaLayout->addWidget(medyaIcon);

            QLabel *medyaIsim = new QLabel();
            medyaIsim->setText(QString::fromStdWString(playlistsMedyalar.at(medyaSayisi)).remove("../musics\\").remove(".mp3") + "\n  " + QString::fromStdWString(playlistsSanacilar.at(medyaSayisi)));
            anaLayout->addWidget(medyaIsim);

            QPushButton *medyaOynat = new QPushButton();
            medyaOynat->setText("Oynat");
            medyaOynat->setMaximumSize(QSize(100,50));
            connect(medyaOynat, &QPushButton::clicked, this,
            [this, medyaSayisi]()
            {
                oynatici->setSource(QUrl::fromLocalFile(QString::fromStdWString(playlistsMedyalar.at(medyaSayisi)).replace("../musics\\","../musics/")));

                oynatici->play();

                gecerliIndex = medyaSayisi;
            });
            anaLayout->addWidget(medyaOynat);

            anaItem->setSizeHint(anaWidget->sizeHint());

            ui->playlistsList->setItemWidget(anaItem, anaWidget);

            medyaSayisi++;
        }
    }

    yuklenmeEkrani->reset();

    delete yuklenmeEkrani;

    show();
}

void MainWindow::medyaArama_textChanged(QString deger)
{
    if(!isPlaylists)
    {
        ui->medyalar->clear();

        for (std::wstring i: medyalar) {
            if(QString::fromStdWString(i).toUpper().contains(deger.toUpper()))
            {
                QListWidgetItem *anaItem = new QListWidgetItem(ui->medyalar);

                QWidget *anaWidget = new QWidget(this);
                anaWidget->setToolTip(QString::fromStdWString(i));
                anaWidget->setObjectName("medyaInfo");
                QHBoxLayout *anaLayout = new QHBoxLayout(anaWidget);
                anaWidget->setMinimumSize(QSize(0, 40));

                QLabel *medyaIcon = new QLabel();
                medyaIcon->setPixmap(QPixmap::fromImage(QImage(QString::fromStdWString(i).replace("../musics\\","../thumbnails/").replace(".mp3", ".jpeg"))));
                medyaIcon->setScaledContents(true);
                medyaIcon->setMaximumSize(QSize(26,26));
                medyaIcon->setMinimumSize(QSize(26,26));
                anaLayout->addWidget(medyaIcon);

                QLabel *medyaIsim = new QLabel();
                medyaIsim->setText(QString::fromStdWString(i).remove("../musics\\").remove(".mp3") + "\n  " + QString::fromStdWString(sanacilar.at(std::distance(medyalar.begin(),std::find(medyalar.begin(), medyalar.end(), i)))));
                anaLayout->addWidget(medyaIsim);

                QPushButton *medyaOynat = new QPushButton();
                medyaOynat->setText("Oynat");
                medyaOynat->setMaximumSize(QSize(100,50));
                connect(medyaOynat, &QPushButton::clicked, this,
                        [this, i]()
                        {
                            oynatici->setSource(QUrl::fromLocalFile(QString::fromStdWString(i).replace("../musics\\","../musics/")));

                            oynatici->play();

                            gecerliIndex = std::distance(medyalar.begin(),std::find(medyalar.begin(), medyalar.end(), i));
                        });
                anaLayout->addWidget(medyaOynat);

                anaItem->setSizeHint(anaWidget->sizeHint());

                ui->medyalar->setItemWidget(anaItem, anaWidget);
            }
        }
    }
    else
    {
        ui->playlistsList->clear();

        for (std::wstring i: playlistsMedyalar) {
            if(QString::fromStdWString(i).toUpper().contains(deger.toUpper()))
            {
                QListWidgetItem *anaItem = new QListWidgetItem(ui->playlistsList);

                QWidget *anaWidget = new QWidget(this);
                anaWidget->setToolTip(QString::fromStdWString(i));
                anaWidget->setObjectName("medyaInfo");
                QHBoxLayout *anaLayout = new QHBoxLayout(anaWidget);
                anaWidget->setMinimumSize(QSize(0, 40));

                QLabel *medyaIcon = new QLabel();
                medyaIcon->setPixmap(QPixmap::fromImage(QImage(QString::fromStdWString(i).replace("../musics\\","../playlistTumbnails/").replace(".mp3", ".jpeg"))));
                medyaIcon->setScaledContents(true);
                medyaIcon->setMaximumSize(QSize(26,26));
                medyaIcon->setMinimumSize(QSize(26,26));
                anaLayout->addWidget(medyaIcon);

                QLabel *medyaIsim = new QLabel();
                medyaIsim->setText(QString::fromStdWString(i).remove("../musics\\").remove(".mp3") + "\n  " + QString::fromStdWString(playlistsSanacilar.at(std::distance(playlistsMedyalar.begin(),std::find(playlistsMedyalar.begin(), playlistsMedyalar.end(), i)))));
                anaLayout->addWidget(medyaIsim);

                QPushButton *medyaOynat = new QPushButton();
                medyaOynat->setText("Oynat");
                medyaOynat->setMaximumSize(QSize(100,50));
                connect(medyaOynat, &QPushButton::clicked, this,
                        [this, i]()
                        {
                            oynatici->setSource(QUrl::fromLocalFile(QString::fromStdWString(i).replace("../musics\\","../musics/")));

                            oynatici->play();

                            gecerliIndex = std::distance(playlistsMedyalar.begin(),std::find(playlistsMedyalar.begin(), playlistsMedyalar.end(), i));
                        });
                anaLayout->addWidget(medyaOynat);

                anaItem->setSizeHint(anaWidget->sizeHint());

                ui->playlistsList->setItemWidget(anaItem, anaWidget);
            }
        }
    }

}

void MainWindow::medyalar_itemActivated(QListWidgetItem* item)
{
    oynatici->setSource(QUrl::fromLocalFile(item->listWidget()->itemWidget(item)->toolTip().replace("../musics\\","../musics/")));
    oynatici->play();
    gecerliIndex = std::distance(medyalar.begin(),std::find(medyalar.begin(), medyalar.end(), QString(item->listWidget()->itemWidget(item)->toolTip()).toStdWString()));
}

void MainWindow::oynatmaListeleri_clicked()
{
    ui->anaPencereleri->setCurrentWidget(ui->oynatmaPage);

    ui->oynatmaListeleri->clear();

    std::vector<long long> playlistZamanListesi;
    std::vector<std::wstring> playlists;

    for (auto i : std::filesystem::directory_iterator("../playlists/")) {
        playlistZamanListesi.emplace_back(i.last_write_time().time_since_epoch().count());
    }

    std::sort(playlistZamanListesi.rbegin(), playlistZamanListesi.rend());

    for(auto i: playlistZamanListesi)
    {
        for (auto ii : std::filesystem::directory_iterator("../playlists/")) {
            if(ii.last_write_time().time_since_epoch().count() == i)
            {
                playlists.emplace_back(QString(ii.path().c_str()).toStdWString());
            }
        }
    }

    for (auto i : playlists)
    {
        QWidget *oynatmaListesiSecenegi = new QWidget(this);
        QHBoxLayout *oynatmaListesiSecenegiLayout = new QHBoxLayout(oynatmaListesiSecenegi);
        oynatmaListesiSecenegi->setMinimumSize(QSize(0, 40));

        QLabel *medyaIcon = new QLabel();
        medyaIcon->setPixmap(QPixmap::fromImage(QImage(QString::fromStdWString(i).replace("../playlists/","../playlistImages/").replace(".txt", ".jpeg"))));
        medyaIcon->setScaledContents(true);
        medyaIcon->setMaximumSize(QSize(26,26));
        medyaIcon->setMinimumSize(QSize(26,26));
        oynatmaListesiSecenegiLayout->addWidget(medyaIcon);

        QLabel *medyaIsim = new QLabel();
        medyaIsim->setText(QString::fromStdWString(i).remove("../playlists/").remove(".txt"));
        oynatmaListesiSecenegiLayout->addWidget(medyaIsim);

        QPushButton *medyaOynat = new QPushButton();
        medyaOynat->setText("Aç");
        medyaOynat->setMaximumSize(QSize(100,50));
        connect(medyaOynat, &QPushButton::clicked, this,
        [this, i, medyaIcon]()
        {
            isReset = true;
            isPlaylists = true;

            playlistsMedyalar.clear();
            playlistsSanacilar.clear();
            oynaticiSayici = 0;

            for (auto i : std::filesystem::directory_iterator("../playlistTumbnails/")) {
                std::filesystem::remove(i.path());
            }

            connect(
                oynatici,
                &QMediaPlayer::mediaStatusChanged,
                this,
                &MainWindow::oynatici_mediaStatusChanged,
                Qt::UniqueConnection
            );
            oynatici->stop();
            gecerliIndex = 0;
            kullaniciKontrol = false;
            mod = 0;

            ui->anaPencereleri->setCurrentWidget(ui->playlistsPage);

            this->setVisible(false);

            ui->playlistsName->setText(QString::fromStdWString(i).replace("../playlists/","").replace(".txt",""));
            ui->playlistsIcon->setPixmap(medyaIcon->pixmap());

            medyalariListele(i);

            isReset = false;
        });
        oynatmaListesiSecenegiLayout->addWidget(medyaOynat);

        QListWidgetItem *oynatmaListesiItem = new QListWidgetItem(ui->oynatmaListeleri);
        oynatmaListesiItem->setSizeHint(oynatmaListesiSecenegi->sizeHint());

        ui->oynatmaListeleri->setItemWidget(oynatmaListesiItem, oynatmaListesiSecenegi);
    }
}

void MainWindow::medyalariListele(std::wstring playlistsName)
{
    ui->playlistsList->clear();

    QFile playlistsFile(QString::fromStdWString(playlistsName));
    char playlistsFileBuffer[1000];

    QFile dosya(QString::fromStdWString(playlistsName));
    if (dosya.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QTextStream in(&dosya);
        in.setEncoding(QStringConverter::Utf8); // UTF-8 garantisi

        while (!in.atEnd()) {
            QString satir = in.readLine();

            playlistsMedyalar.append(satir.toStdWString());
        }

        dosya.close();
    }

    yuklenmeEkrani = new QProgressDialog("Yükleniyor...", "İptal", 0, playlistsMedyalar.count());


    oynatici->setSource(QUrl::fromLocalFile(QString::fromStdWString(playlistsMedyalar.at(0)).replace("../musics\\","../musics/")));
    //oynatici->setSource(QUrl::fromLocalFile(QString::fromStdWString(medyalar       .at(0)).replace("../musics\\","../musics/")));

    //qDebug() << QUrl::fromLocalFile(QString::fromStdWString(playlistsMedyalar.at(0)).replace("../musics\\","../musics/"));
}

void oynatmaListesiOlusturma::oynatmaListesiOlusturmaBitis()
{
    mw2->oynatmaListeleri_clicked();

    mw2->setVisible(true);
}

void oynatmaListesiSilme::oynatmaListesiSilmeBitis()
{
    mw2->oynatmaListeleri_clicked();

    mw2->setVisible(true);
}