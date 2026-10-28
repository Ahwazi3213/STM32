#include "led_fsm.h"
#include "heartbeat.h"
#include "main.h"

extern TIM_HandleTypeDef htim1;

static LedState state = LED_OFF;

/* TODO: fungsi bantu untuk set kecerahan 0 dan 100% (pakai CCR: 0 dan ARR + 1). */
static void led_full(void) { /* TODO */ }
static void led_off(void)  { /* TODO */ }

void led_init(void)
{
    /* TODO: state = LED_OFF dan pastikan LED mati. */
}

void led_handle_event(Event ev, uint32_t T_ms)
{
    switch (state)
    {
        case LED_OFF:
            /* TODO: KLIK -> ON (led_full).  PRESS -> HB (heartbeat_start(T_ms)). */
            break;

        case LED_ON:
            /* TODO: KLIK -> OFF (led_off).  PRESS -> HB. */
            break;

        case LED_HB:
            /* TODO: KLIK -> OFF (heartbeat_stop + led_off).
             *       PRESS -> tetap HB. Keputusanmu: apakah periode diganti dengan T baru?
             *       Tuliskan keputusanmu di README P4. */
            break;
    }
    (void)ev; (void)T_ms; (void)led_full; (void)led_off;
}

void led_update(uint32_t now_ms)
{
    /* TODO: jika state == LED_HB -> heartbeat_update(now_ms). */
    (void)now_ms;
}

LedState led_get_state(void)
{
    return state;
}
