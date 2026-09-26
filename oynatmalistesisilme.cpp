#include "oynatmalistesisilme.h"
#include "ui_oynatmalistesisilme.h"

QVector<std::wstring> seciliMedyalar2;

oynatmaListesiSilme::oynatmaListesiSilme(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::oynatmaListesiSilme)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::WindowType::CoverWindow | Qt::WindowType::WindowStaysOnTopHint);

    this->setWindowIcon(QIcon(":/medyaKontrol/assets/medyaKontrol/NoMedia.png"));

    connect(ui->sil, &QPushButton::clicked, this, &oynatmaListesiSilme::sil_clicked);
    connect(ui->cikis, &QPushButton::clicked, this, [this](){this->setVisible(false); oynatmaListesiSilmeBitis();});
}

oynatmaListesiSilme::~oynatmaListesiSilme()
{
    delete ui;
}

void oynatmaListesiSilme::oynatmaListesiSilmeBaslangic()
{
    ui->oynatmListeler->clear();

    std::vector<long long> medyalarZaman;
    int medyalarZamanCount = 0;

    QVector<std::wstring> medyalar2;

    for(auto i: std::filesystem::directory_iterator("../playlists/"))
    {
        if(!QString(i.path().filename().c_str()).contains("Favoriler.txt"))
        {
            medyalarZaman.emplace_back(i.last_write_time().time_since_epoch().count());

            medyalarZamanCount++;
        }
    }

    std::sort(medyalarZaman.rbegin(), medyalarZaman.rend());

    for(auto i: medyalarZaman)
    {
        for (auto ii : std::filesystem::directory_iterator("../playlists/")) {
            if(i == ii.last_write_time().time_since_epoch().count())
            {
                medyalar2.append(QString(ii.path().c_str()).toStdWString());

                break;
            }
        }
    }

    for(auto i: medyalar2)
    {
        QWidget *anaWidget = new QWidget(this);
        QHBoxLayout *anaLayout = new QHBoxLayout(anaWidget);

        QCheckBox *check = new QCheckBox(QString::fromStdWString(i).remove("../playlists/").remove(".txt"));
        connect(check, &QCheckBox::checkStateChanged, this,
                [this, i](Qt::CheckState deger)
                {
                    if(deger == Qt::CheckState::Checked)
                    {
                        seciliMedyalar2.append(i);
                    }
                    else if(deger == Qt::CheckState::Unchecked)
                    {
                        seciliMedyalar2.remove(std::distance(seciliMedyalar2.begin() ,std::find(seciliMedyalar2.begin(), seciliMedyalar2.end(), i)));
                    }
                });
        anaLayout->addWidget(check);

        QListWidgetItem *anaItem = new QListWidgetItem(ui->oynatmListeler);

        anaItem->setSizeHint(anaWidget->sizeHint());

        ui->oynatmListeler->setItemWidget(anaItem, anaWidget);
    }

    this->show();
}

void oynatmaListesiSilme::sil_clicked()
{
    if(!seciliMedyalar2.isEmpty())
    {
        for (auto i : seciliMedyalar2) {
            QFile::remove(QString::fromStdWString(i));
            QFile::remove(QString::fromStdWString(i).replace("playlists", "playlistImages").replace("txt", "jpeg"));
        }

        this->setVisible(false);

        QMessageBox basarili(QMessageBox::Icon::Information, "Başarılı", "Başarılı! Oynatma listesi silinmiştir.");
        basarili.setWindowIcon(QIcon(":/medyaKontrol/assets/medyaKontrol/NoMedia.png"));

        basarili.exec();

        oynatmaListesiSilmeBitis();
    }
    else
    {
        QMessageBox uyari1(QMessageBox::Icon::Critical, "Seçili Oynatma Listesi Bulunamadı", "Seçili oynatma listesi bulunamadı! Lütfen oynatma listesi seçiniz.");
        uyari1.setWindowIcon(QIcon(":/medyaKontrol/assets/medyaKontrol/NoMedia.png"));

        uyari1.exec();
    }
}