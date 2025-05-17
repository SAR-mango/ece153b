#include "stm32l476xx.h"

static void DAC_Pin_Init(void) {
	// set PA4 mode to analog
	GPIOA->MODER |= GPIO_MODER_MODE4;
	// set PA4 to no pull-up, no pull-down
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD4;
}

void DAC_Write_Value(uint32_t value) {
	uint32_t mask = DAC->DHR12R1;
	mask &= ~0x00000FFF;
	mask |= value;
	DAC->DHR12R1 = mask;
}

void DAC_Init(void) {
	DAC_Pin_Init();
	// Enable DAC clock
	RCC->APB1ENR1 |= RCC_APB1ENR1_DAC1EN;
	// reset DAC
	RCC->APB1RSTR1 |= RCC_APB1RSTR1_DAC1RST;
	RCC->APB1RSTR1 &= ~RCC_APB1RSTR1_DAC1RST;
	// disable DAC
	DAC->CR &= ~DAC_CR_EN1;
	// Enable software trigger mode
	DAC->CR |= DAC_CR_TSEL1;
	// Disable trigger
	DAC->CR &= ~DAC_CR_TEN1;
	// DAC1 connected in normal mode to external pin only with buffer enabled
	DAC->MCR &= ~DAC_MCR_MODE1;
	// Enable DAC channel 1
	DAC->CR |= DAC_CR_EN1;
	DAC_Write_Value(0);
}
