/*
 * ECE 153B
 *
 * Name(s):
 * Section:
 * Lab: 3C
 */
 
#include <stdio.h> 
 
#include "stm32l476xx.h"

void Trigger_Setup() {
	// enable GPIO Clock for port A
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;

	// set PA9 mode to alternative function (10)
	GPIOA->MODER &= ~GPIO_MODER_MODE9_0;
	GPIOA->MODER |= GPIO_MODER_MODE9_1;
	
	// set PA9 output type to push-pull (0)
	GPIOA->OTYPER &= ~GPIO_OTYPER_OT9;
	
	// set PA9 to no pull-up, no pull-down (00)
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD9;
	
	// set PA9 speed to very high (11)
	GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR9;
	
	// set PA9 alternative function to TIM1_CH2
	GPIOA->AFR[1] &= ~GPIO_AFRH_AFSEL9;
	GPIOA->AFR[1] |= GPIO_AFRH_AFSEL9_0;
	
	// no signal on startup
	GPIOA->ODR &= ~GPIO_ODR_OD9;
	
	// disable counter
	TIM1->CR1 &= ~TIM_CR1_CEN;
	
	// Enable the timer clock
	RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
	
  // Prescaler
	TIM1->PSC = 15U; // 16MHz default MSI clock scaled to 1MHz
	
  // Auto-reload
	TIM1->ARR = 0xFFFFU; // scales 1MHz clock to approx. 15Hz
	
	// Disable output compare mode
	TIM1->CCMR1 &= ~TIM_CCMR1_OC2M;
	
	// PWM mode 1
	TIM1->CCMR1 |= (TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_2);
	
	// Output 1 preload enable
	TIM1->CCMR1 |= TIM_CCMR1_OC2PE;
	
	// enable auto reload preload
	TIM1->CR1 |= TIM_CR1_ARPE;
	
	// Select output polarity: active high
	TIM1->CCER &= ~TIM_CCER_CC1P;
	
  // Enable output for ch2
	TIM1->CCER |= TIM_CCER_CC2E;
	
	// Output Compare Register for channel 2
	TIM1->CCR2 = 10U; // 10us on-time
	
	// main output enable, off-state selection for run mode, off-state selection for idle mode
	TIM1->BDTR |= (TIM_BDTR_MOE | TIM_BDTR_OSSR | TIM_BDTR_OSSI);
	
	// enable update generation
	TIM1->EGR |= TIM_EGR_UG;
	
	// enable update interrupt
	TIM1->DIER |= TIM_DIER_UIE;
	
	// clear update interrupt flag
	TIM1->SR &= ~TIM_SR_UIF;
	
	// Counting direction: 0 = up-counting, 1 = down-counting
	TIM1->CR1 &= ~TIM_CR1_DIR;
	TIM1->CR1 &= ~TIM_CR1_CMS; // edge-aligned mode
	
	// Enable counter
	TIM1->CR1 |= TIM_CR1_CEN;
}

void Input_Capture_Setup() {
	// enable GPIO Clock for port B
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOBEN;
	
	// set PB6 mode to alternative function (10)
	GPIOB->MODER &= ~GPIO_MODER_MODE6_0;
	GPIOB->MODER |= GPIO_MODER_MODE6_1;
	
	// set PB6 to no pull-up, no pull-down (00)
	GPIOB->PUPDR &= ~GPIO_PUPDR_PUPD6;
	
	// set PB6 alternative function to TIM4_CH1
	GPIOB->AFR[0] &= ~GPIO_AFRL_AFSEL6;
	GPIOB->AFR[0] |= GPIO_AFRL_AFSEL6_1;
	
	// Enable the timer clock
	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM4EN;
	
	// Prescaler
	TIM4->PSC = 15U; // 16MHz default MSI clock scaled to 1MHz
	
	// enable auto reload preload
	TIM4->CR1 |= TIM_CR1_ARPE;
	
	// Auto-reload
	TIM4->ARR = 0xFFFFU; // scales 1MHz clock to approx. 15Hz
	// TODO everything below needs to be verified
	// map input capture to timer input 1
	TIM4->CCMR1 &= ~TIM_CCMR1_CC1S;
	TIM4->CCMR1 |= TIM_CCMR1_CC1S_0;
	
	// capture rising and falling edges
	TIM4->CCER |= (TIM_CCER_CC1NP | TIM_CCER_CC1P);
	
	// enable capturing
	TIM4->CCER |= TIM_CCER_CC1E;
	
	// enable interrupt requests
	TIM4->DIER |= TIM_DIER_CC1IE;
	
	// enable DMA requests
	TIM4->DIER |= TIM_DIER_CC1DE;
	//TIM4->DIER |= TIM_DIER_UDE; // TODO is this necessary?
	
	// enable update interrupt
	TIM4->DIER |= TIM_DIER_UIE;
	
	// enable update generation
	TIM4->EGR |= TIM_EGR_UG;
	
	// clear update interrupt flag
	TIM4->SR &= ~TIM_SR_UIF;
	
	// Counting direction: 0 = up-counting, 1 = down-counting
	TIM4->CR1 &= ~TIM_CR1_DIR;
	TIM4->CR1 &= ~TIM_CR1_CMS; // edge-aligned mode
	
	// Enable counter
	TIM4->CR1 |= TIM_CR1_CEN;
	
	// Configure and Enable in NVIC
	NVIC_EnableIRQ(TIM4_IRQn);
	NVIC_SetPriority(TIM4_IRQn, 2);
}

uint32_t volatile first_ccr = 0;
uint32_t volatile second_ccr = 0;
uint32_t volatile ccr_diff = 0;
uint32_t volatile dist_cm = 0;

void TIM4_IRQHandler(void) {
	if ((TIM1->SR & TIM_SR_UIF) == TIM_SR_UIF) {
		TIM1->SR &= ~TIM_SR_UIF;
		first_ccr = TIM4->CCR1;
	}
	else {
		second_ccr = TIM4->CCR1;
		if ((TIM4->SR & TIM_SR_UIF) == TIM_SR_UIF) {
			TIM4->SR &= ~TIM_SR_UIF;
			ccr_diff = second_ccr + 65535U - first_ccr + 1;
		}
		else {
			ccr_diff = second_ccr - first_ccr;
		}
		dist_cm = ccr_diff / 58;
	}
}

int main(void) {	
	// Enable High Speed Internal Clock (HSI = 16 MHz)
	RCC->CR |= RCC_CR_HSION;
	while ((RCC->CR & RCC_CR_HSIRDY) == 0); // Wait until HSI is ready
	
	// Select HSI as system clock source 
	RCC->CFGR &= ~RCC_CFGR_SW;
	RCC->CFGR |= RCC_CFGR_SW_HSI;
	while ((RCC->CFGR & RCC_CFGR_SWS) == 0); // Wait until HSI is system clock source
  
	// Input Capture Setup
	Input_Capture_Setup();
	
	// Trigger Setup
	Trigger_Setup();

	
	while(1) {
		// [TODO] Store your measurements on Stack
		
	}
}
