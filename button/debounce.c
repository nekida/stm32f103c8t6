#include "debounce.h"

#include <stddef.h>

#define DEBOUNCE_TIME_MS (5)

static uint32_t cnt = 0;

static bool is_init = false;

static GPIO_TypeDef * port;
static uint8_t pin;
static level_t level;

typedef enum {
    BTN_STATE_RELEASED = 0,
    BTN_STATE_PRESSED,
    BTN_STATE_DEBOUNCE
} button_state_t;

void debounce_filter_init (GPIO_TypeDef * debounce_port, uint8_t debounce_pin, level_t debounce_level)
{
    port = debounce_port;
    pin = debounce_pin;
    level = debounce_level;
    is_init = true;
}

button_state_t button_state = BTN_STATE_RELEASED;
uint32_t last_debounce_time = 0;

bool is_button_pressed (void)
{
    if (!is_init)
        return false;

    uint8_t current_state = port->IDR & (1 << pin);

    switch (button_state) {
        case BTN_STATE_RELEASED:
            if (current_state == level) {
                button_state = BTN_STATE_DEBOUNCE;
                last_debounce_time = cnt;
            }
            break;

        case BTN_STATE_DEBOUNCE:
            if (cnt - last_debounce_time > DEBOUNCE_TIME_MS) {
                if (current_state == level) {
                    button_state = BTN_STATE_PRESSED;
                    return true;
                } else
                    button_state = BTN_STATE_RELEASED;
            }
            break;

        case BTN_STATE_PRESSED:
            if (current_state != level)
                button_state = BTN_STATE_RELEASED;
            break;
    }
    return false;
}

uint32_t * get_debounce_cnt (void)
{
    return &cnt;
}
