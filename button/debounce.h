#ifndef DEBOUNCE_H
#define DEBOUNCE_H

#include "stm32f103xb.h"

#include <stdbool.h>
#include <stdint.h>

typedef enum level_e {
    LOW = 0,
    HIGH = 1
} level_t;

void debounce_filter_init (GPIO_TypeDef * debounce_port, uint8_t debounce_pin, level_t debounce_level);
bool is_button_pressed (void);
uint32_t * get_debounce_cnt (void);

#endif // DEBOUNCE_H
