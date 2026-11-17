# 🎵 Data Music Player

<div align="center">

![C++](https://img.shields.io/badge/C++-17-blue.svg?style=for-the-badge&logo=c%2B%2B)
![Qt](https://img.shields.io/badge/Qt-6.x-41CD52.svg?style=for-the-badge&logo=qt)
![CMake](https://img.shields.io/badge/CMake-3.16+-064F8C.svg?style=for-the-badge&logo=cmake)
![License](https://img.shields.io/badge/Lisans-GPL--3.0-green.svg?style=for-the-badge)
![Platform](https://img.shields.io/badge/Platform-Windows%20%7C%20Linux-lightgrey.svg?style=for-the-badge)

**Modern, hafif ve yüksek düzeyde özelleştirilebilir masaüstü müzik çalar ve medya yönetim aracı.**

[Özellikler](#-özellikler) • [Ekran Görüntüleri](#-ekran-görüntüleri) • [Gereksinimler](#-gereksinimler) • [Derleme ve Kurulum](#-derleme-ve-kurulum) • [Klavye Kısayolları](#-klavye-kısayolları) • [İletişim](#-geliştirici--iletişim)

</div>

---

## 📖 Proje Hakkında

**Data Music Player**, modern masaüstü ortamları için **C++17** ve **Qt 6** altyapısıyla sıfırdan tasarlanmış bir müzik oynatıcısı ve medya merkezidir. 

Ağır ve karmaşık arayüzlerin aksine doğrudan müzik dinleme ve arşiv yönetimi deneyimine odaklanır. Kendi geliştirdiği dinamik SVG tema motoru, masaüstü mini oynatıcısı (*Music Card*), TagLib tabanlı metadata çözümleyicisi ve entegre `yt-dlp` / `FFmpeg` indirme yöneticisi ile tam teşekküllü bir deneyim sunar.

---

## ✨ Özellikler

### 🎧 Gelişmiş Müzik Deneyimi
- **Qt Multimedia Motoru:** Kayıpsız, akıcı ve düşük gecikmeli yerel ses çalma.
- **Çalma Modları:** Tekrar (Loop), Tek Şarkı Tekrarı (Repeat-1) ve Rastgele Çalma (Shuffle).
- **Hassas Kontroller:** Şarkı içi hızlı sarma, ses seviyesi ayarı ve tek tıkla sessize alma (*mute*).

### 🏷️ TagLib ile Metadata ve Kapak Ayrıştırma
- **ID3v2 Desteği:** Şarkı adı, sanatçı bilgisi ve parça sürelerini dosyanın içinden doğrudan okur.
- **APIC (Gömülü Kapak Resmi):** MP3 dosyalarına gömülü albüm kapaklarını anında ayıklar, yuvarlatılmış köşelerle modern biçimde arayüze işler.

### 🎵 Yüzen Mini Oynatıcı (Music Card)
- **Always-on-Top:** Çalışırken ana pencereyi küçültüp ekranın istediğiniz bir köşesinde duran mini oynatıcıya geçebilirsiniz.
- **Kompakt Kontroller:** Parça değiştirme, duraklatma/oynatma ve ilerleme çubuğu içeren şık mini kart.

### 🎨 18 Farklı Renk Seçeneği & Dinamik SVG Teması
- **Geniş Renk Yelpazesi:** Siyah, Beyaz, Gri, Kırmızı, Mavi, Turkuaz, Yeşil, Mor, Sarı ve Turuncu renklerinin hem Açık hem Koyu varyantları.
- **Dinamik SVG Renklendirme:** Statik ikonlar yerine SVG XML'indeki `currentColor` parametresini anlık olarak seçilen temanın paletine göre dönüştüren dinamik tema motoru.

### 📁 Çalma Listeleri & Favoriler
- **Özel Çalma Listeleri:** İstediğiniz parçaları seçip özel kapak resmi atayarak çalma listeleri oluşturabilme.
- **Favori Şarkılar:** Parçalara sağ tıklayarak favorilere ekleme/çıkarma ve tek tıkla favorileri dinleyebilme.
- **Çoklu Dizin Yönetimi:** Kütüphaneye birden fazla müzik klasörü ekleyebilme ve tümünü tek listede harmanlayabilme.

### 📥 Entegre Medya İndirici
- **yt-dlp & FFmpeg Entegrasyonu:** Harici indirme araçlarını doğrudan uygulama içerisinden otomatik olarak indirip kurma ve güncelleme imkânı.
- **Metadata & Kapak Dahil İndirme:** Bağlantısı verilen çevrim içi müzikleri en yüksek ses kalitesiyle, albüm kapağı ve etiketleriyle birlikte otomatik olarak indirip kütüphaneye dahil etme.

---

## 🛠️ Gereksinimler

Data Music Player'ı kaynak kodundan derlemek için aşağıdaki bileşenlerin sisteminizde kurulu olması gerekir:

* **C++ Derleyicisi:** C++17 standartlarını destekleyen bir derleyici (MinGW GCC 11+, MSVC 2019+ veya GCC/Clang).
* **CMake:** Sürüm 3.16 veya üzeri.
* **Qt Framework:** Qt 6.x sürümü (Gerekli Modüller: `Core`, `Widgets`, `Multimedia`, `Network`, `Concurrent`, `Svg`).
* **TagLib:** Ses etiketlerini ve albüm kapaklarını okumak için [TagLib](https://taglib.org/) kütüphanesi.

---

## 🚀 Derleme ve Kurulum

### 1. Depoyu Klonlayın
```bash
git clone https://github.com/DATA35DELETE/Data-Music-Player.git
cd Data-Music-Player
```

### 2. CMake ile Yapılandırın ve Derleyin

#### Windows (MinGW):
```bash
cmake -B build -G "Ninja" -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

#### Linux:
```bash
# Ubuntu / Debian bağımlılıkları:
sudo apt-get install build-essential cmake qt6-base-dev qt6-multimedia-dev qt6-svg-dev libtag1-dev

# Derleme:
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

---

## ⌨️ Klavye Kısayolları

| Kısayol | İşlev |
| :--- | :--- |
| **Alt + M** | Menü Çubuğunu Göster / Gizle |
| **Space** | Oynat / Duraklat |
| **Çift Tıklama** | Listeden seçilen şarkıyı anında başlatır |
| **Sağ Tık Menüsü** | Şarkıyı favorilere veya çalma listelerine ekler |

---

## 📂 Proje Dizin Yapısı

```text
├── assets/                  # SVG ikonlar ve grafik kaynakları
├── assets.qrc               # Qt kaynak dosya tanımları
├── CMakeLists.txt           # Proje derleme ve kütüphane bağlantıları
├── main.cpp                 # Uygulama giriş noktası
├── mainwindow.h/.cpp/.ui    # Ana oynatıcı penceresi ve ana kontrol mantığı
├── datathemeconfigurer.*    # Dinamik tema ve SVG renk dönüştürücü motor
├── musiccard.h/.cpp/.ui     # Yüzen mini oynatıcı bileşeni (Music Card)
├── playlistadd.h/.cpp/.ui   # Oynatma listesi oluşturma diyaloğu
├── musicremove.h/.cpp/.ui   # Müzik silme / kütüphaneden çıkarma arayüzü
├── LICENSE                  # GPL-3.0 Lisans dosyası
└── README.md                # Proje dokümantasyonu
```

---

## 👤 Geliştirici & İletişim

* **Geliştirici:** Muhammet Enes Karaboğa (DATA35DELETE)
* **GitHub:** [@DATA35DELETE](https://github.com/DATA35DELETE)
* **LinkedIn:** [Muhammet Enes Karaboğa](https://www.linkedin.com/in/muhammet-enes-karabo%C4%9Fa-065777385/)

---

## 📄 Lisans

Bu proje **GNU General Public License v3.0 (GPL-3.0)** altında lisanslanmıştır. Detaylar için [LICENSE](LICENSE) dosyasına göz atabilirsiniz.

