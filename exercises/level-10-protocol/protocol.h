#ifndef PROTOCOL_H
#define PROTOCOL_H

#include <stdint.h>

/* Rancangan biner contoh (5 byte): [HEADER][TIPE][T_HI][T_LO][CHK]
 * Jika kamu memilih protokol teks, ubah berkas ini dan uji sesuai desainmu. */
#define PROTO_HEADER           0xAAU
#define PROTO_FRAME_LEN        5U
#define PROTO_BYTE_TIMEOUT_MS  50U

typedef enum { MSG_NONE = 0, MSG_KLIK = 1, MSG_PRESS = 2 } MsgType;

typedef struct {
    MsgType  type;
    uint32_t t_ms;
} Message;

/* Bangun frame; tulis ke out (minimal PROTO_FRAME_LEN byte); kembalikan jumlah byte. */
uint8_t protocol_build(MsgType type, uint32_t t_ms, uint8_t *out);

/* Beri makan satu byte. Kembalikan 1 jika satu pesan utuh baru saja selesai (isi *msg), selain itu 0. */
int protocol_feed(uint8_t byte, uint32_t now_ms, Message *msg);

#endif /* PROTOCOL_H */
