
#ifndef MPRISPLAYER_H
#define MPRISPLAYER_H

#include <QObject>
#include <QDBusAbstractAdaptor>
#include <QDBusConnection>
#include <QDBusObjectPath>
#include <QVariantMap>
#include <QStringList>

class MprisPlayer : public QObject
{
    Q_OBJECT

public:
    explicit MprisPlayer(QObject *parent = nullptr);
    bool start();

signals:
    void playRequested();
    void pauseRequested();
    void playPauseRequested();
    void nextRequested();
    void previousRequested();
};

// MPRIS Root
class MprisRoot : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.mpris.MediaPlayer2")

    Q_PROPERTY(bool CanQuit READ canQuit)
    Q_PROPERTY(bool CanRaise READ canRaise)
    Q_PROPERTY(bool HasTrackList READ hasTrackList)
    Q_PROPERTY(QString Identity READ identity)
    Q_PROPERTY(QString DesktopEntry READ desktopEntry)
    Q_PROPERTY(QStringList SupportedUriSchemes READ supportedUriSchemes)
    Q_PROPERTY(QStringList SupportedMimeTypes READ supportedMimeTypes)

public:
    explicit MprisRoot(QObject *parent);

    bool canQuit() const { return false; }
    bool canRaise() const { return false; }
    bool hasTrackList() const { return false; }

    QString identity() const { return "dataMusicPlayer"; }
    QString desktopEntry() const { return "dataMusicPlayer"; }

    QStringList supportedUriSchemes() const { return {"file"}; }
    QStringList supportedMimeTypes() const { return {"audio/mpeg", "audio/ogg"}; }

public slots:
    void Raise();
    void Quit();
};

// MPRIS Player
class MprisInterface : public QDBusAbstractAdaptor
{
    Q_OBJECT
    Q_CLASSINFO("D-Bus Interface", "org.mpris.MediaPlayer2.Player")

    Q_PROPERTY(QString PlaybackStatus READ playbackStatus)
    Q_PROPERTY(double Rate READ rate WRITE setRate)
    Q_PROPERTY(QVariantMap Metadata READ metadata)
    Q_PROPERTY(double Volume READ volume WRITE setVolume)
    Q_PROPERTY(qlonglong Position READ position)

    Q_PROPERTY(double MinimumRate READ minimumRate)
    Q_PROPERTY(double MaximumRate READ maximumRate)

    Q_PROPERTY(bool CanGoNext READ canGoNext)
    Q_PROPERTY(bool CanGoPrevious READ canGoPrevious)
    Q_PROPERTY(bool CanPlay READ canPlay)
    Q_PROPERTY(bool CanPause READ canPause)
    Q_PROPERTY(bool CanSeek READ canSeek)
    Q_PROPERTY(bool CanControl READ canControl)

public:
    explicit MprisInterface(MprisPlayer *parent);

    QString playbackStatus() const { return "Stopped"; }

    double rate() const { return 1.0; }
    void setRate(double) {}

    QVariantMap metadata() const;

    double volume() const { return 1.0; }
    void setVolume(double) {}

    qlonglong position() const { return 0; }

    double minimumRate() const { return 1.0; }
    double maximumRate() const { return 1.0; }

    bool canGoNext() const { return true; }
    bool canGoPrevious() const { return true; }
    bool canPlay() const { return true; }
    bool canPause() const { return true; }
    bool canSeek() const { return false; }
    bool canControl() const { return true; }

public slots:
    void PlayPause();
    void Next();
    void Previous();
    void Play();
    void Pause();
};

#endif
