#include "gpio.h"

#include <stddef.h>

void config_pin (GPIO_TypeDef * port, uint8_t pin, uint32_t reg_reset, uint32_t reg_set)
{
    const uint8_t max_num_pin = 15;
    if (port == NULL || pin > max_num_pin)
        return;

    volatile uint32_t * reg = NULL;
    const uint8_t num_pin_on_reg = 8;
    if (pin < num_pin_on_reg)
        reg = &port->CRL;
    else
        reg = &port->CRH;

    *reg &= ~reg_reset;
    *reg |= reg_set; 
}
