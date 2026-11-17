#include "musicremove.h"
#include "ui_musicremove.h"

QStringList musicsPath;

QStringList musicRemoveList;

musicremove::musicremove(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::musicremove)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::WindowType::SubWindow | Qt::WindowType::WindowStaysOnTopHint);

    closeButton = ui->pushButton;
    remove = ui->musicRemoveCikar;

    removeIcon = ui->musicRemoveSearchIcon;

    connect(ui->musicPathList, &QComboBox::activated, this, &musicremove::musicPathList_activated);
    connect(ui->musicRemoveSearch, &QLineEdit::textChanged, this, &musicremove::musicRemoveSearch_textChanged);
    connect(ui->musicRemoveCikar, &QPushButton::clicked, this, [=](){ui->musicRemoveList->clear(); end(musicRemoveList);});
    connect(ui->pushButton, &QPushButton::clicked, this, [=](){this->setVisible(false);});
}

musicremove::~musicremove()
{
    delete ui;
}

void musicremove::start(QStringList mp)
{
    this->setVisible(true);

    musicsPath = mp;
    musicRemoveList.clear();

    for (auto i : musicsPath) {
        ui->musicPathList->addItem(i);
    }
}

void musicremove::musicPathList_activated(int i)
{
    if(i < 0)
        return;

    if(ui->musicPathList->itemText(i).isEmpty())
        return;

    ui->musicRemoveList->clear();

    QDir musicPathDir(ui->musicPathList->itemText(i));
    musicPathDir.setFilter(QDir::Filter::Files);
    musicPathDir.setSorting(QDir::SortFlag::Time);

    for (auto ii : musicPathDir.entryList()) {
        QListWidgetItem *item = new QListWidgetItem(ui->musicRemoveList);

        QCheckBox *mainCheck = new QCheckBox(QString(ii).remove(".mp3"));
        mainCheck->setToolTip(QString(ii).remove(".mp3"));
        connect(mainCheck, &QCheckBox::checkStateChanged, this,
        [=](Qt::CheckState state){
            if(state == Qt::CheckState::Checked)
                musicRemoveList.append(QFileInfo(QString(ii).prepend(QString(ui->musicPathList->itemText(i)).append("/"))).filePath());
            else
                musicRemoveList.removeOne(QFileInfo(QString(ii).prepend(QString(ui->musicPathList->itemText(i)).append("/"))).filePath());

            for (auto iii : musicRemoveList) {
                qDebug() << iii;
            }
        });

        ui->musicRemoveList->setItemWidget(item, mainCheck);
    }
}

void musicremove::musicRemoveSearch_textChanged(const QString& text)
{
    QListWidgetItem *item;

    for (int i = 0; i < ui->musicRemoveList->count(); ++i) {
        item = ui->musicRemoveList->item(i);

        item->setHidden(false);
    }

    for (int i = 0; i < ui->musicRemoveList->count(); ++i) {
        item = ui->musicRemoveList->item(i);

        if(ui->musicRemoveList->itemWidget(item)->toolTip().contains(text, Qt::CaseInsensitive))
            item->setHidden(false);
        else
            item->setHidden(true);
    }
}