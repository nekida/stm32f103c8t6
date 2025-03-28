#include "clock.h"

#include "stm32f103xb.h"

bool set_sys_clock_to_72MHz (void)
{
    const uint32_t timeout_val = 1000;

    RCC->CR |= RCC_CR_HSEON;
    uint32_t timeout = timeout_val;
    while (!(RCC->CR & RCC_CR_HSEON) && timeout--)
        ;
    if (timeout == 0)
        return false;

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
        return false;

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
        return false;

    // System clock mux
    RCC->CFGR &= ~RCC_CFGR_SW;
    RCC->CFGR |= RCC_CFGR_SW_PLL;
    timeout = timeout_val;
    while ((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL && timeout--)
        ;
    if (timeout == 0)
        return false;
    return true;
}

void clocking_port (uint32_t io_port_reset, uint32_t clock_en)
{
    RCC->APB2RSTR |= io_port_reset;
    RCC->APB2RSTR &= ~io_port_reset;

    RCC->APB2ENR |= clock_en;
}
