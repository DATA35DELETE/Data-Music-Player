#include "oynatmalistesiolusturma.h"
#include "ui_oynatmalistesiolusturma.h"

QVector<std::wstring> seciliMedyalar;

oynatmaListesiOlusturma::oynatmaListesiOlusturma(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::oynatmaListesiOlusturma)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::WindowType::CoverWindow | Qt::WindowType::WindowStaysOnTopHint);

    this->setWindowIcon(QIcon(":/medyaKontrol/assets/medyaKontrol/NoMedia.png"));

    connect(ui->ekleIcon, &QPushButton::clicked, this, &oynatmaListesiOlusturma::oynatmaListesi_fotoSec);
    connect(ui->ekle, &QPushButton::clicked, this, &oynatmaListesiOlusturma::ekle_clicked);
    connect(ui->cikis, &QPushButton::clicked, this, [this](){this->setVisible(false); oynatmaListesiOlusturmaBitis();});
}

oynatmaListesiOlusturma::~oynatmaListesiOlusturma()
{
    delete ui;
}

void oynatmaListesiOlusturma::oynatmaListesiOlusturmaBaslangic()
{
    ui->medyalar->clear();
    ui->isim->setText("oynatmaListesi");
    ui->ekleIcon->setIcon(QIcon(":/medyaKontrol/assets/medyaKontrol/NoMedia.png"));

    std::vector<long long> medyalarZaman;
    int medyalarZamanCount = 0;

    QVector<std::wstring> medyalar;

    for(auto i: std::filesystem::directory_iterator("../musics"))
    {
        medyalarZaman.emplace_back(i.last_write_time().time_since_epoch().count());

        medyalarZamanCount++;
    }

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

    for(auto i: medyalar)
    {
        QWidget *anaWidget = new QWidget(this);
        QHBoxLayout *anaLayout = new QHBoxLayout(anaWidget);

        QCheckBox *check = new QCheckBox(QString::fromStdWString(i).remove("../musics\\").remove(".mp3"));
        connect(check, &QCheckBox::checkStateChanged, this,
        [this, i](Qt::CheckState deger)
        {
            if(deger == Qt::CheckState::Checked)
            {
                seciliMedyalar.append(i);
            }
            else if(deger == Qt::CheckState::Unchecked)
            {
                seciliMedyalar.remove(std::distance(seciliMedyalar.begin() ,std::find(seciliMedyalar.begin(), seciliMedyalar.end(), i)));
            }
        });
        anaLayout->addWidget(check);

        QListWidgetItem *anaItem = new QListWidgetItem(ui->medyalar);

        anaItem->setSizeHint(anaWidget->sizeHint());

        ui->medyalar->setItemWidget(anaItem, anaWidget);
    }

    this->show();
}

void oynatmaListesiOlusturma::oynatmaListesi_fotoSec()
{
    QFileDialog fotoSecme;

    fotoSecme.setWindowIcon(QIcon(":/medyaKontrol/assets/medyaKontrol/NoMedia.png"));
    fotoSecme.setNameFilter("Fotoğraflar (*.png *.jpeg *.jpg)");

    fotoSecme.exec();

    if(!fotoSecme.selectedFiles().isEmpty())
    {
        ui->ekleIcon->setIcon(QIcon(fotoSecme.selectedFiles().at(0)));
        ui->ekleIcon->setToolTip(fotoSecme.selectedFiles().at(0));
    }
    else
    {
        ui->ekleIcon->setIcon(QIcon(":/medyaKontrol/assets/medyaKontrol/NoMedia.png"));
        ui->ekleIcon->setToolTip("notFound");
    }
}

void oynatmaListesiOlusturma::ekle_clicked()
{
    if(!seciliMedyalar.isEmpty())
    {
        if(!ui->isim->text().isEmpty())
        {
            QString playlistName = "../playlists/";
            playlistName += ui->isim->text();
            playlistName += ".txt";

            std::ofstream playlistFile(playlistName.toStdString().c_str());

            for (auto i : seciliMedyalar) {
                playlistFile << QString(i.c_str()).toStdString();

                if(i != seciliMedyalar.last())
                {
                    playlistFile << "\n";
                }
            }

            if(ui->ekleIcon->toolTip() == "notFound")
            {
                QFile::copy(":/medyaKontrol/assets/medyaKontrol/NoMedia.png", playlistName.replace("playlists", "playlistImages").replace(".txt", ".jpeg"));
            }
            else
            {
                QImageReader playlistImage(ui->ekleIcon->toolTip());
                playlistImage.setScaledSize(QSize(128,128));

                playlistImage.read().save(playlistName.replace("playlists", "playlistImages").replace(".txt", ".jpeg"), "JPEG");
            }

            this->setVisible(false);

            QMessageBox basarili(QMessageBox::Icon::Information, "Başarılı", "Başarılı! Oynatma listesiniz eklenmiştir.");
            basarili.setWindowIcon(QIcon(":/medyaKontrol/assets/medyaKontrol/NoMedia.png"));

            basarili.exec();

            oynatmaListesiOlusturmaBitis();
        }
        else
        {
            QMessageBox uyari2(QMessageBox::Icon::Critical, "İsmi Boş Olamaz", "İsmi boş olamaz! Lütfen bir isim giriniz.");
            uyari2.setWindowIcon(QIcon(":/medyaKontrol/assets/medyaKontrol/NoMedia.png"));

            uyari2.exec();
        }
    }
    else
    {
        QMessageBox uyari1(QMessageBox::Icon::Critical, "Seçili Medya Bulunamadı", "Seçili medya bulunamadı! Lütfen medya seçiniz.");
        uyari1.setWindowIcon(QIcon(":/medyaKontrol/assets/medyaKontrol/NoMedia.png"));

        uyari1.exec();
    }
}