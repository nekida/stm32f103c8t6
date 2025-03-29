#ifndef DEBOUNCE_H
#define DEBOUNCE_H

#include "stm32f103xb.h"

#include <stdbool.h>
#include <stdint.h>

typedef struct debounce_pin_s {
    const GPIO_TypeDef * port;
    uint8_t pin;
    bool last_state;
    bool stable_state;
    uint32_t last_change_time;
} debounce_pin_t;

void debounce_init (debounce_pin_t * const debounce_pin, const GPIO_TypeDef * const port, uint8_t pin);
bool debounce_update (debounce_pin_t * const debounce_pin, uint32_t current_time);

#endif // DEBOUNCE_H
