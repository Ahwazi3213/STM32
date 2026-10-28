#include "heartbeat.h"
#include "main.h"

extern TIM_HandleTypeDef htim1;      /* dari CubeMX: TIM1 CH1 di PA8 */

static uint8_t  active = 0;
static uint32_t period = 2000;
static uint32_t t0     = 0;

/* TODO: tulis 'ccr' ke timer. Hint: __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, ccr); */
static void set_ccr(uint32_t ccr)
{
    (void)ccr;
}

void heartbeat_start(uint32_t period_ms)
{
    /* TODO: simpan period, t0 = HAL_GetTick() (agar mulai dari 0%), active = 1. */
    (void)period_ms;
}

void heartbeat_stop(void)
{
    /* TODO: active = 0 dan set kecerahan ke 0 secara eksplisit. */
}

void heartbeat_update(uint32_t now_ms)
{
    if (!active) return;

    /* TODO:
     *   phase = (now_ms - t0) % period
     *   half  = period / 2
     *   jika phase < half : ccr = phase * 2000 / period            (kalikan dulu, baru bagi)
     *   selain itu        : ccr = (period - phase) * 2000 / period
     *   set_ccr(ccr)
     * Pertanyaan: apa nilai 2000 itu, dan dari mana asalnya?
     */
    (void)now_ms;
    (void)set_ccr;
    (void)t0;
    (void)period;
}
