/*
 * ECE 153B
 *
 * Name(s):
 * Section:
 * Lab: 2C
 */
 
#include "stm32l476xx.h"

#include "LED.h"
#include "RTC.h"
#include "SysClock.h"

char strTime[12] = {0};
char strDate[12] = {0};

void RTC_Alarm_Enable(void);
void RTC_Alarm_Enable(void) {
	// 1a: configure EXTI Trigger
	EXTI->RTSR1 |= EXTI_RTSR1_RT18; // trigger on rising edge
	EXTI->FTSR1 &= ~EXTI_FTSR1_FT18; // don't trigger on falling edge
	
	// 1b: set interrupt and event masks
	EXTI->IMR1 |= EXTI_IMR1_IM18;
	EXTI->EMR1 |= EXTI_EMR1_EM18;
	
	// 1c: clear interrupt pending bit
	EXTI->PR1 |= EXTI_PR1_PIF18;
	
	// 1d: configure and Enable in NVIC
	NVIC_EnableIRQ(RTC_Alarm_IRQn);
	NVIC_SetPriority(RTC_Alarm_IRQn, 0);
}

void RTC_Set_Alarm(void);
void RTC_Set_Alarm(void) {
	RTC_Disable_Write_Protection();
	
	// disable both alarms and their interrupts
	RTC->CR &= ~RTC_CR_ALRAE;
	RTC->CR &= ~RTC_CR_ALRBE;
	RTC->CR &= ~RTC_CR_ALRAIE;
	RTC->CR &= ~RTC_CR_ALRBIE;
	
	// wait for alarm write access flag
	while (((RTC->ISR & RTC_ISR_ALRAWF) != RTC_ISR_ALRAWF) || ((RTC->ISR & RTC_ISR_ALRBWF) != RTC_ISR_ALRBWF)) {}
	
	// set alarm A to current time + 1 second
	uint32_t next_second = (((RTC->TR & RTC_TR_SU) + ((RTC->TR & RTC_TR_ST) >> 4)*10U) + 1U) % 60U;
	RTC->ALRMAR = ((getTens(next_second) << 4) | getOnes(next_second));
	RTC->ALRMAR |= (RTC_ALRMAR_MSK4 | RTC_ALRMAR_MSK3 | RTC_ALRMAR_MSK2);
	// set alarm B to current time + 1 minute
	uint32_t next_minute = (((RTC->TR & RTC_TR_MNU) >> 8) + ((RTC->TR & RTC_TR_MNT) >> 12)*10U + 1U) % 60U;
	RTC->ALRMBR = ((getTens(next_minute) << 12) | (getOnes(next_minute) << 8));
	RTC->ALRMBR |= (RTC_ALRMAR_MSK4 | RTC_ALRMAR_MSK3 | RTC_ALRMAR_MSK1);
	// enable both alarms and their interrupts
	//RTC->CR |= RTC_CR_ALRAE;
	RTC->CR |= RTC_CR_ALRBE;
	//RTC->CR |= RTC_CR_ALRAIE;
	RTC->CR |= RTC_CR_ALRBIE;
	
	RTC_Enable_Write_Protection();
}

void RTC_Alarm_IRQHandler(void);
void RTC_Alarm_IRQHandler(void) {
	// clear interrupt pending bit
	EXTI->PR1 |= EXTI_PR1_PIF18;
	RTC_Set_Alarm();
	Green_LED_Toggle();
}

int main(void) {	
	System_Clock_Init(); // Switch System Clock = 80 MHz
	
	LED_Init();
	
	RTC_Init();
	RTC_Alarm_Enable();
	RTC_Set_Alarm();
	
	while(1) {
		Get_RTC_Calendar(strTime, strDate);
	}
}
