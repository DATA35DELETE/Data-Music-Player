# 🎵 Data Music Player

<div align="center">

![C++](https://img.shields.io/badge/C++-17-blue.svg?style=for-the-badge&logo=c%2B%2B)
![Qt](https://img.shields.io/badge/Qt-6.x%20%7C%205.x-41CD52.svg?style=for-the-badge&logo=qt)
![CMake](https://img.shields.io/badge/CMake-3.16+-064F8C.svg?style=for-the-badge&logo=cmake)
![License](https://img.shields.io/badge/Lisans-GPL--3.0-green.svg?style=for-the-badge)
![Platform](https://img.shields.io/badge/Platform-Windows%20%7C%20Linux-lightgrey.svg?style=for-the-badge)

**Modern, hafif ve yüksek düzeyde özelleştirilebilir masaüstü müzik çalar ve medya yönetim aracı.**

[Özellikler](#-özellikler) • [Mimari ve Modüller](#-proje-dizin-yapısı) • [Gereksinimler](#-gereksinimler) • [Derleme ve Kurulum](#-derleme-ve-kurulum) • [Klavye Kısayolları](#-klavye-kısayolları) • [Geliştirici & İletişim](#-geliştirici--iletişim)

</div>

---

## 📖 Proje Hakkında

**Data Music Player**, modern masaüstü ortamları için **C++17** ve **Qt** altyapısıyla geliştirilmiş hafif bir müzik oynatıcısı ve medya merkezidir.

Ağır ve karmaşık arayüzlerin aksine doğrudan müzik dinleme ve arşiv yönetimi deneyimine odaklanır. Dinamik SVG tema motoru, her zaman üstte duran masaüstü mini oynatıcısı (*Music Card*), TagLib tabanlı çoklu format metadata/kapak çözümleyicisi, Linux MPRIS2 (D-Bus) ve global medya tuşu desteği ile entegre `yt-dlp` / `FFmpeg` indirme yöneticisini tek bir uygulamada birleştirir.

---

## ✨ Özellikler

### 🎧 Müzik Oynatma ve Kontroller
- **Qt Multimedia Motoru:** `QMediaPlayer`, `QAudioOutput` ve `QMediaDevices` üzerinden akıcı yerel ses oynatma.
- **Çoklu Format Desteği:** `.mp3`, `.opus` ve `.m4a` ses dosyalarını destekler.
- **Çalma Modları:** Döngü (*Loop*), Tek Şarkı Tekrarı (*Repeat*) ve Rastgele Çalma (*Shuffle*).
- **Hassas Kontroller:** İlerleme çubuğu üzerinden hızlı sarma, ses seviyesi ayarı ve tek tıkla sessize alma (*mute*).

### 🏷️ TagLib ile Metadata ve Kapak Ayrıştırma
- **Çoklu Etiket Desteği:**
  - **MP3:** ID3v2 etiketleri ve `APIC` gömülü kapak görselleri
  - **Opus:** `XiphComment` ve `FLAC::Picture` kapak görselleri
  - **M4A:** `MP4::Tag` ve `covr` kapak görselleri
- **Kapak Önbellekleme:** Çıkarılan albüm kapakları kırpılıp ölçeklenerek önbelleğe (`../thumbnails/`) kaydedilir ve arayüzde gösterilir.

### 🎵 Yüzen Mini Oynatıcı (Music Card)
- **Always-on-Top:** `WindowStaysOnTopHint` desteğiyle ana pencereyi küçültüp ekranın istediğiniz köşesinde kompakt mini oynatıcıyı kullanabilirsiniz.
- **Kompakt Kontroller:** Parça değiştirme (önceki/sonraki), oynat/duraklat, albüm kapağı ve ilerleme çubuğu.

### 🎨 18 Farklı Tema & Dinamik SVG Renklendirme
- **Geniş Renk Yelpazesi:** Siyah, Beyaz, Gri, Kırmızı, Yeşil, Mavi, Turkuaz (*Cyan*), Mor, Sarı ve Turuncu renklerinin koyu ve açık varyantları.
- **Otomatik Tema Güncelleme & Çevrimdışı Mod:** [config.ini](config.ini) üzerinden `autoThemeUpdate` ve aktif `theme` tercihi yönetilir; `Data-s-QSS` tema paketi otomatik olarak indirilebilir veya yerel `dataTheme/` klasörüyle kullanılabilir.
- **Dinamik SVG İkonlar:** [assets/icons/](assets/icons) altındaki SVG ikonları seçilen temanın renk paletine göre anlık olarak renklendirilir.

### 📁 Çalma Listeleri, Favoriler ve Kütüphane Yönetimi
- **Çoklu Dizin Desteği:** Kütüphaneye birden fazla müzik klasörü eklenebilir (`../musicPath.sett`).
- **Özel Çalma Listeleri:** Parça seçimi ve özel kapak görseli atamasıyla çalma listesi oluşturma (`../playlists/` ve `../thumbnailsPlaylists/`).
- **Favori Şarkılar:** Parçaları favorilere ekleme/çıkarma ve `Favoriler.txt` üzerinden tek listede erişim.
- **Parça Kaldırma Aracı:** Dizin bazlı arama ve çoklu seçim ile kütüphaneden şarkı çıkarma.

### 📥 Entegre Medya İndirici (`yt-dlp` & `FFmpeg`)
- **Araç Yönetimi:** `yt-dlp` aracını uygulama içerisinden indirme/güncelleme; Windows'ta `FFmpeg` indirme ve Linux'ta sistem `ffmpeg` kurulumunu kontrol etme.
- **Kaliteli Ses İndirme:** `opus`, `m4a` veya `mp3` formatında, en yüksek ses kalitesi, gömülü kapak resmi (`--embed-thumbnail`), metadata (`--add-metadata`) ve Node.js runtime (`--js-runtimes node`) desteğiyle indirme ve canlı log takibi.

### 🖥️ Sistem Entegrasyonu (MPRIS2 & Global Kısayollar)
- **MPRIS2 (Linux D-Bus):** `org.mpris.MediaPlayer2` ve `org.mpris.MediaPlayer2.Player` arayüzleri üzerinden masaüstü ortamı medya denetimleriyle (Oynat, Duraklat, Oynat/Duraklat, Sonraki, Önceki) entegrasyon.
- **Global Medya Tuşları (`QHotkey`):** Uygulama arka plandayken bile klavyedeki medya tuşlarıyla kontrol.

---

## 🛠️ Gereksinimler

Data Music Player'ı kaynak kodundan derlemek için aşağıdaki bileşenlerin sisteminizde kurulu olması gerekir:

* **C++ Derleyicisi:** C++17 destekli derleyici (GCC 11+, Clang, MinGW veya MSVC 2019+)
* **CMake:** Sürüm 3.16 veya üzeri
* **Qt Framework:** Qt 6.x (veya Qt 5.x) — Gerekli Modüller: `Widgets`, `Core`, `Network`, `Multimedia`, `Concurrent`, `Svg`, `DBus`
* **TagLib:** Ses etiketleri ve kapak görselleri için [TagLib](https://taglib.org/)
* **İsteğe Bağlı (İndirici için):** `FFmpeg` (Linux'ta sistem paket yöneticisinden) ve `Node.js`

---

## 🚀 Derleme ve Kurulum

### 1. Depoyu Klonlayın
```bash
git clone https://github.com/DATA35DELETE/Data-Music-Player.git
cd Data-Music-Player
```

### 2. CMake ile Yapılandırın ve Derleyin

#### Linux (Ubuntu / Debian):
```bash
# Gerekli geliştirme paketleri:
sudo apt-get install build-essential cmake ninja-build qt6-base-dev qt6-multimedia-dev qt6-svg-dev libtag1-dev libx11-dev

# Derleme:
cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

#### Windows (MinGW / Ninja):
```bash
cmake -B build -G "Ninja" -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

---

## ⌨️ Klavye Kısayolları

| Kısayol | Kapsam | İşlev |
| :--- | :--- | :--- |
| **Alt + M** | Uygulama İçi | Menü çubuğunu göster / gizle |
| **Medya Oynat / Duraklat Tuşu** | Global (`QHotkey`) | Oynat / Duraklat |
| **Sonraki Parça Tuşu** | Global (`QHotkey`) | Sonraki şarkıya geç |
| **Önceki Parça Tuşu** | Global (`QHotkey`) | Önceki şarkıya geç |
| **Çift Tıklama** | Liste Üzerinde | Seçilen şarkıyı anında oynat |
| **Sağ Tık Menüsü** | Liste Üzerinde | Şarkıyı favorilere veya çalma listelerine ekle |

---

## 📂 Proje Dizin Yapısı

```text
├── 3rdparty/QHotkey/        # Global klavye kısayolları kütüphanesi
├── assets/icons/            # Uygulama içi SVG ikon seti
├── assets.qrc               # Qt kaynak dosya tanımları
├── CMakeLists.txt           # CMake derleme ve bağımlılık yapılandırması
├── config.ini               # Tema ve otomatik güncelleme ayarları
├── main.cpp                 # Uygulama giriş noktası
├── mainwindow.h/.cpp/.ui    # Ana pencere, oynatıcı, kütüphane ve indirici mantığı
├── datathemeconfigurer.*    # Dinamik QSS tema ve SVG ikon renklendirme motoru
├── musiccard.h/.cpp/.ui     # Yüzen mini oynatıcı (Music Card)
├── playlistadd.h/.cpp/.ui   # Çalma listesi oluşturma penceresi
├── musicremove.h/.cpp/.ui   # Kütüphaneden parça çıkarma penceresi
├── mprisplayer.h/.cpp       # Linux D-Bus MPRIS2 adaptörü
├── about.h/.cpp/.ui         # Hakkında penceresi
├── LICENSE                  # GPL-3.0 Lisans dosyası
└── README.md                # Proje dokümantasyonu
```

---

## 👤 Geliştirici & İletişim

* **Geliştirici:** Muhammet Enes Karaboğa (DATA35DELETE)
* **E-posta:** [data.delete3509@gmail.com](mailto:data.delete3509@gmail.com)
* **GitHub:** [@DATA35DELETE](https://github.com/DATA35DELETE)
* **LinkedIn:** [Muhammet Enes Karaboğa](https://www.linkedin.com/in/muhammet-enes-karabo%C4%9Fa-065777385/)

---

## 📄 Lisans

Bu proje **GNU General Public License v3.0 (GPL-3.0)** altında lisanslanmıştır. Detaylar için [LICENSE](LICENSE) dosyasına göz atabilirsiniz.
