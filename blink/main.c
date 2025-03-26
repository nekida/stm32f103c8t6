#include "stm32f103xb.h"

#include <stdint.h>

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
    RCC->CFGR |= (RCC_CFGR_PLLSRC | RCC_CFGR_PLLMULL);
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

    flash_config ();
}

int main (void)
{
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;

    GPIOC->CRH &= ~(GPIO_CRH_MODE13 | GPIO_CRH_CNF13);
    GPIOC->CRH |= GPIO_CRH_MODE13_0;

    while (1) {
        GPIOC->BSRR |= GPIO_BSRR_BS13;
        for (volatile uint32_t i = 0; i < 100000; ++i);
        GPIOC->BSRR |= GPIO_BSRR_BR13;
        for (volatile uint32_t i = 0; i < 100000; ++i);
    }
}
