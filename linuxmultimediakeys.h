#ifndef LINUXMULTIMEDIAKEYS_H
#define LINUXMULTIMEDIAKEYS_H

#include <QObject>
#include <QSocketNotifier>
#include <QString>
#include <QVector>

class linuxmultimediakeys : public QObject {
    Q_OBJECT

public:
    explicit linuxmultimediakeys(QObject *parent = nullptr);
    ~linuxmultimediakeys() override;

    void startWatching();

signals:
    void mediaKeyPressed(int keyCode, const QString &keyName);

private:
    struct WatchedDevice {
        int fd = -1;
        QString path;
        QString name;
        QSocketNotifier *notifier = nullptr;
    };

    bool isKeyboardOrMediaDevice(int fd) const;
    static QString mediaKeyName(int code);
    void handleReadyRead(int fd);

    QVector<WatchedDevice> m_devices;
};

#endif
