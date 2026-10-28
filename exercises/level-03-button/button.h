#ifndef BUTTON_H
#define BUTTON_H

#include <stdint.h>

typedef enum {
    BTN_EVT_NONE = 0,
    BTN_EVT_KLIK,
    BTN_EVT_PRESS
} ButtonEvent;

#define BTN_DEBOUNCE_MS    20U      /* abaikan tepi yang lebih rapat dari ini */
#define BTN_KLIK_MAX_MS  1000U      /* T <= ini  -> KLIK, selebihnya PRESS */

void        button_init(void);
void        button_on_exti(uint16_t gpio_pin);        /* dipanggil dari HAL_GPIO_EXTI_Callback */
ButtonEvent button_poll(uint32_t *duration_ms);       /* dipanggil dari main loop */

#endif /* BUTTON_H */
