/*
 * ECE 153B
 *
 * Name(s): Erk Sampat, Robert Kastaman
 * Section: N/A
 * Lab 1
 */
#include "stm32l476xx.h"

void Init(){
    // Enable HSI
    RCC->CR |= ((uint32_t)RCC_CR_HSION);
    while ( (RCC->CR & (uint32_t) RCC_CR_HSIRDY) == 0 );

    // Select HSI as system clock source
    RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
    RCC->CFGR |= (uint32_t)RCC_CFGR_SW_HSI;
    while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) == 0 );

    // Enable GPIO Clock for ports A and C
    RCC->AHB2ENR |= (RCC_AHB2ENR_GPIOAEN | RCC_AHB2ENR_GPIOCEN);

    // Initialize Green LED (PA5)
    // set mode to output (01)
    GPIOA->MODER |= GPIO_MODER_MODE5_0;
    GPIOA->MODER &= ~GPIO_MODER_MODE5_1;
    // set output type to push-pull (0)
    GPIOA->OTYPER &= ~GPIO_OTYPER_OT5;
    // set no pull-up, no pull-down (00)
    GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD5_0;
    GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD5_1;
    // set LED to be off on startup
    GPIOA->ODR &= ~GPIO_ODR_OD5;

    // Initialize User Button (PC13)
    // set mode to input (00)
    GPIOC->MODER &= ~GPIO_MODER_MODE13_0;
    GPIOC->MODER &= ~GPIO_MODER_MODE13_1;
    // set no pull-up, no pull-down (00)
    GPIOC->PUPDR &= ~GPIO_PUPDR_PUPD13_0;
    GPIOC->PUPDR &= ~GPIO_PUPDR_PUPD13_1;
}

int main(void){
    // Initialization
    Init();

    // Polling to Check for User Button Presses
    while (1) {
        if ((GPIOC->IDR & GPIO_IDR_ID13) != GPIO_IDR_ID13) { // if button is pressed
            while ((GPIOC->IDR & GPIO_IDR_ID13) != GPIO_IDR_ID13) {} // do nothing until button is released
            GPIOA->ODR ^= GPIO_ODR_OD5; // toggle LED
        }
    }

    return 0;
}
