#include "linuxmultimediakeys.h"

#include <QDebug>
#include <QDir>
#include <QHash>

#include <cstring>
#include <fcntl.h>
#include <linux/input.h>
#include <sys/ioctl.h>
#include <unistd.h>

namespace {

constexpr int kKeyBits = KEY_MAX + 1;
constexpr int kKeyLongBits = (kKeyBits + 8 * sizeof(unsigned long) - 1) / (8 * sizeof(unsigned long));

bool testBit(const unsigned long *bitmask, int bit) {
    return (bitmask[bit / (8 * sizeof(unsigned long))] >> (bit % (8 * sizeof(unsigned long)))) & 1UL;
}

const QHash<int, QString> &mediaKeyMap() {
    static const QHash<int, QString> map = {
                                             {KEY_PLAYPAUSE,    QStringLiteral("Oynat / Duraklat")},
                                             {KEY_NEXTSONG,     QStringLiteral("Sonraki Şarkı")},
                                             {KEY_PREVIOUSSONG, QStringLiteral("Önceki Şarkı")},
                                             {KEY_STOPCD,       QStringLiteral("Durdur")},
                                             {KEY_VOLUMEUP,     QStringLiteral("Ses Aç")},
                                             {KEY_VOLUMEDOWN,   QStringLiteral("Ses Kıs")},
                                             {KEY_MUTE,         QStringLiteral("Sessize Al")},
                                             };
    return map;
}

} // namespace

linuxmultimediakeys::linuxmultimediakeys(QObject *parent)
    : QObject(parent) {}

linuxmultimediakeys::~linuxmultimediakeys() {
    for (WatchedDevice &dev : m_devices) {
        delete dev.notifier;
        if (dev.fd >= 0) {
            ::close(dev.fd);
        }
    }
}

QString linuxmultimediakeys::mediaKeyName(int code) {
    return mediaKeyMap().value(code, QStringLiteral("Diğer (%1)").arg(code));
}

bool linuxmultimediakeys::isKeyboardOrMediaDevice(int fd) const {
    unsigned long evBits[(EV_MAX + 8 * sizeof(unsigned long)) / (8 * sizeof(unsigned long))];
    std::memset(evBits, 0, sizeof(evBits));

    if (::ioctl(fd, EVIOCGBIT(0, sizeof(evBits)), evBits) < 0) {
        return false;
    }
    if (!testBit(evBits, EV_KEY)) {
        return false;
    }

    unsigned long keyBits[kKeyLongBits];
    std::memset(keyBits, 0, sizeof(keyBits));
    if (::ioctl(fd, EVIOCGBIT(EV_KEY, sizeof(keyBits)), keyBits) < 0) {
        return false;
    }

    for (auto it = mediaKeyMap().cbegin(); it != mediaKeyMap().cend(); ++it) {
        if (testBit(keyBits, it.key())) {
            return true;
        }
    }

    return testBit(keyBits, KEY_A) && testBit(keyBits, KEY_ENTER);
}

void linuxmultimediakeys::startWatching() {
    QDir inputDir(QStringLiteral("/dev/input"));
    const QStringList entries = inputDir.entryList({QStringLiteral("event*")}, QDir::System, QDir::Name);

    for (const QString &entry : entries) {
        const QString fullPath = inputDir.filePath(entry);
        const int fd = ::open(fullPath.toLocal8Bit().constData(), O_RDONLY | O_NONBLOCK);
        if (fd < 0) {
            continue;
        }

        if (!isKeyboardOrMediaDevice(fd)) {
            ::close(fd);
            continue;
        }

        char devName[256] = {0};
        if (::ioctl(fd, EVIOCGNAME(sizeof(devName)), devName) < 0) {
            std::strncpy(devName, "Bilinmeyen Aygıt", sizeof(devName) - 1);
        }

        auto *notifier = new QSocketNotifier(fd, QSocketNotifier::Read, this);
        connect(notifier, &QSocketNotifier::activated, this, [this, fd](int) {
            handleReadyRead(fd);
        });

        m_devices.push_back({fd, fullPath, QString::fromLocal8Bit(devName), notifier});
        qInfo().noquote() << "Dinleniyor:" << m_devices.last().name << "(" << fullPath << ")";
    }

    if (m_devices.isEmpty()) {
        qWarning() << "Dinlenebilir giriş aygıtı bulunamadı. /dev/input/event* izinlerini kontrol edin.";
    }
}

void linuxmultimediakeys::handleReadyRead(int fd) {
    struct input_event ev {};
    while (true) {
        const ssize_t bytesRead = ::read(fd, &ev, sizeof(ev));
        if (bytesRead == static_cast<ssize_t>(sizeof(ev))) {
            // ev.value == 1: tuşa basılma anı (0: bırakma, 2: basılı tutma tekrarı)
            if (ev.type == EV_KEY && ev.value == 1 && mediaKeyMap().contains(ev.code)) {
                emit mediaKeyPressed(ev.code, mediaKeyName(ev.code));
            }
        } else {
            break;
        }
    }
}