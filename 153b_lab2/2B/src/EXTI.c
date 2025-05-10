/*
 * ECE 153B
 *
 * Name(s):
 * Section:
 * Lab: 2B
 */

#include "EXTI.h"
#include "LED.h"

void EXTI_Init(void) {
	// Initialize User Button (PC13)
	// Enable GPIO Clock for port C
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOCEN;
	
	// set mode to input (00)
	GPIOC->MODER &= ~GPIO_MODER_MODE13_0;
	GPIOC->MODER &= ~GPIO_MODER_MODE13_1;
	
	// set no pull-up, no pull-down (00)
	GPIOC->PUPDR &= ~GPIO_PUPDR_PUPD13_0;
	GPIOC->PUPDR &= ~GPIO_PUPDR_PUPD13_1;
	
	// enable system configuration controller
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
	
	// Configure SYSCFG EXTI
	SYSCFG->EXTICR[3] &= ~SYSCFG_EXTICR4_EXTI13;
	SYSCFG->EXTICR[3] |= SYSCFG_EXTICR4_EXTI13_PC;
	
	// Configure EXTI Trigger
	EXTI->RTSR1 &= ~EXTI_RTSR1_RT13;
	EXTI->FTSR1 |= EXTI_FTSR1_FT13;
	
	// Enable EXTI
	EXTI->IMR1 |= EXTI_IMR1_IM13;
	
	// Configure and Enable in NVIC
	NVIC_EnableIRQ(EXTI15_10_IRQn);
	NVIC_SetPriority(EXTI15_10_IRQn, 0);
}

// interrupt handler
void EXTI15_10_IRQHandler(void);
void EXTI15_10_IRQHandler(void) {
	// clear interrupt pending bit
	EXTI->PR1 |= EXTI_PR1_PIF13;
	Green_LED_Toggle();
}
