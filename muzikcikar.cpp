#include "muzikcikar.h"
#include "ui_muzikcikar.h"

#include <QDirIterator>
#include <QFileInfo>
#include <QCheckBox>

QVector<QString> seciliMedyalar;

muzikCikar::muzikCikar(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::muzikCikar)
{
    ui->setupUi(this);

    connect(ui->cikis, &QPushButton::clicked, this, [this](){this->setVisible(false); muzikCikarOlusturmaCikis();});
    connect(ui->cikar, &QPushButton::clicked, this, [this]()
            {   this->setVisible(false);
                muzikCikarOlusturmaCikis(seciliMedyalar);
            }
            );
}

muzikCikar::~muzikCikar()
{
    delete ui;
}

void muzikCikar::muzikCikarOlusturmaBaslangic()
{
    ui->medyalar->clear();

    QVector<long long> medyaZamanlari;
    QVector<QString> medyalar;

    QDirIterator it("../musics");
    while(it.hasNext())
    {
        it.next();

        medyaZamanlari.append(QFileInfo(it.filePath()).fileTime(QFile::FileBirthTime).toMSecsSinceEpoch());
    }

    std::sort(medyaZamanlari.rbegin(), medyaZamanlari.rend());

    for(auto i: medyaZamanlari)
    {
        QDirIterator itt("../musics");
        while(itt.hasNext())
        {
            itt.next();

            if(i == QFileInfo(itt.filePath()).fileTime(QFile::FileBirthTime).toMSecsSinceEpoch())
            {
                medyalar.append(QString(itt.filePath()));

                break;
            }
        }
    }

    for (auto i : medyalar) {
        QWidget *anaWidget = new QWidget(this);
        QHBoxLayout *anaLayout = new QHBoxLayout(anaWidget);

        QCheckBox *check = new QCheckBox(i.remove("../musics/").remove(".mp3"));
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