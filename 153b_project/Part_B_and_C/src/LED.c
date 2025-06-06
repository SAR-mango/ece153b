/*
 * ECE 153B
 *
 * Name(s):
 * Section:
 * Project
 */

#include "LED.h"

void LED_Init(void) {
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
}

void LED_Off(void) {
	GPIOA->ODR &= ~GPIO_ODR_OD5;
}

void LED_On(void) {
	GPIOA->ODR |= GPIO_ODR_OD5;
}

void LED_Toggle(void) {
	GPIOA->ODR ^= GPIO_ODR_OD5;
}
