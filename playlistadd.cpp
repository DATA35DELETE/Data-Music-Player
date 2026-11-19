#include "playlistadd.h"
#include "ui_playlistadd.h"

QStringList selectedMusics;

playlistadd::playlistadd(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::playlistadd)
{
    ui->setupUi(this);

    closeButton = ui->playlistAddClose;
    playlistIconButton = ui->playlistAddIcon;
    searchIcon = ui->label_5;

    this->setWindowFlags(Qt::WindowType::SubWindow | Qt::WindowType::WindowStaysOnTopHint);

    connect(ui->playlistAddSearch, &QLineEdit::textChanged, this, &playlistadd::playlistAddMusic_textChanged);
    connect(ui->playlistAddIcon, &QPushButton::clicked, this, &playlistadd::playlistAddIcon_clicked);
    connect(ui->playlistAddButton, &QPushButton::clicked, this, &playlistadd::playlistAddButton_clicked);
    connect(ui->playlistAddClose, &QPushButton::clicked, this, [=](){ui->playlistAddMusic->clear(); end();});
}

playlistadd::~playlistadd()
{
    delete ui;
}

void playlistadd::start(QStringList musicsPath)
{
    this->setVisible(true);

    ui->playlistAddMusic->clear();

    for (auto i : musicsPath) {
        QDir muzikYollariDir(i);
        muzikYollariDir.setSorting(QDir::SortFlag::Time);
        muzikYollariDir.setFilter(QDir::Filter::Files);

        for (auto ii : muzikYollariDir.entryList()) {
            if(!ii.endsWith(".mp3"))
                continue;

            QListWidgetItem *item = new QListWidgetItem(ui->playlistAddMusic);

            QCheckBox *mainCheck = new QCheckBox(QString(ii).remove(".mp3"));
            mainCheck->setToolTip(QString(ii).remove(".mp3"));
            connect(mainCheck, &QCheckBox::checkStateChanged, this,
                    [=](Qt::CheckState state){
                        if(state == Qt::CheckState::Checked)
                            selectedMusics.append(QFileInfo(QString(ii).prepend(QString(i).append("/"))).filePath());
                        else
                            selectedMusics.removeOne(QFileInfo(QString(ii).prepend(QString(i).append("/"))).filePath());
                    });

            for (auto iii : selectedMusics) {
                qDebug() << iii;
            }

            ui->playlistAddMusic->setItemWidget(item, mainCheck);
        }
    }
}
void playlistadd::playlistAddMusic_textChanged(const QString& text)
{
    QListWidgetItem *item;

    for (int i = 0; i < ui->playlistAddMusic->count(); ++i) {
        item = ui->playlistAddMusic->item(i);

        item->setHidden(false);
    }

    for (int i = 0; i < ui->playlistAddMusic->count(); ++i) {
        item = ui->playlistAddMusic->item(i);

        if(ui->playlistAddMusic->itemWidget(item)->toolTip().contains(text, Qt::CaseInsensitive))
            item->setHidden(false);
        else
            item->setHidden(true);
    }
}

void playlistadd::playlistAddIcon_clicked()
{
    QFileDialog fd;
    fd.setNameFilter(QString("Resim Dosyaları (*.jpeg *.jpg *.png)"));

    fd.exec();

    if(!fd.selectedFiles().isEmpty())
    {
        ui->playlistAddIcon->setIcon(QIcon(fd.selectedFiles().at(0)));

        ui->playlistAddIcon->setToolTip(fd.selectedFiles().at(0));
    }
    else
    {
        QMessageBox(QMessageBox::Icon::Warning, "Bir Fotoğraf Seçilmedi", "Oynatma listesi için ikon seçilmedi.").exec();

        ui->playlistAddIcon->setIcon(QIcon("dataTheme/icons/nota.svg.svg"));
        ui->playlistAddIcon->setToolTip("");
    }
}

void playlistadd::playlistAddButton_clicked()
{
    if(!ui->playlistAddName->text().isEmpty())
    {
        if(!selectedMusics.isEmpty())
        {
            QFile playlistFile(ui->playlistAddName->text().prepend("../playlists/").append(".txt"));
            if(playlistFile.open(QFile::OpenModeFlag::WriteOnly))
            {
                QTextStream stream(&playlistFile);

                for (auto i: selectedMusics) {
                    stream << i << '\n';
                }
            }
            playlistFile.close();

            if(!ui->playlistAddIcon->toolTip().isEmpty())
                QImage(ui->playlistAddIcon->toolTip()).save(QFileInfo(ui->playlistAddName->text()).fileName().remove(".jpeg").remove(".jpg").remove(".png").prepend("../thumbnailsPlaylists/").append(".jpeg"), "JPEG");

            QMessageBox(QMessageBox::Icon::Information, "Başarılı!", "Oynatma listesiniz başarılı bir şekilde oluşturuldu.").exec();

            end();
        }
        else
        {
            QMessageBox(QMessageBox::Icon::Critical, "Oynatma Listesini İçeriği Boş Olamaz!", "Oynatma listesi boş olmamalıdır.").exec();
        }
    }
    else
    {
        QMessageBox(QMessageBox::Icon::Critical, "Oynatma Listesinin İsmi Boş Olamaz!", "Oynatma listesinin ismi boş olmamalıdır.").exec();
    }
}
