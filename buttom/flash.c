#include "flash.h"

#include "stm32f103xb.h"

void flash_config (void)
{
    // config prefetch buf
    FLASH->ACR &= ~FLASH_ACR_PRFTBE;
    FLASH->ACR |= FLASH_ACR_PRFTBE;
    // set latency
    FLASH->ACR &= ~FLASH_ACR_LATENCY;
    FLASH->ACR |= FLASH_ACR_LATENCY_2;
}
