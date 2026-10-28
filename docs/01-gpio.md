# Level 1: GPIO Input dan Output

[← Level 0](00-persiapan.md) · [Beranda](../README.md) · [Level 2: Waktu →](02-waktu.md)

> [!NOTE]
> Tujuan level ini: membaca tombol, menyalakan LED, dan memahami **perbedaan antara keadaan dan peristiwa**. Perbedaan itu adalah akar dari hampir semua bug tombol.

## Tujuan belajar

- [ ] Menjelaskan apa itu pin digital, pull-up, dan active-low
- [ ] Memakai `HAL_GPIO_ReadPin`, `HAL_GPIO_WritePin`, `HAL_GPIO_TogglePin`
- [ ] Menghitung resistor LED
- [ ] Membedakan "tombol *sedang* ditekan" dengan "tombol *baru saja* ditekan"

**Prasyarat:** [Level 0](00-persiapan.md) selesai.

---

## 🧠 Konsep

### 1. Pin digital hanya punya dua nilai

Pin GPIO membaca atau mengeluarkan tegangan, lalu menafsirkannya menjadi dua keadaan:

| Keadaan | Tegangan | Arti |
|---|---|---|
| **LOW** (0, `GPIO_PIN_RESET`) | ≈ 0 V | |
| **HIGH** (1, `GPIO_PIN_SET`) | ≈ 3,3 V | |

Untuk input, di bawah sekitar 1,2 V dibaca LOW, di atas sekitar 1,8 V dibaca HIGH, dan di antaranya **tidak terjamin** (angka kira-kira; lihat datasheet). Itu sebabnya pin input tidak boleh dibiarkan menggantung.

### 2. Mode pin

| Mode | Fungsi | Dipakai untuk |
|---|---|---|
| **Input** | membaca tegangan di pin | tombol |
| **Output push-pull** | pin aktif mendorong ke 3,3 V atau menarik ke 0 V | LED |
| Alternate function | pin dikendalikan periferal (PWM, UART) | Level 4 dan 7 |

### 3. Pin *floating* dan resistor pull-up

Input yang tidak tersambung ke mana pun disebut **floating**: nilainya acak karena menangkap noise. Solusinya: sambungkan pin ke 3,3 V lewat resistor besar (**pull-up**) supaya "kondisi diam" jelas HIGH. STM32 sudah punya resistor ini di dalam chip; kamu tinggal mengaktifkannya (`Pull-up` di CubeMX).

```text
          3,3 V
            │
          [R pull-up ≈ 40 kΩ]   ← di dalam chip
            │
PA0 ────────┼──────── tombol ──── GND
```

| Tombol | Jalur ke GND? | Nilai PA0 |
|---|---|---|
| Dilepas | tidak ada | **HIGH** (ditarik pull-up) |
| Ditekan | ada, hambatannya ≈ 0 | **LOW** (GND "menang") |

> [!IMPORTANT]
> Dengan wiring ini, **ditekan = LOW dan dilepas = HIGH**. Ini terbalik dari intuisi. Tuliskan di catatanmu, karena kamu akan membutuhkannya sampai P4 selesai.

**Hitungan: apakah pull-up aman?** Saat tombol ditekan, arus mengalir lewat pull-up ke GND:

$$ I = \frac{3{,}3\ \text{V}}{40\ \text{k}\Omega} \approx 82\ \mu\text{A} $$

Sangat kecil, jadi aman.

### 4. Menghitung resistor LED

LED butuh resistor seri supaya arusnya terbatas. Rumus dari hukum Ohm:

$$ R = \frac{V_{cc} - V_f}{I} $$

Untuk LED merah: $V_f \approx 2\ \text{V}$, target arus $I = 5\ \text{mA}$:

$$ R = \frac{3{,}3 - 2{,}0}{0{,}005} = 260\ \Omega \;\Rightarrow\; \text{pakai } 330\ \Omega $$

Arus sebenarnya dengan 330 Ω: $I = 1{,}3 / 330 \approx 3{,}9\ \text{mA}$. Cukup terang dan aman.

> [!WARNING]
> Pin **PC13** hanya boleh mengalirkan arus sangat kecil (sekitar 3 mA maksimum) dan bukan pin yang cocok untuk LED eksternal. Pakai PC13 hanya untuk LED bawaan board. LED eksternal ke PA5, dan PA8 mulai Level 4.

### 5. Active-high vs active-low

| LED | Wiring | Menyala saat |
|---|---|---|
| Eksternal (PA5 ke resistor ke LED ke GND) | pin menyuplai arus | pin **HIGH** |
| Bawaan PC13 (3,3 V ke LED ke PC13) | pin menyerap arus | pin **LOW** |

### 6. Polling dan super-loop

Mikrokontroler hanya menjalankan baris demi baris, lalu berhenti jika baris habis. `while (1)` membuat baris-barisnya **berulang selamanya**. Memeriksa tombol terus-menerus di dalam loop itu disebut **polling**.

### 7. 🔑 Keadaan vs peristiwa (konsep terpenting level ini)

| | Pertanyaan | Benar selama | Cocok untuk |
|---|---|---|---|
| **Keadaan** (level) | "Apakah tombol *sedang* ditekan?" | ratusan ms, berarti jutaan putaran loop | LED yang menyala selama tombol ditahan |
| **Peristiwa** (edge / tepi) | "Apakah tombol *baru saja* ditekan?" | **satu putaran** saja | toggle, hitung tekanan, mulai ukur waktu |

Untuk mendeteksi peristiwa, kamu harus **mengingat** nilai pin di putaran sebelumnya, lalu mencari perubahannya:

| Putaran sebelumnya | Putaran ini | Artinya |
|---|---|---|
| HIGH | LOW | **tepi turun**: tombol baru ditekan |
| LOW | HIGH | **tepi naik**: tombol baru dilepas |

---

## 🧪 Latihan

**Setup:** CubeMX: PA0 = `GPIO_Input` dengan **Pull-up** (System Core → GPIO → PA0). PA5 = `GPIO_Output`. PC13 = `GPIO_Output`. Rangkai tombol dan LED seperti di [Level 0](00-persiapan.md#2-rangkaian-dasar-di-breadboard).

### 1A. LED mengikuti tombol (keadaan)

**Aturan:** LED menyala hanya selama tombol ditahan.

Lengkapi tabel kebenaran dulu, baru menulis kode:

| Tombol | PA0 terbaca | LED yang diinginkan | Nilai yang ditulis ke PA5 |
|---|---|---|---|
| Dilepas | ? | mati | ? |
| Ditekan | ? | nyala | ? |

**Kerangka:**

```c
while (1)
{
    if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == ____)
    {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, ____);
    }
    else
    {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, ____);
    }
}
```

**Hasil yang diharapkan:** LED mati saat dilepas, menyala hanya selama ditahan.

<details>
<summary>▶ Petunjuk 1: hasil terbalik</summary>

Kamu menukar `GPIO_PIN_SET` dan `GPIO_PIN_RESET`. Lihat lagi tabel kebenaranmu.

</details>

<details>
<summary>▶ Petunjuk 2: LED tidak bereaksi sama sekali</summary>

Uji bertahap: (1) ganti isi loop dengan `TogglePin` + `HAL_Delay(200)` untuk memastikan PA5 benar-benar output (cek `__HAL_RCC_GPIOA_CLK_ENABLE` ada di `MX_GPIO_Init`); (2) pastikan PA0 memakai `GPIO_PULLUP`; (3) sentuhkan kabel dari PA0 ke GND tanpa tombol; (4) cek arah LED dan kaki tombol. Lihat [troubleshooting](troubleshooting.md).

</details>

### 1B. Eksperimen: toggle tanpa ingatan

Prediksi dulu, lalu coba.

```c
while (1)
{
    if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0) == GPIO_PIN_RESET)
    {
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
    }
}
```

**Tulis prediksimu:** (a) bagaimana tampak LED saat tombol ditahan? (b) bagaimana keadaan LED setelah dilepas?

<details>
<summary>▶ Penjelasan setelah mencoba</summary>

Selama tombol ditahan, toggle terjadi **setiap putaran loop** (jutaan kali per detik). Mata hanya melihat rata-rata, jadi LED tampak **redup**. Setelah dilepas, toggle berhenti pada keadaan acak: kadang LED mati, kadang menyala penuh.

Perkiraan kecepatan: satu putaran sekitar 15 siklus CPU (angka kasar), sehingga $72\,000\,000 / 15 \approx 4{,}8$ juta putaran per detik. Kode ini memperlakukan *keadaan* seolah-olah *peristiwa*.

</details>

### 1C. Toggle sekali per tekanan (deteksi tepi)

**Aturan:** setiap tombol ditekan, LED berganti keadaan **tepat sekali**, dan tetap pada keadaan itu saat tombol dilepas.

**Kerangka:** isi dua bagian yang kosong.

```c
GPIO_PinState prev = GPIO_PIN_SET;    // sebelum loop: anggap tombol dilepas

while (1)
{
    GPIO_PinState now = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0);

    if (/* syarat: tadi ____ DAN sekarang ____ */)
    {
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
    }

    prev = /* ? */;
}
```

**Hasil yang diharapkan:** hampir selalu satu tekanan = satu toggle. **Kadang** masih terjadi toggle ganda. Itu normal, dan itu bahan Level 3.

<details>
<summary>▶ Petunjuk: apa yang harus diingat di akhir putaran?</summary>

Variabel `prev` harus berisi nilai yang baru saja kamu baca (`now`), supaya putaran berikutnya bisa membandingkan dengan putaran ini.

</details>

### 1D. Ukur bouncing

**Tujuan:** mengumpulkan data nyata untuk Level 3.

1. Tambahkan variabel global `volatile uint32_t toggle_count = 0;` dan tambahkan `toggle_count++` di dalam `if` pada 1C.
2. Jalankan Debug dengan `toggle_count` di *Live Expressions*.
3. Tekan tombol **20 kali** dengan kecepatan normal. Catat `toggle_count` setelah tiap 5 tekanan.

| Setelah | Harusnya | Terbaca |
|---|---|---|
| 5 tekanan | 5 | |
| 10 tekanan | 10 | |
| 20 tekanan | 20 | |

Selisihnya adalah jumlah bouncing yang lolos dari kode kamu.

![Ilustrasi bouncing](../assets/button-bounce.png)

---

## ✅ Checkpoint: lulus jika

- [ ] 1A bekerja dengan benar dan kamu bisa menjelaskan tabel kebenarannya
- [ ] Kamu bisa menjelaskan kenapa 1B menghasilkan LED redup
- [ ] 1C menghasilkan toggle yang benar mayoritas waktu
- [ ] Kamu punya data jumlah bouncing dari 1D

## ❓ Cek pemahaman

**1.** Dengan tombol ke GND dan pull-up aktif, berapa nilai PA0 saat tombol dilepas?

**2.** Kenapa pin input tidak boleh dibiarkan floating?

**3.** Resistor LED: sumber 3,3 V, $V_f = 2{,}2\ \text{V}$, target 4 mA. Berapa nilai minimum resistornya?

**4.** Kenapa kode 1B butuh "ingatan" untuk diperbaiki?

<details>
<summary>▶ Kunci jawaban (tulis jawabanmu dulu)</summary>

1. **HIGH**: ditarik oleh pull-up.
2. Nilainya tidak pasti karena menangkap noise, sehingga program membaca nilai acak.
3. $R = (3{,}3 - 2{,}2)/0{,}004 = 275\ \Omega$. Pakai nilai standar terdekat di atasnya, misalnya 330 Ω.
4. Karena yang dicari adalah *peristiwa* (baru ditekan), yaitu perubahan dari nilai sebelumnya ke nilai sekarang. Tanpa menyimpan nilai sebelumnya, kita hanya tahu keadaan saat ini.

</details>

## ⚠️ Kesalahan umum

| Gejala | Penyebab paling mungkin |
|---|---|
| LED selalu menyala/mati, tidak bereaksi | pull-up belum aktif, atau kaki tombol di sisi yang sama |
| Logika terbalik | lupa bahwa ditekan = LOW, atau LED bawaan active-low |
| LED bawaan PC13 tak bereaksi | PC13 belum dijadikan output / belum Generate Code |

---

[← Level 0](00-persiapan.md) · [Beranda](../README.md) · [Level 2: Waktu →](02-waktu.md)
