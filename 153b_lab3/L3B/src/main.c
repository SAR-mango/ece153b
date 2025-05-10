/*
 * ECE 153B
 * 
 * Name(s):
 * Section:
 * Lab: 3B
 */

#include "stm32l476xx.h"
#include "math.h"

void LED_Pin_Init(void);
void TIM2_CH1_Init(void);
void SERVO_Pin_Init(void);
void TIM5_CH1_Init(void);

void LED_Pin_Init(void){
	// enable GPIO Clock for port A
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;

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
	
	// set LED to be off on startup
	GPIOA->ODR &= ~GPIO_ODR_OD5;
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
	TIM2->PSC = 3U; // 4MHz default MSI clock scaled to 1MHz
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

void delayMilli(uint32_t delay_ms) {
	for (uint32_t i = 0; i < 1000 * delay_ms; i++) {
		uint32_t last_cnt = TIM2->CNT;
		while (TIM2->CNT == last_cnt) {}
	}
}

void SERVO_Pin_Init(void){
	// set PA0 mode to alternative function (10)
	GPIOA->MODER &= ~GPIO_MODER_MODE0_0;
	GPIOA->MODER |= GPIO_MODER_MODE0_1;
	
	// set PA0 output type to push-pull (0)
	GPIOA->OTYPER &= ~GPIO_OTYPER_OT0;
	
	// set PA0 to no pull-up, no pull-down (00)
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD0;
	
	// set PA0 speed to very high (11)
	GPIOA->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR0;
	
	// set PA0 alternative function to TIM2_CH1
	GPIOA->AFR[0] &= ~GPIO_AFRL_AFSEL0;
	GPIOA->AFR[0] |= GPIO_AFRL_AFSEL0_1;
	
	// no signal on startup
	GPIOA->ODR &= ~GPIO_ODR_OD0;
}

void TIM5_CH1_Init(void){
	// disable counter
	TIM5->CR1 &= ~TIM_CR1_CEN;
	// Enable the timer clock
	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM5EN;
	// Counting direction: 0 = up-counting, 1 = down-counting
	TIM5->CR1 &= ~TIM_CR1_DIR;
	TIM5->CR1 &= ~TIM_CR1_CMS; // todo: add comment
  // Prescaler
	TIM5->PSC = 39U; // 4MHz default MSI clock scaled to 100kHz
  // Auto-reload
	TIM5->ARR = 1999U; // scales 100kHz clock to 50Hz PWM frequency
	// Disable output compare mode
	TIM5->CCMR1 &= ~TIM_CCMR1_OC1M;
	// PWM mode 1
	TIM5->CCMR1 |= (TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2);
	// Output 1 preload enable
	TIM5->CCMR1 |= TIM_CCMR1_OC1PE;
	// Select output polarity: active high
	TIM5->CCER &= ~TIM_CCER_CC1P;
  // Enable output for ch1
	TIM5->CCER |= TIM_CCER_CC1E;
	// Output Compare Register for channel 1
	TIM5->CCR1 = 100U; // 1000us on-time
	// Enable counter
	TIM5->CR1 |= TIM_CR1_CEN;
}

int main(void) {
	LED_Pin_Init();
	SERVO_Pin_Init();
	TIM2_CH1_Init();
	TIM5_CH1_Init();
	uint32_t duty_cycle = 0U;
	enum Direction {INC, DEC};
	enum Direction duty_cycle_direction = INC;
  while (1U) {
		TIM2->CCR1 = exp(0.04615 * duty_cycle) - 1;
		//TIM2->CCR1 = duty_cycle;
		TIM5->CCR1 = (duty_cycle * 2 + 50U);
		delayMilli(1U);
		if (duty_cycle_direction == INC) {
			duty_cycle++;
			if (duty_cycle == 99U) {
				duty_cycle_direction = DEC;
			}
		}
		else {
			duty_cycle--;
			if (duty_cycle == 0U) {
				duty_cycle_direction = INC;
			}
		}
	}
	
	return 0;
}
