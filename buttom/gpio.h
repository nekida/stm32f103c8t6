#ifndef GPIO_H
#define GPIO_H

#include "stm32f103xb.h"
#include <stdint.h>

void config_pin(GPIO_TypeDef *port, uint8_t pin, uint32_t reg_reset, uint32_t reg_set);

#endif // GPIO_H
