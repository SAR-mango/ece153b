/*
 * ECE 153B
 *
 * Name(s): Robert Kastaman, Erk Sampat
 * Section:
 * Lab: 2A
 */

#include "stm32l476xx.h"
void GPIO_Init(void);
void GPIO_Init(void) {	
	// Enable HSI
	RCC->CR |= ((uint32_t)RCC_CR_HSION);
	while ( (RCC->CR & (uint32_t) RCC_CR_HSIRDY) == 0 );

	// Select HSI as system clock source
	RCC->CFGR &= (uint32_t)((uint32_t)~(RCC_CFGR_SW));
	RCC->CFGR |= (uint32_t)RCC_CFGR_SW_HSI;
	while ((RCC->CFGR & (uint32_t)RCC_CFGR_SWS) == 0 );

	// Enable GPIO Clock for port C
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;

	// set mode to output (01)
	GPIOC->MODER |= (GPIO_MODER_MODE5_0 | GPIO_MODER_MODE6_0 | GPIO_MODER_MODE8_0 | GPIO_MODER_MODE9_0);
	GPIOC->MODER &= ~(GPIO_MODER_MODE5_1 | GPIO_MODER_MODE6_1 | GPIO_MODER_MODE8_1 | GPIO_MODER_MODE9_1);
	
	// set output speed to "fast" (10 -> high speed, or 11 -> very high speed)
	GPIOC->OSPEEDR &= ~(GPIO_OSPEEDR_OSPEED5_0 | GPIO_OSPEEDR_OSPEED6_0 | GPIO_OSPEEDR_OSPEED8_0 | GPIO_OSPEEDR_OSPEED9_0);
	GPIOC->OSPEEDR |= (GPIO_OSPEEDR_OSPEED5_1 | GPIO_OSPEEDR_OSPEED6_1 | GPIO_OSPEEDR_OSPEED8_1 | GPIO_OSPEEDR_OSPEED9_1);

	// set output type to push-pull (0)
	GPIOC->OTYPER &= ~(GPIO_OTYPER_OT5 | GPIO_OTYPER_OT6 | GPIO_OTYPER_OT8 | GPIO_OTYPER_OT9);
	
	// set no pull-up, no pull-down (00)
	GPIOC->PUPDR &= ~(GPIO_PUPDR_PUPD5 | GPIO_PUPDR_PUPD6 | GPIO_PUPDR_PUPD8 | GPIO_PUPDR_PUPD9);
	
	// set all pins to be off on startup
	GPIOC->ODR &= ~(GPIO_ODR_OD5 | GPIO_ODR_OD6 | GPIO_ODR_OD8 | GPIO_ODR_OD9);
}


#define DELAY 3500	// delay between steps of the sequences
#define GPIO_A GPIO_ODR_OD5
#define GPIO_A_PRIME GPIO_ODR_OD6
#define GPIO_B GPIO_ODR_OD8
#define GPIO_B_PRIME GPIO_ODR_OD9

void delay(uint32_t factor);
void delay(uint32_t factor) {
	for (uint32_t i = 0; i < (factor * DELAY); i++) {}
}

void Full_Stepping_Clockwise(void);
void Full_Stepping_Clockwise(void) {
	// reset A', set A and B'
	GPIOC->ODR &= ~GPIO_A_PRIME;
	GPIOC->ODR |= GPIO_A;
	GPIOC->ODR |= GPIO_B_PRIME;
	delay(1);
	// reset B', set B
	GPIOC->ODR &= ~GPIO_B_PRIME;
	GPIOC->ODR |= GPIO_B;
	delay(1);
	// reset A, set A'
	GPIOC->ODR &= ~GPIO_A;
	GPIOC->ODR |= GPIO_A_PRIME;
	delay(1);
	// reset B, set B'
	GPIOC->ODR &= ~GPIO_B;
	GPIOC->ODR |= GPIO_B_PRIME;
	delay(1);
}

void Full_Stepping_CounterClockwise(void);
void Full_Stepping_CounterClockwise(void) {
	// reset B', set A' and B
	GPIOC->ODR &= ~GPIO_B_PRIME;
	GPIOC->ODR |= GPIO_A_PRIME;
	GPIOC->ODR |= GPIO_B;
	delay(1);
	// reset A', set A
	GPIOC->ODR &= ~GPIO_A_PRIME;
	GPIOC->ODR |= GPIO_A;
	delay(1);
	// reset B, set B'
	GPIOC->ODR &= ~GPIO_B;
	GPIOC->ODR |= GPIO_B_PRIME;
	delay(1);
	// reset A, set A'
	GPIOC->ODR &= ~GPIO_A;
	GPIOC->ODR |= GPIO_A_PRIME;
	delay(1);
}

void Half_Stepping_Clockwise(void);
void Half_Stepping_Clockwise(void) {
	// set A and B'
	GPIOC->ODR |= GPIO_A;
	GPIOC->ODR |= GPIO_B_PRIME;
	delay(1);
	// reset B'
	GPIOC->ODR &= ~GPIO_B_PRIME;
	delay(1);
	// set B
	GPIOC->ODR |= GPIO_B;
	delay(1);
	// reset A
	GPIOC->ODR &= ~GPIO_A;
	delay(1);
	// set A'
	GPIOC->ODR |= GPIO_A_PRIME;
	delay(1);
	// reset B
	GPIOC->ODR &= ~GPIO_B;
	delay(1);
	// set B'
	GPIOC->ODR |= GPIO_B_PRIME;
	delay(1);
	// reset A'
	GPIOC->ODR &= ~GPIO_A_PRIME;
	delay(1);
}

void Half_Stepping_CounterClockwise(void);
void Half_Stepping_CounterClockwise(void) {
	// reset A
	GPIOC->ODR &= ~GPIO_A;
	delay(1);
	// set B
	GPIOC->ODR |= GPIO_B;
	delay(1);
	// reset B'
	GPIOC->ODR &= ~GPIO_B_PRIME;
	delay(1);
	// set A
	GPIOC->ODR |= GPIO_A;
	delay(1);
	// reset A'
	GPIOC->ODR &= ~GPIO_A_PRIME;
	delay(1);
	// set B'
	GPIOC->ODR |= GPIO_B_PRIME;
	delay(1);
	// reset B
	GPIOC->ODR &= ~GPIO_B;
	delay(1);
	// set A' and B
	GPIOC->ODR |= GPIO_A_PRIME;
	GPIOC->ODR |= GPIO_B;
	delay(1);
}


int main(void) {
	GPIO_Init();
	
	while (1) {
		delay(1);
		// rotate clockwise, 360 degrees, full stepping
		uint16_t degrees = 360;
		uint16_t n_pattern = (degrees/360)*512;
		for (uint16_t i = 0; i < n_pattern; i++) {
			Full_Stepping_Clockwise();
		}
		delay(1);
		// reset GPIO outputs
		GPIOC->ODR &= ~(GPIO_ODR_OD5 | GPIO_ODR_OD6 | GPIO_ODR_OD8 | GPIO_ODR_OD9);
		delay(1);
		// rotate counterclockwise, 360 degrees, half stepping
		for (uint16_t i = 0; i < n_pattern; i++) {
			Half_Stepping_CounterClockwise();
		}
		delay(1);
		// reset GPIO outputs
		GPIOC->ODR &= ~(GPIO_ODR_OD5 | GPIO_ODR_OD6 | GPIO_ODR_OD8 | GPIO_ODR_OD9);
	}
}
