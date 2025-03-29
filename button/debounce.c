#include "debounce.h"

#include <stddef.h>

#include "debounce.h"
#include <stdbool.h>

#define DEBOUNCE_TIME_MS (20)

static const uint8_t bit = 1;

void debounce_init (debounce_pin_t * const debounce_pin, const GPIO_TypeDef * const port, uint8_t pin)
{
    if (debounce_pin == NULL || port == NULL || pin > 15)
        return;

    debounce_pin->port = port;
    debounce_pin->pin = pin;
    debounce_pin->last_state = (port->IDR & (bit << pin)) ? true : false;
    debounce_pin->stable_state = debounce_pin->last_state;
    debounce_pin->last_change_time = 0;
}

bool debounce_update (debounce_pin_t * const debounce_pin, uint32_t current_time)
{
    if (debounce_pin == NULL)
        return false;

    bool current_state = (debounce_pin->port->IDR & (bit << debounce_pin->pin)) ? true : false;
  
    if (current_state != debounce_pin->last_state) {
        debounce_pin->last_change_time = current_time;
        debounce_pin->last_state = current_state;
        return false;
    }
  
    if (current_state != debounce_pin->stable_state) {
        if ((current_time - debounce_pin->last_change_time) >= DEBOUNCE_TIME_MS) {
            debounce_pin->stable_state = current_state;
            return true;
        }
    }
  
    return false;
}
