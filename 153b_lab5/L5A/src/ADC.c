#include "ADC.h"

#include "stm32l476xx.h"

#include <stdint.h>

void ADC_Wakeup(void) {
    int wait_time;

    // To start ADC operations, the following sequence should be applied
    // DEEPPWD = 0: ADC not in deep-power down
    // DEEPPWD = 1: ADC in deep-power-down (default reset state)
    if ((ADC1->CR & ADC_CR_DEEPPWD) == ADC_CR_DEEPPWD)
        ADC1->CR &= ~ADC_CR_DEEPPWD; // Exit deep power down mode if still in that state

    // Enable the ADC internal voltage regulator
    // Before performing any operation such as launching a calibration or enabling the ADC, the ADC
    // voltage regulator must first be enabled and the software must wait for the regulator start-up
    // time.
    ADC1->CR |= ADC_CR_ADVREGEN;

    // Wait for ADC voltage regulator start-up time
    // The software must wait for the startup time of the ADC voltage regulator (T_ADCVREG_STUP)
    // before launching a calibration or enabling the ADC.
    // T_ADCVREG_STUP = 20 us
    wait_time = 20 * (80000000 / 1000000);
    while (wait_time != 0) {
        wait_time--;
    }
}

void ADC_Common_Configuration() {
	// enable IO analog switch voltage booster
	SYSCFG->CFGR1 |= SYSCFG_CFGR1_BOOSTEN;
	// enable VREFINT in ADC common control register
	ADC123_COMMON->CCR |= ADC_CCR_VREFEN;
	// ensure that ADC clock is not divided
	ADC123_COMMON->CCR &= ~ADC_CCR_PRESC;
	// ensure that ADC clock scheme is set to HCLK/1 synchronous
	ADC123_COMMON->CCR &= ~ADC_CCR_CKMODE;
	ADC123_COMMON->CCR |= ADC_CCR_CKMODE_0;
	// ensure that all ADCs are operating independently
	ADC123_COMMON->CCR &= ~ADC_CCR_DUAL;
}

void ADC_Pin_Init(void) {
	// enable GPIO Clock for port A
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
	// set PA1 mode to analog
	GPIOA->MODER |= GPIO_MODER_MODE1;
	// set PA1 to no pull-up, no pull-down
	GPIOA->PUPDR &= ~GPIO_PUPDR_PUPD1;
	// connect PA1 to ADC input
	GPIOA->ASCR |= GPIO_ASCR_ASC1;
}

void ADC_Init(void) {
	// Enable & Reset ADC Clock
	RCC->AHB2ENR |= RCC_AHB2ENR_ADCEN;
	RCC->AHB2RSTR |= RCC_AHB2RSTR_ADCRST;
	RCC->AHB2RSTR &= ~RCC_AHB2RSTR_ADCRST;
	// Other ADC Initialization
	ADC_Pin_Init();
	ADC_Common_Configuration();
	ADC_Wakeup();
	// Other Configuration
	// disable ADC1
	ADC1->CR &= ~ADC_CR_ADEN;
	// configure ADC1 to have 12-bit resolution
	ADC1->CFGR &= ~ADC_CFGR_RES;
	// configure ADC1 to have right alignment
	ADC1->CFGR &= ~ADC_CFGR_ALIGN;
	// set ADC1 sequence length to 1
	ADC1->SQR1 &= ~ADC_SQR1_L;
	// use ADC1 channel 6 for first conversion
	ADC1->SQR1 &= ~ADC_SQR1_SQ1;
	ADC1->SQR1 |= (6UL << 6UL);
	// set ADC1 channel 6 to single-ended mode
	ADC1->DIFSEL &= ~(0b1 << 6UL);
	// set ADC1 channel 6 sample time to 24.5 ADC clock cycles
	ADC1->SMPR1 &= ~ADC_SMPR1_SMP6;
	ADC1->SMPR1 |= (0b011 << 18UL);
	// set ADC1 to single conversion mode
	ADC1->CFGR &= ~ADC_CFGR_CONT;
	// disable ADC1 hardware trigger detection
	ADC1->CFGR &= ~ADC_CFGR_EXTEN;
	// enable ADC1
	ADC1->CR |= ADC_CR_ADEN;
	// wait for ADC1 ready
	while ((ADC1->ISR & ADC_ISR_ADRDY) != ADC_ISR_ADRDY);
}
