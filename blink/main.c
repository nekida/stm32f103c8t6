#include "stm32f103xb.h"

// Номер пина светодиода (PC13 на Blue Pill)
#define LED_PIN 13

// Задержка (приблизительная)
void delay_ms(uint32_t ms) {
    for (uint32_t i = 0; i < ms * 2000; i++) {
        __asm__("nop");
    }
}

int main(void) {
    // Включение тактирования порта C
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
    
    // Настройка PC13 как выхода (push-pull, 2 МГц)
    GPIOC->CRH &= ~(GPIO_CRH_MODE13 | GPIO_CRH_CNF13);
    GPIOC->CRH |= GPIO_CRH_MODE13_0;  // Output mode, max speed 2 MHz
    
    while (1) {
        GPIOC->ODR ^= (1 << LED_PIN);  // Переключить светодиод
        delay_ms(500);                 // Задержка 500 мс
    }
}