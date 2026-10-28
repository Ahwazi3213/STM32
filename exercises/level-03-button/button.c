#include "button.h"
#include "main.h"          /* HAL */

/* TODO: tentukan pin tombol (PA0). */
#define BTN_GPIO_PORT  GPIOA
#define BTN_GPIO_PIN   GPIO_PIN_0

/* Variabel yang dibagi antara ISR dan main -> volatile. static: hanya terlihat di file ini. */
static volatile uint8_t  pressed      = 0;     /* pelacakan state */
static volatile uint32_t t_press      = 0;
static volatile uint32_t press_ms     = 0;
static volatile uint8_t  event_ready  = 0;
static          uint32_t t_last_edge  = 0;

void button_init(void)
{
    /* TODO: reset semua variabel di atas ke keadaan awal. */
}

/* Dipanggil dari HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) di main.c.
 * Aturan ISR: singkat, tanpa HAL_Delay, hanya catat waktu dan set flag. */
void button_on_exti(uint16_t gpio_pin)
{
    if (gpio_pin != BTN_GPIO_PIN) return;

    uint32_t now = HAL_GetTick();

    /* TODO (A): abaikan jika selisih dari t_last_edge < BTN_DEBOUNCE_MS. */

    GPIO_PinState level = HAL_GPIO_ReadPin(BTN_GPIO_PORT, BTN_GPIO_PIN);

    /* TODO (B): level menunjukkan DITEKAN dan state belum 'pressed'  -> catat t_press, set pressed. */
    /* TODO (C): level menunjukkan DILEPAS dan state sedang 'pressed' -> hitung press_ms, set event_ready, clear pressed. */
    /* TODO (D): perbarui t_last_edge (pikirkan: setiap interrupt, atau hanya tepi yang diterima?). */

    (void)now;
    (void)level;
}

/* Dipanggil dari main loop. Kembalikan event baru (sekali), atau BTN_EVT_NONE. */
ButtonEvent button_poll(uint32_t *duration_ms)
{
    /* TODO: jika event_ready:
     *   - salin press_ms ke *duration_ms
     *   - kosongkan event_ready
     *   - kembalikan KLIK jika press_ms <= BTN_KLIK_MAX_MS, selain itu PRESS
     * Jika tidak ada event: kembalikan BTN_EVT_NONE.
     */
    (void)duration_ms;
    return BTN_EVT_NONE;
}
