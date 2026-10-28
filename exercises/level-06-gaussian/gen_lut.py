#!/usr/bin/env python3
"""Pembuat lookup table (LUT) Gaussian untuk heartbeat (Level 6).

Rumus (u = phase / T, 0 <= u < 1):
    B(u) = exp(-18 * (u - 0.5)**2)
Nilai CCR = round(B(u) * FULL), dengan FULL = ARR + 1 (mis. 1000).
"""
import math

N = 256       # jumlah entri LUT
FULL = 1000   # ARR + 1


def brightness(u: float) -> float:
    # TODO: kembalikan B(u) sesuai rumus di atas (nilai 0.0 .. 1.0)
    raise NotImplementedError


def main() -> None:
    values = []
    for i in range(N):
        u = i / N          # TODO: coba juga (i + 0.5) / N, lalu bandingkan
        # TODO: hitung nilai CCR dari brightness(u) dan FULL, bulatkan ke integer
        values.append(0)

    # Cetak sebagai array C
    print(f"static const uint16_t gauss_lut[{N}] = {{")
    for i in range(0, N, 8):
        row = ", ".join(str(v) for v in values[i:i + 8])
        print(f"    {row},")
    print("};")

    # Pemeriksaan cepat: puncak harus mendekati FULL, ujung harus mendekati ~1%
    print(f"// lut[0]={values[0]}  lut[{N // 2}]={values[N // 2]}  lut[{N - 1}]={values[N - 1]}")


if __name__ == "__main__":
    main()
