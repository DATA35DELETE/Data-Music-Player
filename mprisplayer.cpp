#include "mprisplayer.h"

#include <QDBusError>
#include <QDebug>

MprisPlayer::MprisPlayer(QObject *parent)
    : QObject(parent)
{
    new MprisRoot(this);
    new MprisInterface(this);
}

bool MprisPlayer::start()
{
    auto bus = QDBusConnection::sessionBus();

    if (!bus.isConnected())
    {
        qDebug() << "D-Bus bağlantı hatası:"
                 << bus.lastError().message();
        return false;
    }

    if (!bus.registerService("org.mpris.MediaPlayer2.dataMusic"))
    {
        qDebug() << "Service kayıt hatası:"
                 << bus.lastError().name()
                 << bus.lastError().message();
        return false;
    }

    if (!bus.registerObject(
            "/org/mpris/MediaPlayer2",
            this,
            QDBusConnection::ExportAdaptors))
    {
        qDebug() << "Object kayıt hatası:"
                 << bus.lastError().name()
                 << bus.lastError().message();

        bus.unregisterService("org.mpris.MediaPlayer2.dataMusic");
        return false;
    }

    qDebug() << "MPRIS başarıyla başlatıldı!";
    return true;
}

MprisRoot::MprisRoot(QObject *parent)
    : QDBusAbstractAdaptor(parent)
{
}

void MprisRoot::Raise() {}
void MprisRoot::Quit() {}

MprisInterface::MprisInterface(MprisPlayer *parent)
    : QDBusAbstractAdaptor(parent)
{
}

QVariantMap MprisInterface::metadata() const
{
    QVariantMap data;

    data.insert(
        "mpris:trackid",
        QVariant::fromValue(
            QDBusObjectPath("/org/mpris/MediaPlayer2/TrackList/NoTrack")
            )
        );

    return data;
}

void MprisInterface::PlayPause()
{
    emit static_cast<MprisPlayer*>(parent())->playPauseRequested();
}

void MprisInterface::Next()
{
    emit static_cast<MprisPlayer*>(parent())->nextRequested();
}

void MprisInterface::Previous()
{
    emit static_cast<MprisPlayer*>(parent())->previousRequested();
}

void MprisInterface::Play()
{
    emit static_cast<MprisPlayer*>(parent())->playRequested();
}

void MprisInterface::Pause()
{
    emit static_cast<MprisPlayer*>(parent())->pauseRequested();
}