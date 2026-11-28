# Data Music Player 1.2v : Opus and AAC Update

### ✨ Eklenenler
- **Medya Silme Desteği (`musicremove`):** Kütüphane dizinlerindeki ses dosyalarını listeleyip arayabilen, çoklu seçimle seçilen parçaları kütüphaneden/diskten kaldırabilen yeni medya silme penceresi eklendi.
- **Opus ve AAC (`.m4a`) Desteği:**
  - `.mp3` formatına ek olarak `.opus` ve `.m4a` (AAC) ses dosyalarının oynatılması ve kütüphane taramasına dahil edilmesi sağlandı.
  - **TagLib Entegrasyonu:** Opus (`XiphComment` / `FLAC::Picture`) ve M4A (`MP4::Tag` / `covr`) dosyalarından gömülü albüm kapaklarının ayrıştırılması ve önbelleğe alınması eklendi.
  - **Medya İndirici Format Seçeneği:** `yt-dlp` üzerinden indirme yaparken `opus`, `m4a` (AAC) ve `mp3` format seçimi desteği eklendi.
- **Bileşen ToolTip'leri:** Arayüzdeki butonlar, kontroller ve liste bileşenleri için açıklayıcı ipucu metinleri (`ToolTip`) eklendi.

### 🐛 Düzeltilenler & İyileştirilenler
- **Linux Multimedya Tuşları:** Linux masaüstü ortamlarında multimedya tuşlarıyla yaşanan sorunlar giderildi; **MPRIS2** (`org.mpris.MediaPlayer2` / `MprisPlayer`) D-Bus arayüzü ve **QHotkey** medya kısayollarıyla (`Play/Pause`, `Next`, `Previous`) kararlı kontrol sağlandı.

---