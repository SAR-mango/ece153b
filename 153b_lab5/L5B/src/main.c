/*
 * ECE 153B
 *
 * Name(s):
 * Section:
 * Lab: 5B
 */


#include "ADC.h"
#include "DAC.h"
#include "EXTI.h"
#include "PWM.h"
#include "SysClock.h"
#include "stm32l476xx.h"
#include <stdio.h>
#include "math.h"

void LED_Pin_Init(void);
void TIM2_CH1_Init(void);

void LED_Pin_Init(void){
	// set PA5 mode to alternative function (10)
	GPIOA->MODER &= ~GPIO_MODER_MODE5_0;
	GPIOA->MODER |= GPIO_MODER_MODE5_1;
	
	// set PA5 output type to push-pull (0)
	GPIOA->OTYPER &= ~GPIO_OTYPER_OT5;
	
	// set PA5 to no pull-up, no pull-down (00)
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD5;
	
	// set PA5 speed to very high (11)
	GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR5;
	
	// set PA5 alternative function to TIM2_CH1
	GPIOA->AFR[0] &= ~GPIO_AFRL_AFSEL5;
	GPIOA->AFR[0] |= GPIO_AFRL_AFSEL5_0;
}

void TIM2_CH1_Init(void){
	// disable counter
	TIM2->CR1 &= ~TIM_CR1_CEN;
	// Enable the timer clock
	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN;
	// Counting direction: 0 = up-counting, 1 = down-counting
	TIM2->CR1 &= ~TIM_CR1_DIR;
	TIM2->CR1 &= ~TIM_CR1_CMS; // edge-aligned mode
  // Prescaler
	TIM2->PSC = 7U; // 8MHz MSI clock scaled to 1MHz
  // Auto-reload
	TIM2->ARR = 99U; // scales 1MHz clock to 10kHz PWM frequency
	// Disable output compare mode
	TIM2->CCMR1 &= ~TIM_CCMR1_OC1M;
	// PWM mode 1
	TIM2->CCMR1 |= (TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2);
	// Output 1 preload enable
	TIM2->CCMR1 |= TIM_CCMR1_OC1PE;
	// Select output polarity: active high
	TIM2->CCER &= ~TIM_CCER_CC1P;
  // Enable output for ch1
	TIM2->CCER |= TIM_CCER_CC1E;
	// Output Compare Register for channel 1
	TIM2->CCR1 = 0U; // directly corresponds to duty cycle percent
	// Enable counter
	TIM2->CR1 |= TIM_CR1_CEN;
}

uint32_t adc_data = 0U;

int main(void) {
	// Initialization
	System_Clock_Init(); // Switch System Clock = 16 MHz

	ADC_Init();
	DAC_Init();
	EXTI_Init();

	// Initialize PWM
	LED_Pin_Init();
	TIM2_CH1_Init();
	uint32_t duty_cycle = 0U;

	while (1) {
		// trigger ADC
		ADC1->CR |= ADC_CR_ADSTART;
		// wait until ADC conversion is complete
		while ((ADC123_COMMON->CSR & ADC_CSR_EOC_MST) != ADC_CSR_EOC_MST);
		// read ADC data
		adc_data = ADC1->DR;
		// reset ADC conversion complete flag
		ADC1->ISR &= ~ADC_ISR_EOC;
		// LED behavior based on ADC result
		duty_cycle = (100UL * adc_data) / 4096;
		TIM2->CCR1 = exp(0.04615 * duty_cycle) - 1;
	}
}
