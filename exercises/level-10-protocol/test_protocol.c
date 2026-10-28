#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "protocol.h"

static int failures = 0;
#define CHECK(cond, name) do { \
    if (cond) printf("PASS  %s\n", name); \
    else      { printf("FAIL  %s\n", name); failures++; } \
} while (0)

/* Beri makan sebuah deretan byte dengan jeda waktu antar-byte (ms).
 * Kembalikan jumlah pesan yang terbit; pesan terakhir disimpan di *last. */
static int feed_bytes(const uint8_t *data, int n, uint32_t *t, uint32_t gap_ms, Message *last)
{
    int count = 0;
    for (int i = 0; i < n; i++) {
        Message m;
        if (protocol_feed(data[i], *t, &m)) { count++; *last = m; }
        *t += gap_ms;
    }
    return count;
}

int main(void)
{
    uint8_t f[8];
    Message m = { MSG_NONE, 0 };
    uint32_t t = 1000;

    /* --- build --- */
    uint8_t n = protocol_build(MSG_PRESS, 1500, f);
    const uint8_t expect_press[] = { 0xAA, 0x02, 0x05, 0xDC, 0xDB };
    CHECK(n == 5 && memcmp(f, expect_press, 5) == 0, "build PRESS 1500 = AA 02 05 DC DB");

    n = protocol_build(MSG_KLIK, 300, f);
    const uint8_t expect_klik[] = { 0xAA, 0x01, 0x01, 0x2C, 0x2C };
    CHECK(n == 5 && memcmp(f, expect_klik, 5) == 0, "build KLIK 300 = AA 01 01 2C 2C");

    /* --- 1: frame PRESS benar --- */
    int c = feed_bytes(expect_press, 5, &t, 1, &m);
    CHECK(c == 1 && m.type == MSG_PRESS && m.t_ms == 1500, "1) frame PRESS 1500 diparse");

    /* --- 2: frame KLIK benar --- */
    c = feed_bytes(expect_klik, 5, &t, 1, &m);
    CHECK(c == 1 && m.type == MSG_KLIK, "2) frame KLIK diparse");

    /* --- 3: checksum salah --- */
    uint8_t bad[] = { 0xAA, 0x02, 0x05, 0xDC, 0x00 };
    c = feed_bytes(bad, 5, &t, 1, &m);
    CHECK(c == 0, "3) checksum salah ditolak");

    /* --- 4: TODO: sampah acak (tanpa 0xAA) lalu frame benar -> pesan benar tetap terbit --- */
    printf("TODO  4) sampah lalu frame benar\n");

    /* --- 5: TODO: frame terpotong (mis. 3 byte), jeda > PROTO_BYTE_TIMEOUT_MS, lalu frame benar --- */
    printf("TODO  5) frame terpotong + timeout lalu frame benar\n");

    /* --- 6: TODO: dua frame berurutan tanpa jeda -> dua pesan --- */
    printf("TODO  6) dua frame berurutan\n");

    /* --- Tantangan: sampah yang MENGANDUNG 0xAA tepat sebelum frame benar. Apa yang terjadi? --- */

    printf("\n%s (%d gagal)\n", failures == 0 ? "SEMUA UJI YANG ADA LOLOS" : "ADA YANG GAGAL", failures);
    return failures != 0;
}
