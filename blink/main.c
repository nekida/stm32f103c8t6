#include "stm32f103xb.h"

#include <stdint.h>
#include <stddef.h>

static void error_handler (void)
{
    while (1) {
        // Handle error
    }
}

static inline void flash_config (void)
{
    // config prefetch buf
    FLASH->ACR &= ~FLASH_ACR_PRFTBE;
    FLASH->ACR |= FLASH_ACR_PRFTBE;
    // set latency
    FLASH->ACR &= ~FLASH_ACR_LATENCY;
    FLASH->ACR |= FLASH_ACR_LATENCY_2;
}

static inline void set_sys_clock_to_72MHz (void)
{
    flash_config ();

    const uint32_t timeout_val = 1000;
    RCC->CR |= RCC_CR_HSEON;
    uint32_t timeout = timeout_val;
    while (!(RCC->CR & RCC_CR_HSEON) && timeout--)
        ;
    if (timeout == 0)
        error_handler ();

    // AHB no div. HCLK = SYSCLK
    RCC->CFGR &= ~RCC_CFGR_HPRE;
    RCC->CFGR |= RCC_CFGR_HPRE_DIV1;

    // PCLK1 = HCLK / 2
    RCC->CFGR &= ~RCC_CFGR_PPRE1;
    RCC->CFGR |= RCC_CFGR_PPRE1_DIV2;

    // PCLK2 = SYSCLK
    RCC->CFGR &= ~RCC_CFGR_PPRE2;
    RCC->CFGR |= RCC_CFGR_PPRE2_DIV1;

    // PLL off
    RCC->CR &= ~RCC_CR_PLLON;
    timeout = timeout_val;
    while (!(RCC->CR & RCC_CR_PLLRDY) && timeout--)
        ;
    if (timeout == 0)
        error_handler ();

    // PLL config
    RCC->CFGR &= ~(RCC_CFGR_PLLSRC | RCC_CFGR_PLLXTPRE | RCC_CFGR_PLLMULL);
    // HSE 8 MHz -> PLL x 9 -> 72 MHz
    RCC->CFGR |= (RCC_CFGR_PLLSRC | RCC_CFGR_PLLMULL9);
    // Enable PLL
    RCC->CR |= RCC_CR_PLLON;
    timeout = timeout_val;
    while (!(RCC->CR & RCC_CR_PLLRDY) && timeout--)
        ;
    if (timeout == 0)
        error_handler ();

    // System clock mux
    RCC->CFGR &= ~RCC_CFGR_SW;
    RCC->CFGR |= RCC_CFGR_SW_PLL;
    timeout = timeout_val;
    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL && timeout--)
        ;
    if (timeout == 0)
        error_handler ();

}

void clocking_port (uint32_t io_port_reset, uint32_t clock_en)
{
    RCC->APB2RSTR |= io_port_reset;
    RCC->APB2RSTR &= ~io_port_reset;

    RCC->APB2ENR |= clock_en;
}

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

int main (void)
{
    set_sys_clock_to_72MHz ();
    clocking_port (RCC_APB2RSTR_IOPBRST, RCC_APB2ENR_IOPBEN);
    // led
    uint8_t led_pin = 2;
    config_pin (GPIOB, led_pin, GPIO_CRL_MODE2 | GPIO_CRL_CNF2, GPIO_CRL_MODE2_0);
    // debug
    uint8_t debug_pin = 1;
    config_pin (GPIOB, debug_pin, GPIO_CRL_MODE1 | GPIO_CRL_CNF1, GPIO_CRL_MODE1_0);

    const size_t delay = 250000;
    while (1) {
        GPIOB->ODR ^= GPIO_ODR_ODR2;
        GPIOB->ODR ^= GPIO_ODR_ODR1;
        for (volatile size_t i = 0; i < delay; ++i);
    }
}
