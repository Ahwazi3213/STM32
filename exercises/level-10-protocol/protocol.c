#include "protocol.h"

/* TODO: definisikan state parser, mis.
 *   typedef enum { WAIT_HEADER, WAIT_TYPE, WAIT_T_HI, WAIT_T_LO, WAIT_CHK } ParserState;
 * dan variabel static: state, type, t_hi, t_lo, t_last_byte.
 */

uint8_t protocol_build(MsgType type, uint32_t t_ms, uint8_t *out)
{
    /* TODO:
     *   - batasi t_ms ke 65535 (atau tentukan kebijakan lain dan tuliskan alasannya)
     *   - out[0] = PROTO_HEADER, out[1] = tipe, out[2] = byte atas T, out[3] = byte bawah T
     *   - out[4] = XOR(out[1], out[2], out[3])
     *   - return PROTO_FRAME_LEN
     */
    (void)type; (void)t_ms; (void)out;
    return 0;
}

int protocol_feed(uint8_t byte, uint32_t now_ms, Message *msg)
{
    /* TODO:
     *   1) timeout: jika state != WAIT_HEADER dan (now_ms - t_last_byte) > PROTO_BYTE_TIMEOUT_MS
     *      -> reset ke WAIT_HEADER
     *   2) t_last_byte = now_ms
     *   3) switch (state): proses byte sesuai diagram state di docs/10-protokol.md
     *   4) di WAIT_CHK: hitung ulang XOR; jika cocok -> isi *msg dan return 1
     *      jika tidak cocok -> buang, kembali ke WAIT_HEADER, return 0
     */
    (void)byte; (void)now_ms; (void)msg;
    return 0;
}
