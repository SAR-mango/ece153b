#include "EXTI.h"

#include "DAC.h"

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

#define DAC_MIN 0
#define DAC_MAX 4095
#define DAC_INCREMENT 1024

static uint32_t dac_value = 0;
static enum {
    DOWN,
    UP,
} direction = UP;

// Interrupt handler
void EXTI15_10_IRQHandler(void);
void EXTI15_10_IRQHandler(void) {
	// clear interrupt pending bit
	EXTI->PR1 |= EXTI_PR1_PIF13;
	if (dac_value == DAC_MAX) {
		direction = DOWN;
	}
	if (dac_value == DAC_MIN) {
		direction = UP;
	}
	if (direction == UP) {
		if (dac_value == DAC_MIN) {
			dac_value = DAC_INCREMENT - 1;
		}
		else {
			dac_value += DAC_INCREMENT;
		}
	}
	else {
		if (dac_value == DAC_MAX) {
			dac_value -= (DAC_INCREMENT - 1);
		}
		else {
			dac_value -= DAC_INCREMENT;
		}
	}
	DAC_Write_Value(dac_value);
}