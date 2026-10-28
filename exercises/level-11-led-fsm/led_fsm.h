#ifndef LED_FSM_H
#define LED_FSM_H

#include <stdint.h>

typedef enum { LED_OFF = 0, LED_ON, LED_HB } LedState;
typedef enum { EV_NONE = 0, EV_KLIK, EV_PRESS } Event;

void     led_init(void);                              /* state awal OFF, kecerahan 0 */
void     led_handle_event(Event ev, uint32_t T_ms);   /* ubah state sesuai tabel transisi */
void     led_update(uint32_t now_ms);                 /* kerja berulang (HB) */
LedState led_get_state(void);                         /* untuk debug/log */

#endif /* LED_FSM_H */
