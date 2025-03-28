#include "stm32f103xb.h"
#include "CMSIS/core_cm3.h"

#include <stdint.h>
#include <stddef.h>

#include "flash.h"
#include "clock.h"
#include "gpio.h"
#include "debounce.h"

static void error_handler (void)
{
    while (1) {
        // Handle error
    }
}

int main (void)
{
    flash_config ();

    if (!set_sys_clock_to_72MHz ())
        error_handler ();

    const size_t MAX_CLK = 72000000;
    const size_t DIV_FOR_MSEC = 1000;
    SysTick_Config (MAX_CLK / DIV_FOR_MSEC);

    // led, debug
    clocking_port (RCC_APB2RSTR_IOPBRST, RCC_APB2ENR_IOPBEN);
    // button
    clocking_port (RCC_APB2RSTR_IOPARST, RCC_APB2ENR_IOPAEN);

    // led
    uint8_t led_pin = 2;
    config_pin (GPIOB, led_pin, GPIO_CRL_MODE2 | GPIO_CRL_CNF2, GPIO_CRL_MODE2_0);
    // debug
    uint8_t debug_pin = 1;
    config_pin (GPIOB, debug_pin, GPIO_CRL_MODE1 | GPIO_CRL_CNF1, GPIO_CRL_MODE1_0);
    // button
    uint8_t button_pin = 0;
    config_pin (GPIOA, button_pin, GPIO_CRL_MODE0 | GPIO_CRL_CNF0, GPIO_CRL_CNF0_1);

    __enable_irq ();

    debounce_filter_init (GPIOA, button_pin, HIGH);
    while (1) {
        if (is_button_pressed ()) {
            GPIOB->ODR ^= GPIO_ODR_ODR2;
            GPIOB->ODR ^= GPIO_ODR_ODR1;
        }
    }
}

void SysTick_Handler (void)
{
    volatile uint32_t * cnt = get_debounce_cnt ();
    *cnt++;
}
