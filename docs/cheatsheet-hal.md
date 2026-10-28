# Cheatsheet: HAL dan Dasar C untuk Embedded

[← Beranda](../README.md)

Halaman rujukan cepat. Tidak perlu dihafal: buka saat butuh.

## 1. Pola nama HAL

`HAL_<periferal>_<aksi>`: GPIO, TIM, UART. Autocomplete: ketik `HAL_GPIO_` lalu `Ctrl+Space`.

## 2. GPIO

| Tujuan | Kode |
|---|---|
| Baca pin | `HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0)` → `GPIO_PIN_SET` / `GPIO_PIN_RESET` |
| Tulis pin | `HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET)` |
| Balik pin | `HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13)` |
| Callback EXTI | `void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)` |

`GPIO_PIN_SET` = HIGH = 1, `GPIO_PIN_RESET` = LOW = 0. PA0 = `GPIOA` + `GPIO_PIN_0`; PC13 = `GPIOC` + `GPIO_PIN_13`.

## 3. Waktu

| Tujuan | Kode |
|---|---|
| ms sejak boot | `uint32_t now = HAL_GetTick();` |
| Berapa lama berlalu | `now - start` (selalu selisih, tipe `uint32_t`) |
| Tunda (blocking, hindari) | `HAL_Delay(ms)` |

## 4. Timer / PWM

| Tujuan | Kode |
|---|---|
| Mulai PWM | `HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);` |
| Ubah duty (CCR) | `__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, ccr);` |
| Baca ARR | `__HAL_TIM_GET_AUTORELOAD(&htim1)` |

Rumus: $f_{TIM} = f_{HCLK}/(PSC+1)$, $f_{PWM} = f_{TIM}/(ARR+1)$, $D = CCR/(ARR+1)$.

## 5. UART

| Tujuan | Kode |
|---|---|
| Kirim (blocking) | `HAL_UART_Transmit(&huart1, data, len, timeout);` |
| Terima (blocking) | `HAL_UART_Receive(&huart1, buf, len, timeout);` |
| Terima 1 byte (interrupt) | `HAL_UART_Receive_IT(&huart1, &b, 1);` |
| Kirim (interrupt) | `HAL_UART_Transmit_IT(&huart1, data, len);` |
| Callback terima | `void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)` |
| Callback error | `void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)` |
| Format teks | `snprintf(buf, sizeof(buf), "T=%lu\r\n", (unsigned long)t);` |

Hasil fungsi HAL: `HAL_OK`, `HAL_ERROR`, `HAL_BUSY`, `HAL_TIMEOUT`.

## 6. Dasar C yang sering dipakai

### Tipe data bertuliskan ukuran (`<stdint.h>`)

| Tipe | Ukuran | Rentang |
|---|---|---|
| `uint8_t` | 8 bit | 0 .. 255 |
| `uint16_t` | 16 bit | 0 .. 65 535 |
| `uint32_t` | 32 bit | 0 .. 4 294 967 295 |
| `int32_t` | 32 bit | ±2,1 miliar |

### Operator penting

| Operator | Arti | Contoh |
|---|---|---|
| `%` | sisa bagi (modulo) | `7300 % 2000 == 1300` |
| `/` pada integer | **memotong pecahan** | `5 / 2 == 2`, `1 / 4 == 0` |
| `&` `\|` `^` `~` | AND, OR, XOR, NOT per bit | `0x02 ^ 0x05 == 0x07` |
| `<<` `>>` | geser bit | `1 << 13` = bit ke-13 |
| `>>` 8 | ambil byte atas dari 16 bit | `(t >> 8) & 0xFF` |

> [!WARNING]
> Pada integer, **kalikan dulu, baru bagi**. `a * b / c` benar; `a / c * b` sering menghasilkan 0.

### Memecah dan menyusun 16 bit menjadi dua byte

```c
uint8_t hi = (t >> 8) & 0xFF;
uint8_t lo =  t       & 0xFF;
uint16_t t2 = ((uint16_t)hi << 8) | lo;   // menyusun kembali
```

### `volatile`

Tandai variabel yang bisa berubah di luar alur kode yang terlihat (ISR, hardware). Tanpanya, kompiler boleh menyimpan nilai di register dan tidak pernah membacanya ulang.

### `static`

- Variabel `static` di dalam fungsi: nilainya **bertahan** antar pemanggilan.
- Variabel/fungsi `static` di tingkat file: **hanya terlihat** di file itu (mirip *private*).

### `enum` dan `switch`

```c
typedef enum { LED_OFF, LED_ON, LED_HB } LedState;
switch (state) {
    case LED_OFF: /* ... */ break;
    case LED_ON:  /* ... */ break;
    default:      break;
}
```

### Header guard (file `.h`)

```c
#ifndef BUTTON_H
#define BUTTON_H
/* deklarasi */
#endif
```

### `extern`

Untuk memakai variabel global dari file lain, deklarasikan di header: `extern uint8_t rx_byte;`, definisikan sekali di satu `.c`.

## 7. Peta blok `USER CODE`

| Blok | Isi |
|---|---|
| `Includes` | `#include` |
| `PV` | variabel global |
| `PFP` | prototipe fungsi |
| `0` | fungsi dan callback buatanmu |
| `2` | kode sekali jalan sebelum loop |
| `WHILE` | kode super-loop |

## 8. Glosarium singkat

| Istilah | Arti |
|---|---|
| **GPIO** | pin masuk/keluar digital umum |
| **Pull-up** | resistor penarik ke 3,3 V |
| **Active-low** | aktif saat LOW |
| **Bouncing** | pantulan kontak tombol |
| **EXTI** | interrupt eksternal dari pin |
| **ISR** | rutin layanan interrupt |
| **NVIC** | pengatur interrupt Cortex-M |
| **SysTick** | penghitung 1 ms untuk `HAL_GetTick` |
| **PWM** | modulasi lebar pulsa |
| **PSC / ARR / CCR** | prescaler / periode / pembanding timer |
| **Duty cycle** | persentase waktu ON per periode |
| **UART** | komunikasi serial asinkron |
| **Baud** | bit per detik |
| **Frame** | satu unit data (UART) atau pesan (protokol) |
| **Ring buffer** | antrean melingkar |
| **FSM** | *finite state machine* |
| **LUT** | *lookup table* |
