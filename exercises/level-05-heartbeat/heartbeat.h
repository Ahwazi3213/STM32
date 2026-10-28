#ifndef HEARTBEAT_H
#define HEARTBEAT_H

#include <stdint.h>

void heartbeat_start(uint32_t period_ms);   /* mulai atau mulai ulang dengan periode T (ms) */
void heartbeat_stop(void);                  /* berhenti; kecerahan kembali 0 */
void heartbeat_update(uint32_t now_ms);     /* dipanggil tiap putaran main loop */

#endif /* HEARTBEAT_H */
