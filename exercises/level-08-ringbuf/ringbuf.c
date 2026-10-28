#include "ringbuf.h"

void rb_init(RingBuf *rb)
{
    /* TODO: head = 0, tail = 0, dropped = 0. */
    (void)rb;
}

int rb_push(RingBuf *rb, uint8_t b)
{
    /* TODO:
     *   next = (head + 1) & (RB_SIZE - 1)
     *   jika next == tail : penuh -> dropped++, return -1
     *   tulis buf[head] = b      <- tulis DATA dulu
     *   head = next              <- baru umumkan lewat head
     *   return 0
     */
    (void)rb; (void)b;
    return -1;
}

int rb_pop(RingBuf *rb, uint8_t *b)
{
    /* TODO:
     *   jika head == tail : kosong -> return -1
     *   *b = buf[tail]
     *   tail = (tail + 1) & (RB_SIZE - 1)
     *   return 0
     */
    (void)rb; (void)b;
    return -1;
}

uint16_t rb_count(const RingBuf *rb)
{
    /* TODO: (head - tail) mod RB_SIZE, tanpa bilangan negatif. */
    (void)rb;
    return 0;
}
