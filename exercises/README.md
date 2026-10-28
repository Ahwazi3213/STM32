# Kerangka Kode Latihan

Folder ini berisi **kerangka (skeleton)**, bukan jawaban. Setiap berkas punya penanda `TODO` yang harus kamu isi sendiri.

| Folder | Level | Jenis | Cara uji |
|---|---:|---|---|
| `level-03-button/` | 3 | firmware STM32 | flash ke board |
| `level-05-heartbeat/` | 5 | firmware STM32 | flash ke board |
| `level-06-gaussian/` | 6 | skrip Python | jalankan di laptop |
| `level-08-ringbuf/` | 8 | C murni | **uji di laptop dengan gcc** |
| `level-10-protocol/` | 10 | C murni | **uji di laptop dengan gcc** |
| `level-11-led-fsm/` | 11 | firmware STM32 | flash ke board |

## Memasukkan berkas firmware ke project CubeIDE

1. Salin `*.c` ke folder `Core/Src` dan `*.h` ke `Core/Inc` di project CubeIDE.
2. Di `main.c`, tambahkan `#include "button.h"` (dan seterusnya) di blok `USER CODE BEGIN Includes`.
3. Klik kanan project → **Refresh** (`F5`), lalu Build.

## Menguji berkas C murni di laptop

Berkas `ringbuf` dan `protocol` tidak memakai library HAL, sehingga bisa dikompilasi di laptop:

```bash
cd level-08-ringbuf
gcc -Wall -Wextra -o test_ringbuf test_ringbuf.c ringbuf.c && ./test_ringbuf
```

Awalnya sebagian besar uji akan **GAGAL**. Itu normal: kerangka sengaja kosong. Targetmu adalah membuat semuanya `PASS`.
