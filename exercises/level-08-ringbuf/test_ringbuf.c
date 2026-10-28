#include <stdio.h>
#include <stdint.h>
#include "ringbuf.h"

static int failures = 0;
#define CHECK(cond, name) do { \
    if (cond) printf("PASS  %s\n", name); \
    else      { printf("FAIL  %s\n", name); failures++; } \
} while (0)

int main(void)
{
    RingBuf rb;
    uint8_t v;

    rb_init(&rb);
    CHECK(rb_count(&rb) == 0,        "buffer baru: count = 0");
    CHECK(rb_pop(&rb, &v) == -1,     "pop dari buffer kosong gagal");

    rb_push(&rb, 10); rb_push(&rb, 20); rb_push(&rb, 30);
    CHECK(rb_count(&rb) == 3,        "setelah 3 push: count = 3");
    CHECK(rb_pop(&rb, &v) == 0 && v == 10, "pop pertama = 10 (FIFO)");
    CHECK(rb_pop(&rb, &v) == 0 && v == 20, "pop kedua = 20");
    CHECK(rb_pop(&rb, &v) == 0 && v == 30, "pop ketiga = 30");
    CHECK(rb_pop(&rb, &v) == -1,     "kosong lagi setelah 3 pop");

    /* Penuh: kapasitas efektif RB_SIZE - 1 */
    rb_init(&rb);
    int ok = 1;
    for (int i = 0; i < RB_SIZE - 1; i++) if (rb_push(&rb, (uint8_t)i) != 0) ok = 0;
    CHECK(ok,                        "RB_SIZE-1 push pertama sukses");
    CHECK(rb_push(&rb, 99) == -1,    "push saat penuh gagal");
    CHECK(rb.dropped == 1,           "dropped bertambah 1");
    CHECK(rb_count(&rb) == RB_SIZE - 1, "count = RB_SIZE-1 saat penuh");

    /* Wrap-around */
    rb_init(&rb);
    for (int i = 0; i < 50; i++) rb_push(&rb, (uint8_t)i);
    for (int i = 0; i < 40; i++) rb_pop(&rb, &v);               /* tail maju ke 40 */
    for (int i = 100; i < 140; i++) rb_push(&rb, (uint8_t)i);   /* head melewati batas array */
    ok = 1;
    for (int i = 40; i < 50; i++)   { if (rb_pop(&rb, &v) != 0 || v != (uint8_t)i) ok = 0; }
    for (int i = 100; i < 140; i++) { if (rb_pop(&rb, &v) != 0 || v != (uint8_t)i) ok = 0; }
    CHECK(ok,                        "urutan benar setelah wrap-around");
    CHECK(rb_count(&rb) == 0,        "kosong setelah semua dibaca");

    printf("\n%s (%d gagal)\n", failures == 0 ? "SEMUA LOLOS" : "ADA YANG GAGAL", failures);
    return failures != 0;
}
