#ifndef RINGBUF_H
#define RINGBUF_H

#include <stdint.h>

#define RB_SIZE 64                 /* HARUS pangkat dua (agar bisa pakai & (RB_SIZE-1)) */

typedef struct {
    uint8_t           buf[RB_SIZE];
    volatile uint16_t head;        /* ISR menulis di sini */
    volatile uint16_t tail;        /* main membaca dari sini */
    volatile uint32_t dropped;     /* jumlah byte yang dibuang karena penuh */
} RingBuf;

void     rb_init(RingBuf *rb);
int      rb_push(RingBuf *rb, uint8_t b);     /* 0 = sukses, -1 = penuh */
int      rb_pop (RingBuf *rb, uint8_t *b);    /* 0 = sukses, -1 = kosong */
uint16_t rb_count(const RingBuf *rb);         /* jumlah byte yang tersimpan */

#endif /* RINGBUF_H */
